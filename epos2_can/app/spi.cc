
#include <alarm.h>
#include <spi.h>
#include <utility/ostream.h>
#include <gpio.h>

using namespace EPOS;

//OStream cout;

#define MFRC522_write 0
#define MFRC522_read 1

unsigned int MFRC522_C(bool mode, unsigned int address);

unsigned int spi_com(SPI& spi, unsigned int output);

int main(){
    
    SPI spi(0, Traits<CPU>::CLOCK, SPI::FORMAT_MOTO_1, SPI::MASTER, 1000000, 8);
    
    Alarm::delay(500);
    
    unsigned char i = 10;
    unsigned char prev = -1;

    while(1) {
        
        spi_com(spi, MFRC522_C(MFRC522_write, i));
        
        Alarm::delay(500);
        
        spi_com(spi, 21);
        
        Alarm::delay(500);
        
        for(int j = 0; j <= 20; j++){
        
            spi_com(spi, MFRC522_C(MFRC522_read, i));
            
            Alarm::delay(500);
            
            //cout << "value " << prev << ": " << spi_com(spi, 0) << endl;
            
            //cout << MFRC522_C(MFRC522_read, i) << endl;
            
            Alarm::delay(500);
        }
        
        Alarm::delay(500);
        
        prev = i;
        
        i++;
        
        if(i > 60) i = 10;
        
        Alarm::delay(100000);
        
    }
}

unsigned int MFRC522_C(bool mode, unsigned int address){
    
    assert(address < 64);
    
    return (mode ? 0x80:0) | (address << 1);
}
    
unsigned int spi_com(SPI& spi, unsigned int output){
    
    spi.put_data_non_blocking(output);
        
    unsigned int tmp = spi.get_data();
    
    return tmp;
}
