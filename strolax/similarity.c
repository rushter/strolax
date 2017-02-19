#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "murmurhash.h"

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))
#define MAX(a, b) ((a) > (b) ? a : b)
#define MURMURHASH_SEED 0112

int cmpfunc(const void *a, const void *b) {
  return (*(int *) a - *(int *) b);
}


int levenshtein_distance(char *a, char *b) {

  size_t x, y, a_len, b_len;
  a_len = strlen(a);
  b_len = strlen(b);

  unsigned int matrix[b_len + 1][a_len + 1];
  matrix[0][0] = 0;

  for (x = 1; x <= b_len; x++)
    matrix[x][0] = matrix[x - 1][0] + 1;

  for (y = 1; y <= a_len; y++)
    matrix[0][y] = matrix[0][y - 1] + 1;

  for (x = 1; x <= b_len; x++)
    for (y = 1; y <= a_len; y++)
      matrix[x][y] =
          MIN3(matrix[x - 1][y] + 1, matrix[x][y - 1] + 1, matrix[x - 1][y - 1] + (a[y - 1] == b[x - 1] ? 0 : 1));

  return (matrix[b_len][a_len]);
}


struct Ngram {
  size_t length;
  uint32_t *array;

};

void dedup(struct Ngram *ngram) {
  size_t j = 0;
  for (size_t i = 1; i < ngram->length; i++) {
    if (ngram->array[i] != ngram->array[j]) {
      ngram->array[++j] = ngram->array[i];
    }
  }
  ngram->length = (j + 1);

}


void find_ngrams(struct Ngram *ngram, char *string, uint32_t n, bool sort_n_dedup) {

  size_t string_len = strlen(string);
  size_t length = MAX(0, string_len - n + 1);

  ngram->length = length;
  ngram->array = calloc(length, sizeof(uint32_t));

  for (size_t i = 0; i < length; i++) {
    uint32_t hash = murmurhash(&string[i], n, MURMURHASH_SEED);
    ngram->array[i] = hash;
  }
  if (sort_n_dedup) {
    qsort(ngram->array, ngram->length, sizeof(uint32_t), cmpfunc);
    dedup(ngram);
  }
}


void set_intersection(struct Ngram *a, struct Ngram *b, struct Ngram *c, bool store_values) {
  size_t i, j;
  i = j = 0;

  c->length = 0;
  if (store_values) {
    c->array = calloc(MAX(a->length, b->length), sizeof(uint32_t));
  }

  while (i < a->length && j < b->length) {
    if (a->array[i] == b->array[j]) {
      if (store_values) {
        c->array[c->length] = a->array[i];
      }

      c->length++;

      i++;
      j++;
    }
    else if (a->array[i] < b->array[j]) i++;
    else j++;
  }

}


void set_union(struct Ngram *a, struct Ngram *b, struct Ngram *c, bool store_values) {
  size_t i, j;
  i = j = 0;

  c->length = 0;
  if (store_values) {
    c->array = calloc(a->length + b->length, sizeof(uint32_t));
  }

  while (i < a->length && j < b->length) {
    if (a->array[i] < b->array[j]) {
      i++;
      if (store_values) {
        c->array[c->length] = a->array[i];
      }

      c->length++;
    }
    else if (b->array[j] < a->array[i]) {
      j++;
      if (store_values) {
        c->array[c->length] = b->array[j];
      }

      c->length++;
    }
    else {
      j++;
      if (store_values) {
        c->array[c->length] = b->array[j];
      }

      c->length++;
      i++;
    }
  }

  while (i < a->length) {
    i++;
    if (store_values) {
      c->array[c->length] = a->array[i];
    }

    c->length++;

  }

  while (j < b->length) {
    j++;
    if (store_values) {
      c->array[c->length] = a->array[j];
    }

    c->length++;
  }

}


double jaccard_distance(char *a, char *b, uint32_t n) {

  struct Ngram a_ngrams, b_ngrams, intersection, sunion;

  find_ngrams(&a_ngrams, a, n, true);
  find_ngrams(&b_ngrams, b, n, true);

  set_intersection(&a_ngrams, &b_ngrams, &intersection, false);
  set_union(&a_ngrams, &b_ngrams, &sunion, false);
  if (sunion.length == 0) {
    return 0.0;
  }
  free(a_ngrams.array);
  free(b_ngrams.array);
  return ((double) intersection.length) / ((double) sunion.length);

}


double ngram_distance(char *a, char *b, uint32_t n) {
  struct Ngram a_ngrams, b_ngrams, intersection, sunion;
  double dist = 1.0;

  find_ngrams(&a_ngrams, a, n, true);
  find_ngrams(&b_ngrams, b, n, true);

  set_intersection(&a_ngrams, &b_ngrams, &intersection, false);
  set_union(&a_ngrams, &b_ngrams, &sunion, false);

  if (intersection.length > 0) {
    dist = 1.0 - ((double) intersection.length / (double) sunion.length);
  }
  free(a_ngrams.array);
  free(b_ngrams.array);
  return dist;
}


double dice_distance(char *a, char *b, uint32_t n) {
  struct Ngram a_ngrams, b_ngrams, intersection;

  find_ngrams(&a_ngrams, a, n, true);
  find_ngrams(&b_ngrams, b, n, true);

  set_intersection(&a_ngrams, &b_ngrams, &intersection, false);
  double denominator = (double) a_ngrams.length + b_ngrams.length;
  if (denominator == 0.0)
    denominator = 1.0;
  double distance = 2.0 * (double) intersection.length / denominator;
  free(a_ngrams.array);
  free(b_ngrams.array);

  return distance;
}

