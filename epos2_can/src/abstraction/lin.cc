// EPOS Alarm Abstraction Implementation

#include <lin.h>

// Methods
LIN::LIN(bool addr_user,unsigned int data_frame): UART(0, 4800, 8, 0, 1)
{
	if(addr_user==1)MODE=MASTER;
	else MODE=SLAVE;

	ADDR_SEND=ADDR_MASTER; // Identificador de envio de dados padrão (ADDR MASTER)
	DATA_BYTE=2;
	
	EN  = new GPIO('C', 1, GPIO::OUT, GPIO::UP, 0); //Enable CI
	INH  = new GPIO('C', 0, GPIO::IN);
	State = STANDBY;
	N_app=0;
};

LIN::~LIN(){
	delete EN;
	delete INH;
};
//*************************** SET LIN APP ********************************/
//Esta função adiciona 1 à 1 os emissores e receptores para cada ID (de aplicação) na tabela do Protocolo

/*Um nó mestre deve conhecer todos os destinos das mensagens enviadas ao barramento assim como o tempo de execução das tarefas de cada aplicação. Por outro lado o nó escravo é responsavel somente por sí próprio e tambem deve conter em seu registro as devidas ações para cada ADDR recebida (recebimento, envio, descarte). Estas escolhas são feitas pelo usuário*/

// Esta função recebe as tarefas por ordem (Uso exclusivo do MASTER). Para o nó mestre, as primeiras a serem executadas são as primeiras a serem adicionadas.
int LIN::add_assignment_lintable(char addr_app, char addr_sender, char addr_receiver, long int time){
	static int count_pos=0;
	if(MODE==SLAVE){
		return -1; // Não é permitido o uso desta função (Tabela Slave)
	}
	if(MODE==MASTER && (addr_app==0 || addr_sender==0 || addr_receiver==0 || time==0)){
		return 0;
	}
	if(count_pos>=59){return 0;}
	Lin_table[count_pos].app=addr_app;				//aplicação (header Frame)
	Lin_table[count_pos].sender=addr_sender;		//remetente (Savar endereços para resolução de conflitos)
	Lin_table[count_pos].receivier=addr_receiver;	//destinatário (Savar endereços para resolução de conflitos)
	Lin_table[count_pos].time=time;					//tempo espera (Evita sobrescrever Frames no barramento)
	count_pos++;
	return 1;
}

//*************************** NORMAL MODE ********************************/

void LIN::clear_buffer(){
	char exclude;
	while(ready_to_get()){
		exclude=UART::get();
	}
}
	

int LIN::send_header(int assignment){
	if(State!=NORMAL){
		State=NORMAL;
		EN->set(1);
	}
	if(MODE==MASTER){
		UART::put(SYNC_BREAK);
		UART::put(SYNC_BREAK);
		UART::put(SYNC_FIELD);
		UART::put(get_addr_app(assignment));
		return 1;
	}
	else return 0;
}

int LIN::receive_header(char* assignment){
	char verification=0;
	if(State!=NORMAL){ // TENTAR MODIFICAR PARA DESPERTAR SOMENTE QUANDO RECEBER O DADO
		State=NORMAL;
		EN->set(1);
	}
	while(verification!=SYNC_FIELD){
		verification=UART::get();
	}

	*assignment=UART::get();
	return 1;
}

int LIN::send(char* data){
	if(data==0){
		return 0;
	}
	for(int i=0;i<DATA_BYTE;i++){ //sendo o menor DATA_BYTE==2 não há preocupação com valor menor
		UART::put(data[i]);
	}
	UART::put(CHECKSUM);
	return 1;
}

int LIN::receive(char* data){
	char verification;
	for(int i=0;i<DATA_BYTE;i++){ //sendo o menor DATA_BYTE==2 não há preocupação com valor menor
		data[i]=UART::get();
	}
	verification=UART::get();
	return 1;
}

