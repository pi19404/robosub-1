#ifndef __TILT_DATA_H__
#define __TILT_DATA_H__

#include "../Math/Vector.h"

// Tilt Data is not coordinate, but is 3 dimensional
// (Pitch, Roll, Temp) Abuse this characteristic
template <class T>
class TiltData
	: 
	public Vector<T>
{
	public:  //??why two public declarations? readability??
		enum Enum 
		{
			 Pitch          // 10ths of a degree
			,Roll           // 10ths of a degree
			,Temperature    // 10ths of a degree
		};

	public: 
		TiltData() : Vector<T>() { }
		TiltData( const TiltData<T>& tiltData ) : Vector<T>( tiltData ) { }
		TiltData( const Vector<T>& coordVec ) : Vector<T>( coordVec ) { }

		const T& operator[]( Enum en ) const;
		T& operator[]( Enum en );
};

#include "TiltData.cpp"

#endif //__TILT_DATA_H__
