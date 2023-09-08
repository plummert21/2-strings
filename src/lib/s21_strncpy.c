#include "../s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *start = dest;
  s21_size_t i = 0;
  while (i < n && src[i]) {
    dest[i] = src[i];
    i++;
  }
  while (i < n) dest[i++] = '\0';
  return start;
}