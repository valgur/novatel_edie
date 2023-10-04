from pathlib import Path

import pytest
import novatel_edie as ne


@pytest.fixture(scope="session")
def project_root():
    return Path(__file__).parent.parent.parent


@pytest.fixture(scope="session")
def hw_interface_test_resources(project_root):
    return project_root / "src" / "hw_interface" / "stream_interface" / "test" / "resources"


@pytest.fixture(scope="session")
def decoders_test_resources(project_root):
    return project_root / "src" / "decoders" / "novatel" / "test" / "resources"


@pytest.fixture(scope="session")
def json_db_path():
    return str(ne.JSON_DB_PATH)

@pytest.fixture(scope="session")
def json_db():
    return ne.load_message_database()
