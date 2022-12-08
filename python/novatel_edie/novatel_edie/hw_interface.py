from ctypes import *

from ._util import _load_shared_library

HWINTERFACE_DLL = _load_shared_library('hwinterface_dynamic_library')

HWINTERFACE_DLL.ifs_init.restype = c_void_p
HWINTERFACE_DLL.ifs_init.argtypes = [c_char_p]
HWINTERFACE_DLL.is_read.restype = None
HWINTERFACE_DLL.is_read.argtypes = [c_void_p, c_void_p, c_char_p, POINTER(c_uint)]
HWINTERFACE_DLL.is_del.restype = None
HWINTERFACE_DLL.is_del.argtypes = [c_void_p]

c_read_callback = CFUNCTYPE(c_uint, py_object, c_void_p, c_uint)
HWINTERFACE_DLL.is_wrapper.restype = c_void_p
HWINTERFACE_DLL.is_wrapper.argtypes = [py_object, c_read_callback]


class StreamReadStatus(Structure):
    # Hint available types
    stream_percentage_read: c_uint
    stream_current_read: c_uint
    stream_length: c_ulonglong
    stream_end: c_bool
    """StreamReadStatus is the equivalent to the StreamReadStatus in the dll.

    The __init__ with allocate a pointer to a StreamReadStatus object. The __del__ will delete the memory of this
    object. Currently fields in this object are read only.
    """
    _fields_ = [("stream_percentage_read", c_uint),
                ("stream_current_read", c_uint),
                ("stream_length", c_ulonglong),
                ("stream_end", c_bool)]

    def __str__(self):
        return "{} {} {} {}".format(
            self.stream_percentage_read,
            self.stream_current_read,
            self.stream_length,
            self.stream_end,
        )


class InputBuffer:
    """Buffer to hold the data coming from the InputFileStream class"""

    def __init__(self, buf_size=1024):
        self.buf_size = buf_size
        self.buf = create_string_buffer(self.buf_size + 1)


class InputFileStream:
    """InputFileStream is the equivalent to the InputFileStream in the dll. The __init__ with allocate a pointer to
    a InputFileStream object. The __del__ will delete the memory of this object. Currently fields in this object are
    read only.

    Args:
        file_to_read (str): path to file to read.
        data_buf (:obj:`InputBuffer`, optional): InputBuffer object that read data will get copied too.

    """

    def __init__(self, file_to_read, data_buf=None):
        self.data_stream = HWINTERFACE_DLL.ifs_init(file_to_read.encode())
        if data_buf:
            self.data_buf = data_buf
        else:
            self.data_buf = InputBuffer()

    def __del__(self):
        self.close()

    def close(self):
        if self.data_stream:
            HWINTERFACE_DLL.is_del(self.data_stream)
            self.data_stream = None

    def read(self, bytes_to_read=1024):
        """Read data from the input steam

        Args:
            bytes_to_read: Number of bytes to read. If larger than the buffer size it will read up to the max size of
            the buffer.

        Returns:
            (:obj:`tuple`) of (:meth:`StreamReadStatus`, data).

        """
        if bytes_to_read > self.data_buf.buf_size:
            bytes_to_read = self.data_buf.buf_size
        status = StreamReadStatus()
        HWINTERFACE_DLL.is_read(
            self.data_stream,
            byref(status),
            cast(self.data_buf.buf, c_char_p),
            byref(c_uint(bytes_to_read)),
        )
        return status, self.data_buf.buf.value


class InputByteStream:
    """
    Args:
        byte_stream (io.BytesIO): file-like byte stream object to read from.
    """

    def __init__(self, byte_stream):
        self._byte_stream = byte_stream
        self.data_stream = HWINTERFACE_DLL.is_wrapper(self._byte_stream, self._read_callback)

    @staticmethod
    @c_read_callback
    def _read_callback(byte_stream, buf_ptr, buf_size):
        buf = (c_char * buf_size).from_address(buf_ptr)
        return byte_stream.readinto(buf)

    def __del__(self):
        self.close()

    def close(self):
        if self.data_stream:
            HWINTERFACE_DLL.is_del(self.data_stream)
            self.data_stream = None
        self._byte_stream.close()
