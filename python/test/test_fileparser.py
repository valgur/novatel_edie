################################################################################
#
# COPYRIGHT NovAtel Inc, 2022. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
################################################################################
#                            DESCRIPTION
#
# \file novateltest.hpp
# \brief Unit tests for OEM Framer, HeaderDecoder, MessageDecoder,
# Encoder and Filter.
################################################################################

import novatel_edie as ne
import pytest
from novatel_edie import STATUS, ENCODEFORMAT


# -------------------------------------------------------------------------------------------------------
# Unit Tests
# -------------------------------------------------------------------------------------------------------

@pytest.fixture(scope="function")
def fp(json_db):
    # return ne.FileParser(json_db)
    parser = ne.FileParser()
    parser.load_json_db(json_db)
    return parser


def test_FILEPARSER_INSTANTIATION(json_db_path):
    fp1 = ne.FileParser()
    fp2 = ne.FileParser(json_db_path)

    db = ne.JsonReader()
    db.load_file(json_db_path)
    # fp4 = ne.FileParser(db)
    fp4 = ne.FileParser()
    fp4.load_json_db(db)


def test_RANGE_CMP(fp):
    fp.decompress_range_cmp = True
    assert fp.decompress_range_cmp
    fp.decompress_range_cmp = False
    assert not fp.decompress_range_cmp


def test_UNKNOWN_BYTES(fp):
    fp.return_unknown_bytes = True
    assert fp.return_unknown_bytes
    fp.return_unknown_bytes = False
    assert not fp.return_unknown_bytes


def test_PARSE_FILE_WITH_FILTER(fp, json_db_path, decoders_test_resources):
    # Reset the with the database because a previous test assigns it to the nullptr
    fp = ne.FileParser(json_db_path)
    fp.filter = ne.Filter()
    fp.filter.logger.set_level(ne.LogLevel.DEBUG)

    test_gps_file = decoders_test_resources / "BESTUTMBIN.GPS"
    input_file_stream = ne.InputFileStream(str(test_gps_file))
    assert fp.set_stream(input_file_stream)

    success = 0
    expected_meta_data_length = [213, 195]
    expected_milliseconds = [270605000, 172189053]
    expected_message_length = [213, 195]

    status = STATUS.UNKNOWN
    fp.encode_format = ENCODEFORMAT.ASCII
    assert fp.encode_format == ENCODEFORMAT.ASCII

    while status != STATUS.STREAM_EMPTY:
        status, message_data, meta_data = fp.read()
        if status == STATUS.SUCCESS:
            assert meta_data.length == expected_meta_data_length[success]
            # FIXME: fp.read() segfaults after a few iterations if message_data is accessed
            # assert meta_data.milliseconds == approx(expected_milliseconds[success])
            assert len(message_data.message) == expected_message_length[success]
            success += 1
    assert fp.percent_read == 100
    assert success == 2


def test_RESET(fp):
    fp = ne.FileParser()
    assert len(fp.internal_buffer) > 0
    assert fp.reset()
