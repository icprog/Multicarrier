// EPOS - CAN (Uses SPI interface for MCP2515 control, and GPIO for MCP2562) 

#include <alarm.h>
#include <gpio.h>
#include <spi.h>
#include <machine.h>
#include <cpu.h>
#include <utility/ostream.h>

/************************************* SPI INSTRUCTION FOR CAN COMMUNICATION ******************************************/
#define RESET 				0xC0 		// Reset CI MCP2515
#define READ				0x03 		// Instrução para ler de um registrador
//#define READ_RX_BUFFER	1001 0nm0 	// Este comando precisa do endereço do buffer (lê apenas os dados do buffer RX especificado).	
#define WRITE				0x02	 	// Instrução para escreve em um registrador
//#define LOAD_TX_BUFFER 	0100 0abc	// Este comando precisa do endereço do buffer (escreve apenas os dados do buffer TX especificado)
//#define RTS 				1000 0nnn 	// Instrui o controlador a iniciar a seqüência de transmissão qualquer um dos buffers TX
#define READ_STATUS 		0xA0		// Pesquesa rápida, verifica o status dos registradores de recebimento e transmissão
#define RX_STATUS			0xB0		// Verifica a correspondência do filtro e o tipo de mensagem (padrão, estendido e/ou remoto) da mensagem recebida.
#define BIT_MODIFY			0x05		// Instrução para modificar um bits individuais dos registradores (apenas alguns registradores tem esta possibilidade)
/********************************************************************************************************************/

__BEGIN_SYS

class CAN{
	public:
		CAN();
		~CAN();
		void enable_EN();
		void disable_EN();

		//Settings
		void init_MCP2515();
		void config();
		void config_filters(int MODE);

		//Useful Functions
		void reset();
		
		void read_buffer(char* data,char dataLength, char _register);
		void read_rx_buffer(bool n, bool m,char* data);						
		
		void write(char adress, char data);
		void write_tx_buffer(char adress,char* data);
		void load_tx_buffer(bool a, bool b, bool c, char* data);				
		void request_to_send(bool tx_0,bool tx_1,bool tx_2); 

		void read_status(char* status_receive);	
		void read_register(char* status_receive, char _register);

		void bit_modify(char adress,char mask, char data);			
		
		//Functions SPI
		void spi_send(unsigned int);
		unsigned int spi_receive();
	private:

		// Variáveis Globais ao Protocolo
		SPI 				spi_;
		GPIO				EN;					//Enable		(OUT)
		GPIO				INT;				//Interrupt 	(IN)
		GPIO 				CSCAN;
		GPIO 				CAN_Clock;
		// Variáveis Auxiliares à implementação
		
};

__END_SYS
