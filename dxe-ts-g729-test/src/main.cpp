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

#include "include/codec729.h"

#define TEST(expr)\
  if(expr)printf("   PASS: (%s) at %d:%s\n", #expr, __LINE__, __FILE__);\
  else  printf("!  FAIL: (%s) at %d:%s\n", #expr, __LINE__, __FILE__)

void test1(){

  vector<uint8_t> res;
  //тестовые данные
  uint8_t g729_data[80];
  for(int i=0; i<80; i++)
    g729_data[i] = 0;

  CdxeCodec_G729 g729;
  g729.addData(g729_data, 80);
  g729.addData(g729_data, 80);

  int len = g729.decode(res);

  TEST(len == 80);//???
  //тесты сам придумаешь на входные данные

}


int main() {
  //_IOFBF
    setvbuf(   stdout, NULL, _IOLBF , 0);
    setvbuf(   stderr, NULL, _IOLBF , 0);
    setvbuf(   stdin, NULL, _IOLBF , 0);

  test1();

	return 0;
}
