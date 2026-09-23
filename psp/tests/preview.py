"""Convert the host renderer's test frames to PNG using only the standard library.

Run after world_systems_test. These are software previews, not PSP captures.
"""
import pathlib
import struct
import zlib


def chunk(kind, payload):
    return (struct.pack('>I', len(payload)) + kind + payload
            + struct.pack('>I', zlib.crc32(kind + payload)))


output = pathlib.Path(__file__).resolve().parent.parent / 'previews'
output.mkdir(exist_ok=True)
for name in ('dialogue', 'encounter'):
    data = (output / (name + '.ppm')).read_bytes()
    magic, dimensions, maximum, pixels = data.split(b'\n', 3)
    assert magic == b'P6' and maximum == b'255'
    width, height = map(int, dimensions.split())
    assert len(pixels) == width * height * 3
    scanlines = b''.join(b'\0' + pixels[y*width*3:(y+1)*width*3]
                         for y in range(height))
    png = (b'\x89PNG\r\n\x1a\n'
           + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 2, 0, 0, 0))
           + chunk(b'IDAT', zlib.compress(scanlines)) + chunk(b'IEND', b''))
    (output / (name + '.png')).write_bytes(png)
