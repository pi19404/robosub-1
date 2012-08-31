#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "..\new\new.h"

template <class T>
class Queue
{
    private:
        struct Node {
            explicit Node( const T& val = T() ) 
                : 
                 mVal(val)
                ,mNext(0)
                ,mPrev(0)
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
             mHead(0)
            ,mTail(0)
        { }
        
        ~Queue()
        {
            Clear();
        }
        
        bool Empty()
        {
            return !(mHead || mTail);
        }
        
        void Enqueue( const T& val )
        {
            // Case: no Elements
            if( Empty() )
            {
                // Make new
                mHead = MakeNode(val);
                mTail = mHead;
            }
            // Case: one element
            else if( mTail == mHead )
            {
                // Make new
                Node* tmp = MakeNode(val);
                
                if( tmp )
                {
                    mHead = tmp;
                    mHead->mNext = mTail;
                    mTail->mPrev = mHead;
                }
            }
            // Case: more than one element
            else
            {
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
        
        T Dequeue()
        {
            // Case: at least one node
            if( !Empty() )
            {
                // Case: only one node
                if( mTail == mHead )
                {
                    T val = mTail->mVal;
                    delete mTail;
                    mHead = mTail = 0;
                    return val;
                }
                // More than one:
                else
                {
                    Node* tmp = mTail;
                    T val = tmp->mVal;
                    mTail->mPrev->mNext = mTail->mNext;
                    mTail = mTail->mPrev;
                    delete tmp;
                    return val;
                }
            }
            return T();
        }
        
    private:
        
        void Clear()
        {
           while( !Empty() ){ Dequeue(); }
        }
        
        Node* MakeNode( const T& val )
        {
            return new Node(val);
        }
};

#endif //__QUEUE_H__