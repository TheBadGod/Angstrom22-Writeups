#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>


char memory_buf[244] = { 0xca, 0x5f, 0x91, 0xae, 0x61, 0x52, 0xbf, 0x83, 0x8c, 0x49, 0xec, 0xb5, 0xba, 0x85, 0x14, 0xf5, 0x06, 0x4f, 0xba, 0xa3, 0xa4, 0xd0, 0x32, 0xef, 0x0b, 0xd5, 0x26, 0x50, 0x01, 0x2f, 0xf8, 0xb6, 0x3f, 0x0e, 0x37, 0x09, 0xe4, 0xd6, 0x8c, 0x4e, 0x7e, 0xc5, 0x17, 0x05, 0xaa, 0x23, 0x08, 0xb7, 0x8a, 0x4e, 0x6c, 0xba, 0x16, 0xa3, 0xc7, 0x8d, 0x59, 0x5c, 0x2d, 0x71, 0xa7, 0x33, 0x74, 0xa8, 0x9d, 0xb3, 0x8a, 0xa4, 0x22, 0xa0, 0xfe, 0x44, 0x91, 0xfc, 0x16, 0x66, 0x0c, 0xbc, 0xbd, 0x52, 0xf8, 0x71, 0xa3, 0x16, 0xa9, 0x85, 0xd9, 0x71, 0x8f, 0x08, 0xe1, 0xc8, 0xcd, 0x2d, 0x8e, 0x64, 0x43, 0x4c, 0x2d, 0xde, 0x7a, 0x15, 0x0f, 0xc3, 0xe0, 0xe4, 0xe2, 0xaf, 0x65, 0xe3, 0x87, 0x38, 0xd5, 0x14, 0xaa, 0x97, 0x78, 0x98, 0x28, 0x5a, 0xe8, 0xe1, 0x47, 0x4b, 0x73, 0x93, 0xc3, 0x64, 0xee, 0xdb, 0xa8, 0x76, 0x11, 0x5b, 0xd5, 0x63, 0x24, 0xa9, 0xf6, 0x27, 0x11, 0x31, 0x3f, 0x8f, 0xc2, 0x79, 0x3b, 0x58, 0x36, 0xb0, 0xf0, 0xb9, 0x05, 0x95, 0x24, 0xbc, 0xd2, 0xbb, 0x6c, 0x32, 0xcb, 0x17, 0x47, 0xa8, 0x07, 0x36, 0x0b, 0x6f, 0xac, 0x2e, 0xac, 0x36, 0xa3, 0x3d, 0x6a, 0xc8, 0x10, 0x69, 0xf2, 0x45, 0x61, 0x8b, 0xea, 0x8b, 0x58, 0x83, 0x6a, 0x62, 0x46, 0xc2, 0xe7, 0x3a, 0x6f, 0x02, 0xd6, 0xd2, 0x44, 0x98, 0x31, 0x66, 0x79, 0x9e, 0x02, 0x5d, 0x2a, 0xba, 0x9a, 0xa5, 0x6e, 0x64, 0x04, 0xd1, 0xc3, 0xe8, 0x45, 0x9b, 0x23, 0xeb, 0x39, 0x98, 0xc3, 0x55, 0x05, 0xf6, 0x0a, 0x50, 0x3c, 0xff, 0x00, 0x00, 0x00, 0x00, 0x5d, 0x50, 0x95, 0x7c, 0x1b, 0x6c, 0x58, 0x4f, 0xc6, 0x80, 0x7c, 0x77 };

int popcount(unsigned x)
{
    int c = 0;
    for (; x != 0; x >>= 1)
        if (x & 1)
            c++;
    return c;
}

void swap_two_chars(long in, unsigned int counter_1, int counter_2)
{
  char v4; // [rsp+1Bh] [rbp-15h]
  int i; // [rsp+1Ch] [rbp-14h]
  long v6; // [rsp+20h] [rbp-10h]
  long v7; // [rsp+28h] [rbp-8h]

  srand(counter_1);
  v6 = 0LL;
  for ( i = 0; i <= 31; ++i )
  {
    v7 = rand() + v6;
    v6 *= counter_2;
    v4 = *(char *)(v7 % 32 + in);
    *(char *)(in + v7 % 32) = *(char *)(v6 % 32 + in);
    *(char *)(in + v6 % 32) = v4;
  }
}


unsigned long sub_140E(const char *in, unsigned int counter_1, int counter_2, unsigned int *rom)
{
  unsigned int *v4; // rax
  int setbits_in_instr_minnus_11; // [rsp+28h] [rbp-48h] BYREF
  int count_memcpy; // [rsp+2Ch] [rbp-44h]
  int v10; // [rsp+30h] [rbp-40h]
  unsigned int instruction; // [rsp+34h] [rbp-3Ch]
  void *ptr; // [rsp+38h] [rbp-38h]
  int *v13 = NULL; // [rsp+40h] [rbp-30h]
  int *v14 = NULL; // [rsp+48h] [rbp-28h]
  int *v15 = NULL; // [rsp+50h] [rbp-20h]
  int *v16 = NULL; // [rsp+58h] [rbp-18h]
  size_t size; // [rsp+60h] [rbp-10h]

  size = 0x2000LL;
  count_memcpy = 0;
LABEL_15:
  while ( *rom )
  {	
  	
    setbits_in_instr_minnus_11 = 0;
    v4 = rom++;
    instruction = *v4;
    setbits_in_instr_minnus_11 = popcount(instruction) - 11;
    if ( setbits_in_instr_minnus_11 <= 8 )
    {
      v10 += 8;
      fprintf(stderr,"In the switch\n");
      switch ( setbits_in_instr_minnus_11 )
      {
        case 0:
        case 4:
          goto LABEL_15;
        case 1:
        	fprintf(stderr,"Swapping Chars Case 1\n");
          swap_two_chars((long)in, counter_1, counter_2);
          break;
        case 2:
       		fprintf(stderr,"Copying in to ptr, malloc v16 with size %ld\n", size);
          memcpy((char *)ptr + 32 * count_memcpy++, in, 32uLL);
            if(v16 != NULL){
   					free(v16);
   				}
          v16 = (int *)malloc(size);
          break;
        case 3:
         	fprintf(stderr,"Swapping Chars Case 3\n");
          swap_two_chars((long)in, counter_1 + 1, counter_2 + 1);
          v15 = malloc(v10);
          *v15 = v10;
          break;
        case 5:
        	fprintf(stderr,"Realloc of ptr and alloc of v13\n");
          ptr = realloc(ptr, size);
          //size *= 2LL;
          if(v13 != NULL){
   					free(v13);
   				}
          v13 = (int *)malloc(size);
          break;
        case 6:
        	fprintf(stderr,"Swapping Chars Case 6\n");
          swap_two_chars((long)in, counter_1 - 1, counter_2 - 1);
          break;
        case 7:
       	 	fprintf(stderr,"Reset size to 32\n");
          //size = 32LL;
          ptr = malloc(0x100uLL);
          //size *= 2LL;
          fprintf(stderr,"size is %ld", size);
          v14 = (int *)malloc(0x400000000uLL);
          break;
        default:
          setbits_in_instr_minnus_11 = 0;
          rom = (unsigned int *)&setbits_in_instr_minnus_11;
          break;
      }
    }
    else
    {
    	fprintf(stderr,"Else clause\n");
    	int n = 0;
    	fprintf(stderr,"size is %ld\n", size);
      for ( *v14 = 0; *v14 < (long)size / 2; ++*v14 ) {
      	//fprintf(stderr, "v14 value is  : %d\n", *v14);
      	n += 1;
        *((char *)ptr + *v14) *= (char)setbits_in_instr_minnus_11;
      }
      fprintf(stderr,"v14 length %d\n", n);
    }
  }
  

  for ( *v13 = 0; *v13 <= 31; ++*v13 )
    *((char *)in + *v13) = 0;
  *v15 = 0;
  for ( *v16 = 0; *v16 < (long)size / 2; ++*v16 )
  {
    *((char *)in + *v15 % 32) ^= *((char *)ptr + *v16);
    *v15 = (*v13 >> 5) + *v16;
  }
  return 0;
}

int main (void) {
	int i; // [rsp+Ch] [rbp-54h]
	long v5[4]; // [rsp+10h] [rbp-50h]
	char dummy_flag[40]; // [rsp+30h] [rbp-30h] BYREF
	fprintf(stderr,"entering main\n");
	
	strncpy(dummy_flag, "actf{l_plus_ratio_plus_no_flags}",32);
	v5[0] = 0x5646564D6104635FLL;
	v5[1] = 0xD2DA506ACC801521LL;
	v5[2] = 0x660A7262DAB5E734LL;
	v5[3] = 0x8C0A3EFC87E57E8CLL;
	sub_140E(dummy_flag, 1LL, 1LL, (int*)memory_buf);
	for ( i = 0; i <= 31; ++i )
	dummy_flag[i] ^= *((char *)v5 + i);
	printf("%32s\n", dummy_flag);
	return 0LL;
}
