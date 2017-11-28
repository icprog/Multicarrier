#include <smart_data.h>
#include <utility/ostream.h>
#include <thread.h>

using namespace EPOS;



int main(){

    //GPIO led('C',3, GPIO::OUT);

    // create the spot smart data on the device
	new CAN_Data(0, 1000000, CAN_Data::ADVERTISED);
	
    Thread::self()->suspend();

    return 0;
}

