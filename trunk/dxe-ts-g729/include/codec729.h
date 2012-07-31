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

#include <stdio.h>
#include <stdlib.h>

#include "../EasyG729A.h"

using namespace std;

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
	if(direction==_coder)hEncoder = init_encoder( );
    if(direction==_decoder)hDecoder = init_decoder( );
  }

  ~CdxeCodec_G729(){
	  if(direction==_coder)release_encoder(hEncoder);
	  if(direction==_decoder)release_decoder(hDecoder);
  }
private:
  ///направление кодирования
  const eCodeDir direction;
//...
  vector<uint8_t> buffer;
  uint8_t *tmp_buffer;

  //хэндл кодера
  CODER_HANDLE hEncoder,hDecoder;
};

#endif /* CODEC729_H_ */
