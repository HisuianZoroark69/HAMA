#include "Auxiliaries.h"

int getSign(float num)
{
	if (num == 0) return 0;
	else return (signbit(num) ? -1 : 1);
}