# cython: boundscheck=False, wraparound=False, nonecheck=False, language_level=3str, embedsignature=False

cdef extern from "Python.h":
        cdef enum:
            PyUnicode_1BYTE_KIND = 1
            PyUnicode_2BYTE_KIND = 2
            PyUnicode_4BYTE_KIND = 4
        cdef int PyUnicode_KIND(object o)
        object PyUnicode_New(Py_ssize_t size, Py_UCS4 maxchar)
        void PyUnicode_WriteChar(object u, Py_ssize_t index, Py_UCS4 value)
        object PyUnicode_FromKindAndData(int kind, const void *buffer, Py_ssize_t size)
