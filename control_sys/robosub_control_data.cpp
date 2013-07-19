///////////////////////////////////////////////////////////////////////////////
// \file: robosub_control_data.cpp
///////////////////////////////////////////////////////////////////////////////
// Author: Jay Sidhu <jaypal.sidhu@gmail.com>
// Project: RoboSub Joystick Communication
// Description: Data exchange class for converting RoboSubCommand objects into
//              control commands that are sent to the arduino. Also wrapper 
//              class for RoboSubControlMessage objects.
//
// Major Changes:
// 01-Jun-2013      JS      Created File.
// 06-Jun-2013      JS      Fixed Scaling.
///////////////////////////////////////////////////////////////////////////////
#include "robosub_control_data.h"

// Math stuff for the
// coordinate transformations
#include <cmath>

// Default Constructor.
RoboSubControlData::RoboSubControlData()
    :
    RoboSubControlCommand()
{

}

// Copy Constructor.
RoboSubControlData::RoboSubControlData( const RoboSubControlData& data )
    :
    RoboSubControlCommand()
{
    Data.Thruster_Fore_L = data.Data.Thruster_Fore_L;
    Data.Thruster_Fore_R = data.Data.Thruster_Fore_R;
    Data.Thruster_Aft_L = data.Data.Thruster_Aft_L;
    Data.Thruster_Aft_R = data.Data.Thruster_Aft_R;
    Data.Thruster_Roll_L = data.Data.Thruster_Roll_L;
    Data.Thruster_Roll_R = data.Data.Thruster_Roll_R;
    Data.Torpedo1_Fire = data.Data.Torpedo1_Fire;
    Data.Torpedo2_Fire = data.Data.Torpedo2_Fire;
    Data.Marker1_Drop = data.Data.Marker1_Drop;
    Data.Marker2_Drop = data.Data.Marker2_Drop;
    Data.Claw_Latch = data.Data.Claw_Latch;
}

// Command Copy Constructor.
RoboSubControlData::RoboSubControlData( const RoboSubCommand& cmd )
    :
    RoboSubControlCommand()
{
    // Get shorter names 
    // for the cmd values.
    double x, // x power of cmd
           y, // y power of cmd
           h, // heading power of cmd
           d; // depth power of cmd
    x = cmd.PowerX;
    y = cmd.PowerY;
    h = cmd.Heading;
    d = cmd.Depth;

    ///////////////////////////////////////////////////////////////////////
    // Non-roll Thrusters
    // The thrusters are mounted at approximately
    // a 45 degree angle on the four corners of the sub,
    // when looking at it in a top-down view.
    // The Y axis (for our XYZ coordinate system) increases out the front
    // of the sub, and X increases out the right of the sub.
    //
    // The goal is use a standard linear combination of PowerY (forward),
    // PowerX (rightward), and Heading (Angular power where 0 is maintain
    // present course, left is negative, and right is positive), for each
    // thruster in the XY grid.
    ////////////////////////////////////////////////////////////////////////
   
    ////////////////////////////////
    // Case: X = 0 = Y
    ////////////////////////////////
    if( (0 == x) && (0 == y) )
    {
        Data.Thruster_Fore_L = 0;
        Data.Thruster_Fore_R = 0;
        Data.Thruster_Aft_L = 0;
        Data.Thruster_Aft_R = 0;
    }
    ///////////////////////////////////////
    // Case: Do Coordinate Transformations
    ///////////////////////////////////////
    else
    {
        // Compute the direction vector
        double r = sqrt( x*x + y*y );
        double theta = atan2(y,x);

        // Precompute some math constants we need
        // making these static const so we can tie them to this
        // method, and only have to compute them once.
        static const double qtr_pi       = 1. *  atan(1.);
        static const double three_qtr_pi = 3. * atan(1.);

        // We want to map the X-Y plane onto a circle that is circumscribed
        // by the plane. However, the process below would make the unit circle
        // the process below will make the circle enclose the plnae, (which is
        // the opposite of what we want). Therefore, use a scale factor to
        // make the circle smaller.
        static const double scale = 2./sqrt(2.);

        // Use the assigned angles for each thruser
        // (these are static because we only need to
        //  compute them once )
        static const double angles[] = { qtr_pi,       /* Fore Left  */
                                         three_qtr_pi, /* Fore Right */
                                         three_qtr_pi, /* Aft Left   */
                                         -three_qtr_pi };//qtr_pi };     /* Aft Right  */

        // Compute the thruster components from the direction vector
        // Since the heading command is presently the
        // magnitude of power for turning, it points along
        // the thrusters. For thrusters fore_l and aft_l the rotation
        // is the same direction as the heading, and for the other two it
        // is the opposite. Adjust accordingly.
        const double headings[] = {  h,   /* Fore Left  */
                                    -h,   /* Fore Right */
                                     h,   /* Aft Left   */
                                    -h }; /* Aft Right  */

        // Get a ptr to each
        // thruster data member so we can iterate over them.
        int32_t* thrusters[] = { &(Data.Thruster_Fore_L),
                                 &(Data.Thruster_Fore_R),
                                 &(Data.Thruster_Aft_L ),
                                 &(Data.Thruster_Aft_R ) };

        for( unsigned int i=0; i < sizeof(thrusters)/sizeof(int32_t*); ++i )
        {
            // Map the X-Y plane to the circle
            double val   = r * cos( theta - angles[i] ) * scale;
            
            // add in the heading power (which adjusts the 
            // raw pwms, no transformations here
            val +=  headings[i];

            // update the thruster data
            *(thrusters[i]) = static_cast<int32_t>(val);

            // Scale the thruster values
            if( *(thrusters[i]) < -100 )
            {
               *(thrusters[i]) = -100;
            }
            else if( *(thrusters[i]) > 100 )
            {
               *(thrusters[i]) = 100;
            }
        }

    }
    
    //////////////////////////////////////////
    // Roll Thrusters
    // Just apply the depth power to each one
    //////////////////////////////////////////
    Data.Thruster_Roll_L = static_cast<int32_t>( d );
    Data.Thruster_Roll_R = static_cast<int32_t>( d );

    ////////////////////////////
    // Pneumatics
    // copy the values directly
    ////////////////////////////
    Data.Torpedo1_Fire = cmd.Torpedo1_Fire;
    Data.Torpedo2_Fire = cmd.Torpedo2_Fire;
    Data.Marker1_Drop  = cmd.Marker1_Drop;
    Data.Marker2_Drop  = cmd.Marker2_Drop;
    Data.Claw_Latch    = cmd.Claw_Latch;
}

// Assignment Operator
RoboSubControlData& RoboSubControlData::operator=( 
                                                const RoboSubControlData& rhs )
{
    if( this != &rhs )
    {
        Data.Thruster_Fore_L = rhs.Data.Thruster_Fore_L;
        Data.Thruster_Fore_R = rhs.Data.Thruster_Fore_R;
        Data.Thruster_Aft_L = rhs.Data.Thruster_Aft_L;
        Data.Thruster_Aft_R = rhs.Data.Thruster_Aft_R;
        Data.Thruster_Roll_L = rhs.Data.Thruster_Roll_L;
        Data.Thruster_Roll_R = rhs.Data.Thruster_Roll_R;
        Data.Torpedo1_Fire = rhs.Data.Torpedo1_Fire;
        Data.Torpedo2_Fire = rhs.Data.Torpedo2_Fire;
        Data.Marker1_Drop = rhs.Data.Marker1_Drop;
        Data.Marker2_Drop = rhs.Data.Marker2_Drop;
        Data.Claw_Latch = rhs.Data.Claw_Latch;
    }
    return *this;
}

// Equivalence Operator
bool RoboSubControlData::operator==( const RoboSubControlData& rhs )
{
    return  ( Data.Thruster_Fore_L == rhs.Data.Thruster_Fore_L ) &&
            ( Data.Thruster_Fore_R == rhs.Data.Thruster_Fore_R ) &&
            ( Data.Thruster_Aft_L == rhs.Data.Thruster_Aft_L ) &&
            ( Data.Thruster_Aft_R == rhs.Data.Thruster_Aft_R ) &&
            ( Data.Thruster_Roll_L == rhs.Data.Thruster_Roll_L ) &&
            ( Data.Thruster_Roll_R == rhs.Data.Thruster_Roll_R ) &&
            ( Data.Torpedo1_Fire == rhs.Data.Torpedo1_Fire ) &&
            ( Data.Torpedo2_Fire == rhs.Data.Torpedo2_Fire ) &&
            ( Data.Marker1_Drop == rhs.Data.Marker1_Drop ) &&
            ( Data.Marker2_Drop == rhs.Data.Marker2_Drop ) &&
            ( Data.Claw_Latch == rhs.Data.Claw_Latch );
}

// Inequivalence Operator
bool RoboSubControlData::operator!=( const RoboSubControlData& rhs )
{
    return !(*this == rhs);
}


std::ostream& operator<<( std::ostream& os,
                                 const RoboSubControlData& rhs )
{
    os << "RoboSubControlData: \n";
    os << "{\n";
    os << "  Thruster_Fore_L: " << rhs.Data.Thruster_Fore_L << "\n";
    os << "  Thruster_Fore_R: " << rhs.Data.Thruster_Fore_R << "\n";
    os << "  Thruster_Aft_L: " << rhs.Data.Thruster_Aft_L << "\n";
    os << "  Thruster_Aft_R: " << rhs.Data.Thruster_Aft_R << "\n";
    os << "  Thruster_Roll_L: " << rhs.Data.Thruster_Roll_L << "\n";
    os << "  Thruster_Roll_R: " << rhs.Data.Thruster_Roll_R << "\n";
    os << "  Torpedo1_Fire: " << rhs.Data.Torpedo1_Fire << "\n";
    os << "  Torpedo2_Fire: " << rhs.Data.Torpedo2_Fire << "\n";
    os << "  Marker1_Drop: " << rhs.Data.Marker1_Drop << "\n";
    os << "  Marker2_Drop: " << rhs.Data.Marker2_Drop << "\n";
    os << "  Claw_Latch: " << rhs.Data.Claw_Latch << "\n";
    os << "}" << std::endl;

    return os;
}

// FromCommand
// \brief sets the data from a command
void RoboSubControlData::FromCommand( const RoboSubCommand* cmd )
{
    if( cmd )
    {
        // Leverage copy constructor
        *this = *cmd;
    }
}

