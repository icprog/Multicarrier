#include <lin.h>
#include <gpio.h>
#include <uart.h>
#include <alarm.h>
#include <thread.h>
#include <semaphore.h>
#include <utility/queue.h>
#include <utility/vector.h>
#include <utility/malloc.h>
#include <utility/ostream.h>

using namespace EPOS;
	
OStream cout;

UART uart_hdr(0, 4800, 16, 0, 0);
UART uart_msg(0, 4800, 8, 0, 1);

GPIO Pin_EN  ('C', 1, GPIO::OUT, GPIO::UP, 0);
GPIO Pin_INH ('C', 0, GPIO::OUT, GPIO::UP, 0);
GPIO Pin_LED ('C', 3, GPIO::OUT);

void teste(bool);
void send_char();

int main()
{	
	//teste(true);
	teste(false);
    return (0);
}

void teste(bool m){
		
//for (volatile int i = 0; i < 90; i++) {

	if(m){
		
		int sync;
		char msg;

	    while(1) {

	    	msg = '!';
	    	static int i=0;

			cout << "Habilita Chip" << endl;
			Pin_EN.set(1);
			Pin_LED.set(1);

			cout << "Envia sync brake" << endl;
			sync = 0x00;
			uart_hdr.put(sync);
			uart_hdr.put(sync);

			cout << "Envia sync field" << endl;
			sync = 0x55;
			uart_msg.put(sync);

			cout << "Envia mensagem" << endl;			
			uart_msg.put(msg + i);
			cout << "enviou: " << msg + i << endl; 

			if (i==92)
				i=0;
			i++;

			Alarm::delay(100000);
/*
			cout << "Desabilita Chip" << endl;
			Pin_EN.set(0);
			Pin_LED.set(0);
			//send_char();
			Alarm::delay(10000);
*/
		}

	} else {

		Pin_EN.set(1);
		Pin_LED.set(1);
		while(1) {
			cout << "recebeu: " << uart_msg.get() << endl;
		}

	}
}

void send_char(){
	;
}
