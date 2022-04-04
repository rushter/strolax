"""A set of utilities for text cleaning."""
from libc.stdint cimport  uint32_t
from libc.stdlib cimport malloc, free

cpdef str remove_emojis(text: str):
    """Removes emojis from the specified text.

    Parameters
    ----------
    
    text : str
        The text to replace emojis in. 
    
    Returns
    -------
    
    text : str    
    """
    cdef int kind = PyUnicode_KIND(text)
    if kind < 4:
        # Internally, Python uses tree sizes for unicode string: 1,2 and 4 bytes.
        # Emoji codepoints can only fit in 4-byte strings.
        return text

    cdef Py_UCS4 character
    cdef uint32_t pos = 0
    cdef Py_UCS4 * c_buffer = <Py_UCS4 *> malloc(len(text) * sizeof(Py_UCS4))
    cdef str output

    for character in text:
        # https://unicode.org/emoji/charts/full-emoji-list.html
        if 0x1F600 <= character <= 0xE007F:
            continue
        c_buffer[pos] = character
        pos += 1

    output = PyUnicode_FromKindAndData(PyUnicode_4BYTE_KIND, c_buffer, pos - 1) if pos else ""
    free(c_buffer)
    return output
