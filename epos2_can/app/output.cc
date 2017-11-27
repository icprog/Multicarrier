 
#include <utility/ostream.h>
#include <gpio.h>

using namespace EPOS;

int main(){
    
    GPIO g('D', 5, GPIO::OUT);
    
    while(1){
        
        g.set(true);
        
        for(volatile int t=0;t<0xfffff;t++);
        
        g.set(false);
        
        for(volatile int t=0;t<0xfffff;t++);
    }
    
}
