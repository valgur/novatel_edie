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
from novatel_edie import STATUS, FIELD_TYPE, DATA_TYPE
from pytest import approx

# -------------------------------------------------------------------------------------------------------
# Novatel Types Unit Tests
# -------------------------------------------------------------------------------------------------------

CHAR_MIN   = -128
CHAR_MAX   = 127
UCHAR_MAX  = 255
SHRT_MIN   = -32768
SHRT_MAX   = 32767
USHRT_MAX  = 65535
INT_MIN    = -2147483648
INT_MAX    = 2147483647
UINT_MAX   = 4294967295
LLONG_MIN  = -9223372036854775808
LLONG_MAX  = 9223372036854775807
ULLONG_MAX = 18446744073709551615


class Helper:
    def __init__(self, db):
        self.message_decoder = ne.MessageDecoder(db)
        self.encoder = ne.Encoder(db)
        self.msg_def_fields = []

    def create_base_field(self, name, field_type, conversion, length, data_type):
        field = ne.BaseField(name, field_type, conversion, length, data_type)
        self.msg_def_fields.append(field)

    def create_enum_field(self, name, value, description):
        enum_field = ne.EnumField(name, [ne.EnumDataType(name, value, description)])
        self.msg_def_fields.append(enum_field)

    def test_decode_ascii(self, msg_def_fields, data):
        return self.message_decoder._decode_ascii(msg_def_fields, data)

    def test_decode_binary(self, msg_def_fields, data):
        fields_size = sum(field.data_type.length for field in msg_def_fields)
        return self.message_decoder._decode_binary(msg_def_fields, data, fields_size)


@pytest.fixture(scope="function")
def helper(min_json_db):
    return Helper(min_json_db)


def test_get_default_database():
    db = ne.get_default_database()
    assert isinstance(db, ne.JsonReader)


def test_LOGGER():
    name = "message_decoder"
    level = ne.LogLevel.OFF
    logger = ne.MessageDecoder().logger
    logger.set_level(level)
    assert logger.name == name
    assert logger.level == level
    assert ne.Logging.get(name) is not None


def test_ASCII_CHAR_BYTE_VALID(helper):
    helper.create_base_field("INT_1", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)
    helper.create_base_field("INT_2", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)
    helper.create_base_field("INT_3", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)
    helper.create_base_field("INT_4", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)
    helper.create_base_field("INT_5", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)
    helper.create_base_field("INT_6", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)
    helper.create_base_field("INT_7", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)
    helper.create_base_field("INT_8", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)
    helper.create_base_field("INT_9", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)

    input = b"-129,-128,-127,-1,0,1,127,128,129"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.INT_1 == 127
    assert intermediate_format.INT_2 == -128
    assert intermediate_format.INT_3 == -127
    assert intermediate_format.INT_4 == -1
    assert intermediate_format.INT_5 == 0
    assert intermediate_format.INT_6 == 1
    assert intermediate_format.INT_7 == 127
    assert intermediate_format.INT_8 == -128
    assert intermediate_format.INT_9 == -127


def test_ASCI_UCHAR_BYTE_VALID(helper):
    helper.create_base_field("INT_1", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_2", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_3", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_4", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_5", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_6", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_7", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_8", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_9", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.UCHAR)

    input = b"-256,-255,-254,-1,0,1,254,255,256"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.INT_1 == 0
    assert intermediate_format.INT_2 == 1
    assert intermediate_format.INT_3 == 2
    assert intermediate_format.INT_4 == 255
    assert intermediate_format.INT_5 == 0
    assert intermediate_format.INT_6 == 1
    assert intermediate_format.INT_7 == 254
    assert intermediate_format.INT_8 == 255
    assert intermediate_format.INT_9 == 0


def test_ASCII_UCHAR_BYTE_INVALID(helper):
    helper.create_base_field("INT_1", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)
    input = b"0.1"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    assert intermediate_format.INT_1 == 0


def test_ASCII_CHAR_VALID(helper):
    helper.create_base_field("CHAR_1", FIELD_TYPE.SIMPLE, "%c", 1, DATA_TYPE.CHAR)
    helper.create_base_field("CHAR_2", FIELD_TYPE.SIMPLE, "%c", 1, DATA_TYPE.CHAR)
    helper.create_base_field("CHAR_3", FIELD_TYPE.SIMPLE, "%c", 1, DATA_TYPE.CHAR)

    input = b"#,A,;"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.CHAR_1 == ord('#')
    assert intermediate_format.CHAR_2 == ord('A')
    assert intermediate_format.CHAR_3 == ord(';')


def test_ASCII_CHAR_INVALID(helper):
    helper.create_base_field("CHAR", FIELD_TYPE.SIMPLE, "%c", 2, DATA_TYPE.CHAR)
    input = b""
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    assert status == STATUS.MALFORMED_INPUT


@pytest.mark.xfail(reason="%uc is broken")
def test_ASCII_UCHAR_VALID(helper):
    helper.create_base_field("uint8_1", FIELD_TYPE.SIMPLE, "%uc", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("uint8_2", FIELD_TYPE.SIMPLE, "%uc", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("uint8_3", FIELD_TYPE.SIMPLE, "%uc", 1, DATA_TYPE.UCHAR)

    input = b"#,A,;"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.uint8_1 == ord('#')
    assert intermediate_format.uint8_2 == ord('A')
    assert intermediate_format.uint8_3 == ord(';')


@pytest.mark.xfail(reason="%uc is broken")
def test_ASCII_UCHAR_INVALID(helper):
    helper.create_base_field("uint8_t", FIELD_TYPE.SIMPLE, "%uc", 2, DATA_TYPE.UCHAR)
    input = b""
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    assert status == STATUS.MALFORMED_INPUT


@pytest.mark.xfail
def test_ASCII_INT_VALID(helper):
    helper.create_base_field("INT_1", FIELD_TYPE.SIMPLE, "%d", 2, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_2", FIELD_TYPE.SIMPLE, "%d", 2, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_3", FIELD_TYPE.SIMPLE, "%d", 2, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_4", FIELD_TYPE.SIMPLE, "%d", 2, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_5", FIELD_TYPE.SIMPLE, "%d", 2, DATA_TYPE.UCHAR)
    helper.create_base_field("INT_6", FIELD_TYPE.SIMPLE, "%d", 2, DATA_TYPE.UCHAR)

    input = b"-32769,-32768,-32767,32767,32768,32769"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.INT_1 == 32767
    assert intermediate_format.INT_2 == -32768
    assert intermediate_format.INT_3 == -32767
    assert intermediate_format.INT_4 == 32767
    assert intermediate_format.INT_5 == -32768
    assert intermediate_format.INT_6 == -32767


@pytest.mark.xfail
def test_ASCII_INT_INVALID(helper):
    helper.create_base_field("INT_2", FIELD_TYPE.SIMPLE, "%d", 3, DATA_TYPE.UCHAR)
    input = b""
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    assert status == STATUS.MALFORMED_INPUT


def test_ASCII_FLOAT_VALID(helper):
    helper.create_base_field("Und", FIELD_TYPE.SIMPLE, "%e", 4, DATA_TYPE.FLOAT)
    helper.create_base_field("LatStd", FIELD_TYPE.SIMPLE, "%e", 4, DATA_TYPE.FLOAT)
    helper.create_base_field("LongStd", FIELD_TYPE.SIMPLE, "%e", 4, DATA_TYPE.FLOAT)

    input = b"51.11636937989,-114.03825348307,0"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.Und == approx(51.11636937989, rel=1e-6)
    assert intermediate_format.LatStd == approx(-114.03825348307, rel=1e-6)
    assert intermediate_format.LongStd == approx(0, rel=1e-6)


@pytest.mark.xfail
def test_ASCII_FLOAT_INVALID(helper):
    helper.create_base_field("Und", FIELD_TYPE.SIMPLE, "%e", 5, DATA_TYPE.FLOAT)
    input = b""
    with pytest.raises(RuntimeError) as excinfo:
        helper.test_decode_ascii(helper.msg_def_fields, input)
    assert excinfo.value.args[0] == "invalid float length"


def test_ASCII_DOUBLE_VALID(helper):
    helper.create_base_field("Lat", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("Long", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("Ht", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("longitude1", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("longitude2", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("longitude3", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("longitude4", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)

    input = b"51.11636937989,-114.03825348307,0,1.7e+308,-1.7e+308,1.7e+309,-1.7e+309"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    inf = float("inf")
    status.raise_on_error()

    assert intermediate_format.Lat == approx(51.11636937989, rel=1e-15)
    assert intermediate_format.Long == approx(-114.03825348307, rel=1e-15)
    assert intermediate_format.Ht == approx(0, rel=1e-15)
    assert intermediate_format.longitude1 == approx(1.7e+308, rel=1e-15)
    assert intermediate_format.longitude2 == approx(-1.7e+308, rel=1e-15)
    assert intermediate_format.longitude3 == approx(inf, rel=1e-15)
    assert intermediate_format.longitude4 == approx(-inf, rel=1e-15)


@pytest.mark.xfail
def test_ASCII_DOUBLE_INVALID(helper):
    helper.create_base_field("Lat", FIELD_TYPE.SIMPLE, "%lf", 9, DATA_TYPE.DOUBLE)
    input = b""
    with pytest.raises(RuntimeError) as excinfo:
        helper.test_decode_ascii(helper.msg_def_fields, input)
    assert excinfo.value.args[0] == "invalid float length"


def test_ASCII_BOOL_VALID(helper):
    helper.create_base_field("B_True", FIELD_TYPE.SIMPLE, "%d", 4, DATA_TYPE.BOOL)
    helper.create_base_field("B_False", FIELD_TYPE.SIMPLE, "%d", 4, DATA_TYPE.BOOL)

    input = b"TRUE,FALSE"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert bool(intermediate_format.B_True)
    assert not bool(intermediate_format.B_False)


@pytest.mark.xfail
def test_ASCII_BOOL_INVALID(helper):
    helper.create_base_field("B_True", FIELD_TYPE.SIMPLE, "%d", 4, DATA_TYPE.BOOL)
    input = b"True"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    assert status == STATUS.MALFORMED_INPUT


def test_ASCII_UINT_VALID(helper):
    helper.create_base_field("toe1", FIELD_TYPE.SIMPLE, "%u", 4, DATA_TYPE.UINT)
    helper.create_base_field("toe2", FIELD_TYPE.SIMPLE, "%u", 4, DATA_TYPE.UINT)
    helper.create_base_field("toe3", FIELD_TYPE.SIMPLE, "%u", 4, DATA_TYPE.UINT)
    helper.create_base_field("toe4", FIELD_TYPE.SIMPLE, "%u", 4, DATA_TYPE.UINT)

    input = b"-1,0,4294967294,4294967295"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.toe1 == 4294967295
    assert intermediate_format.toe2 == 0
    assert intermediate_format.toe3 == 4294967294
    assert intermediate_format.toe4 == 4294967295


def test_ASCII_GPSTIME_MSEC_VALID(helper):
    helper.create_base_field("Sec1", FIELD_TYPE.SIMPLE, "%T", 4, DATA_TYPE.DOUBLE)
    helper.create_base_field("Sec2", FIELD_TYPE.SIMPLE, "%T", 4, DATA_TYPE.DOUBLE)
    helper.create_base_field("Sec3", FIELD_TYPE.SIMPLE, "%T", 4, DATA_TYPE.DOUBLE)
    helper.create_base_field("Sec4", FIELD_TYPE.SIMPLE, "%T", 4, DATA_TYPE.DOUBLE)

    input = b"-1.000,0.000,604800.000,4294967295.000"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    # If GPS time exceeds 4,294,967.295 (seconds) the conversion to milliseconds is wrong
    # But the limit should be 604,800 (seconds) as that's the number of seconds in a GPS reference week
    assert intermediate_format.Sec1 == 4294966296  # 4,294,967,295 + 1 - 1,000 = 4,294,966,296
    assert intermediate_format.Sec2 == 0
    assert intermediate_format.Sec3 == 604800000
    assert intermediate_format.Sec4 == 4294966296


def test_ASCII_SCIENTIFIC_NOTATION_FLOAT_VALID(helper):
    helper.create_base_field("Sec1", FIELD_TYPE.SIMPLE, "%e", 4, DATA_TYPE.FLOAT)
    helper.create_base_field("Sec2", FIELD_TYPE.SIMPLE, "%e", 4, DATA_TYPE.FLOAT)
    helper.create_base_field("Sec3", FIELD_TYPE.SIMPLE, "%e", 4, DATA_TYPE.FLOAT)
    helper.create_base_field("Sec4", FIELD_TYPE.SIMPLE, "%g", 5, DATA_TYPE.FLOAT)

    input = b"-1.0,0.0,1.175494351e-38,3.402823466e+38"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.Sec1 == approx(-1, rel=1e-6)
    assert intermediate_format.Sec2 == approx(0, rel=1e-6)
    assert intermediate_format.Sec3 == approx(1.175494351e-38, rel=1e-6)
    assert intermediate_format.Sec4 == approx(3.402823466e+38, rel=1e-6)


def test_ASCII_SCIENTIFIC_NOTATION_DOUBLE_VALID(helper):
    helper.create_base_field("Sec1", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("Sec2", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("Sec3", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("Sec4", FIELD_TYPE.SIMPLE, "%lg", 8, DATA_TYPE.DOUBLE)

    input = b"-1.0,0.0,2.2250738585072014e-308,1.7976931348623158e+308"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.Sec1 == approx(-1, rel=1e-15)
    assert intermediate_format.Sec2 == approx(0, rel=1e-15)
    assert intermediate_format.Sec3 == approx(2.2250738585072014e-308, rel=1e-15)
    assert intermediate_format.Sec4 == approx(1.7976931348623158e+308, rel=1e-15)


def test_ASCII_ULONG_VALID(helper):
    helper.create_base_field("rx_chars1", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars2", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars3", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars4", FIELD_TYPE.SIMPLE, "%u", 2, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars5", FIELD_TYPE.SIMPLE, "%u", 2, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars6", FIELD_TYPE.SIMPLE, "%u", 2, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars7", FIELD_TYPE.SIMPLE, "%lu", 4, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars8", FIELD_TYPE.SIMPLE, "%lu", 4, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars9", FIELD_TYPE.SIMPLE, "%lu", 4, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars10", FIELD_TYPE.SIMPLE, "%llu", 8, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars11", FIELD_TYPE.SIMPLE, "%llu", 8, DATA_TYPE.ULONG)
    helper.create_base_field("rx_chars12", FIELD_TYPE.SIMPLE, "%llu", 8, DATA_TYPE.ULONG)

    input = b"-1,0,255,-1,0,65535,-1,0,4294967295,-1,0,18446744073709551615"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.rx_chars1 == 255
    assert intermediate_format.rx_chars2 == 0
    assert intermediate_format.rx_chars3 == 255
    assert intermediate_format.rx_chars4 == 65535
    assert intermediate_format.rx_chars5 == 0
    assert intermediate_format.rx_chars6 == 65535
    assert intermediate_format.rx_chars7 == 4294967295
    assert intermediate_format.rx_chars8 == 0
    assert intermediate_format.rx_chars9 == 4294967295
    assert intermediate_format.rx_chars10 == ULLONG_MAX
    assert intermediate_format.rx_chars11 == 0
    assert intermediate_format.rx_chars12 == ULLONG_MAX


@pytest.mark.xfail(reason="Need to add enum definitions to the database as well")
def test_ASCII_ENUM_VALID(helper):
    helper.create_enum_field("UNKNOWN", 20, "Unknown or unspecified type")
    helper.create_enum_field("APPROXIMATE", 60, "Approximate time")
    helper.create_enum_field("SATTIME", 200, "")

    input = b"UNKNOWN,APPROXIMATE,SATTIME"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.UNKNOWN == 20
    assert intermediate_format.APPROXIMATE == 60
    assert intermediate_format.SATTIME == 200


def test_ASCII_STRING_VALID(helper):
    helper.create_base_field("MESSAGE", FIELD_TYPE.STRING, "%", 1, DATA_TYPE.UNKNOWN)

    input = b"#RAWEPHEMA,COM1,100"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.MESSAGE == "#RAWEPHEMA"


def test_ASCII_EMPTY_STRING_VALID(helper):
    helper.create_base_field("MESSAGE", FIELD_TYPE.STRING, "%", 1, DATA_TYPE.UNKNOWN)

    input = b"\"\""
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.MESSAGE == ""


def test_ASCII_TYPE_INVALID(helper):
    helper.create_base_field("", FIELD_TYPE.UNKNOWN, "%", 1, DATA_TYPE.UNKNOWN)
    input = b"123"
    with pytest.raises(RuntimeError) as excinfo:
        helper.test_decode_ascii(helper.msg_def_fields, input)
    assert excinfo.value.args[0] == "DecodeAscii(): Unknown field type"


def test_BINARY_BOOL_VALID(helper):
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.BOOL)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.BOOL)

    input = bytes([1, 0])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert bool(intermediate_format.field1)
    assert not bool(intermediate_format.field2)


def test_BINARY_HEXBYTE_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.HEXBYTE)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.HEXBYTE)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.HEXBYTE)

    input = bytes([0x00, 0x01, 0xFF])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.field0 == 0
    assert intermediate_format.field1 == 1
    assert intermediate_format.field2 == UCHAR_MAX


def test_BINARY_uint8_t_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("field3", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("field4", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.UCHAR)

    input = bytes([0x23, 0x41, 0x3B, 0x00, 0xFF])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.field0 == ord('#')
    assert intermediate_format.field1 == ord('A')
    assert intermediate_format.field2 == ord(';')
    assert intermediate_format.field3 == 0
    assert intermediate_format.field4 == UCHAR_MAX


def test_BINARY_USHORT_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%", 2, DATA_TYPE.USHORT)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 2, DATA_TYPE.USHORT)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%", 2, DATA_TYPE.USHORT)
    helper.create_base_field("field3", FIELD_TYPE.SIMPLE, "%", 2, DATA_TYPE.USHORT)

    input = bytes([0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0xFF, 0xFF])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.field0 == 0
    assert intermediate_format.field1 == 1
    assert intermediate_format.field2 == 16
    assert intermediate_format.field3 == USHRT_MAX


def test_BINARY_SHORT_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%", 2, DATA_TYPE.SHORT)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 2, DATA_TYPE.SHORT)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%", 2, DATA_TYPE.SHORT)
    helper.create_base_field("field3", FIELD_TYPE.SIMPLE, "%", 2, DATA_TYPE.SHORT)

    input = bytes([0x00, 0x80, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0x7F])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.field0 == SHRT_MIN
    assert intermediate_format.field1 == -1
    assert intermediate_format.field2 == -0
    assert intermediate_format.field3 == SHRT_MAX


def test_BINARY_INT_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.INT)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.INT)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.INT)
    helper.create_base_field("field3", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.INT)

    input = bytes([
        0x00, 0x00, 0x00, 0x80,
        0x00, 0x00, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0x7F
    ])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.field0 == INT_MIN
    assert intermediate_format.field1 == -65536
    assert intermediate_format.field2 == 0
    assert intermediate_format.field3 == INT_MAX


def test_BINARY_UINT_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.UINT)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.UINT)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.UINT)
    helper.create_base_field("field3", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.UINT)

    input = bytes([
        0x00, 0x00, 0x00, 0x80,
        0xFF, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF
    ])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.field0 == 2147483648
    assert intermediate_format.field1 == 65535
    assert intermediate_format.field2 == 0
    assert intermediate_format.field3 == UINT_MAX


def test_BINARY_ULONG_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.ULONG)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.ULONG)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.ULONG)
    helper.create_base_field("field3", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.ULONG)

    input = bytes([
        0x00, 0x00, 0x00, 0x80,
        0xFF, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF
    ])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.field0 == 2147483648
    assert intermediate_format.field1 == 65535
    assert intermediate_format.field2 == 0
    assert intermediate_format.field3 == UINT_MAX


def test_BINARY_CHAR_BYTE_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.CHAR)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.CHAR)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.CHAR)
    helper.create_base_field("field3", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.CHAR)

    input = bytes([0x80, 0xFF, 0x00, 0x7F])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.field0 == CHAR_MIN
    assert intermediate_format.field1 == -1
    assert intermediate_format.field2 == 0
    assert intermediate_format.field3 == CHAR_MAX


def test_BINARY_FLOAT_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.FLOAT)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 4, DATA_TYPE.FLOAT)

    input = bytes([0x9A, 0x99, 0x99, 0x3F, 0xCD, 0xCC, 0xBC, 0xC0])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.field0 == approx(1.2, rel=1e-6)
    assert intermediate_format.field1 == approx(-5.9, rel=1e-6)


def test_BINARY_DOUBLE_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%", 8, DATA_TYPE.DOUBLE)
    helper.create_base_field("field3", FIELD_TYPE.SIMPLE, "%", 8, DATA_TYPE.DOUBLE)

    input = bytes([
        0x12, 0x71, 0x1C, 0x31, 0xE5, 0x8E, 0x49, 0x40,
        0x99, 0xAF, 0xBC, 0xBE, 0x72, 0x82, 0x5C, 0xC0,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x7F
    ])
    status, intermediate_format = helper.test_decode_binary(helper.msg_def_fields, input)
    status.raise_on_error()

    assert intermediate_format.field0 == approx(51.11636937989, rel=1e-15)
    assert intermediate_format.field1 == approx(-114.03825348307, rel=1e-15)
    assert intermediate_format.field2 == approx(0, rel=1e-15)
    assert intermediate_format.field3 == approx(float("inf"), rel=1e-15)


def test_BINARY_SIMPLE_TYPE_INVALID(helper):
    helper.create_base_field("", FIELD_TYPE.SIMPLE, "%", 1, DATA_TYPE.UNKNOWN)
    input = None
    with pytest.raises(Exception):
        helper.test_decode_binary(helper.msg_def_fields, input)


def test_BINARY_TYPE_INVALID(helper):
    helper.create_base_field("", FIELD_TYPE.UNKNOWN, "%", 1, DATA_TYPE.UNKNOWN)
    input = None
    with pytest.raises(Exception):
        helper.test_decode_binary(helper.msg_def_fields, input)


def test_SIMPLE_FIELD_WIDTH_VALID(helper):
    helper.create_base_field("field0", FIELD_TYPE.SIMPLE, "%d", 4, DATA_TYPE.BOOL)
    helper.create_base_field("field1", FIELD_TYPE.SIMPLE, "%XB", 1, DATA_TYPE.HEXBYTE)
    helper.create_base_field("field2", FIELD_TYPE.SIMPLE, "%UB", 1, DATA_TYPE.UCHAR)
    helper.create_base_field("field3", FIELD_TYPE.SIMPLE, "%B", 1, DATA_TYPE.CHAR)
    helper.create_base_field("field4", FIELD_TYPE.SIMPLE, "%hu", 2, DATA_TYPE.USHORT)
    helper.create_base_field("field5", FIELD_TYPE.SIMPLE, "%hd", 2, DATA_TYPE.SHORT)
    helper.create_base_field("field6", FIELD_TYPE.SIMPLE, "%u", 4, DATA_TYPE.UINT)
    helper.create_base_field("field7", FIELD_TYPE.SIMPLE, "%lu", 4, DATA_TYPE.ULONG)
    helper.create_base_field("field8", FIELD_TYPE.SIMPLE, "%d", 4, DATA_TYPE.INT)
    helper.create_base_field("field9", FIELD_TYPE.SIMPLE, "%ld", 4, DATA_TYPE.LONG)
    helper.create_base_field("field10", FIELD_TYPE.SIMPLE, "%llu", 8, DATA_TYPE.ULONGLONG)
    helper.create_base_field("field11", FIELD_TYPE.SIMPLE, "%lld", 8, DATA_TYPE.LONGLONG)
    helper.create_base_field("field12", FIELD_TYPE.SIMPLE, "%e", 4, DATA_TYPE.FLOAT)
    helper.create_base_field("field13", FIELD_TYPE.SIMPLE, "%lf", 8, DATA_TYPE.DOUBLE)

    input = b"TRUE,0x63,227,56,2734,-3842,38283,54244,-4359,5293,79338432,-289834,2.54,5.44061788e+03"
    status, intermediate_format = helper.test_decode_ascii(helper.msg_def_fields, input)
    status.raise_on_error()
    
    assert intermediate_format.field0 is True  # bool
    assert intermediate_format.field1 == 99  # uint8_t
    assert intermediate_format.field2 == 227  # uint8_t
    assert intermediate_format.field3 == 56  # int8_t
    assert intermediate_format.field4 == 2734  # uint16_t
    assert intermediate_format.field5 == -3842  # int16_t
    assert intermediate_format.field6 == 38283  # uint32_t
    assert intermediate_format.field7 == 54244  # uint32_t
    assert intermediate_format.field8 == -4359  # int32_t
    assert intermediate_format.field9 == 5293  # int32_t
    assert intermediate_format.field10 == 79338432  # uint64_t
    assert intermediate_format.field11 == -289834  # int64_t
    assert intermediate_format.field12 == approx(2.54, abs=0.001)  # float
    assert intermediate_format.field13 == approx(5.44061788e+03, abs=0.000001)  # double
