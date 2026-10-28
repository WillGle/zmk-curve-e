#!/usr/bin/env python3
"""Archive a main-firmware rebuild without promoting it to an installed release."""
import datetime
import hashlib
import json
import pathlib
import shutil
import sys
from product import ROOT, load_product, revision

work = pathlib.Path(sys.argv[1])
product = load_product()
archive = ROOT / 'firmware/verification' / work.name
archive.mkdir(parents=True, exist_ok=False)
for source, name in [(work / 'zephyr/zmk.uf2', 'zmk.uf2'), (work / 'build.log', 'build.log'),
                     (work / 'zephyr/.config', 'effective.config'), (work / 'zephyr/zephyr.dts', 'zephyr.dts'),
                     (work / 'west-frozen.yml', 'west-frozen.yml'), (work / 'dependencies.json', 'dependencies.json')]:
    shutil.copyfile(source, archive / name)

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

reference = ROOT / product['firmware']['current'] / 'zmk.uf2'
config = (archive / 'effective.config').read_text()
assert 'CONFIG_ZMK_BLE=y' in config
assert 'CONFIG_CURVE_ESB=y' not in config
sources = [ROOT / 'product.json', ROOT / 'zephyr/module.yml']
sources += list((ROOT / 'tools').glob('*.py')) + list((ROOT / 'tools').glob('*.sh'))
sources += [p for directory in ('boards/curve_e', 'config') for p in (ROOT / directory).rglob('*') if p.is_file()]
(archive / 'source-checksums.json').write_text(json.dumps({str(p.relative_to(ROOT)): digest(p) for p in sorted(sources)}, indent=2) + '\n')
manifest = {'created_utc': datetime.datetime.now(datetime.timezone.utc).isoformat(),
            'product': product['name'], 'target': product['board'], 'zmk': revision('zmk'),
            'zephyr': revision('zephyr'), 'image': product['build']['image'],
            'status': 'offline rebuild; not flashed or promoted', 'baseline': product['firmware']['current'],
            'baseline_byte_identical': digest(archive / 'zmk.uf2') == digest(reference),
            'uf2_sha256': digest(archive / 'zmk.uf2')}
(archive / 'build.json').write_text(json.dumps(manifest, indent=2) + '\n')
(archive / 'SHA256SUMS').write_text(''.join(f'{digest(p)}  {p.name}\n' for p in sorted(archive.iterdir()) if p.is_file()))
print(f'Archived main build: {archive.relative_to(ROOT)}')
print(f'Baseline byte-identical: {manifest["baseline_byte_identical"]}')
