#include "../s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  while (*str1 != '\0') {
    const char *p = str2;
    while (*p != '\0') {
      if (*str1 == *p) return (char *)str1;
      p++;
    }
    str1++;
  }
  return S21_NULL;
}
