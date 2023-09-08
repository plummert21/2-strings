#include "../s21_string.h"

int s21_start_i(const char *, const char *, s21_size_t, s21_size_t);
int s21_end_i(const char *, const char *, s21_size_t, s21_size_t);

void *s21_trim(const char *src, const char *trim_chars) {
  char *trim_src = (char *)src;
  char *trim_chars_minor = (char *)trim_chars;
  char *arr;
  int error = 1;

  if (trim_chars_minor == S21_NULL) {
    trim_chars = " ";
    trim_chars_minor = (char *)trim_chars;
  }

  if (trim_src == S21_NULL) {
    error = 0;
  }

  if (error) {
    s21_size_t src_len = s21_strlen(src);
    s21_size_t trim_chars_len = s21_strlen(trim_chars);
    arr = (char *)malloc((src_len + 1) * sizeof(char));
    int arr_i = 0;
    int st_i = s21_start_i(trim_src, trim_chars_minor, src_len, trim_chars_len);
    int en_i = s21_end_i(trim_src, trim_chars_minor, src_len, trim_chars_len);
    for (int i = st_i; i < en_i; i++) {
      arr[arr_i] = src[i];

      arr_i++;
    }
    arr[arr_i] = '\0';
  }
  return error == 0 ? S21_NULL : (char *)arr;
}

int s21_start_i(const char *trim_src, const char *trim_chars_minor,
                s21_size_t src_len, s21_size_t trim_chars_len) {
  int start_i = 0;
  for (s21_size_t i = 0; i < src_len; i++) {
    int flag1 = 0;
    for (s21_size_t j = 0; j < trim_chars_len; j++) {
      if (trim_src[i] == trim_chars_minor[j]) {
        flag1 = 1;
        break;
      }
    }
    if (flag1 == 1) {
      start_i++;
    } else {
      break;
    }
  }
  return start_i;
}

int s21_end_i(const char *trim_src, const char *trim_chars_minor,
              s21_size_t src_len, s21_size_t trim_chars_len) {
  int end_i = src_len;
  for (int i = src_len - 1; i > -1; i--) {
    int flag2 = 0;
    for (s21_size_t j = 0; j < trim_chars_len; j++) {
      if (trim_src[i] == trim_chars_minor[j]) {
        flag2 = 1;
        break;
      }
    }
    if (flag2 == 1) {
      end_i--;
    } else {
      break;
    }
  }
  return end_i;
}