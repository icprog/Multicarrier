
#include <uart.h>
#include <utility/ostream.h>

using namespace EPOS;

OStream cout;
 
int main(){
    
    // RX - PA0
    // TX - PA1
    UART u(0, 115200, 8, 0, 1);
    
    while(1){
        
        for(volatile int i = 0; i < 255; i++){
            
            for(volatile int j = 0; j < 0xfffff; j++);
            
            u.put(i);
            
            cout << "enviou: " << i << endl;
            
            int value = u.get();
            
            cout << "recebeu: " << value << endl;
            
        }
        
    }
    
}
