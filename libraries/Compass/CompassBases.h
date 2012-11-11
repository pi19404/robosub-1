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

	enum Enum 
	{
		 OrientationLevel
		,OrientationUprightEdge
		,OrientationUprightFront
	};

	//??previously, constructor appeared immediately after UprightFrontOrientation(); is the new configuration okay?  I think it is, but I should check??
	CompassBases();
	BasisVector LevelOrientation(){ return mLevelOrientation; }
	BasisVector UprightEdgeOrientation(){ return mUprightEdgeOrientation; }
	BasisVector UprightFrontOrientation() { return mUprightFrontOrientation; }
	BasisVector operator[]( Enum orientation );

	private:
		BasisVector mLevelOrientation;
		BasisVector mUprightEdgeOrientation;
		BasisVector mUprightFrontOrientation;
};

#endif //__COMPASS_BASES_H__
