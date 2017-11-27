#include <thread.h>
#include <smart_data.h>

using namespace EPOS;

int main()
{
    // Local data source, advertised to the network
    Temperature_Demonstration t(0, 1000000, Temperature_Demonstration::ADVERTISED);
    t = 10;

    Thread::self()->suspend();

    return 0;
}
