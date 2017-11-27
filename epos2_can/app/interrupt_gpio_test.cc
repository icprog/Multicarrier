/* TESTE INTERRUPÇÃO POR GPIO */

#include <machine.h>
#include <alarm.h>
#include <gpio.h>
#include <cpu.h>
#include <utility/ostream.h>

using namespace EPOS;

OStream cout;


/* Interrupt_Id é gerado na inicialização da GPIO cado Interrupt_Handler seja passado como parâmetro*/
void handler_GPIO(const unsigned int&) 
{
	GPIO g('C', 3, GPIO::OUT);
	static bool cont=false;
	if(cont==true){
		g.set(0);
		cont=false;
	}
	else{
		g.set(1);
		cont=true;
	}
    cout<<"INTERRUPÇÃO GERADA!!"<<'\n';   
}

int main()
{
    GPIO pin_('A', 6, GPIO::IN, GPIO::FLOATING, &handler_GPIO);
    Alarm::delay(100);
    pin_.int_enable();
    //pin_.int_enable(GPIO::RISING);
    while(1){;}
    return 0;
}

