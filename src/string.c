#include <stdint.h>
void str_cpy(const char *src,char *dst){
//Copy string from A to B
  int i = 0;
  while(src[i] != '\0'){
    dst[i] = src[i];
    i++;
    
  }
  dst[i] = '\0';
  return;
}

int str_len(const char *string){
  int i = 0;
  while (string[i] == '\0'){
    i++;
  }
  return i;
}
void str_cat(char *dst, const char *src) {
    int i = 0;
    int j = 0;

    while (dst[i] != '\0') {
        i++;
    }

    while (src[j] != '\0') {
        dst[i] = src[j];
        i++;
        j++;
    }

    dst[i] = '\0';
}
int str_eq(const char *a, const char *b) {
    while(*a && * b && *a == *b){
      a++;
      b++;  
    }
    return *a == '\0' && *b == '\0';
} 
