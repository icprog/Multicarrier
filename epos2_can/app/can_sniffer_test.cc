#include <machine.h>
#include <alarm.h>
#include <can.h>
#include <gpio.h>
#include <cpu.h>
#include <utility/ostream.h>

using namespace EPOS;

OStream cout;

void loop_can_getstatus();
void read_buffer(CAN *can);

int main()
{
    Alarm::delay(10000);
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
    can.config_filters(mode); //Listen-Only

    can.write(0x0F,0b01100100); //set Listen-Only MODE
    can.write(0x0F,0b01100100); //set Listen-Only MODE 

    //clear all Interrupt flags
    can.bit_modify(0b00101100,0b11111111,0b00000000);
    //Enable Interrupt for RXB0
    can.bit_modify(0b00101011, 0b11111111,0b00000001);
    can.enable_EN(); // MCP2562
    g.set(1);
    while(1) {
        Alarm::delay(10); 
        can.read_register(&Status,0x0F); //read register CTRL
        can.read_register(&Status,0x0E); //read register STATUS
        can.read_register(&Interrupt, 0b00101100);
        if((Interrupt & 0x01)== 0b00000001){
            read_buffer(&can);
            cout<<hex<<(unsigned int) Interrupt<<'\n';
            can.bit_modify(0b00101100,0b11111111,0b00000000);
        }


    }
}

void read_buffer(CAN *can){
    char data[8];
    char ID;
    unsigned int standardID=0;
    can->read_rx_buffer(0,0,&ID);       // read RXB0SIDH
    standardID |= (ID << 3);
    can->read_register(&ID, 0b01100010); // read RXB0SIDL
    standardID |= (ID >> 5);
    can->read_rx_buffer(0,1,data);
    cout<<standardID<<" ";
    for(int i=0;i<8;i++){
        cout <<hex<<(unsigned int) data[i];
        cout << " ";
    }
    cout<<"   ";
}