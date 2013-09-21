#ifndef __MAGNETO_DATA_H__
#define __MAGNETO_DATA_H__

#include "../Math/Vector.h"

// Magnetometer Data
template <class T>
class MagnetoData
: 
public Vector<T>
{
	public:
		enum Enum 
		{
			 X      // X field strength in milligauss
			,Y      // Y field strength in milligauss
			,Z      // Z field strength in milligauss
		};

	public: 
		MagnetoData() : Vector<T>() { }
		MagnetoData( const MagnetoData<T>& magnetoData ) : Vector<T>( magnetoData ) { }
		MagnetoData( const Vector<T>& coordVec ) : Vector<T>( coordVec ) { }

		const T& operator[]( Enum en ) const;
		T& operator[]( Enum en );
};

#include "MagnetoData.cpp"

#endif //__MAGNETO_DATA_H__
