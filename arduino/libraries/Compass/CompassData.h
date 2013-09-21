#ifndef __COMPASS_DATA_H__
#define __COMPASS_DATA_H__

#include "../Math/Vector.h"

template <class T>
class CompassData
	: 
	public Vector<T>
{
	public:
		enum Enum 
		{
			 Heading
			,Pitch
			,Roll
		};

	public: 
		CompassData() : Vector<T>() { }
		CompassData( const CompassData<T>& tiltData ) : Vector<T>( tiltData ) { }
		CompassData( const Vector<T>& coordVec ) : Vector<T>( coordVec ) { }

		using Vector<T>::operator+=;
		using Vector<T>::operator-=;
		using Vector<T>::operator*=;
		using Vector<T>::operator/=;
		using Vector<T>::operator--;
		using Vector<T>::operator++;
		using Vector<T>::operator+;
		using Vector<T>::operator-;
		using Vector<T>::operator*;
		using Vector<T>::operator/;
		using Vector<T>::operator=;
		using Vector<T>::operator,;
		using Vector<T>::operator[];
		using Vector<T>::operator==;
		using Vector<T>::operator!=;

		const T& operator[]( Enum en ) const;
		T& operator[]( Enum en );
};

#include "CompassData.cpp"

#endif //__COMPASS_DATA_H__
