#!/usr/bin/env bash
# Build only: never flash hardware or run west update.
set -euo pipefail
repo_root=$(cd "$(dirname "$0")/.." && pwd)
mode=${1:-all}
case "$mode" in all|receiver|keyboard|baseline|prepare) ;; *) echo "Usage: $0 [all|receiver|keyboard|baseline|prepare]" >&2; exit 2;; esac
build_tag=$(date -u +%Y%m%dT%H%M%SZ)
export repo_root
python3 - <<'PY'
import os, pathlib, shutil, re
root=pathlib.Path(os.environ['repo_root'])
sys_path = root / 'tools'
import sys
sys.path.insert(0, str(sys_path))
from product import load_product
product = load_product()
work=root/product['build']['workspace']/'esb-work'
work.mkdir(parents=True,exist_ok=True)
conf=work/'config';conf.mkdir(exist_ok=True)
# Filter only experimental snapshot: BLE production configuration remains untouched.
s=(root/product['config']/'curve_e.conf').read_text()
s='\n'.join(l for l in s.splitlines() if not re.match(r'CONFIG_(BT_|ZMK_BLE|ZMK_BATTERY_REPORTING|ZMK_SLEEP|ZMK_IDLE_SLEEP_TIMEOUT|ZMK_STUDIO)',l))+'\n'
(conf/'curve_e.conf').write_text(s)
k=(root/'boards/curve_e/curve_e.keymap').read_text()
k=re.sub(r'&bt\s+BT_SEL\s+[0-4]', '&none', k)
k=k.replace('&bt BT_CLR','&none').replace('OUT_BLE','OUT_ESB')
(conf/'curve_e.keymap').write_text(k)
private=root/'firmware/experimental/private';private.mkdir(parents=True,exist_ok=True,mode=0o700)
os.chmod(private,0o700)
key=private/'pair-key.h'
if not key.exists():
    data=os.urandom(16)
    with key.open('x') as f:f.write('/* Local secret: do not commit or publish firmware containing this key. */\n#define CURVE_ESB_PAIR_KEY {'+', '.join(f'0x{x:02x}' for x in data)+'}\n')
    os.chmod(key,0o600)
PY
image=$(python3 "$repo_root/tools/product.py" build.image)
workspace=$(python3 "$repo_root/tools/product.py" build.workspace)
mkdir -p "$repo_root/$workspace/config"
cp "$repo_root/config/west.yml" "$repo_root/$workspace/config/west.yml"
docker run --rm --network none --user "$(id -u):$(id -g)" \
  -e HOME=/tmp/curve-esb-build-home -e CE_BUILD_MODE="$mode" -e CE_BUILD_ID="$build_tag" \
  -v "$repo_root:/repo" -v "$repo_root/$workspace:/work" -w /work \
  "$image" bash -euo pipefail -c '
    mkdir -p "$HOME"
    python3 /repo/experimental/esb/prepare-workspace.py
    mkdir -p /work/esb-work/zmk
    zmk_revision=$(python3 /repo/tools/product.py --revision zmk)
    git -C /work/zmk archive "$zmk_revision" | tar -x -C /work/esb-work/zmk
    west zephyr-export > /work/esb-work/zephyr-export.log 2>&1
    if git -C /work/esb-work/zmk apply --check /repo/experimental/esb/patches/zmk-esb.patch 2>/dev/null; then
      git -C /work/esb-work/zmk apply /repo/experimental/esb/patches/zmk-esb.patch
    else
      git -C /work/esb-work/zmk apply --reverse --check /repo/experimental/esb/patches/zmk-esb.patch
    fi
    build_one() {
      local role="$1"
      local output="/work/esb-work/$role"
      local source=/work/esb-work/zmk/app
      local modules
      local board snippet config receiver_board receiver_shield esb_module
      board=$(python3 /repo/tools/product.py board)
      snippet=$(python3 /repo/tools/product.py build.snippet)
      config=$(python3 /repo/tools/product.py config)
      receiver_board=$(python3 /repo/tools/product.py experimental.esb.receiver_board)
      receiver_shield=$(python3 /repo/tools/product.py experimental.esb.receiver_shield)
      esb_module=$(python3 /repo/tools/product.py experimental.esb.module)
      modules="/repo;/repo/$esb_module"
      local args=()
      if [ "$role" = baseline ]; then
        source=/work/zmk/app
        modules=/repo
        args=(-b "$board" -S "$snippet" -- "-DZMK_CONFIG=/repo/$config")
      elif [ "$role" = keyboard ]; then
        args=(-b "$board" -S "$snippet" -- -DZMK_CONFIG=/work/esb-work/config "-DEXTRA_CONF_FILE=/repo/experimental/esb/config/common.conf;/repo/experimental/esb/config/keyboard.conf")
      else
        args=(-b "$receiver_board" -- "-DSHIELD=$receiver_shield" "-DEXTRA_CONF_FILE=/repo/experimental/esb/config/common.conf;/repo/experimental/esb/config/receiver.conf")
      fi
      if ! west build -p always -s "$source" -d "$output" "${args[@]}" \
        "-DZMK_EXTRA_MODULES=$modules" \
        -DCURVE_ESB_PAIR_HEADER=/repo/firmware/experimental/private/pair-key.h > "$output.log" 2>&1; then
        tail -65 "$output.log"
        return 1
      fi
      tail -12 "$output.log"
      python3 /repo/experimental/esb/archive-build.py "$role"
    }
    case "$CE_BUILD_MODE" in
      prepare) ;;
      all) build_one receiver; build_one keyboard; build_one baseline;;
      *) build_one "$CE_BUILD_MODE";;
    esac
    west manifest --freeze --active-only > /work/esb-work/west-frozen.yml
  '
