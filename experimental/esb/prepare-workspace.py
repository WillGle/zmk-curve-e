#!/usr/bin/env python3
"""Validate locked dependencies, then replace only the generated ESB source copy."""
import pathlib
import shutil
import subprocess

root = pathlib.Path(__file__).resolve().parents[2]
workspace = pathlib.Path('/work/esb-work')
subprocess.run(['python3', str(root / 'tools/verify-dependencies.py'), str(workspace / 'dependencies.json')], check=True)
source = workspace / 'zmk'
if source.exists():
    shutil.rmtree(source)
source.mkdir()
