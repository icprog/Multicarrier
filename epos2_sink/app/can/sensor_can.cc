#include <smart_data.h>

using namespace EPOS;

int main()
{
    // Construct your Smart Data here

	Smart_Data<Mass_Sensor>* mass = new Smart_Data<Mass_Sensor>(0, 1000000, Smart_Data<Mass_Sensor>::Mode::ADVERTISED);
    // Do not change below
    Thread::self()->suspend();

    return 0;
}
