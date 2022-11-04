#include "defenitions.h"
#define SIZE_ARE 3
#define SIZE_FUNCT 5
#define SIZE_RD 3
#define SIZE_OD 2
#define SIZE_RS 3
#define SIZE_OS 2

/*this function convert the given codebitfield into an integer*/
int codeBitFieldToInt(codebitfield field)
{
	int num;
	/*we need to move each of the fields into the right place in the integer*/
	num = field.ARE;
	num += field.funct << SIZE_ARE;
	num += field.rd << (SIZE_ARE+SIZE_FUNCT);
	num += field.od << (SIZE_ARE+SIZE_FUNCT+SIZE_RD);
	num += field.rs << (SIZE_ARE+SIZE_FUNCT+SIZE_RD+SIZE_OD);
	num += field.os << (SIZE_ARE+SIZE_FUNCT+SIZE_RD+SIZE_OD+SIZE_RS);
	num += field.opcode << (SIZE_ARE+SIZE_FUNCT+SIZE_RD+SIZE_OD+SIZE_RS+SIZE_OS);
	return num;
}

/*this function convert the given databitfield into an integer*/
int DataBitFieldToInt(databitfield field)
{
	int num;
	/*we need to move each of the fields into the right place in the integer*/
	num = field.ARE;
	num += field.value << SIZE_ARE;
	return num;
}
