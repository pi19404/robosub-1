#ifndef __GYRO_DATA_H__
#define __GYRO_DATA_H__

#include "..\Math\Vector.h"

template <class T>
class GyroData
    : 
    public Vector<T>
{
    public:
        enum Enum {
             X
            ,Y
            ,Z
        };
    public: 
        GyroData() : Vector<T>() { }
        GyroData( const GyroData<T>& tiltData ) : Vector<T>( tiltData ) { }
        GyroData( const Vector<T>& coordVec ) : Vector<T>( coordVec ) { }
        
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

#endif //__GYRO_DATA_H__