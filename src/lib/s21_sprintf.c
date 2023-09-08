#include "../s21_string.h"

// Helpers
int s21_isdigit(char c);
int s21_atoi(const char *str);

// Convert
void num_to_string(int64_t value, char *ret, int base);
void unsigned_num_to_string(uint64_t value, char *ret, int base);
bool is_integer_specifier(char c);
bool is_all_zeroes(char *buff);
void prepend_ox_specs(char *buff, flags f);
void double_to_string(long double value, char *ret, flags f);
void prepend_exp_symbols(char *str, int pow, char sign);
void remove_trail_zeroes(char *buff);

// Set some flags
const char *set_flags(const char *format, flags *f);
const char *set_width(const char *format, flags *f, va_list va);
const char *set_accuracy(const char *format, flags *f, va_list va);
const char *set_length(const char *format, flags *f);

// Parse some specifieres
void parse_specs(flags f, char *buff, va_list va);
void parse_integer(flags, char *buff, va_list va);
void parse_unsigned(flags f, char *buff, va_list va);
void parse_octal(flags f, char *buff, va_list va);
void parse_hexadecimal(flags f, char *buff, va_list va);
void parse_character(flags f, char *buff, va_list va);
void parse_string(flags f, char *buff, va_list va);
void parse_pointer(flags f, char *buff, va_list va);
void parse_float(flags f, char *buff, va_list va);
void parse_exp_spec(flags f, char *buff, va_list va);
void parse_general_float(flags f, char *buff, va_list va);

// Format witch flags (space, plus, zero)
void format_accuracy(char *buff, flags f);
void format_flags(char *buff, flags f);
void format_general_accuracy(char *buff, int accuracy);
void format_wchar(flags f, char *buff, wchar_t w_c);
void format_char(flags f, char *buff, char c);
void format_string(flags f, char *buff, char *str);
void format_wide_string(flags f, char *buff, wchar_t *wstr);

char *s21_strcat(char *dest, const char *src);
char *s21_strcpy(char *dest, const char *src);
void *s21_memmove(void *dst, const void *src, s21_size_t len);
void to_upper(char *str);

int s21_sprintf(char *str, const char *format, ...) {
  flags flag = {0};
  va_list arg;
  va_start(arg, format);
  char *str_begin = str;
  while (*format) {
    if (*format != '%') {
      *str++ = *format++;
      continue;
    } else {
      format++;
      s21_memset(&flag, 0, sizeof(flags));
    }

    format = set_flags(format, &flag);
    format = set_width(format, &flag, arg);
    format = set_accuracy(format, &flag, arg);
    format = set_length(format, &flag);

    flag.specifier = *format;
    format++;

    char buff[SIZE] = {'\0'};
    parse_specs(flag, buff, arg);
    for (int i = 0; buff[i]; i++, str++) *str = buff[i];

    if (flag.specifier == 'n') {
      int *ret = va_arg(arg, int *);
      *ret = str - str_begin;
    }
  }

  *str = '\0';
  va_end(arg);
  return str - str_begin;
}

const char *set_flags(const char *format, flags *flag) {
  while (*format == '-' || *format == '+' || *format == ' ' || *format == '0' ||
         *format == '#') {
    switch (*format) {
      case '0':
        flag->zero = true;
        break;
      case '-':
        flag->minus = true;
        break;
      case '+':
        flag->plus = true;
        break;
      case ' ':
        flag->space = true;
        break;
      case '#':
        flag->hash = true;
        break;
    }
    format++;
  }
  return format;
}

const char *set_width(const char *format, flags *flag, va_list arg) {
  if (*format == '*') {
    format++;
    flag->width = va_arg(arg, int);
  } else if (s21_isdigit(*format)) {
    char tmp[SIZE] = {'\0'};
    for (int i = 0; s21_isdigit(*format); i++, format++) tmp[i] = *format;
    flag->width = s21_atoi(tmp);
  }
  return format;
}

const char *set_accuracy(const char *format, flags *flag, va_list arg) {
  if (*format == '.') {
    flag->is_accuracy_set = true;
    format++;
  }

  if (*format == '*') {
    format++;
    flag->accuracy = va_arg(arg, int);
  }

  if (s21_isdigit(*format)) {
    char tmp[SIZE] = {'\0'};
    for (int i = 0; s21_isdigit(*format); i++, format++) tmp[i] = *format;
    flag->accuracy = s21_atoi(tmp);
  }

  return format;
}

const char *set_length(const char *format, flags *flag) {
  switch (*format) {
    case 'h':
      flag->length = 'h';
      format++;
      break;
    case 'l':
      flag->length = 'l';
      format++;
      break;
    case 'L':
      flag->length = 'L';
      format++;
  }
  return format;
}

void parse_specs(flags flag, char *buff, va_list arg) {
  if (flag.specifier == 'd' || flag.specifier == 'i')
    parse_integer(flag, buff, arg);
  else if (flag.specifier == 'u')
    parse_unsigned(flag, buff, arg);
  else if (flag.specifier == 'o')
    parse_octal(flag, buff, arg);
  else if (flag.specifier == 'x' || flag.specifier == 'X')
    parse_hexadecimal(flag, buff, arg);
  else if (flag.specifier == '%')
    buff[0] = '%';
  else if (flag.specifier == 'c')
    parse_character(flag, buff, arg);
  else if (flag.specifier == 's')
    parse_string(flag, buff, arg);
  else if (flag.specifier == 'p')
    parse_pointer(flag, buff, arg);
  else if (flag.specifier == 'f')
    parse_float(flag, buff, arg);
  else if (flag.specifier == 'e' || flag.specifier == 'E')
    parse_exp_spec(flag, buff, arg);
  else if (flag.specifier == 'g' || flag.specifier == 'G')
    parse_general_float(flag, buff, arg);
  if (flag.specifier == 'G' || flag.specifier == 'E' || flag.specifier == 'X')
    to_upper(buff);
}

void parse_integer(flags flag, char *buff, va_list arg) {
  int64_t value = va_arg(arg, int64_t);

  switch (flag.length) {
    case 0:
      value = (int32_t)value;
      break;
    case 'h':
      value = (int16_t)value;
  }

  num_to_string(value, buff, 10);
  format_accuracy(buff, flag);
  format_flags(buff, flag);
}

void num_to_string(int64_t value, char *str, int base) {
  char tmp[SIZE] = {'\0'};
  int index = SIZE - 2;

  bool negative = value < 0 ? 1 : 0;

  value = negative ? -value : value;

  if (value == 0) tmp[index] = '0';

  while (value > 0) {
    index--;
    tmp[index] = "0123456789abcdef"[value % base];
    value /= base;
  }

  for (int j = 0; tmp[index]; index++, j++) {
    if (negative && j == 0) {
      str[j++] = '-';
    }

    str[j] = tmp[index];
  }
}

void format_accuracy(char *buff, flags flag) {
  char tmp[SIZE] = {'\0'};

  int sign = 0;
  int len = s21_strlen(buff);

  if (buff[0] == '-') {
    tmp[0] = '-';
    len--;
    sign = 1;
  }

  if (flag.accuracy > len) {
    int index;
    for (index = sign; index < flag.accuracy - len + sign; index++)
      tmp[index] = '0';

    for (int i = sign; buff[i]; i++, index++) tmp[index] = buff[i];

    s21_strcpy(buff, tmp);
  }

  if (flag.is_accuracy_set && flag.accuracy == 0 &&
      is_integer_specifier(flag.specifier) && buff[0] == '0')  // nothing output
    buff[0] = '\0';
}

bool is_integer_specifier(char c) {
  char specs[] = {'d', 'i', 'o', 'u', 'x', 'X'};

  bool res = false;

  for (s21_size_t i = 0; i < sizeof(specs); i++) {
    if (specs[i] == c) {
      res = true;
      break;
    }
  }
  return res;
}

void format_flags(char *buff, flags flag) {
  char tmp[SIZE + 1] = {'\0'};
  if (flag.plus && flag.specifier != 'u') {
    tmp[0] = buff[0] == '-' ? buff[0] : '+';
    s21_strcpy(tmp + 1, buff[0] == '-' ? buff + 1 : buff);
    s21_strcpy(buff, tmp);
  } else if (flag.space && buff[0] != '-' && flag.specifier != 'u') {
    tmp[0] = ' ';
    s21_strcpy(tmp + 1, buff);
    s21_strcpy(buff, tmp);
  }
  if (flag.width > (int)s21_strlen(buff)) {
    int index = flag.width - s21_strlen(buff);
    if (!flag.minus) {
      s21_memset(tmp, flag.zero ? '0' : ' ', index);
      s21_strcpy(tmp + index, buff);
    } else {
      s21_strcpy(tmp, buff);
      s21_memset(tmp + s21_strlen(tmp), ' ', index);
    }
    s21_strcpy(buff, tmp);
  }
}

void unsigned_num_to_string(uint64_t value, char *str, int base) {
  char buff[SIZE + 1] = {'\0'};
  int index = SIZE - 1;

  if (value == 0) {
    buff[index] = '0';
    index--;
  }

  for (; value && index; --index, value /= base)
    buff[index] = "0123456789abcdef"[value % base];
  for (int j = 0; buff[index + 1]; index++, j++) str[j] = buff[index + 1];
}

void parse_unsigned(flags flag, char *buff, va_list arg) {
  uint64_t value = va_arg(arg, uint64_t);
  switch (flag.length) {
    case 'h':
      value = (uint16_t)value;
      break;
    case 'l':
      value = (uint64_t)value;
      break;
    case 0:
      value = (uint32_t)value;
  }
  unsigned_num_to_string(value, buff, 10);
  format_accuracy(buff, flag);
  format_flags(buff, flag);
}

void parse_octal(flags flag, char *buff, va_list arg) {
  buff[0] = '0';
  num_to_string(va_arg(arg, int64_t), buff + flag.hash, 8);
  format_accuracy(buff, flag);
  format_flags(buff, flag);
}

bool is_all_zeroes(char *buff) {
  for (int i = 0; buff[i]; i++)
    if (buff[i] != '0') return false;
  return true;
}

void parse_hexadecimal(flags flag, char *buff, va_list arg) {
  uint64_t value = va_arg(arg, uint64_t);
  switch (flag.length) {
    case 0:
      value = (uint32_t)value;
      break;
    case 'h':
      value = (uint16_t)value;
      break;
    case 'l':
      value = (uint64_t)value;
      break;
  }
  unsigned_num_to_string(value, buff, 16);
  format_accuracy(buff, flag);
  if (flag.hash) {
    prepend_ox_specs(buff, flag);
  }
  format_flags(buff, flag);
}

void prepend_ox_specs(char *buff, flags flag) {
  if (!is_all_zeroes(buff) || flag.specifier == 'p') {
    s21_memmove(buff + 2, buff, s21_strlen(buff));
    buff[0] = '0';
    buff[1] = 'x';
  }
}

void parse_character(flags flag, char *buff, va_list arg) {
  if (flag.length == 'l') {
    wchar_t w_c;
    w_c = va_arg(arg, wchar_t);
    format_wchar(flag, buff, w_c);
  } else {
    char ch;
    ch = va_arg(arg, int);
    format_char(flag, buff, ch);
  }
}

void format_wchar(flags flag, char *buff, wchar_t w_c) {
  if (!flag.minus && flag.width) {
    char tmp[SIZE] = {'\0'};
    wcstombs(tmp, &w_c, SIZE);
    for (s21_size_t i = 0; i < flag.width - s21_strlen(tmp); i++) buff[i] = ' ';
    s21_strcat(buff, tmp);
  } else if (flag.width) {
    wcstombs(buff, &w_c, SIZE);
    for (int i = s21_strlen(buff); i < flag.width; i++) buff[i] = ' ';
  } else {
    wcstombs(buff, &w_c, SIZE);
  }
}

void format_char(flags flag, char *buff, char ch) {
  if (!flag.minus && flag.width) {
    for (int i = 0; i < flag.width; i++) {
      buff[i] = ' ';
      if (i == flag.width - 1) buff[i] = ch;
    }
  } else if (flag.width) {
    buff[0] = ch;
    for (int i = 1; i < flag.width; i++) buff[i] = ' ';
  } else {
    buff[0] = ch;
  }
}

void parse_string(flags flag, char *buff, va_list arg) {
  if (flag.length == 'l') {
    wchar_t *wstr = va_arg(arg, wchar_t *);
    format_wide_string(flag, buff, wstr);
  } else {
    char *str = va_arg(arg, char *);
    format_string(flag, buff, str);
  }
}

void format_string(flags flag, char *buff, char *str) {
  char tmp[SIZE] = {'\0'};
  s21_strcpy(tmp, str);
  if (flag.is_accuracy_set) tmp[flag.accuracy] = '\0';

  int shift = flag.width - s21_strlen(tmp);
  int len = s21_strlen(tmp);

  if (flag.minus && shift > 0) {
    s21_strcpy(buff, tmp);
    s21_memset(buff + len, ' ', shift);
  } else if (shift > 0) {
    s21_memset(buff, ' ', shift);
    s21_strcpy(buff + shift, tmp);
  } else {
    s21_strcpy(buff, tmp);
  }
}

void format_wide_string(flags flag, char *buff, wchar_t *wstr) {
  char tmp[SIZE] = {'\0'};
  char str[SIZE] = {'\0'};

  wcstombs(str, wstr, SIZE);
  s21_strcpy(tmp, str);
  if (flag.is_accuracy_set) tmp[flag.accuracy] = '\0';

  int shift = flag.width - s21_strlen(tmp);
  int len = s21_strlen(tmp);

  if (flag.minus && shift > 0) {
    s21_strcpy(buff, tmp);
    s21_memset(buff + len, ' ', shift);
  } else if (shift > 0) {
    s21_memset(buff, ' ', shift);
    s21_strcpy(buff + shift, tmp);
  } else {
    s21_strcpy(buff, tmp);
  }
}

void parse_pointer(flags flag, char *buff, va_list arg) {
  unsigned_num_to_string(va_arg(arg, uint64_t), buff, 16);
  format_accuracy(buff, flag);
  prepend_ox_specs(buff, flag);
  format_flags(buff, flag);
}

void parse_float(flags flag, char *buff, va_list arg) {
  long double value = 0;
  if (flag.length == 'L') {
    value = va_arg(arg, long double);
  } else {
    value = va_arg(arg, double);
  }

  if (!flag.is_accuracy_set) {
    flag.accuracy = 6;
  }

  double_to_string(value, buff, flag);
  format_flags(buff, flag);
}

void double_to_string(long double value, char *str, flags flag) {
  char buff[SIZE] = {'\0'};
  int index = SIZE - 2;
  bool negative = value < 0 ? 1 : 0;
  value = negative ? value * -1 : value;
  long double l = 0, r = modfl(value, &l);
  if (flag.accuracy == 0) {
    l = roundl(value);
    r = 0;
  }
  char fractions[SIZE] = {'\0'};
  for (int i = 0; i < flag.accuracy; i++) {
    r = r * 10;
    // set length
    fractions[i] = (int)r + '0';
  }
  long long right = roundl(r), left = l;
  if (!right) {
    for (int i = 0; i < flag.accuracy; index--, i++) buff[index] = '0';
  } else {
    for (int i = s21_strlen(fractions); right || i > 0;
         right /= 10, index--, i--)
      buff[index] = (int)(right % 10 + 0.05) + '0';
  }
  if ((flag.is_accuracy_set && flag.accuracy != 0) || (int)r ||
      (!flag.is_accuracy_set && value == 0) || s21_strlen(fractions))
    buff[index--] = '.';
  if (!left) {
    buff[index] = '0';
    index--;
  } else {
    for (; left; left /= 10, index--) buff[index] = (int)(left % 10) + '0';
  }
  for (int i = 0; buff[index + 1]; index++, i++) {
    if (negative && i == 0) {
      str[i] = '-';
      i++;
    }
    str[i] = buff[index + 1];
  }
}

void parse_exp_spec(flags flag, char *buff, va_list arg) {
  long double value = 0;
  if (flag.length == 'L') {
    value = va_arg(arg, long double);
  } else {
    value = va_arg(arg, double);
  }
  int pow = 0;
  char sign = (int)value == 0 ? '-' : '+';

  if ((int)value - value) {
    while ((int)value == 0) {
      pow++;
      value *= 10;
    }
  } else {
    sign = '+';
  }
  while ((int)value / 10 != 0) {
    pow++;
    value /= 10;
  }

  if (!flag.is_accuracy_set) flag.accuracy = 6;
  double_to_string(value, buff, flag);
  prepend_exp_symbols(buff, pow, sign);
  format_flags(buff, flag);
}

void prepend_exp_symbols(char *str, int pow, char sign) {
  int len = s21_strlen(str);
  str[len] = 'e';
  str[len + 1] = sign;
  str[len + 3] = pow % 10 + '0';
  pow /= 10;
  str[len + 2] = pow % 10 + '0';
  str[len + 4] = '\0';
}

void parse_general_float(flags flag, char *buff, va_list va) {
  long double value = 0;
  if (flag.length == 'L') {
    value = va_arg(va, long double);
  } else {
    value = va_arg(va, double);
  }

  if (!flag.is_accuracy_set) {
    flag.accuracy = 6;
  }
  if (flag.accuracy == 0) flag.accuracy = 1;
  int accuracy = flag.accuracy;
  long double m_value = value;
  int pow = 0;
  if ((int)value - value) {
    while ((int)m_value == 0) {
      pow++;
      m_value *= 10;
    }
  }
  if (pow > 4) {
    flag.accuracy = 0;
    double_to_string(m_value, buff, flag);
  } else {
    flag.accuracy = 10;
    double_to_string(value, buff, flag);
  }
  format_general_accuracy(buff, accuracy);
  if (pow > 4) prepend_exp_symbols(buff, pow, '-');
  remove_trail_zeroes(buff);
  format_flags(buff, flag);
}

void remove_trail_zeroes(char *buff) {
  int len = s21_strlen(buff);
  char *dot = s21_strchr(buff, '.');
  if (dot) {
    for (int i = len - 1; buff[i] != '.'; i--) {
      if (buff[i] == '0')
        buff[i] = '\0';
      else
        break;
    }
    if (dot[1] == '\0') dot[0] = '\0';
  }
}

void format_general_accuracy(char *buff, int accuracy) {
  int signify_digits = 0;
  s21_size_t len = s21_strlen(buff);
  int not_zero_found = 0;
  for (s21_size_t i = 0; i < s21_strlen(buff); i++) {
    if ((buff[i] == '0' && !not_zero_found) || buff[i] == '.')
      continue;
    else
      not_zero_found = 1;

    if (s21_isdigit(buff[i]) && not_zero_found) {
      signify_digits++;
    }
    if (signify_digits == accuracy && i + 1 < len) {
      int next = buff[i + 1] == '.' ? 2 : 1;
      buff[i] = buff[i + next] - '0' > 5 ? (char)(buff[i] + 1) : buff[i];
      buff[i + 1] = '\0';
      break;
    }
  }
}

int s21_isdigit(char c) { return (c >= '0' && c <= '9'); }

int s21_atoi(const char *str) {
  int res = 0;
  int sign = 1;
  int overflow = 0;

  while (*str == ' ') str++;

  if (*str == '-') {
    str++;
    sign = -1;
  }

  if (*str == '+') {
    str++;
  }

  while (*str && *str >= '0' && *str <= '9') {
    res = res * 10 + (*str - '0');
    if (res < 0) {
      overflow = 1;
      break;
    }
    str++;
  }
  if (overflow)
    res = sign > 0 ? INT32_MAX : INT32_MIN;
  else
    res *= sign;

  return res;
}

char *s21_strcat(char *dest, const char *src) {
  s21_size_t i, j;

  for (i = 0; dest[i] != '\0'; i++)
    ;

  for (j = 0; src[j] != '\0'; j++) dest[i + j] = src[j];

  dest[i + j] = '\0';

  return dest;
}

char *s21_strcpy(char *dest, const char *src) {
  char *flag;
  int i = 0;
  while (src[i] != '\0') {
    dest[i] = src[i];
    i++;
  }
  dest[i] = '\0';
  flag = dest;
  return flag;
}

void *s21_memmove(void *dst, const void *src, s21_size_t n) {
  unsigned char buffer[n];
  for (s21_size_t i = 0; i < n; i++) {
    buffer[i] = *((unsigned char *)(src) + i);
  }

  for (s21_size_t i = 0; i < n; i++) {
    *((unsigned char *)(dst) + i) = buffer[i];
  }
  return dst;
}

void to_upper(char *str) {
  while (*str) {
    if (*str >= 'a' && *str <= 'z') *str = *str - 'a' + 'A';
    str++;
  }
}