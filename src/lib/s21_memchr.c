#include "../s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  void *result = S21_NULL;
  const unsigned char *s = (const unsigned char *)str;
  const unsigned char ch = (const unsigned int)c;

  for (s21_size_t i = 0; i < n; i++) {
    if (s[i] == ch) {
      result = (void *)(s + i);
      break;
    }
  }

  return result;
}