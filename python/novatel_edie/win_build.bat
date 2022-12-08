echo "Building EDIE 32-bit"
python setup.py clean --all bdist_wheel -p win32
echo "Building EDIE 64-bit"
python setup.py clean --all bdist_wheel -p win_amd64
