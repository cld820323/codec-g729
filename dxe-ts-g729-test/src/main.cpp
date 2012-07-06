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

const char* inputFile = "linear.wav";
const char* encdecFile = "output.wav";

void test1(){

  FILE *f_original;               /* File of speech data                   */
  FILE *f_codec;               /* File of serial bits for transmission  */

  // get length of file:
  f_original = fopen(inputFile, "rb");
  fseek (f_original, 0, SEEK_END);
  int nFileLen = ftell(f_original);
  fclose (f_original);

  if ( (f_original = fopen(inputFile, "rb")) == NULL) {
     printf("Error opening file  %s !!\n", inputFile);
     return;
  }
  printf("Linear file    :  %s\n", inputFile);

  if ( (f_codec = fopen(encdecFile, "wb")) == NULL) {
     printf("Error opening file  %s !!\n", encdecFile);
     return;
  }
  printf("Output encoded and decoded file:  %s\n", encdecFile);

  //копируем заголовок аудиофайла и втыкаем его в выходной файл
  printf("Copy 0x%x bytes of wav header\n", HEADER_SIZE);
  uint8_t buffer[2000];
  //fread(buffer, sizeof(uint8_t), HEADER_SIZE, f_original);
  //fwrite(buffer, sizeof(uint8_t), HEADER_SIZE, f_codec);

  CdxeCodec_G729 *coder = new CdxeCodec_G729(CdxeCodec_G729::_coder);//кодер
  CdxeCodec_G729 *decoder = new CdxeCodec_G729(CdxeCodec_G729::_decoder);//декодер
  int size = 1, getSize = 0;
  Init_Pre_Process();
  Init_Coder_ld8k();


  extern Word16 *new_speech;     /* Pointer to new speech data            */
  Word16 prm[PRM_SIZE];          /* Analysis parameters.                  */
  Word16 serial[SERIAL_SIZE];    /* Output bitstream buffer               */
  Word16 syn[L_FRAME] = {0};           /* Buffer for synthesis speech           */

  Word16 *tmpMass = new Word16[nFileLen];
  Word16 Buf[L_FRAME];

  int frame =0;
  float start=clock();
  while( fread(Buf, sizeof(Word16), L_FRAME, f_original) == L_FRAME){
	  for(int i=0; i<L_FRAME; i++){
		  tmpMass[frame*L_FRAME]=Buf[i];
	  }
	  frame++;
  }
  frame =0;
  for(int i=0; i<500; i++)
  {
    for(int i=0; i<L_FRAME; i++){
    	new_speech[i]=tmpMass[frame*L_FRAME];
	}
	Pre_Process(new_speech, L_FRAME);
    Coder_ld8k(prm, syn);
    prm2bits_ld8k( prm, serial);

    frame++;
    fwrite(serial, sizeof(Word16), SERIAL_SIZE, f_codec);
  }
  float stop=clock();
  printf("Coded %d frames\r", frame);
  printf("Time = %f seconds\r", (stop-start)/CLOCKS_PER_SEC);
  delete[] tmpMass;

  /*while(size){
	  uint8_t *b;
	  while(getSize==0){
		size = fread(buffer, sizeof(uint8_t), HEADER_SIZE, f_original);
		if(size==0)break;
		coder->addData(buffer,size);
		b = const_cast<uint8_t*>(coder->getResult(getSize));
	  }

	  //decoder->addData(const_cast<uint8_t*>(coder->getResult(getSize)),getSize);
	  //decoder->addData(b,getSize);
	  fwrite(b, sizeof(uint8_t), getSize, f_codec);
	  //fwrite(decoder->getResult(getSize), sizeof(uint8_t), getSize, f_codec);
	  //coder->releaseResult();
	  //decoder->releaseResult();
  }*/
}

int main() {
  //_IOFBF
    setvbuf(   stdout, NULL, _IOLBF , 0);
    setvbuf(   stderr, NULL, _IOLBF , 0);
    setvbuf(   stdin, NULL, _IOLBF , 0);

    test1();

	return 0;
}
