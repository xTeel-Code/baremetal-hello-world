#include <stdint.h>
int str_eq(const char *a, const char *b) {
    while(*a && * b && *a == *b){
      a++;
      b++;  
    }
    return *a == '\0' && *b == '\0';
}
