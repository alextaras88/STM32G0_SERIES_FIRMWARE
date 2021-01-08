#include "data_convert.h"

/******************************************************/
float ConvertByteToFloat(uint8_t * buff){

	float *res;
	uint8_t x[4];
	uint8_t i;

	for(i=0;i<4; i++){

		x[i] = buff[i];

	}
	res = (float *)x;

	return res[0];

}
/******************************************************/

