import os
import shutil
import sys
from distutils.command.build import build
from pathlib import Path

from setuptools import find_packages
from setuptools import setup
from setuptools.command.test import test as test_command

PACKAGE_NAME = 'novatel_edie'
VERSION_FILENAME = 'version.py'
DESCRIPTION = 'Package for working with NovAtel receiver data'
AUTHOR = 'rdoris'
AUTHOR_EMAIL = 'rdoris@novatel.com'
INSTALL_REQUIRES = []

SCRIPT_DIR = Path(__file__).parent.absolute()


def copy_libraries(package_root, arch, ext):
    def copy(src, dst):
        print(f'Copying "{src}" -> {dst}"')
        shutil.copy(src, dst)

    package_root = Path(package_root).resolve()
    resource_dir = SCRIPT_DIR / PACKAGE_NAME / 'resources'
    resource_dir.mkdir(parents=True, exist_ok=True)
    bin_root = package_root / 'bin'
    pattern = f'**/Release-{arch}/*.{ext}'
    print("Globbing", bin_root / pattern)
    package_so = sorted(bin_root.glob(pattern))
    if not package_so:
        raise Exception('Unable to find the required .so/.dll files for EDIE')
    for file in package_so:
        if arch == 'x64':
            copy(file, resource_dir / f'{file.stem}_x64.{ext}')
        else:
            copy(file, resource_dir / f'{file.stem}_x32.{ext}')
    copy(package_root / 'database/novatel_log_definitions.json', resource_dir)


def get_platform_info(plat_name):
    """Derives the architecture ID and shared library extension from the platform name.

    The platform name must be in the format returned by sysconfig.get_platform().
    See: https://docs.python.org/3/library/sysconfig.html#sysconfig.get_platform
    """
    arch = plat_name.rsplit('-', 1)[-1]
    if plat_name.startswith('win'):
        if plat_name == 'win32':
            return 'Win32', 'dll'
        if arch == 'amd64':
            return 'x64', 'dll'
        return arch, 'dll'
    else:
        if arch in ['ppc', 'i386', 'fat', 'i686']:
            return 'x32', 'so'
        if arch in ['x86_64', 'amd64', 'ppc64']:
            return 'x64', 'so'
        return arch, 'so'


class CopyBinaries(build):
    def run(self):
        edie_root = os.environ.get('EDIE_ROOT') or SCRIPT_DIR.parent.parent
        print(f'EDIE_ROOT set to "{edie_root}"')
        arch, ext = get_platform_info(self.plat_name)
        copy_libraries(edie_root, arch, ext)
        super().run()


def parse_version():
    version_path = SCRIPT_DIR / PACKAGE_NAME / VERSION_FILENAME
    main_ns = {}
    exec(version_path.read_text(), main_ns)
    return main_ns['__version__']


def parse_description():
    return (SCRIPT_DIR / 'readme.md').read_text()


class PyTestCommand(test_command):
    user_options = [('pytest-args=', 'a', 'Arguments to pass to pytest')]

    def initialize_options(self):
        test_command.initialize_options(self)
        self.pytest_args = ''

    def run_tests(self):
        import shlex
        import pytest
        print(self.pytest_args)
        errno = pytest.main(shlex.split(self.pytest_args))
        sys.exit(errno)


setup(
    name=PACKAGE_NAME,
    version=parse_version(),
    description=DESCRIPTION,
    long_description=parse_description(),
    author=AUTHOR,
    author_email=AUTHOR_EMAIL,
    packages=find_packages(exclude=['test', 'doc']),
    cmdclass={'build': CopyBinaries, 'test': PyTestCommand},
    tests_require=['pytest'],
    install_requires=INSTALL_REQUIRES,
    entry_points={},
    include_package_data=True,
    platforms=["Windows", "Linux"],
    package_data={
        'novatel_edie': ['novatel_edie/resources/*.dll',
                         'novatel_edie/resources/*.so',
                         'novatel_edie/resources/*.json']
    },
    zip_safe=False,
)
