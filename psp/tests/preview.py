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
for name in ('dialogue', 'ready-prompt', 'ready-prompt-no', 'npc-battle',
             'east-challenger', 'east-challenger-ready', 'east-challenger-battle', 'east-challenger-victory',
             'forest-gatekeeper-locked', 'forest-gatekeeper-open',
             'encounter', 'battle-menu', 'battle-moves', 'learn-move', 'evolution', 'partner',
             'capture', 'captured', 'party', 'collection', 'collection-swap', 'battle-switch',
             'collection-empty', 'collection-full', 'items', 'shop',
             'player-menu', 'field-items', 'options', 'marsh', 'lantern-rest',
             'zappip', 'bubfin', 'battle-impact', 'spotlight-idle',
             'spotlight-ally', 'spotlight-enemy', 'saved-dialogue') + tuple(f'pet-{i:03d}' for i in range(1,31)):
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
