const T& CompassData::operator[]( Enum en ) const
{
	int index = (int)en;
	return (*this)[index];
}

T& CompassData::operator[]( Enum en )
{
	int index = (int)en;
	return (*this)[index];
}