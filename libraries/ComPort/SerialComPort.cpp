SerialComPort::SerialComPort
	( 
		HardwareSerial& serial, 
		uint8_t device, 
		unsigned long timeOutMillis 
	)
	:
	ComPort(device),
	mSerial(serial),
	mTimeOut(timeOutMillis)
{
	// Nothing left to do here.
}

SerialComPort::~SerialComPort()
{
	// Nothing left to do here.
}

void SerialComPort::write(uint8_t *data, uint8_t nBytes)
{
	for( int i = 0; i < nBytes; ++i )
	{
		mSerial.write(data[i]);
	}
}

void SerialComPort::read(uint8_t *data, uint8_t nBytes)
{    
	read(0, 0, data, nBytes);
}

void SerialComPort::read(uint8_t* wrdata, uint8_t w_nBytes, uint8_t* rddata, uint8_t r_nBytes)
{
	// Do write part //??why write in read??
	for( int i = 0; i < w_nBytes; ++i )
	{
		mSerial.write( wrdata[i] );
	}

	// Do read part
	int i;
	for( i = 0; i < r_nBytes; ++i )
	{   
		while( !mSerial.available() );
		rddata[i] = mSerial.read();
	}
}
