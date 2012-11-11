Queue::~Queue()
{
	Clear();
}

bool Queue::Empty()
{
	return !(mHead || mTail);
}

void Queue::Enqueue( const T& val )
{
	if( Empty() )
	{
		// Case: no Elements

		// Make new
		mHead = MakeNode(val);
		mTail = mHead;
	}
	else if( mTail == mHead )
	{
		// Case: one element

		// Make new
		Node* tmp = MakeNode(val);

		if( tmp )
		{
			mHead = tmp;
			mHead->mNext = mTail;
			mTail->mPrev = mHead;
		}
	}
	else
	{
		// Case: more than one element

		// Make new
		Node* newNode = MakeNode( val );

		// Insert at beginning of buffer
		if( newNode )
		{
			newNode->mNext = mHead;
			mHead->mPrev = newNode;
			mHead = newNode;
		}
	}
}

T Queue::Dequeue()
{
	// Case: at least one element
	if( !Empty() )
	{
		if( mTail == mHead )
		{
			// Case: only one element

			T val = mTail->mVal;
			delete mTail;
			mHead = mTail = 0;
			return val;
		}
		else
		{
			// Case: more than one element

			Node* tmp = mTail;
			T val = tmp->mVal;
			mTail->mPrev->mNext = mTail->mNext;
			mTail = mTail->mPrev;
			delete tmp;
			return val;
		}
	}
	
	//??return what??
	return T();
}

void Queue::Clear()
{
	while( !Empty() ){ Dequeue(); }
}

Node* Queue::MakeNode( const T& val )
{
	return new Node(val);
}
