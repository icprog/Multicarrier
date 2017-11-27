 

#include <alarm.h>
#include <spi.h>
#include <utility/ostream.h>
#include <gpio.h>
#include <uart.h>

using namespace EPOS;

OStream cout;

#define MFRC522_write 0
#define MFRC522_read 1

unsigned int MFRC522_C(bool mode, unsigned int address);

unsigned int spi_com(SPI& spi, unsigned int output);

int main(){
    
    // SCK - PA2
    // SDA - PA3
    // MISO - PA4
    // MOSI - PA5
    SPI spi(0, Traits<CPU>::CLOCK, SPI::FORMAT_MOTO_1, SPI::MASTER, 1000000, 8);
    
    // RX - PA0
    // TX - PA1
    UART u(0, 115200, 8, 0, 1);
    
    Alarm::delay(500);
    
    unsigned char i = 10;
    unsigned char j = 0;
    unsigned char prev = -1;

    while(1) {
        
        u.put(j);
        
        cout << "UART sent: " << j << endl;
        
        spi_com(spi, MFRC522_C(MFRC522_write, i));
        
        Alarm::delay(500);
        
        spi_com(spi, 32);
        
        Alarm::delay(500);
        
        for(int t = 0; t <= 20; t++){
        
            spi_com(spi, MFRC522_C(MFRC522_read, i));
            
            Alarm::delay(500);
            
            cout << "SPI read: " << prev << ": " << spi_com(spi, 0) << endl;
            
            Alarm::delay(500);
        }
        
        Alarm::delay(500);
        
        prev = i;
        
        i++; j++;
        
        if(i > 20) i = 10;
        
        if(u.ready_to_get()) cout << "UART received: " << (unsigned int) u.get() << endl;
        
        Alarm::delay(100000);
        
    }
}

unsigned int MFRC522_C(bool mode, unsigned int address){
    
    if(address >= 64) address = 0;
    
    return (mode ? 128:0) | (address << 1);
}
    
unsigned int spi_com(SPI& spi, unsigned int output){
    
    spi.put_data_non_blocking(output);
        
    unsigned int tmp = spi.get_data();
    
    return tmp;
}
