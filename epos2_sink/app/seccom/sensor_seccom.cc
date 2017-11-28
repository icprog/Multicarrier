#include <smart_data.h>
#include <gpio.h>
#include <utility/ostream.h>

using namespace EPOS;

int main()
{
    // Instantiate your SmartData here
    Switch_Sensor s(0, 'C', 3, GPIO::OUT);
    Smart_Data<Switch_Sensor>* mass = new Smart_Data<Mass_Sensor>(0, 1000000, Smart_Data<Mass_Sensor>::Mode::ADVERTISED);

    Thread::self()->suspend();

    return 0;
}
