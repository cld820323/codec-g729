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
  fread(buffer, sizeof(uint8_t), HEADER_SIZE, f_original);
  fwrite(buffer, sizeof(uint8_t), HEADER_SIZE, f_codec);

  CdxeCodec_G729 *coder = new CdxeCodec_G729(CdxeCodec_G729::_coder);//кодер
  CdxeCodec_G729 *decoder = new CdxeCodec_G729(CdxeCodec_G729::_decoder);//декодер
  int size = 1, getSize;
  while(size){
	  size = fread(buffer, sizeof(uint8_t), HEADER_SIZE, f_original);

	  coder->addData(buffer,size);
	  //decoder->addData(const_cast<uint8_t*>(coder->getResult(getSize)),getSize);
	  uint8_t *b = const_cast<uint8_t*>(coder->getResult(getSize));
	  decoder->addData(b,getSize);

	  fwrite(decoder->getResult(getSize), sizeof(uint8_t), getSize, f_codec);
	  coder->releaseResult();
	  decoder->releaseResult();
  }
}

int main() {
  //_IOFBF
    setvbuf(   stdout, NULL, _IOLBF , 0);
    setvbuf(   stderr, NULL, _IOLBF , 0);
    setvbuf(   stdin, NULL, _IOLBF , 0);

    test1();

	return 0;
}
