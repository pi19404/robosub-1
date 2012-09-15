#ifndef __BASIS_VECTOR_H__
#define __BASIS_VECTOR_H__

#include "Vector.h"

namespace Axis {
enum Enum {
     Xpos =  1
    ,Ypos =  2
    ,Zpos =  3
    ,Xneg = -1
    ,Yneg = -2
    ,Zneg = -3
};
}

// Defines Mapping Between
// 3-Axis Orthogonal Bases
//
// Ex:
// | -1  0  0 |
// |  0  0  1 | = [ -1  3  2 ] :  X -> -X', Y -> Z', Z -> Y'
// |  0  1  0 | 
//
class BasisVector
        : public Vector<int,3>
{
    public:
        BasisVector() : Vector<int,3>()
        {
            using namespace Axis;
            // Init to Std Basis Vector: <1,2,3> which maps
            // X->X, Y->Y, Z->Z
            (*this).Vector<int,3>::operator[](0) = (int)Xpos;
            (*this).Vector<int,3>::operator[](1) = (int)Ypos;
            (*this).Vector<int,3>::operator[](2) = (int)Zpos;
        }
        
        BasisVector( const BasisVector& vec ) : Vector<int,3>(vec) { }
        
        ~BasisVector() {}
    
        // Flip Direction of Basis Axis
        void MirrorAxis( Axis::Enum axis )
        {
            int&(Vector<int,3>::*vecIndex)(int) = &Vector<int,3>::operator[];
            int index = abs((int)axis);
            index -= 1;
            (this->*vecIndex)( index ) = -(*this)[index];
        }
        
        // Interchange Two Axes
        void InterchangeAxes( Axis::Enum axis1, Axis::Enum axis2 )
        {
             int&(Vector<int,3>::*vecIndex)(int) = &Vector<int,3>::operator[];
             
             int index1 = abs((int)axis1)-1;
             int index2 = abs((int)axis2)-1;
             
             if( axis1 < 0 ){ MirrorAxis( axis1 ); }
             if( axis2 < 0 ){ MirrorAxis( axis2 ); }
             
             int temp = (*this)[ index1 ];
             (this->*vecIndex)( index1 ) = (*this)[ index2 ];
             (this->*vecIndex)( index2 ) = (int)temp;
        }
        
        using Vector<int,3>::GetSize;
        
        int operator[]( int index ) 
        {
            return (*this).Vector<int,3>::operator[]( index );
        }
        
        const BasisVector& operator=( const BasisVector& rhs )
        {
            if( *this != rhs )
            {
                (*this) = rhs;
            }
            return *this;
        }
};

#endif //__BASIS_VECTOR_H__
