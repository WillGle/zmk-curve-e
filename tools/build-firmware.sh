#!/usr/bin/env bash
# Offline build only. Bootstrap instructions are in guides/build-firmware.md.
set -euo pipefail
if [ "$#" -ne 0 ]; then echo "Usage: $0 (offline; no flashing)" >&2; exit 2; fi
repo_root=$(cd "$(dirname "$0")/.." && pwd)
image=$(python3 "$repo_root/tools/product.py" build.image)
workspace=$(python3 "$repo_root/tools/product.py" build.workspace)
build_tag=$(date -u +%Y%m%dT%H%M%SZ)-product
mkdir -p "$repo_root/$workspace/config"
# This is the generated west workspace manifest, not a source of board settings.
cp "$repo_root/config/west.yml" "$repo_root/$workspace/config/west.yml"
docker run --rm --network none --user "$(id -u):$(id -g)" \
  -e HOME=/tmp/curve-product-home -e CE_BUILD_ID="$build_tag" \
  -v "$repo_root:/repo" -v "$repo_root/$workspace:/work" -w /work \
  "$image" bash -euo pipefail -c '
    mkdir -p "$HOME"
    output="/work/product-work/$CE_BUILD_ID"
    mkdir -p "$output"
    python3 /repo/tools/verify-dependencies.py "$output/dependencies.json"
    west zephyr-export > "$output/zephyr-export.log" 2>&1
    board=$(python3 /repo/tools/product.py board)
    snippet=$(python3 /repo/tools/product.py build.snippet)
    config=$(python3 /repo/tools/product.py config)
    module=$(python3 /repo/tools/product.py module)
    if ! west build -p always -s /work/zmk/app -d "$output" -b "$board" -S "$snippet" -- \
      "-DZMK_CONFIG=/repo/$config" "-DZMK_EXTRA_MODULES=/repo/$module" > "$output/build.log" 2>&1; then
      tail -65 "$output/build.log"
      exit 1
    fi
    tail -12 "$output/build.log"
    west manifest --freeze --active-only > "$output/west-frozen.yml"
    python3 /repo/tools/archive-firmware.py "$output"
  '
