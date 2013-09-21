const T& MagnetoData::operator[]( Enum en ) const
{
	int index = (int)en;
	return (*this).Vector<T>::operator[](index);
}

T& MagnetoData::operator[]( Enum en )
{
	int index = (int)en;
	return (*this).Vector<T>::operator[](index);
}