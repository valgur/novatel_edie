from pathlib import Path

import pytest


@pytest.fixture
def project_root():
    return Path(__file__).parent.parent.parent


@pytest.fixture
def hw_interface_test_resources(project_root):
    return project_root / "src" / "hw_interface" / "stream_interface" / "test" / "resources"


@pytest.fixture
def decoders_test_resources(project_root):
    return project_root / "src" / "decoders" / "novatel" / "test" / "resources"
