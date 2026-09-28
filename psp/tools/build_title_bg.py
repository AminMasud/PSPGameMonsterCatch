"""Convert the title PNG to the PSP's RGBA4444 texture format.

Uses only the standard library so a PSP SDK setup does not also need Pillow.
"""
from pathlib import Path
import struct
import zlib

ROOT = Path(__file__).resolve().parents[1]


def png_rows(path):
    data = path.read_bytes()
    assert data[:8] == b'\x89PNG\r\n\x1a\n'
    position = 8
    chunks = {}
    compressed = bytearray()
    while position < len(data):
        length = struct.unpack('>I', data[position:position + 4])[0]
        kind = data[position + 4:position + 8]
        payload = data[position + 8:position + 8 + length]
        position += length + 12
        if kind == b'IDAT': compressed.extend(payload)
        else: chunks[kind] = payload
    width, height, depth, color, compression, filtering, interlace = struct.unpack('>IIBBBBB', chunks[b'IHDR'])
    assert (width, height, depth, color, compression, filtering, interlace) == (480, 272, 8, 3, 0, 0, 0)
    raw = zlib.decompress(compressed)
    previous = bytearray(width)
    rows = []
    offset = 0
    for _ in range(height):
        filter_type = raw[offset]
        row = bytearray(raw[offset + 1:offset + 1 + width])
        offset += width + 1
        for i, value in enumerate(row):
            left = row[i - 1] if i else 0
            above = previous[i]
            upper_left = previous[i - 1] if i else 0
            if filter_type == 1: row[i] = (value + left) & 255
            elif filter_type == 2: row[i] = (value + above) & 255
            elif filter_type == 3: row[i] = (value + ((left + above) >> 1)) & 255
            elif filter_type == 4:
                estimate = left + above - upper_left
                predictor = min(((abs(estimate - left), left), (abs(estimate - above), above),
                                 (abs(estimate - upper_left), upper_left)))[1]
                row[i] = (value + predictor) & 255
            else: assert filter_type == 0
        rows.append(row)
        previous = row
    return rows, chunks[b'PLTE'], chunks.get(b'tRNS', b'')


rows, palette, alpha = png_rows(ROOT / 'assets/pets/bg.png')
values = []
for row in rows:
    for index in row:
        red, green, blue = palette[index * 3:index * 3 + 3]
        opacity = alpha[index] if index < len(alpha) else 255
        values.append((red >> 4) | ((green >> 4) << 4) | ((blue >> 4) << 8) | ((opacity >> 4) << 12))
    values.extend([0] * 32)  # 480 pixels padded to the 512-pixel GU stride.
(ROOT / 'assets/generated/title_bg.rgba4444').write_bytes(struct.pack('<%dH' % len(values), *values))
print('Compiled title background: 512x272 RGBA4444')
