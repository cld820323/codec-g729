#include "../include/codec729.h"
/*
 * codec729.cpp
 *
 *  Created on: 02.07.2012
 *      Author: kulpanov!
 */


#include "codec729.h"

int CdxeCodec_G729::addData(uint8_t* _data, int _size){
	buffer.insert(buffer.end(),_data,_data+_size);
	return 0;
}

uint8_t* CdxeCodec_G729::releaseResult(){
	delete [] tmp_buffer;
	return 0;
}


const uint8_t* CdxeCodec_G729::getResult(int& _size) {
	tmp_buffer = new uint8_t[buffer.size()];
	Word16 frame[SERIAL_SIZE];
	Word16 prm[PRM_SIZE] = {0};
	Word16 syn[L_FRAME] = {0};

	_size = 0;
	while( buffer.size()/2 >= L_FRAME)
	{
		_size += L_FRAME*2;
		for(int i;i<L_FRAME*2;i+=2){
			frame[i] = buffer[i];
			frame[i] |= (Word16)buffer[i+1]<<8;
		}
		buffer.erase(buffer.begin(),buffer.begin()+L_FRAME*2);

		Pre_Process(frame, L_FRAME);
		Coder_ld8k(prm, syn);
		prm2bits_ld8k( prm, frame);

		for(int i;i<SERIAL_SIZE*2;i+=2){
			tmp_buffer[i] = frame[i]&0xFF;
			tmp_buffer[i+1] = frame[i]>>8;
		}
	}

	return tmp_buffer;
}
