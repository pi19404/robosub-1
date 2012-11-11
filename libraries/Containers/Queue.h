#ifndef __QUEUE_H__
#define __QUEUE_H__

template <class T>
class Queue
{
	private:
		struct Node 
			{
			explicit Node( const T& val = T() ) 
				: 
				mVal(val),
				mNext(0),
				mPrev(0)
			{ }

			T            mVal;
			struct Node* mNext;
			struct Node* mPrev;
		};

	Node* mHead;
	Node* mTail;

	public:
		Queue()
			:
			mHead(0),
			mTail(0)
		{ }

		~Queue();
		bool Empty();
		void Enqueue( const T& val );
		T Dequeue();

	private:
		void Clear();
		Node* MakeNode( const T& val );
};

#include "Queue.cpp"

#endif //__QUEUE_H__
