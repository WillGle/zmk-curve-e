#!/usr/bin/env python3
"""Archive compile evidence separately from firmware that embeds the private pair key."""
import datetime
import hashlib
import json
import os
import pathlib
import shutil
import struct
import sys

root = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(root / 'tools'))
from product import load_product, revision
product = load_product()
work = pathlib.Path('/work/esb-work')
role = sys.argv[1]
assert role in ('receiver', 'keyboard', 'baseline')
tag = os.environ['CE_BUILD_ID']
assert tag and all(c.isalnum() or c in '-_' for c in tag)
public = root / 'firmware/experimental/builds' / tag / role
private = root / 'firmware/experimental/private' / tag / role
public.mkdir(parents=True, exist_ok=False)
private.mkdir(parents=True, exist_ok=False, mode=0o700)
os.chmod(private.parent, 0o700)
os.chmod(private, 0o700)

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

binary_records = []
for name in ('zmk.uf2', 'zmk.hex', 'zmk.bin', 'zmk.elf'):
    source = work / role / 'zephyr' / name
    if not source.exists():
        continue
    target = private / name
    shutil.copyfile(source, target)
    os.chmod(target, 0o600)
    binary_records.append({'path': str(target.relative_to(root)), 'bytes': target.stat().st_size,
                           'sha256': digest(target)})
for source, name in [(work / f'{role}.log', 'build.log'),
                     (work / role / 'zephyr/.config', 'effective.config'),
                     (work / role / 'zephyr/zephyr.dts', 'zephyr.dts'),
                     (work / 'dependencies.json', 'dependencies.json')]:
    shutil.copyfile(source, public / name)

sources = [p for p in (root / 'experimental/esb').rglob('*') if p.is_file()]
sources += [root / 'product.json', root / 'zephyr/module.yml']
sources += list((root / 'tools').glob('*.py')) + list((root / 'tools').glob('*.sh'))
sources += [p for directory in ('boards/curve_e', 'config') for p in (root / directory).rglob('*') if p.is_file()]
source_records = {str(p.relative_to(root)): digest(p) for p in sorted(sources)}
(public / 'source-checksums.json').write_text(json.dumps(source_records, indent=2) + '\n')
config = (public / 'effective.config').read_text()
if role == 'baseline':
    reference = root / product['firmware']['current'] / 'zmk.uf2'
    identical = digest(private / 'zmk.uf2') == digest(reference)
    assert identical, 'Baseline USB/BLE changed; do not claim the experiment preserves it'
    assert 'CONFIG_ZMK_BLE=y' in config and '# CONFIG_CURVE_ESB' not in config
else:
    identical = None
    assert 'CONFIG_CURVE_ESB=y' in config and '# CONFIG_ZMK_BLE is not set' in config
    assert '# CONFIG_ZMK_STUDIO is not set' in config
uf2_info = None
uf2 = private / 'zmk.uf2'
if role == 'keyboard':
    data = uf2.read_bytes()
    assert len(data) % 512 == 0
    ranges = []
    for offset in range(0, len(data), 512):
        words = struct.unpack_from('<8I', data, offset)
        assert words[:2] == (0x0a324655, 0x9e5d5157)
        assert struct.unpack_from('<I', data, offset + 508)[0] == 0x0ab16f30
        address, size = words[3:5]
        assert size <= 476 and address >= 0x26000 and address + size <= 0xea000
        ranges.append((address, address + size))
    uf2_info = {'blocks': len(ranges), 'start': hex(min(a for a, _ in ranges)),
                'end_exclusive': hex(max(b for _, b in ranges)), 'app_bounds_pass': True}
manifest = {'created_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
            'role': role, 'status': 'compile-only; not flashed or radio-tested',
            'zmk': revision('zmk'),
            'zephyr': revision('zephyr'),
            'image': product['build']['image'],
            'target': product['experimental']['esb']['receiver_board'] if role == 'receiver' else product['board'],
            'private_artifacts': binary_records, 'uf2': uf2_info,
            'baseline_byte_identical': identical}
(public / 'build.json').write_text(json.dumps(manifest, indent=2) + '\n')
# Do not allow a pair key accidentally printed in compiler diagnostics into public evidence.
secret_line = (root / 'firmware/experimental/private/pair-key.h').read_text().splitlines()[-1]
for path in public.iterdir():
    assert secret_line not in path.read_text(), 'Private key text found in public evidence'
(public / 'SHA256SUMS').write_text(''.join(f'{digest(p)}  {p.name}\n' for p in sorted(public.iterdir()) if p.is_file()))
print(f'Archived {role}: {public.relative_to(root)} (binary artifacts private)')
