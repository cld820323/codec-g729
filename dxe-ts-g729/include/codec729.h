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

#include "typedef.h"
#include "basic_op.h"
#include "ld8k.h"

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
    buffer.reserve(1);
    /*Init_Pre_Process();
    Init_Coder_ld8k();*/
  }
private:
  ///направление кодирования
  const eCodeDir direction;
//...
  vector<uint8_t> buffer;
  uint8_t *tmp_buffer;
  int SIZE;
};

#endif /* CODEC729_H_ */
