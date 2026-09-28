"""Compile the user's PNGs into embedded PSP textures. Requires Pillow.

Run from any directory with Python 3. Originals are never modified.
"""
from pathlib import Path
import hashlib
import json
import struct
import zlib
try:
    from PIL import Image, ImageDraw
except ModuleNotFoundError:
    Image = ImageDraw = None

ROOT = Path(__file__).resolve().parents[1]
FILES = sorted((ROOT / 'assets/pets').glob('[0-9][0-9][0-9].*.png'))
assert len(FILES) == 60, 'Expected exactly 60 numbered pet PNGs'
output = ROOT / 'assets/generated'
output.mkdir(exist_ok=True)
preview = ROOT / 'previews'
preview.mkdir(exist_ok=True)

def compile_without_pillow():
    """Portable RGBA PNG path for the supplied non-interlaced source art."""
    def png_rgba(path):
        raw = path.read_bytes()
        assert raw[:8] == b'\x89PNG\r\n\x1a\n'
        offset = 8; chunks = []; width = height = None
        while offset < len(raw):
            size = struct.unpack('>I', raw[offset:offset + 4])[0]
            tag = raw[offset + 4:offset + 8]; data = raw[offset + 8:offset + 8 + size]
            offset += size + 12
            if tag == b'IHDR':
                width, height, depth, color, compression, filtering, interlace = struct.unpack('>IIBBBBB', data)
                assert (depth, color, compression, filtering, interlace) == (8, 6, 0, 0, 0)
            elif tag == b'IDAT': chunks.append(data)
            elif tag == b'IEND': break
        assert width and height
        encoded = zlib.decompress(b''.join(chunks)); stride = width * 4; rows = []; previous = bytearray(stride); at = 0
        for _ in range(height):
            kind = encoded[at]; at += 1; row = bytearray(encoded[at:at + stride]); at += stride
            for i in range(stride):
                left = row[i - 4] if i >= 4 else 0; up = previous[i]; up_left = previous[i - 4] if i >= 4 else 0
                if kind == 1: row[i] = (row[i] + left) & 255
                elif kind == 2: row[i] = (row[i] + up) & 255
                elif kind == 3: row[i] = (row[i] + ((left + up) // 2)) & 255
                elif kind == 4:
                    p = left + up - up_left; pa, pb, pc = abs(p-left), abs(p-up), abs(p-up_left)
                    row[i] = (row[i] + (left if pa <= pb and pa <= pc else up if pb <= pc else up_left)) & 255
                elif kind != 0: raise AssertionError(f'Unsupported PNG filter {kind}')
            rows.append(row); previous = row
        return width, height, b''.join(rows)

    packed = bytearray(); manifest = []
    for index, path in enumerate(FILES):
        number, name, extension = path.name.split('.')
        assert int(number) == index + 1 and extension == 'png'
        width, height, rgba = png_rgba(path)
        opaque = [(i // 4 % width, i // 4 // width) for i in range(3, len(rgba), 4) if rgba[i]]
        assert opaque, f'Empty image: {path}'
        left, right = min(p[0] for p in opaque), max(p[0] for p in opaque)
        top, bottom = min(p[1] for p in opaque), max(p[1] for p in opaque)
        crop_w, crop_h = right-left+1, bottom-top+1; limit = (76, 84, 92)[index % 3]
        scale = min(1.0, limit / crop_w, limit / crop_h); out_w, out_h = max(1, int(crop_w*scale)), max(1, int(crop_h*scale))
        canvas = bytearray(128 * 128 * 4); x0, y0 = (96-out_w)//2, 94-out_h
        for y in range(out_h):
            sy = top + min(crop_h-1, int(y / scale))
            for x in range(out_w):
                sx = left + min(crop_w-1, int(x / scale)); src = (sy*width+sx)*4; dst = ((y0+y)*128+x0+x)*4
                canvas[dst:dst+4] = rgba[src:src+4]
        values = [(canvas[i] >> 4) | ((canvas[i+1] >> 4) << 4) | ((canvas[i+2] >> 4) << 8) | ((canvas[i+3] >> 4) << 12)
                  for i in range(0, len(canvas), 4)]
        texture_bytes = struct.pack('<16384H', *values); packed.extend(texture_bytes)
        manifest.append({'id': index, 'number': number, 'name': name, 'source': path.relative_to(ROOT).as_posix(),
                         'sha256': hashlib.sha256(path.read_bytes()).hexdigest(), 'texture_sha256': hashlib.sha256(texture_bytes).hexdigest()})
    (output / 'pets.rgba4444').write_bytes(packed)
    (output / 'pets.json').write_text(json.dumps(manifest, indent=2) + '\n')
    print(f'Compiled {len(FILES)} pets without Pillow: {len(packed):,} bytes, RGBA4444, 128x128 each.')

if Image is None:
    compile_without_pillow()
    raise SystemExit(0)
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
