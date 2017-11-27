 
#include <utility/ostream.h>
#include <gpio.h>

using namespace EPOS;

OStream cout;

int main(){
    
    ADC adc(ADC::SINGLE_ENDED_ADC7);
    
    while(1){
        
        for(volatile int i = 0; i < 0xfffff; i++);
        
        cout << adc.read() << endl;
    }
    
}
