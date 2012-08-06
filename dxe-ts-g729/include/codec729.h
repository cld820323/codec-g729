/*
 * codec729.h
 *
 *  Created on: 02.07.2012
 *      Author: kulpanov!
 */

#ifndef CODEC729_H_
#define CODEC729_H_

#include <stdint.h>
#include <vector>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include <windows.h>

#include "../EasyG729A.h"

using namespace std;

typedef CODER_HANDLE (*PROC_INIT)(void);
typedef bool (*PROC_ENCODE) (CODER_HANDLE hEncoder, short *speech, unsigned char *bitstream);
typedef bool (*PROC_DECODE) (CODER_HANDLE hDecoder, unsigned char *bitstream, short *synth_short);
typedef bool (*PROC_RELEASE) (CODER_HANDLE hEncoder);

class CdxeCodec_G729{
public:

  /** Добавить данные для кодирования/декодирования
   * \param _data - массив данных
   * \param _size - размер
   * \return размер  результата*/
  int addData(uint8_t* _data, int _size);

  /** Очистить буфер результата
   * \return NULL*/
  uint8_t* releaseResult();

  /** Закодировать/декодировать, от выбранного направления.
   * Ввернуть укзатель на результат
   * @return результат   */
  const uint8_t* getResult(int& _size);

public:
  enum eCodeDir{
      _coder = 0
    , _decoder
  };
public:
  CdxeCodec_G729(eCodeDir _dir)
  : direction(_dir)
  {
    buffer.reserve(300);
    string strID;
    sprintf(const_cast<char*>(strID.c_str()),"%d",(int)GetCurrentThreadId());
    string libName = "libEasyG729";
    libName = libName+strID.c_str()+".dll";
    CopyFile("libEasyG729.dll", libName.c_str(),FALSE);

    hinstLib = LoadLibrary(libName.c_str());

	if(direction==_coder){
		initAdr = (PROC_INIT) GetProcAddress(hinstLib, "init_encoder");
		if (NULL != initAdr)
		{
			hEncoder = (initAdr)();
		}
		//hEncoder = init_encoder( );
	}
    if(direction==_decoder){
    	initAdr = (PROC_INIT) GetProcAddress(hinstLib, "init_encoder");
		if (NULL != initAdr)
		{
			hEncoder = (initAdr)();
		}
    	//hDecoder = init_decoder( );
    }
  }

  ~CdxeCodec_G729(){
	  FreeLibrary(hinstLib);
	  if(direction==_coder){
		  releaseAdr = (PROC_RELEASE) GetProcAddress(hinstLib, "release_encoder");
		  //release_encoder(hEncoder);
	  }
	  if(direction==_decoder){
		  releaseAdr = (PROC_RELEASE) GetProcAddress(hinstLib, "release_decoder");
		  //release_decoder(hDecoder);
	  }
  }
private:
  ///направление кодирования
  const eCodeDir direction;
//...
  vector<uint8_t> buffer;
  uint8_t *tmp_buffer;

  //хэндл кодера
  CODER_HANDLE hEncoder,hDecoder;
  HINSTANCE hinstLib;
  PROC_INIT initAdr;
  PROC_ENCODE encodeAdr;
  PROC_DECODE decodeAdr;
  PROC_RELEASE releaseAdr;
};

#endif /* CODEC729_H_ */
