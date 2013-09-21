//??why a new template before each function definition??

// Default Constructor
template <class T>
Array<T>::Array( size_t size )
	:
	mArray(0),
	mNumElements(0),
	mSize(0)
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

// Copy Constructor
template <class T>
Array<T>::Array( const Array<T>& rhs )
	:
	mArray(0),
	mNumElements(0),
	mSize(0)
{
	// Use Assignment Operator:
	*this = rhs;
}

// Destructor
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
			//??means...what??
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

//??correct implementation?  Array:: instead of Array<T>:: ??
size_t Array::MaxSize() 
{ 
	return mSize; 
}

size_t Array::Size()
{ 
	return mNumElements; 
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