// EPOS - Can Abstraction Implementation
#include <alarm.h>
#include <can.h>

__BEGIN_SYS
// Methods
CAN::CAN():spi_(0, Traits<CPU>::CLOCK, SPI::FORMAT_MOTO_0, SPI::MASTER, 2000000, 8),
			EN('D', 0, GPIO::OUT, GPIO::UP, 0), //EN -Enable CI MCP2562
			INT('A', 6, GPIO::IN),				//INT - Receives Interrupt Generated by MCP2515
			CSCAN('A',3,GPIO::OUT),
			CAN_Clock('A', 7, GPIO::IN)
{
	disable_EN();  // Nível alto -> Standby; Nível baixo -> MCP2562 em modo NORMAL
	init_MCP2515();	
};

CAN::~CAN(){};

void CAN::enable_EN(){
	EN.set(0);
}
void CAN::disable_EN(){
	EN.set(1);
}

//*************************** SET CAN FUNCTIONS WITH SPI INSTRUCTIONS ********************************/

//The MCP2515 must be initialized prior to activation. Its initialization will occur after RESET and SELECT MODE
void CAN::init_MCP2515(){
	CSCAN.set(0);
	reset();			// Configuration Mode Select
	while(spi_.is_busy()){}
	CSCAN.set(1);
}

// Bit Timing Configuration Registers
void CAN::config(){
	char CNF1=0b00101010;
	char CNF2=0b00101001;
	char CNF3=0b00101000;
	/* According to CAN with baud rate of 500 Khz; Oscillator with 32MHz; BRP = 0 to determine Tqs=16*/
	write(CNF1, 0b00000001); //SJW=1*Tqs='00'; BRP=2='000001' // for 1MHz of baude set BRP = 0
	write(CNF2, 0b10110001); //BTLMODE= Length of PS2=Definido em CNF3='1'; SAM='0'; PHSEG1=PS1 Lenght='000'; PRSEG=Propagation='001'
	write(CNF3, 0b00000101); //SOF='0'; WAKFIL='0'; PHSEG2<2:0>='101'
	
}

//Setting up masks and filters
/*The configuration described in this function determines the receipt of any message from the bus, whether 
standard, extended, error flag or acknowledge signals*/
void CAN::config_filters(int MODE){
	if(MODE==1){ //Listen-Only mode
		// Edit RXB0 (Buffer 0)
		char RXB0CTRL=0b01100000; 
		char ADRESS_RXB0CTRL=0b01100000;
		write(ADRESS_RXB0CTRL, RXB0CTRL);

		// Edit RXB1 (Buffer 1)
		char RXB1CTRL=0b01100000; 
		char ADRESS_RXB1CTRL=0b01110000;
		write(ADRESS_RXB1CTRL, RXB1CTRL);
		//Edit Mask and Filter off in Listen-Only Mode		
	}
	else{ //Normal mode
		/********** SET BUFFER0 ***********/
		char RXM0SIDH_ADRESS=0b0000000; 	// RXM0SIDH -> MASK 0 STANDARD IDENTIFIER HIGH
		write(RXM0SIDH_ADRESS,0);

		char RXM0SIDL_ADRESS=0b00100001;		// RXM0SIDL -> MASK 0 STANDARD IDENTIFIER LOW
		write(RXM0SIDL_ADRESS,0);

		char RXB0CTRL=0b00100000; 
		char ADRESS_RXB0CTRL=0b01100000;
		write(ADRESS_RXB0CTRL, RXB0CTRL);


		/********** SET BUFFER1 ***********/
		char RXM1SIDH_ADRESS=0b00100100; 	// RXM1SIDH -> MASK 1 STANDARD IDENTIFIER HIGH
		write(RXM1SIDH_ADRESS,0);
		
		char RXM1SIDL_ADRESS=0b00100101;		// RXM1SIDL -> MASK 1 STANDARD IDENTIFIER LOW
		write(RXM1SIDL_ADRESS,0x0);

		char RXB1CTRL=0b00100000; 
		char ADRESS_RXB1CTRL=0b01110000;
		write(ADRESS_RXB1CTRL,RXB1CTRL);

	}
}
	
//*************************** TRANSMISSION/RECEIVE MODE ********************************/

// Reset MCP2515 (all registers return to their default values)	
void CAN::reset(){
	CSCAN.set(0);
	spi_send(RESET);
	while(spi_.is_busy()){}
	CSCAN.set(1);
}

void CAN::read_buffer(char* data,char dataLength, char _register){
	CSCAN.set(0);
	spi_send(0x03);
	spi_send(_register);
	while(spi_.is_busy()){}
	for(int i=0;i<dataLength;i++){
		data[i]=spi_receive();
	}
	CSCAN.set(1);
}

// RX buffers read instruction (nm select RX buffer -> RXBnm, where m select between SIDH or D0)								
void CAN::read_rx_buffer(bool n, bool m,char* data){
	char READ_RX_BUFFER;
	if(n==0){
		if(m==0){
			READ_RX_BUFFER=0b10010000; // Buffer 0 - RXB0SIDH (The 8 most significant bits of the ID of a Standard message)
		}
		else{
			READ_RX_BUFFER=0b10010010; // Buffer 0 - RXB0D0 (First byte of message data)
		}
	}
	else{
		if(m==0){
			READ_RX_BUFFER=0b10010100; // Buffer 1 - RXB1SIDH
		}
		else{
			READ_RX_BUFFER=0b10010110; // Buffer 1 - RXB1D0
		}
	}
	CSCAN.set(0);
	spi_send(READ_RX_BUFFER);	
	while(spi_.is_busy()){}
	if(m==0){
		*data=spi_receive(); // recebe apenas o identificador de msg Standard (8 bits mais significativos - faltam 3 bits)
	}
	else{
		/*CSCAN.set(0);
		read_register(data,0b01100101);//RXBnDLC (the last 3 bits indicate the NUMBER OF BYTES received in the buffer)
		CSCAN.set(1);*/
		for(int i=0;i<8;i++){
			*data=spi_receive();
			data++;
		}
	}
	while(spi_.is_busy()){}
	CSCAN.set(1);
	
	
}

// Writes the data to the specified recorder
void CAN::write(char adress, char data){
	CSCAN.set(0);
	spi_send(WRITE);
	spi_send(adress);
	spi_send(data);
	while(spi_.is_busy()){}
	CSCAN.set(1);
}

// Writes the data into the specified TX buffer 
void CAN::write_tx_buffer(char adress,char* data){
	CSCAN.set(0);
	spi_send(WRITE);
	spi_send(adress);
  	for(int i = 0; i <8; i++)
  	{
    	spi_send(data[i]);
  	}
  	while(spi_.is_busy()){}
  	CSCAN.set(1);
}

// Write instruction in TX buffers (abc = '110' or '111' are invalid) (you can use this or 'write_tx_buffer')
/*function can only be used to write to the TX0 buffer. See datasheet MCP2515 for more deployment options*/			
void CAN::load_tx_buffer(bool a, bool b, bool c,char *data){
	char Instruction=0;
	if(a==0 && b==0 && c==0){
		Instruction=0b01000000; //Send for TXB0SIDH
	}
	else if(a==0 && b==0 && c==1){
		Instruction=0b01000001;	//Send for TXB0D0
	}
	CSCAN.set(0);
	spi_send(Instruction);
	if(c==0){
		spi_send(*data);
	}
	else{
		for(int i = 0; i <8; i++)
	  	{
	    	spi_send(data[i]);
	  	}
	}
	while(spi_.is_busy()){}
	CSCAN.set(1);

}

// It requests the sending of the data contained in the TX buffers (all = '111' is not implemented, the value '000' is ignored)			
void CAN::request_to_send(bool tx_0,bool tx_1,bool tx_2){
	char RTS;
	if (tx_0==true) {
		RTS=0b10000001;
	}
	else if (tx_1==true) {
		RTS=0b10000010;
	}
	else if (tx_2==true) {
		RTS=0b10000100;
	}
	else{
		RTS=0b10000001;
	}
	CSCAN.set(0);
	spi_send(RTS);
	while(spi_.is_busy()){}
	CSCAN.set(1);
} 

// Returns Status of all RX and TX buffers in a single byte
void CAN::read_status(char* status_receive){
	CSCAN.set(0);
	spi_send(READ_STATUS);
    while(spi_.is_busy()){}
	*status_receive = spi_receive();
	CSCAN.set(1);
}	

// Returns the data of a specified register
void CAN::read_register(char* status_receive, char _register){
	CSCAN.set(0);
	spi_send(0x03);
	spi_send(_register);
	while(spi_.is_busy()){}
	*status_receive = spi_receive();
	CSCAN.set(1);
}

// It does the bitwise modification of the desired register (only some registers support this option). 
//The address of the registered, the mask of the bits to be modified and the values desired for modification must be passed.								
void CAN::bit_modify(char adress,char mask,char data){
	CSCAN.set(0);
	spi_send(BIT_MODIFY);
	spi_send(adress);
	spi_send(mask);
	spi_send(data);
	while(spi_.is_busy()){}
	CSCAN.set(1);
}	



//****************************************** FUNCTIONS SPI *********************************//
void CAN::spi_send(unsigned int data){
	spi_.put_data(data);
}		

unsigned int CAN::spi_receive(){
	spi_.get_datamod();
	spi_send(0x00);
    unsigned int data = spi_.get_data();
	while(spi_.is_busy()){}
	return data;
}

__END_SYS
