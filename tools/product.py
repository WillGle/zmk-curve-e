#!/usr/bin/env python3
"""Read the product catalog and its immutable dependency revisions."""
import json
import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]


def load_product():
    return json.loads((ROOT / 'product.json').read_text())


def locked_projects():
    import yaml  # Available in the pinned build image; metadata reads need only stdlib.
    manifest = yaml.safe_load((ROOT / load_product()['manifest']).read_text())['manifest']
    projects = manifest['projects']
    if any(not re.fullmatch(r'[0-9a-f]{40}', p['revision']) for p in projects):
        raise ValueError('Every dependency must use a full commit SHA')
    if any('import' in p for p in projects):
        raise ValueError('The current manifest must be resolved, without nested imports')
    return projects


def revision(name):
    return next(p['revision'] for p in locked_projects() if p['name'] == name)


if __name__ == '__main__':
    if sys.argv[1] == '--revision':
        print(revision(sys.argv[2]))
        raise SystemExit(0)
    value = load_product()
    for key in sys.argv[1].split('.'):
        value = value[key]
    if not isinstance(value, str):
        raise SystemExit('Requested catalog field must be a string')
    print(value)
