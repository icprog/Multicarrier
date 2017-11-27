 
#include <utility/ostream.h>
#include <gpio.h>

using namespace EPOS;

OStream cout;

int main(){
    
    GPIO g('D', 5, GPIO::IN);
    
    while(1){
        
        cout << g.get();
        
        for(volatile int t=0;t<0xfffff;t++);
    }
    
}
