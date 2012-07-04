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
  int addData(uint8_t* _data, int _size);

  /** Закодировать/декодировать, от выбранного направления
   * \return размер  результата*/
  int run();

  /** Ввернуть укзатель на результат
   * @return результат   */
  const uint8_t* getResult() const;

public:
  enum eCodeDir{
      _toLinear = 0
    , _toCodec
  };
public:
  CdxeCodec_G729(eCodeDir _dir)
  : direction(_dir)
  {
    buffer.reserve(1024);
  }
private:
  ///направление кодирования
  const eCodeDir direction;
//...
  vector<uint8_t> buffer;
};

#endif /* CODEC729_H_ */
