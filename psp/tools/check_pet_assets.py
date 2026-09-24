"""Check embedded sprites match their numbered source PNGs and species IDs."""
import hashlib
import json
import re
from pathlib import Path

root = Path(__file__).resolve().parents[1]
manifest = json.loads((root / 'assets/generated/pets.json').read_text())
binary = (root / 'assets/generated/pets.rgba4444').read_bytes()
ids = re.findall(r'\bSPECIES_([A-Z]+)\b', (root / 'include/creature.h').read_text())
ids = ids[:ids.index('COUNT')]
assert len(manifest) == len(ids) == 30
assert len(binary) == 30 * 128 * 128 * 2
files = sorted((root / 'assets/pets').glob('*.png'))
assert len(files) == 30
for i, entry in enumerate(manifest):
    assert entry['id'] == i and entry['number'] == f'{i+1:03d}'
    assert entry['name'].upper() == ids[i]
    path = root / entry['source']
    assert path.resolve() == files[i].resolve()
    assert hashlib.sha256(path.read_bytes()).hexdigest() == entry['sha256'], \
        f'{path.name} changed: run tools/build_pet_assets.py first'
    texture = binary[i*32768:(i+1)*32768]
    assert hashlib.sha256(texture).hexdigest() == entry['texture_sha256']
print('PASS: all 30 numbered PNGs, roster IDs, and compiled texture checksums match')
