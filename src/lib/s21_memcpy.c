#include "../s21_string.h"

void *s21_memcpy(void *str1, const void *str2, s21_size_t n) {
  size_t i = n;
  while (n) {
    *(char *)((char *)str1 + (i - n)) = *(char *)((char *)str2 + (i - n));
    n--;
  }
  return str1;
}
