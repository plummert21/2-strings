#ifndef _PLDFSLFDJ__PS
#define _PLDFSLFDJ__PS

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../s21_string.h"

//#include "s21_strchr.h"
#define S21_NAN (-(0.0 / 0.0))
#define SPEC_LIST "gGeExXopcdfsui%%n"
#define LEN_LIST "hlL"
#define INT_MAX 9223372036854775807
#define INT_MIN -9223372036854775808
//#define STR "g1e124"
//#define FORMAT "%e"

typedef struct {
  int star;
  int weight;
  char lenght;
  char d_lenght;
  char spec;
  int n;
  char *str;
  short int over;
  short int finish;
  int n_count;
  int some_read;
} specifier;

int s21_sscanf(const char *str, const char *format, ...);
const char *parsing(const char *format, specifier *spec);
int write_var(specifier *spec, const char *str, va_list ap, const char *start);
int setS(specifier *spec, const char *str, va_list ap);
int setC(specifier *spec, const char *str, va_list ap);
int setD(specifier *spec, const char *str, va_list ap, int minus);
int setI(specifier *spec, const char *str, va_list ap);
int setE(specifier *spec, const char *str, va_list ap);
int setO(specifier *spec, const char *str, va_list ap, int minus);
int setX(specifier *spec, const char *str, va_list ap, int minus);
int setN(specifier *spec, const char *str, va_list ap);
void spec_zero(specifier *spec);

#endif /*_PLDFSLFDJ__PS*/