#include <gpio.h>
#include <alarm.h>
#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

int main()
{
    
    GPIO g('C',3, GPIO::OUT);
    for(bool b=false;;b=(b+1)%2)
    {
        cout << "teste\n";
        g.set(b);
        for(volatile int t=0;t<0xfffff;t++);
    }
    return 0;
}
