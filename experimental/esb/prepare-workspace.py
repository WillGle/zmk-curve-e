#!/usr/bin/env python3
"""Fail closed when the cached dependency tree differs from the validated baseline."""
import json
import pathlib
import subprocess
import shutil
import yaml

workspace = pathlib.Path('/work')
manifest_path = pathlib.Path('/repo/firmware/releases/2026-10-28-zmk-4dd95d7-ble-compat-curved/west-frozen.yml')
manifest = yaml.safe_load(manifest_path.read_text())['manifest']
verified = []
for project in manifest['projects']:
    path = workspace / project.get('path', project['name'])
    revision = subprocess.check_output(['git', '-C', str(path), 'rev-parse', 'HEAD'], text=True).strip()
    if revision != project['revision']:
        raise SystemExit(f"Dependency revision mismatch: {project['name']}")
    dirty = subprocess.check_output(['git', '-C', str(path), 'status', '--porcelain', '--untracked-files=normal'], text=True)
    if dirty:
        raise SystemExit(f"Dependency is dirty: {project['name']}; preserve and inspect it before building")
    verified.append({'name': project['name'], 'revision': revision})
(workspace / 'esb-work/dependencies.json').write_text(json.dumps(verified, indent=2) + '\n')
print(f'Verified {len(verified)} pinned, clean dependencies')

# Only this generated source copy is replaced; the cache and production sources are read-only.
source = workspace / 'esb-work/zmk'
if source.exists():
    shutil.rmtree(source)
source.mkdir()
