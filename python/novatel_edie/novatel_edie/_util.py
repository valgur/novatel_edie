import sys
from ctypes import CDLL
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent.absolute()
RESOURCES_DIR = SCRIPT_DIR / 'resources'

if sys.maxsize > 2 ** 32:
    ARCH = 'x64'
else:
    ARCH = 'x32'


def _load_shared_library(lib_name):
    if sys.platform == 'win32':
        prefix, ext = '', 'dll'
    else:
        prefix, ext = 'lib', 'so'
    try:
        return CDLL(str(RESOURCES_DIR / f'{prefix}{lib_name}_{ARCH}.{ext}'))
    except Exception:
        return CDLL(f'{prefix}{lib_name}.{ext}')
