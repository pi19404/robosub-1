
CompassBases::CompassBases()
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

BasisVector CompassBases::LevelOrientation() { return mLevelOrientation; }
BasisVector CompassBases::UprightEdgeOrientation() { return mUprightEdgeOrientation; }
BasisVector CompassBases::UprightFrontOrientation() { return mUprightFrontOrientation; }
BasisVector operator[]( Enum orientation )
{        
	if( orientation == OrientationLevel ) { return mLevelOrientation; }
	else if( orientation == OrientationUprightEdge ) { return mUprightEdgeOrientation; }
	else { return mUprightFrontOrientation; }
}