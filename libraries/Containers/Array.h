#ifndef __Array_H__
#define __Array_H__

#include "../new/new.h"
#include <stddef.h>

//////////////////////////////////////////////////////////////
// -name Array
// -desc A smart array class
// -desc which automatically changes in size based on
// -desc a user's needs. This occurs when adding/removing
// -desc elements.
//////////////////////////////////////////////////////////////
template <class T>
class Array
{           
    private:
        T*     mArray;
        size_t mNumElements;
        size_t mSize;
        
    public:
        
        // Default Constructor
        explicit Array( size_t size = 1 );
        
        // Copy Constructor
        Array( const Array<T>& );
        
        // Destructor
        ~Array();
        
        // Assignment Operator:
        const Array<T>& operator=( const Array<T>& );
        
        // Indexing Operators
        T& operator[]( size_t );
        const T& operator[]( size_t ) const;
        
        // Stack Methods
        size_t PushBack( const T& );
        T      PopBack();
           
        // Getter Methods
        size_t MaxSize() { return mSize; }
        size_t Size()    { return mNumElements; }
        
    private:
        bool Grow( size_t );
        bool Shrink( size_t );
                
};

template <class T>
Array<T>::Array( size_t size )
    :
     mArray(0)
    ,mNumElements(0)
    ,mSize(0)
{
    if( size == 0 )
    {
        size = 1;
    }
    
    // Keep trying smaller sizes
    while( !Grow( size ) && (size > 1) )
    {
        size >>= 1;
    }    
}

template <class T>
Array<T>::Array( const Array<T>& rhs )
    :
     mArray(0)
    ,mNumElements(0)
    ,mSize(0)
{
    // Use Assignment Operator:
    *this = rhs;
}

template <class T>
Array<T>::~Array()
{
    if( mArray )
    {
        delete[] mArray;
    }
}

template <class T>
const Array<T>& Array<T>::operator=( const Array<T>& rhs )
{
    // Check against self assignment
    if( this != &rhs )
    {
        // Change array size:
        bool result;
        size_t elems = mNumElements;
        mNumElements = 0;
        if( mSize > rhs.mNumElements )
        {
            result = Shrink( rhs.mNumElements );
        }
        else if( mSize < rhs.mNumElements )
        {
            result = Grow( rhs.mNumElements );
        }
        else
        {
            result = true;
        }
        
        // Allocation Succeeded
        if( result )
        {
            // Copy Data
            for( size_t i = 0; i < rhs.mNumElements; ++i )
            {
                mArray[i] = rhs.mArray[i];
            }
            mNumElements = rhs.mNumElements;
        }
        else
        {
            mNumElements = elems;
        }
    }
    return *this;
}

template <class T>
T& Array<T>::operator[]( size_t index )
{
     if( index < mNumElements )
    {
        return mArray[index];
    }
    else if ( mNumElements )
    {
        return mArray[mNumElements-1];
    }
    else
    {
        return mArray[0];
    }
}

template <class T>
const T& Array<T>::operator[]( size_t index ) const
{
     if( index < mNumElements )
    {
        return mArray[index];
    }
    else if ( mNumElements )
    {
        return mArray[mNumElements-1];
    }
    else
    {
        return mArray[0];
    }
}

template <class T>
size_t Array<T>::PushBack( const T& val )
 {
    if( mNumElements < mSize )
    {
        mArray[mNumElements++] = val;
        return mNumElements-1;
    }
    else
    {
        // Try to grow
        bool success = Grow( mNumElements+1 );
        
        if( success )
        {
            mArray[mNumElements++] = val;
            return mNumElements-1;
        }
        else if ( mNumElements )
        {
            mArray[mNumElements-1] = val;
            return mNumElements-1;
        }
        else
        {
            mArray[0] = val;
            return 0;
        }
    }
}

template <class T>
T Array<T>::PopBack()
{
    if( mNumElements )
    {
        --mNumElements;
        T elem = mArray[mNumElements];
        Shrink( mNumElements );
        return elem;
    }
    else
    {
        T result;
        return result;
    }
}

template <class T>
bool Array<T>::Grow( size_t size )
{
    if( size > mSize )
    {
        // Try to double in size:
        size_t newSize = mSize;
        do
        {
            newSize *= 2;
            newSize += 1;
            
        } while ( size > ((newSize * 2)+1) );
        
        // Allocate Memory
        T* newArr = new T[newSize];
        
        // Not enough, shrink
        while( !newArr && (newSize > size) )
        {
            newArr = new T[--newSize];
        }
        
        // Allocation failed
        if( !newArr )
        {
            return false;
        }
        
        // Update size
        mSize = newSize;
        
        // Copy Elements
        for( size_t i = 0; i < mNumElements; ++i )
        {
            newArr[i] = mArray[i];
        }
        
        // Change ptrs
        T* tmp = mArray;
        mArray = newArr;
        if( tmp )
        {
            delete[] tmp;
        }
        
        // succeeded
        return true;
    }
    return false;
}

template <class T>
bool Array<T>::Shrink(size_t size)
{
    if( (size < mSize)&&( mSize > 1) )
    {
        if( size < mNumElements )
        {
            return false;
        }
        
        size_t newSize = mSize;
        do
        {
            newSize >>= 1;
        } while( (newSize >> 1) > size );
        
        T* newArr = new T[newSize];
        
        while( !newArr && (newSize > size) )
        {
            newArr = new T[--size];
        }
        
        if( !newArr )
        {
            return false;
        }
        
        mSize = newSize;
        
        for( size_t i = 0; i < mNumElements; ++i )
        {
            newArr[i] = mArray[i];
        }
        
        T* tmp = mArray;
        mArray = newArr;
        if( tmp )
        {
            delete[] tmp;
        }
        
        return true;
    }
    return false;
}

#endif //__Array_H__
