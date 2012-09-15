#ifndef __COORDINATE_SYSTEM_H__
#define __COORDINATE_SYSTEM_H__

#include "Vector.h"
#include "BasisVector.h"

#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

class CoordinateSystem
{
    public:
        
        // Get the basis vector's of the local coordinate system
        // (relative to vehicle coordinate system)
        virtual BasisVector GetBasisVector() const = 0;
        
        // Normalize the coordinate data to the vehicle coordinate system
        template <class T>
        static Vector<T> ChangeBasis( const Vector<T>& coordVec
                                       ,BasisVector& basis );
};

template <class T>
Vector<T> CoordinateSystem::ChangeBasis( const Vector<T>& coordVec
                                          ,BasisVector& basis )
{
    ///////////////////////////////////////////////////////////////////////////
    //  Algorithm: Change basis to/from vehicle coords
    //
    //  | 0  1  0 |
    //  | 1  0  0 | --> [ 2  1  -3 ]  = B, or the basis map    
    //  | 0  0 -1 |
    //
    //  x = [ x1  x2  x3  ], original coord vector
    //  r = [ x1' x2' x3' ], vehicle coord vector
    //  
    //  r[i] = x[j]*( B[i] / (j+1) ), if j+1 == abs(B[i])
    //  i = 0, 1, 2, 3
    //  j = 0, 1, 2, 3
    //
    // Basis contains index and multiplier.
    //  Since |X| = 1, |Y| = 2, |Z| = 3,
    //  and multiplier is 1 or -1, the
    //  basis maps it's index to the axis
    //  stored at that index.
    //
    // Ex. if basis.Basis[0] == -3, then
    //  the X-coordinate corresponds to the -Z
    //  axis of the vehicle and thus:
    //  result[ abs(-3)-1 ] = result[2] = coord[0]*(-3 / abs(-3))
    //
    // So, when we are at index 2 of the coord vector,
    //  ( which is in general, is when j+1 == abs(basis.Basis[i]) )
    //  perform the mapping.
    //
    //(This algorithm reduces the overhead of creating a 3x3 transformation
    //  matrix because only the diagonal entries matter)
    ///////////////////////////////////////////////////////////////////////////
    // Create coord NVector
    Vector<T> resultVec;
                    
    // Loop through basis vector
    for( int i = 0; i < 3; ++i )
    {
        // Loop through coordinate vector
        for( int j = 0; j < 3; ++j )
        {
            if( (j+1) == abs(basis[i]) )
            {
                resultVec[i] = coordVec[j]*(basis[i] / (j+1));
            }
        }
    }
    
    return resultVec;
}

#endif //__COORDINATE_SYSTEM_H__
