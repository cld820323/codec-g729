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
#include <string.h>
#include <time.h>
#include <windows.h>
#include <process.h>

#include "include/codec729.h"

#define TEST(expr)\
  if(expr)printf("   PASS: (%s) at %d:%s\n", #expr, __LINE__, __FILE__);\
  else  printf("!  FAIL: (%s) at %d:%s\n", #expr, __LINE__, __FILE__)


void toG729(int Counter){

  FILE *f_original;               /* File of speech data                   */
  FILE *f_coded;               /* File of serial bits for transmission  */

  string inPCM = "linear";
  string outG729 = "output";

  string number;
  sprintf(const_cast<char*>(number.c_str()),"%d",Counter);

  inPCM = inPCM + ".in";
  outG729 = outG729+ number.c_str() + ".cod";
  if ( (f_original = fopen(inPCM.c_str(), "rb")) == NULL) {
     printf("Error opening file  %s !!\n", inPCM.c_str());
     return;
  }
  printf("Linear file    :  %s\n", inPCM.c_str());

  if ( (f_coded = fopen(outG729.c_str(), "wb")) == NULL) {
     printf("Error opening file  %s !!\n", outG729.c_str());
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


void toPCM(int Counter){

  FILE *f_coded;
  FILE *f_decoded;

  string inG729 = "output";
  string outPCM = "output";
  string number;
  sprintf(const_cast<char*>(number.c_str()),"%d",Counter);

  inG729 = inG729 + number.c_str()+ ".cod";
  outPCM = outPCM+ number.c_str() + ".out";

  if ( (f_coded = fopen(inG729.c_str(), "rb")) == NULL) {
	 printf("Error opening file  %s !!\n", inG729.c_str());
	 return;
  }
  printf("Codede file    :  %s\n", inG729.c_str());

  if ( (f_decoded = fopen(outPCM.c_str(), "wb")) == NULL) {
	 printf("Error opening file  %s !!\n", outPCM.c_str());
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
	int size;
	const uint8_t *encdec = decoder->getResult(size);
	fwrite(encdec, sizeof(char), size, f_decoded);
	decoder->releaseResult();

	printf("Decode frame %d\r", ++frame);
  }

  fclose(f_coded);
  fclose(f_decoded);

  finish = clock();
  duration = (double)(finish - start) / CLOCKS_PER_SEC;
  printf( "\n%2.1f seconds\n", duration );

  delete decoder;
}


#define THREAD_NUM	1

unsigned Counter=THREAD_NUM;
unsigned __stdcall threadProc( void* pArguments )
{
	//printf( "ID of %d-th thread = %d \n", Counter, (int)GetCurrentThreadId());

	toG729((int)GetCurrentThreadId());//опаньки, и закодировали
	toPCM((int)GetCurrentThreadId());//опаньки, и раскодировали

	Counter--;
    _endthreadex( 0 );
    return 0;
}

int main() {
  //_IOFBF
    setvbuf(   stdout, NULL, _IOLBF , 0);
    setvbuf(   stderr, NULL, _IOLBF , 0);
    setvbuf(   stdin, NULL, _IOLBF , 0);

    HANDLE hThread[THREAD_NUM];
    for(int i=0; i<THREAD_NUM; i++)
    {
		unsigned threadID;
		hThread[i] = (HANDLE)_beginthreadex( NULL, 0, &threadProc, NULL, 0, &threadID );
    }
    while(Counter){Sleep(0);}
    for(int i=0; i<THREAD_NUM; i++)CloseHandle(hThread[i]);

	return 0;
}

