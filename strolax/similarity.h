#include <stdint.h>

double dice_distance(char *a, char *b, uint32_t n);
double jaccard_distance(char *a, char *b, uint32_t n);
int levenshtein_distance(char *a, char *b);
double ngram_distance(char *a, char *b, uint32_t n);