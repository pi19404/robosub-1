#include "av8r.h"
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <iostream>
#include <iomanip>


using std::string;
using std::stringstream;
using std::runtime_error;

using std::cout;
using std::endl;
using std::cerr;
using std::setw;

namespace av = Av8rEvents;

void update( Av8r& js )
{
	if(	js.Update() > 0 )
	{

			// print buttons:
			cout << "Buttons: \n\t";
			for( int i = av::FIRST_BUTTON; i < av::NUM_BUTTONS; ++i ) 
            {
				cout << static_cast<av::BUTTON>(i) << ": ";
				cout << js.Buttons[i] << " ";
			}

			// print axes:
			cout << "\nAxes: \n\t";
			for( int i = av::FIRST_AXIS; i < av::NUM_AXES; ++i ) 
            {
				cout << static_cast<av::AXIS>(i) << ": ";
			    cout << setw(6);
				cout << js.Axes[i] << " ";
		    }
			cout << endl << endl;
	}
}

int main( int argc, char **argv ) {

	Av8r roboSub;

	if( argc < 2 )
	{
		cout << "Usage: device\n";
		exit(1);
	}

	try {
		string devStr(argv[1], strlen(argv[1]));
		roboSub.Init( devStr );
	}
	catch( runtime_error e ) {
		cerr << e.what() << "\n";
		return 1;
	}

	while( true ) {
		update(roboSub);
	}
}


