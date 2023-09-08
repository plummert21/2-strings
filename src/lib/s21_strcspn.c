#include "../s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  const unsigned char *s = (const unsigned char *)str1;
  const unsigned char *symb = (const unsigned char *)str2;

  s21_size_t res = 0;

  while (s[res]) {
    for (size_t j = 0; symb[j]; j++) {
      if (s[res] == symb[j]) {
        return res;
      }
    }
    res++;
  }

  return res;
}