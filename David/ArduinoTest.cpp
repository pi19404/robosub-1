#include <cstdlib>
#include <unistd.h>
#include <stdint.h>
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <exception>

using namespace std;
using namespace boost::asio;

#define B_SZ 5
#define T_DEPTH 130
#define UD_SPD   30
#define NUM_THRUSTERS 6

namespace SendIndex {
enum {
	 STATUS = 0
	,X
	,Y
	,Z
	,HDG
};
}

struct ReceiveData
{
	ReceiveData()
		:
		 Status()
		,Heading()
		,Depth()
	{ }

	uint8_t  Status;
	int16_t Heading;
	int16_t Depth;
}; 

int main(int argc, char **argv )
{
	char  sendBuf[B_SZ]
		 ,recvBuf[B_SZ]
		 ,pwmVals[NUM_THRUSTERS];

	ReceiveData adata;
	io_service io;
	serial_port arduino(io);

	for( int i = 0; i < B_SZ; ++i ){ sendBuf[B_SZ] = recvBuf[B_SZ] = 0; }
	for( int i = 0; i < NUM_THRUSTERS; ++i ){ pwmVals[i] = 0; }

	if( argc < 3 )
	{
		cerr << "Usage: COM_PORT BAUD_RATE" << endl;
		return 1;
	}

	try {
		arduino.open( argv[1] );
		arduino.set_option( serial_port_base::baud_rate( atoi( argv[2] ) ) );
		arduino.set_option( serial_port_base::flow_control( serial_port_base::flow_control::none ));
		arduino.set_option( serial_port_base::stop_bits( serial_port_base::stop_bits::one ));

		usleep(3000000);
	}
	catch(... ){
		cerr << "Could not open port: " << argv[1];
		cerr << endl;
		return 1;
	}

	while( true ) {

		// read status, compass heading, 
		arduino.read_some( buffer( recvBuf, 5 ) );
		adata.Status  = recvBuf[0];
		assert( recvBuf[0] == 'A' );
		adata.Heading = ( recvBuf[1] << 8 ) + recvBuf[2];
		adata.Depth   = ( recvBuf[3] << 8 ) + recvBuf[4];

		// Check Depth:
		if( adata.Depth < T_DEPTH ) {
			// go Down
			sendBuf[SendIndex::Z] = UD_SPD;
		}
		else {
			// go Up
			sendBuf[SendIndex::Z] = -UD_SPD;
		}

		stringstream ssInts;
		ssInts << adata.Status;
		cout << "R-> Status: " << ssInts.str();
		ssInts.str(""); ssInts << adata.Heading;
		cout << " Heading: " << ssInts.str();
		ssInts.str(""); ssInts << adata.Depth;
		cout << " Depth: " << ssInts.str();
		cout << endl;
		
		arduino.write_some( buffer( sendBuf, 5 ) );

		usleep(50000);
	}	
}
