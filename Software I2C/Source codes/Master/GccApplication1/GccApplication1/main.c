#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define SDA PB1
#define SCL PB0
#define SLAVE_ADDRESS 0x50  // 7-bit I2C address

void SDA_High(){PORTB  |=  (1<<SDA);}
void SCL_High(){PORTB  |=  (1<<SCL);}
void SDA_Low() { PORTB &= ~(1<<SDA);}
void SCL_Low() { PORTB &= ~(1<<SCL);}
	

void I2C_Initialization()
{
	//Setting SDA and SCL in master MCU to output and high for idle state
	DDRB  |= (1<<SDA) |(1<<SCL);
	PORTB |= (1<<SDA) |(1<<SCL);	
}

void I2C_Start()
{
	//Pulling the SDA low while the SCL is high 
	//to initiate communication and then Pulling the SCL low
	SDA_Low();
	_delay_us(5);
	SCL_Low();
}

void I2C_Stop()
{
	SCL_High();
	_delay_us(5);
	SDA_High();
	_delay_us(5);
}

void I2C_Send_Bit(uint8_t bit)
{	
	//writing the desired bit boolean value on the SDA 
	if(bit)
	{
		SDA_High();
	}
	else
	{
		SDA_Low();
	}
	_delay_us(1);
	//Releasing the SCL to high for some time so slave can read the bit on SDA
	SCL_High();_delay_us(5);
	//Pulling SCL back to low so another bit can be sent
	SCL_Low(); _delay_us(1);
}

void I2C_Send_byte(uint8_t byte)
{
	for (int i = 0 ;i<8;i++)
	{
		//Sending the most significant bit as a boolean to be written on SDA
		I2C_Send_Bit((byte & 0b10000000)!=0);
		//Shifting the byte right to check for the next bit
		byte <<=1;
	}
}

void I2C_Read_Ack()
{
	//Setting SDA as input pulled up to read the ack
	DDRB &= ~(1<<SDA); SDA_High(); _delay_us(1);
	SCL_High(); _delay_us(5);/* code to read ack*/ SCL_Low();
	DDRB |= (1<<SDA);
}

int main(void)
{
	DDRC = 0; PORTC = 0xFF;
	I2C_Initialization();
	while(1)
	{
		I2C_Start();
		I2C_Send_byte(SLAVE_ADDRESS << 1);
		I2C_Read_Ack();
		I2C_Send_byte(~PINC);
		I2C_Read_Ack();
		I2C_Stop();
		_delay_ms(10);
	}
}