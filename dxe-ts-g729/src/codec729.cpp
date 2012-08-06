/*
 * codec729.cpp
 *
 *  Created on: 02.07.2012
 *      Author: krasnozhon!
 */

#include "codec729.h"

int CdxeCodec_G729::addData(uint8_t* _data, int _size){
	buffer.insert(buffer.end(),_data,_data+_size);
	return 0;
}

uint8_t* CdxeCodec_G729::releaseResult(){
	delete [] tmp_buffer;
	return NULL;
}

const uint8_t* CdxeCodec_G729::getResult(int& _size) {

	if(direction==_coder){
		tmp_buffer = new uint8_t[buffer.size()*2];
		_size = 0;
		while (buffer.size() >= L_G729A_FRAME*2){

			short inBuf[L_G729A_FRAME] = {0};
			for(int i=0,j=0; j<L_G729A_FRAME; i+=2,j++){
				inBuf[j] = buffer[i]|(short)buffer[i+1]<<8;
			}
			buffer.erase(buffer.begin(),buffer.begin()+L_G729A_FRAME*2);

			encodeAdr = (PROC_ENCODE) GetProcAddress(hinstLib, "encoder");
			(encodeAdr)(hEncoder, inBuf, tmp_buffer+_size);

			//encoder(hEncoder, inBuf, tmp_buffer+_size);
			_size+=L_G729A_FRAME_COMPRESSED;
		}
	}else
	if(direction==_decoder){
		short *outBuf = new short [buffer.size()*2*10];
		_size = 0;
		while(buffer.size() >= L_G729A_FRAME_COMPRESSED){
			uint8_t serial[L_G729A_FRAME_COMPRESSED] = {0};
			for(int i=0; i<L_G729A_FRAME_COMPRESSED; i++){
				serial[i] = buffer[i];
			}
			buffer.erase(buffer.begin(),buffer.begin()+L_G729A_FRAME_COMPRESSED);

			decodeAdr = (PROC_DECODE) GetProcAddress(hinstLib, "decoder");
			(decodeAdr)(hDecoder, serial, outBuf+_size/2);

			//decoder(hDecoder, serial, outBuf+_size/2);
			_size+=L_G729A_FRAME*2;
		}
		tmp_buffer = new uint8_t[_size];
		for(int i=0,j=0; i<_size; i+=2,j++){
			tmp_buffer[i] = outBuf[j]&0xFF;
			tmp_buffer[i+1] = (outBuf[j]>>8)&0xFF;
		}

		delete [] outBuf;
	}

	return tmp_buffer;
}
