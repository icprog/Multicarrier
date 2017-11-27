// EPOS - LIN 

#pragma once

#include <alarm.h>
#include <gpio.h>
#include <uart.h>
#include <utility/ostream.h>

/*Frame length depending on ID
________________________________________________________
	ID range	|	Frame length		|
________________________________________________________|
0-31	0x00-0x1f	|	     2			|
32-47   0x20-0x2f	|	     4			|
48-63   0x30-0x3f	|	     8			|
________________________________________________________|

60 à 63 (0x3C à 0x3F) são IDs reservados para Diagnostico e/ou aplicações futuras*/

// tipos de transmição
// Unconditional Frames; Event Triggered Frames; Sporadic Frames

//Para economizar energia, os nós escravos serão colocados no modo de suspensão após 4 segundos de inatividade do
// barramento ou se o mestre enviou um comando de suspensão. A ativação do modo de suspensão é feita por um nível
// dominante no barramento que todos os nós podem criar.

/****************************** MACROS LIN bus COMMUNICATION ******************************************/
#define ADDR_MASTER 				0x01 // Aplicação exclusiva para o MASTER (além do cabeçalho envia os dados)
#define SYNC_BREAK					0x00 // tem que ter pelo menos 13bits dominantes 
#define SYNC_FIELD					0x55	
#define CHECKSUM					0xFF // VALOR NÃO CORRESPONDE A UM CHECKSUM REAL
#define DIAGNOSTIC_MASTER_REQUEST 	0X3C // ID do diagnostico solicitado pelo nó mestre, envia frame aos escravos
#define DIAGNOSTIC_MASTER_RESPONSE 	0X3D // ID da resposta do diagnostico solicitado, escravo envia frame ao mestre
#define GO_TO_SLEEP 0x3C //(Diagnostic Request) frame que deve ser enviado com este cabeçalho é-> 0x00,0xff,...,0xff

using namespace EPOS;

class LIN : public UART{
	public:
		LIN(bool,unsigned int); // entre com 0=MASTER/1=SLAVE; 2;4;8=NUMERO DE BYTES para os dados
		~LIN();
		//void routine()const;
		int add_assignment_lintable(char addr_app, char addr_sender, char addr_receiver, long int time);
		int send(char* data);
		int receive(char* data);
		int send_header(int assignment);
		int receive_header(char* assignment);

		void clear_buffer();
		
		char get_addr_app(int assignment){return Lin_table[assignment].app;}
		char get_addr_sender(int assignment){return Lin_table[assignment].sender;}
		char get_addr_receiver(int assignment){return Lin_table[assignment].receivier;}
		char get_addr_time(int assignment){return Lin_table[assignment].time;}

	private:
		enum Status_CI
		{
			STANDBY,
			NORMAL,
			SLEEP,
			EXIT
		};
		enum Mode
		{
			MASTER,
			SLAVE
		};
		struct transmission_type{
			char app;			//aplicação
			char sender;		//remetente
			char receivier;		//destinatário
			long int time;		//tempo
		};

		// Variáveis Globais ao Protocolo
		GPIO*				EN;	//Enable
		GPIO*				INH;
	
		// Variáveis Locais ao Estado
		Mode				MODE; 	//Modo de operação 0 indica MASTER
		Status_CI 			State;
	
		// Variáveis Exclusivas à aplicação
		char				ADDR_SEND; //Identificador de 8bits (0x01->0x59) indica o destino dos dados 
		int					DATA_BYTE;
		transmission_type	Lin_table[59]; //Armazena a tabela de informação de emissor/receptor para cada app
		int					N_app;

		// Variáveis Auxiliares à implementação
		OStream 			_cout;
};

