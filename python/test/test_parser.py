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
from novatel_edie import STATUS, ENCODE_FORMAT


@pytest.fixture(scope="function")
def parser():
    return ne.Parser()

@pytest.fixture(scope="module")
def test_gps_file(decoders_test_resources):
    return decoders_test_resources / "BESTUTMBIN.GPS"

def test_logger():
    # Parser logger
    level = ne.LogLevel.OFF
    parser = ne.Parser()
    logger = parser.logger
    logger.set_level(level)
    assert logger.name == "novatel_parser"
    assert logger.level == level
    # Parser logger
    parser.enable_framer_decoder_logging(level, "novatel_parser.log")


@pytest.mark.skip(reason="Slow and redundant")
def test_parser_instantiation(json_db, json_db_path):
    parser = ne.Parser()
    parser.load_json_db(json_db)
    ne.Parser(json_db_path)
    ne.Parser(json_db)


def test_range_cmp(parser):
    parser.decompress_range_cmp = True
    assert parser.decompress_range_cmp
    parser.decompress_range_cmp = False
    assert not parser.decompress_range_cmp


def test_unknown_bytes(parser):
    parser.return_unknown_bytes = True
    assert parser.return_unknown_bytes
    parser.return_unknown_bytes = False
    assert not parser.return_unknown_bytes


def test_parse_file_with_filter(parser, test_gps_file):
    parser.filter = ne.Filter()
    parser.filter.logger.set_level(ne.LogLevel.DEBUG)
    parser.encode_format = ENCODE_FORMAT.ASCII
    assert parser.encode_format == ENCODE_FORMAT.ASCII
    with test_gps_file.open("rb") as f:
        success = 0
        while chunk := f.read(32):
            parser.write(chunk)
            for status, message_data, meta_data in parser:
                print(status)
                if status == STATUS.SUCCESS:
                    assert meta_data.length == [213, 195][success]
                    assert meta_data.milliseconds == pytest.approx([270605000, 172189053][success])
                    assert len(message_data.message) == [213, 195][success]
                    success += 1
    assert parser.flush(return_flushed_bytes=True) == b""
    assert success == 2
