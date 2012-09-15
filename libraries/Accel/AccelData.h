#ifndef __ACCEL_DATA_H__
#define __ACCEL_DATA_H__

#include "../Math/Vector.h"

template <class T>
class AccelData
    : 
    public Vector<T>
{
    public:
        enum Enum {
             X      // X acceleration in Gs
            ,Y      // Y acceleration in Gs
            ,Z      // Z acceleration in Gs
        };
    public: 
        AccelData() : Vector<T>() { }
        AccelData( const AccelData<T>& accelData ) : Vector<T>( accelData ) { }
        AccelData( const Vector<T>& coordVec ) : Vector<T>( coordVec ) { }
        
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
        
        const T& operator[]( Enum en ) const
        {
            int index = (int)en;
            return (*this)[index];
        }
        
        T& operator[]( Enum en )
        {
            int index = (int)en;
            return (*this)[index];
        }
};

#endif //__ACCEL_DATA_H__
