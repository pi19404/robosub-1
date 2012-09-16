#ifndef __COMPASS_BASES_H__
#define __COMPASS_BASES_H__

#include "CompassCfgData.h"
#include "../Utility/Singleton.h"
#include "../Math/BasisVector.h"

class CompassBases
    :
    public Singleton<CompassBases>
{
    public:
        virtual ~CompassBases() { }
        
        enum Enum {
             OrientationLevel
            ,OrientationUprightEdge
            ,OrientationUprightFront
        };
        
        BasisVector LevelOrientation(){ return mLevelOrientation; }
        BasisVector UprightEdgeOrientation(){ return mUprightEdgeOrientation; }
        BasisVector UprightFrontOrientation() { return mUprightFrontOrientation; }
        
        CompassBases()
        {
            // Level:  X+ forward, Y+ left, Z+ up
            // We want Y+ forward, X+ right, Z+ up
            mLevelOrientation.InterchangeAxes( Axis::Xpos, Axis::Yneg ); // X+ right, Y+ forward, Z+ up
            
            // Upright Edge: X+ forward, Y+ up   , Z+ right
            // We want       Y+ forward, X+ right, Z+ up
            mUprightEdgeOrientation.InterchangeAxes( Axis::Ypos, Axis::Zpos ); // X+ forward, Y+ right, Z+ up
            mUprightEdgeOrientation.InterchangeAxes( Axis::Xpos, Axis::Ypos ); // Y+ forward, X+ right, Z+ up
            
            // Upright Front: Z+ forward, Y+ left , X+ down
            // We want        Y+ forward, X+ right, Z+ up
            mUprightFrontOrientation.MirrorAxis( Axis::Xpos ); // Z+ fw, Y+ l, X+ u
            mUprightFrontOrientation.InterchangeAxes( Axis::Xpos, Axis::Zpos ); // X+ fw, Y+ l , Z+ u
            mUprightFrontOrientation.InterchangeAxes( Axis::Xpos, Axis::Yneg ); // X+ r , Y+ fw, Z+ u
        }
        
        BasisVector operator[]( Enum orientation )
        {        
            if( orientation == OrientationLevel ){ return mLevelOrientation; }
            else if( orientation == OrientationUprightEdge ) { return mUprightEdgeOrientation; }
            else { return mUprightFrontOrientation; }
        }

    private:
        BasisVector mLevelOrientation;
        BasisVector mUprightEdgeOrientation;
        BasisVector mUprightFrontOrientation;
};

#endif //__COMPASS_BASES_H__
