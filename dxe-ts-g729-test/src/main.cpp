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
#include <windows.h>
#include <process.h>

#include "include/codec729.h"

#define TEST(expr)\
  if(expr)printf("   PASS: (%s) at %d:%s\n", #expr, __LINE__, __FILE__);\
  else  printf("!  FAIL: (%s) at %d:%s\n", #expr, __LINE__, __FILE__)

const int HEADER_SIZE = 0x30;

const char* inPCM = "in.pcm.raw";
const char* outPCM = "out.pcm.raw";
const char* inG729 = "in.g729.raw";
const char* outG729 = "out.pcm.pcm";

void toG729(){

  FILE *f_original;               /* File of speech data                   */
  FILE *f_coded;               /* File of serial bits for transmission  */

  if ( (f_original = fopen(inPCM, "rb")) == NULL) {
     printf("Error opening file  %s !!\n", inPCM);
     return;
  }
  printf("Linear file    :  %s\n", inPCM);

  if ( (f_coded = fopen(outG729, "wb")) == NULL) {
     printf("Error opening file  %s !!\n", outG729);
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

void toPCM(){

  FILE *f_coded;
  FILE *f_decoded;

  if ( (f_coded = fopen(inG729, "rb")) == NULL) {
	 printf("Error opening file  %s !!\n", inG729);
	 return;
  }
  printf("Codede file    :  %s\n", inG729);

  if ( (f_decoded = fopen(outPCM, "wb")) == NULL) {
	 printf("Error opening file  %s !!\n", outPCM);
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

void __cdecl thread_proc(void* pParam)
{
printf("Hello from thread!\n");
}

int main() {
  //_IOFBF
    setvbuf(   stdout, NULL, _IOLBF , 0);
    setvbuf(   stderr, NULL, _IOLBF , 0);
    setvbuf(   stdin, NULL, _IOLBF , 0);

    _beginthread(thread_proc, 0, 0);

    /*
   toG729();//опаньки, и закодировали
   toPCM();//опаньки, и раскодировали*/

	return 0;
}
