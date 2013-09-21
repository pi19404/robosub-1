#ifndef __Array_H__
#define __Array_H__

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
		size_t MaxSize();
		size_t Size();

	private:
		bool Grow( size_t );
		bool Shrink( size_t );

	private:
		T*     mArray;
		size_t mNumElements;
		size_t mSize;
};

#include "Array.cpp"

#endif //__Array_H__
