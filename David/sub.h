#include <boost/asio.hpp>
#include <stdint.h>
#include <string>

class Sub
{
public:
	// command data
	uint8_t pneumatics, x,y,z,heading;

	// receive data
	uint8_t rStatus;
	int16_t rHeading, rDepth;

private:

	std::string comPort;
    std::string baudRate;

	// io service
	boost::asio::io_service io;
	boost::asio::serial_port arduino;

public:

	Sub( std::string // com port
		,std::string // baud rate
		);

	void update();
};
