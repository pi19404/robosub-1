#ifndef __VECTOR_H__
#define __VECTOR_H__

#if defined ARDUINO
    #if(ARDUINO >= 100)
        #include <Arduino.h>
    #else
        #include <WProgram.h>
    #endif
#else
    #include <cmath>
#endif

template <class T, int Size = 3>
class Vector {
public:
        // Default Constructor
        explicit Vector( T val = T() );
        
        // Same Type and Size Constructor
        Vector( const Vector<T,Size>& vector);
        
        // Different Type, Same Size Constructor
        template <class T2>
        Vector( const Vector<T2,Size>& vector );
        
        // Array Constructor
        Vector( T arr[Size] );
        
        // Destructor
        virtual ~Vector();
        
        // Methods
        T   Magnitude( ) const;
        int GetSize  ( ) const;
        
        // Assignment Operators
        const Vector<T,Size>& operator= ( const Vector<T,Size>& rhs );  
        const Vector<T,Size>& operator+=( const Vector<T,Size>& rhs );
        const Vector<T,Size>& operator-=( const Vector<T,Size>& rhs );
        const Vector<T,Size>& operator*=( const T& rhs );
        const Vector<T,Size>& operator/=( const T& rhs );
        const Vector<T,Size>& operator++();
        const Vector<T,Size>& operator--();
        
        // Arithmetic Operators
        Vector<T,Size> operator++(int) const;
        Vector<T,Size> operator--(int) const;
        Vector<T,Size> operator+( const Vector<T,Size>& rhs ) const;
        Vector<T,Size> operator+() const;
        Vector<T,Size> operator-( const Vector<T,Size>& rhs ) const;
        Vector<T,Size> operator-() const;
        Vector<T,Size> operator*( const T& rhs ) const;
        Vector<T,Size> operator/( const T& rhs ) const;
        
        // List / assignment
        Vector<T,Size>& operator=( const T& rhs );
        Vector<T,Size>& operator,( const T& rhs );
            
        // Subscript Operator
        const T& operator[]( int index ) const;
        T& operator[]( int index );
        
        // Comparison Operators
        bool operator==( const Vector<T,Size>& rhs ) const;
        bool operator!=( const Vector<T,Size>& rhs ) const;
        
  // Private Members
  private: 
    T mVector[Size];
    int mAssignIndex;
};

// Default Constructor
template <class T, int Size>
Vector<T,Size>::Vector( T val )
    :
    mAssignIndex(0)
{
    for( int i = 0; i < Size; ++i )
    {
        mVector[i] = val;
    }
}

// Same Type and Size Constructor
template <class T, int Size>
Vector<T,Size>::Vector( const Vector<T,Size>& vector)
    :
    mAssignIndex(0)
{
    for( int i = 0; i < Size; ++i )
    {
        mVector[i] = vector[i];
    }
}

// Different Type, Same Size Constructor
template <class T, int Size>
template <class T2>
Vector<T,Size>::Vector( const Vector<T2,Size>& vector )
    :
    mAssignIndex(0)
{
    for( int i = 0; i < Size; ++i )
    {
        mVector[i] = static_cast<T>( vector[i] );
    }
}

// Array Constructor
template <class T, int Size>
Vector<T,Size>::Vector( T arr[Size] )
{
    for( int i = 0; i < Size; ++i )
    {
        mVector[i] = arr[i];
    }
}

// Destructor
template <class T, int Size>
Vector<T,Size>::~Vector()
{
    // Nothing to do
}

// Methods
template <class T, int Size>
T Vector<T,Size>::Magnitude( ) const
{
    double res_squared = 0.0;
    for( int i = 0; i < Size; ++i )
    {
        res_squared += pow( abs((double)mVector[i]), 2.0 );
    }
    return T( sqrt(res_squared) );
}

template <class T, int Size>
int Vector<T,Size>::GetSize ( ) const
{
    return Size;
}

// Assignment Operators
template <class T, int Size>
const Vector<T,Size>& Vector<T,Size>::operator=( const Vector<T,Size>& rhs )
{
    if( *this != rhs )
    {
        for( int i = 0; i < Size; ++i )
        {
            mVector[i] = rhs[i];
        }
    }
    return *this;
}

template <class T, int Size>
const Vector<T,Size>& Vector<T,Size>::operator+=( const Vector<T,Size>& rhs )
{
    if( *this != rhs )
    {
        for( int i = 0; i < Size; ++i )
        {
            mVector[i] += rhs[i];
        }
    }
    return *this;
}

template <class T, int Size>
const Vector<T,Size>& Vector<T,Size>::operator-=( const Vector<T,Size>& rhs )
{
    if( *this != rhs )
    {
        for( int i = 0; i < Size; ++i )
        {
            mVector[i] -= rhs[i];
        }
    }
    return *this;
}

template <class T, int Size>
const Vector<T,Size>& Vector<T,Size>::operator*=( const T& rhs )
{
    for( int i = 0; i < Size; ++i )
    {
        mVector[i] *= rhs;
    }
    return *this;
}
template <class T, int Size>
const Vector<T,Size>& Vector<T,Size>::operator/=( const T& rhs )
{
    if( rhs )
    {
        for( int i = 0; i < Size; ++i )
        {
            mVector[i] /= rhs;
        }
    }
    return *this;
}

template <class T, int Size>
const Vector<T,Size>& Vector<T,Size>::operator++()
{
    for( int i = 0; i < Size; ++i )
    {
        ++(*this)[i];
    }
    return *this;
}

template <class T, int Size>
const Vector<T,Size>& Vector<T,Size>::operator--()
{
    for( int i = 0; i < Size; ++i )
    {
        --(*this)[i];
    }
    return *this;
}

// Arithmetic Operators
template <class T, int Size>
Vector<T,Size> Vector<T,Size>::operator++(int) const
{
    Vector<T,Size> result(*this);
    ++result;
    return result;
}

template <class T, int Size>
Vector<T,Size> Vector<T,Size>::operator--(int) const
{
    Vector<T,Size> result(*this);
    --result;
    return result;
}
        
template <class T, int Size>
Vector<T,Size> Vector<T,Size>::operator+( const Vector<T,Size>& rhs ) const
{
    Vector<T,Size> result(*this);
    result += rhs;
    return result;
}

template <class T, int Size>
Vector<T,Size> Vector<T,Size>::operator+() const
{
    Vector<T,Size> result(*this);
    for( int i = 0; i < Size; ++i )
    {
        result[i] = +result[i];
    }
    return result;
}

template <class T, int Size>
Vector<T,Size> Vector<T,Size>::operator-( const Vector<T,Size>& rhs ) const
{
    Vector<T,Size> result(*this);
    result -= rhs;
    return result;
}

template <class T, int Size>
Vector<T,Size> Vector<T,Size>::operator-() const
{
    Vector<T,Size> result(*this);
    for( int i = 0; i < Size; ++i )
    {
        result[i] = -result[i];
    }
    return result;
}

template <class T, int Size>
Vector<T,Size> Vector<T,Size>::operator*( const T& rhs ) const
{
    Vector<T,Size> result(*this);
    result *= rhs;
    return result;
}


template <class T, int Size>
Vector<T,Size> Vector<T,Size>::operator/( const T& rhs ) const
{
    Vector<T,Size> result(*this);
    result /= rhs;
    return result;
}


// List assignment
template <class T, int Size>
Vector<T,Size>& Vector<T,Size>::operator=( const T& rhs )
{
    mAssignIndex = 0;
    this->mVector[mAssignIndex++] = rhs;
    return *this;
}

template <class T, int Size>
Vector<T,Size>& Vector<T,Size>::operator,( const T& rhs )
{
    if( mAssignIndex < Size )
    {
        mVector[mAssignIndex++] = rhs;
    }
    return *this;
}

// Subscript Operator
template <class T, int Size>
const T& Vector<T,Size>::operator[]( int index ) const
{
    // Check indices 
    if( (index >= 0) && (index < Size) )
    {
        return mVector[index];
    }
    else if (index >= 0)
    {
        return mVector[Size-1];
    }
    else if ( ( (index + Size) < Size )&& ( (index + Size) >= 0 ) )
    {
        return mVector[index+Size];
    }
    else
    {
        return mVector[0];
    }
}

template <class T, int Size>
T&       Vector<T,Size>::operator[]( int index )
{
    // Check indices 
    if( (index >= 0) && (index < Size) )
    {
        return mVector[index];
    }
    else if (index >= 0)
    {
        return mVector[Size-1];
    }
    else if ( ( (index + Size) < Size )&& ( (index + Size) >= 0 ) )
    {
        return mVector[index+Size];
    }
    else
    {
        return mVector[0];
    }
}

// Comparison Operators
template <class T, int Size>
bool Vector<T,Size>::operator==( const Vector<T,Size>& rhs ) const
{
    for( int i = 0; i < Size; ++i )
    {
        if( mVector[i] != rhs[i] )
        {
            return false;
        }
    }
    return true;
}

template <class T, int Size>
bool Vector<T,Size>::operator!=( const Vector<T,Size>& rhs ) const
{
    return !(*this == rhs);
}


#endif //__VECTOR_H__
