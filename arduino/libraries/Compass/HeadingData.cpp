const T& HeadingData::operator[]( Enum en ) const
{
	int index = (int)en;
	return (*this).Vector<T>::operator[](index);
}

T& HeadingData::operator[]( Enum en )
{
	int index = (int)en;
	return (*this).Vector<T>::operator[](index);
}