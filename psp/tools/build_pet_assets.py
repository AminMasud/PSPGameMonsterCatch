"""Compile the user's PNGs into embedded PSP textures. Requires Pillow.

Run from any directory with Python 3. Originals are never modified.
"""
from pathlib import Path
import hashlib
import json
import struct
from PIL import Image, ImageDraw

ROOT = Path(__file__).resolve().parents[1]
FILES = sorted((ROOT / 'assets/pets').glob('*.png'))
assert len(FILES) == 30, 'Expected exactly 30 numbered pet PNGs'
output = ROOT / 'assets/generated'
output.mkdir(exist_ok=True)
preview = ROOT / 'previews'
preview.mkdir(exist_ok=True)
sheet = Image.new('RGB', (600, 10 * 126), '#15212b')
draw = ImageDraw.Draw(sheet)
packed = bytearray()
manifest = []
for index, path in enumerate(FILES):
    number, name, extension = path.name.split('.')
    assert int(number) == index + 1 and extension == 'png'
    source = Image.open(path).convert('RGBA')
    bounds = source.getchannel('A').getbbox()
    assert bounds, f'Empty image: {path}'
    pet = source.crop(bounds)
    size = (76, 84, 92)[index % 3]
    pet.thumbnail((size, size), Image.Resampling.LANCZOS)
    texture = Image.new('RGBA', (128, 128))
    texture.paste(pet, ((96 - pet.width) // 2, 94 - pet.height))
    # GU_PSM_4444: low nibble R, then G, B, A. Quantize after resizing.
    raw = texture.tobytes()
    rgba = zip(raw[0::4], raw[1::4], raw[2::4], raw[3::4])
    values = [(r >> 4) | ((g >> 4) << 4) | ((b >> 4) << 8) | ((a >> 4) << 12)
              for r, g, b, a in rgba]
    texture_bytes = struct.pack('<16384H', *values)
    packed.extend(texture_bytes)
    # Preview the actual quantized data against a game-like background.
    quantized = Image.new('RGBA', (128, 128))
    quantized.putdata([((v & 15) * 17, ((v >> 4) & 15) * 17,
                        ((v >> 8) & 15) * 17, (v >> 12) * 17) for v in values])
    x, y = (index % 3) * 200, (index // 3) * 126
    sheet.paste(quantized, (x + 52, y), quantized)
    draw.text((x + 22, y + 104), f'{number} {name.upper()}', fill='#f1d59e')
    manifest.append({'id': index, 'number': number, 'name': name,
                     'source': path.relative_to(ROOT).as_posix(),
                     'sha256': hashlib.sha256(path.read_bytes()).hexdigest(),
                     'texture_sha256': hashlib.sha256(texture_bytes).hexdigest()})
(output / 'pets.rgba4444').write_bytes(packed)
(output / 'pets.json').write_text(json.dumps(manifest, indent=2) + '\n')
sheet.save(preview / 'pet-roster.png')
print(f'Compiled {len(FILES)} pets: {len(packed):,} bytes, RGBA4444, 128x128 each.')
