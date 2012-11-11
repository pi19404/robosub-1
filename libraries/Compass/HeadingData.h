#ifndef __HEADING_DATA_H__
#define __HEADING_DATA_H__

#include "../Math/Vector.h"

template <class T>
class HeadingData
	: 
	public Vector<T>
{
	public:
		enum Enum 
		{
			 Pitch      // Pitch in 10ths of a degree
			,Roll       // Roll in 10ths of a degree
			,Heading    // Heading in 10ths of a degree
		};

	public: 
		HeadingData() : Vector<T>() { }
		HeadingData( const HeadingData<T>& headingData ) : Vector<T>( headingData ) { }
		HeadingData( const Vector<T>& coordVec ) : Vector<T>( coordVec ) { }

		const T& operator[]( Enum en ) const;
		T& operator[]( Enum en );
};

#include "HeadingData.cpp"

#endif //__HEADING_DATA_H__
