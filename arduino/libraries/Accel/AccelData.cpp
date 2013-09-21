#include "AccelData.h"

const T& AccelData::operator[]( Enum en ) const
{
	int index = (int)en;
	return (*this)[index];
}

T& AccelData::operator[]( Enum en )
{
	int index = (int)en;
	return (*this)[index];
}