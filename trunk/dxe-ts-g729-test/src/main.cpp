//============================================================================
// Name        : dxe-ts-g729-test.cpp
// Author      : Kulpanov & comrades
// Version     :
// Copyright   : LGPL
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "include/codec729.h"

#define TEST(expr)\
  if(expr)printf("   PASS: (%s) at %d:%s\n", #expr, __LINE__, __FILE__);\
  else  printf("!  FAIL: (%s) at %d:%s\n", #expr, __LINE__, __FILE__)

const int HEADER_SIZE = 0x30;

const char* inputFile = "linear.in";
const char* encdecFile = "encdec.out";
const char* codFile = "coded.cod";

void test1(){

  FILE *f_original;               /* File of speech data                   */
  FILE *f_coded;               /* File of serial bits for transmission  */

  if ( (f_original = fopen(inputFile, "rb")) == NULL) {
     printf("Error opening file  %s !!\n", inputFile);
     return;
  }
  printf("Linear file    :  %s\n", inputFile);

  if ( (f_coded = fopen(codFile, "wb")) == NULL) {
     printf("Error opening file  %s !!\n", codFile);
     return;
  }

  CdxeCodec_G729 *coder = new CdxeCodec_G729(CdxeCodec_G729::_coder);//кодер

  uint8_t speech[L_G729A_FRAME*2];
  clock_t start, finish;
  double duration;
  start = clock();
  int frame = 0;

  while (fread(speech, sizeof(uint8_t), L_G729A_FRAME*2, f_original) == L_G729A_FRAME*2)
  {
	coder->addData(speech,L_G729A_FRAME*2);
	int size;
  	const uint8_t *serial = coder->getResult(size);
	fwrite(serial, sizeof(char), size, f_coded);
	coder->releaseResult();
	printf("Encode frame %d\r", ++frame);
  }
  fclose(f_original);
  fclose(f_coded);

  finish = clock();
  duration = (double)(finish - start) / CLOCKS_PER_SEC;
  printf( "\n%2.1f seconds\n", duration );

  delete coder;
}

void test2(){

  FILE *f_coded;
  FILE *f_decoded;

  if ( (f_coded = fopen(codFile, "rb")) == NULL) {
	 printf("Error opening file  %s !!\n", codFile);
	 return;
  }
  printf("Codede file    :  %s\n", codFile);

  if ( (f_decoded = fopen(encdecFile, "wb")) == NULL) {
	 printf("Error opening file  %s !!\n", encdecFile);
	 return;
  }

  CdxeCodec_G729 *decoder = new CdxeCodec_G729(CdxeCodec_G729::_decoder);//декодер

  uint8_t serial[L_G729A_FRAME_COMPRESSED];
  clock_t start, finish;
  double duration;
  start = clock();
  int frame = 0;

  while (fread(serial, sizeof(char), L_G729A_FRAME_COMPRESSED, f_coded) == L_G729A_FRAME_COMPRESSED)
  {
	decoder->addData(serial,L_G729A_FRAME_COMPRESSED);

	printf("Decode frame %d\r", ++frame);
  }
	int size;
	const uint8_t *encdec = decoder->getResult(size);
	fwrite(encdec, sizeof(char), size, f_decoded);
	decoder->releaseResult();

  fclose(f_coded);
  fclose(f_decoded);

  finish = clock();
  duration = (double)(finish - start) / CLOCKS_PER_SEC;
  printf( "\n%2.1f seconds\n", duration );

  delete decoder;
}

int main() {
  //_IOFBF
    setvbuf(   stdout, NULL, _IOLBF , 0);
    setvbuf(   stderr, NULL, _IOLBF , 0);
    setvbuf(   stdin, NULL, _IOLBF , 0);

    test1();//опаньки, и закодировали
    test2();//опаньки, и раскодировали

	return 0;
}
