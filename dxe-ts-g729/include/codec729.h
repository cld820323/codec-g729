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

using namespace std;

class CdxeCodec_G729{
public:

  /** Добавить данные для кодирования/декодирования
   * \param _data - массив данных
   * \param _size - размер
   * \return размер  результата*/
  int addData(uint8_t _data[], int _size);

  /** Закодировать в кодек
   * \return размер  результата*/
  int encode(vector<uint8_t>& _res);

  /** Декодировать в линейный
   * \return размер  результата*/
  int decode(vector<uint8_t>& _res);

public:
  CdxeCodec_G729()
  {
    buffer.reserve(1024);
  }
private:
//...
  vector<uint8_t> buffer;
};

#endif /* CODEC729_H_ */
