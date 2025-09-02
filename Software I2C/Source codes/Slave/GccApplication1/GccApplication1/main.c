#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define SDA			PB1
#define SCL			PB0
#define MCU_ADDRESS 0x50  
#define DDRI2C		DDRB
#define PORTI2C     PORTB
#define PINI2C		PINB

void Slave_Initialization()
{
	//Set SCL and SDA as input Pulled-up high
	DDRB &= ~((1 << SDA) | (1 << SCL)); 
	PORTB |= (1 << SDA) | (1 << SCL);    
}

uint8_t Start_Condition() 
{
	//Wait while SDA and SCL are both High (Idel state)
	while ((PINB & (1 << SDA)) || !(PINB & (1 << SCL)));
	 _delay_us(5);
	 return 1;
}
	
uint8_t Read_Bit()
{	
	
	//while SCL is LOW wait
	while (!(PINB & (1 << SCL)));  
	_delay_us(1);
	//Read bit on SDA line
	uint8_t bit = !bit_is_clear(PINI2C,SDA);
	_delay_us(1);
	//wait until the SCL is low again to exit
	while (PINB & (1 << SCL));
	return bit;
}

uint8_t Read_Byte()
{	
	uint8_t byte = 0 ;
	for (int i = 0 ; i< 8 ; i++)
	{
		//Right shift the byte then read the Masters MSB
		byte <<= 1;
		byte |= Read_Bit();
	}
	return byte;
}

void Send_Ack()
{
	//switch SDA from input to output and set it as low to set the line to 0
	PORTI2C &= ~ (1<<SDA); DDRI2C |= (1<<SDA);
	//Wait while SCL is High
	while (!(PINB & (1 << SCL)));
	_delay_us(1);
	// Wait while it's low again to rest the SDA states
	while (PINB & (1 << SCL));   
	_delay_us(1);
	 DDRI2C &= ~ (1<<SDA); PORTI2C |= (1<<SDA);
}

int main()
{
	DDRC = 0xFF; PORTC = 0;
	Slave_Initialization();
	while(1)
	{	
		//Wait for start condition
		if(Start_Condition())
		{
			uint8_t address = Read_Byte();
			Send_Ack();
			//Check if addresses match to start the R/W operations
			if((address >> 1) == MCU_ADDRESS)
			{
				//Read data
				if ((address & 1) == 0)
				{
					uint8_t data;
					data = Read_Byte();
					Send_Ack();
					PORTC = data;
					_delay_ms(10);
				}
			}
		}
	}
}