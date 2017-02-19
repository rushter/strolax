#cython: boundscheck=False, wraparound=False, nonecheck=False
from libc.stdint cimport uint32_t

cdef extern from "similarity.h":
    double dice_distance(char *a, char *b, uint32_t n)
    double jaccard_distance(char *a, char *b, uint32_t n)
    int levenshtein_distance(char *a, char *b)
    double ngram_distance(char *a, char *b, uint32_t n)


cpdef double dice(basestring a, basestring b, uint32_t n):
    a_pybyte = a.encode('UTF-8')
    cdef char* a_c_string = a_pybyte

    b_pybyte = b.encode('UTF-8')
    cdef char* b_c_string = b_pybyte

    return dice_distance(a_c_string, b_c_string, n)


cpdef double jaccard(basestring a, basestring b, uint32_t n):
    a_pybyte = a.encode('UTF-8')
    cdef char* a_c_string = a_pybyte

    b_pybyte = b.encode('UTF-8')
    cdef char* b_c_string = b_pybyte

    return jaccard_distance(a_c_string, b_c_string, n)


cpdef int levenshtein(basestring a, basestring b):
    a_pybyte = a.encode('UTF-8')
    cdef char* a_c_string = a_pybyte

    b_pybyte = b.encode('UTF-8')
    cdef char* b_c_string = b_pybyte

    return levenshtein_distance(a_c_string, b_c_string)

cpdef double ngram(basestring a, basestring b, uint32_t n):
    a_pybyte = a.encode('UTF-8')
    cdef char* a_c_string = a_pybyte

    b_pybyte = b.encode('UTF-8')
    cdef char* b_c_string = b_pybyte

    return ngram_distance(a_c_string, b_c_string, n)