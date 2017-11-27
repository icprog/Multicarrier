#include <machine.h>
#include <alarm.h>
#include <can.h>
#include <gpio.h>
#include <cpu.h>
#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

void loop_can_getstatus();
void transmitCANMessage(CAN *can);

int main()
{
    Alarm::delay(10000);

    for(volatile unsigned int t = 0; t < 0xfffff; t++)
                    ;
    for(volatile unsigned int t = 0; t < 0xfffff; t++)
                    ;
    for(volatile unsigned int t = 0; t < 0xfffff; t++)
                    ;
    loop_can_getstatus();
    return 0;
}

void loop_can_getstatus()
{
    int mode=1;
    char Status=0, Interrupt=0;
    GPIO g('C', 3, GPIO::OUT);
    CAN can;
    can.config();
    can.write(0x0F,0b00000111); //set Normal MODE
    can.write(0x0F,0b00000111); //set Normal MODE

    can.enable_EN(); 
    while(1) {
        can.enable_EN();
        transmitCANMessage(&can); 
        Alarm::delay(100);       
    }
}

void tx_2(CAN *can, unsigned int *val, char *data){
  can->load_tx_buffer(0, 0, 0, (char *)val);
  can->load_tx_buffer(0, 0, 1, data);
  can->request_to_send(true,false,false); //Buffer0,Buffer1,Buffer2 
}

void transmitCANMessage(CAN *can)
{
  unsigned long int startTime, endTime;
  bool sentMessage, rtr;
  unsigned int val;
  int i;
  char dataLength=0x08;
  unsigned int standardID = 0b0110;
  sentMessage = false;
  char data[8]={0xff,0xee,0xdd,0xcc,0xbb,0xaa,0x99,0x88};

  //Write standard ID registers
  val = standardID >> 3;
  can->write(0b00110001,val); //TXB0SIDH
  val = standardID << 5;
  can->write(0b00110010,val);//TXB0SIDL

  val = dataLength & 0x0f;
  
  tx_2(can,&val,data);

  char valor,error;
  sentMessage = false;

  can->bit_modify(0b00110000,0b00001000,0x00); //TXB0CTRL,TXREQ,0

  can->bit_modify(0b00101100,0b00000100,0x00); //TXB0CTRL,TXREQ,1
}