#ifndef BASE64_H  
#define BASE64_H  

#define _CRT_SECURE_NO_DEPRECATE

#include <stdlib.h>  
#include <string.h>  

// ����
unsigned char *base64_encode(unsigned char *str);


//����
unsigned char *bae64_decode(unsigned char *code);

#endif