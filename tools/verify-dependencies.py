#!/usr/bin/env python3
"""Verify cached sources against the product lock before either build path."""
import json
import pathlib
import subprocess
import sys
from product import locked_projects

workspace = pathlib.Path('/work')
verified = []
for project in locked_projects():
    path = workspace / project.get('path', project['name'])
    head = subprocess.check_output(['git', '-C', str(path), 'rev-parse', 'HEAD'], text=True).strip()
    if head != project['revision']:
        raise SystemExit(f"Dependency revision mismatch: {project['name']}")
    dirty = subprocess.check_output(['git', '-C', str(path), 'status', '--porcelain', '--untracked-files=normal'], text=True)
    if dirty:
        raise SystemExit(f"Dependency is dirty: {project['name']}; preserve it and inspect before building")
    verified.append({'name': project['name'], 'revision': head})
pathlib.Path(sys.argv[1]).write_text(json.dumps(verified, indent=2) + '\n')
print(f'Verified {len(verified)} pinned, clean dependencies')
