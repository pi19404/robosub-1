#include "sub.h"

#include <iostream>
#include <stdlib.h>
#include <assert.h>
#define B_SZ 5

using namespace std;
using namespace boost::asio;

Sub::Sub( string port, string rate )
	:
	 pneumatics()
	,x()
	,y()
	,z()
	,heading()
	,rStatus()
	,rHeading()
	,rDepth()
	,comPort(port)
	,baudRate(rate)
	,arduino(io)
{

}

void Sub::update()
{
	char  sendBuf[B_SZ]
		 ,recvBuf[B_SZ];

	for( int i = 0; i < B_SZ; ++i ){ sendBuf[B_SZ] = recvBuf[B_SZ] = 0; }
	
	try {
		arduino.open( comPort );
		arduino.set_option( serial_port_base::baud_rate( atoi( baudRate.c_str()  ) ) );
		arduino.set_option( serial_port_base::flow_control( serial_port_base::flow_control::none ));
		arduino.set_option( serial_port_base::stop_bits( serial_port_base::stop_bits::one ));

	}
	catch(... ){
		cerr << "Could not open port: " << comPort;
		cerr << endl;
		assert( false );
	}

		// read status, compass heading, 
		arduino.read_some( buffer( recvBuf, 5 ) );

		// update received variables
		rStatus  = recvBuf[0];
		assert( recvBuf[0] == 'A' );
		rHeading = ( recvBuf[1] << 8 ) + recvBuf[2];
		rDepth   = ( recvBuf[3] << 8 ) + recvBuf[4];

		// update response variables
		sendBuf[0] = pneumatics;
		sendBuf[1] = x;
		sendBuf[2] = y;
		sendBuf[3] = z;
		sendBuf[4] = heading;
		arduino.write_some( buffer( sendBuf, 5 ) );
} 
