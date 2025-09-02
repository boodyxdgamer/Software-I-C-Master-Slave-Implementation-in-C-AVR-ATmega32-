#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define SDA PB1
#define SCL PB0
#define SLAVE_ADDRESS 0x50  // Must match master

// Initialize SDA and SCL as inputs with pull-ups
void i2c_slave_init() {
	DDRB &= ~((1 << SDA) | (1 << SCL));  // Inputs
	PORTB |= (1 << SDA) | (1 << SCL);    // Enable pull-ups
	DDRC = 0xFF;  PORTC = 0x00;          // PORTC as output (LEDs off)
}

// Wait for START condition: SDA goes LOW while SCL is HIGH
uint8_t wait_for_start_condition() {
	while ((PINB & (1 << SDA)) || !(PINB & (1 << SCL)));
	_delay_us(5);
	return 1;
}

// Read a single bit from SDA
uint8_t read_bit() {
	DDRB &= ~(1 << SDA);  // Ensure SDA is input
	while (!(PINB & (1 << SCL)));  // Wait for SCL HIGH
	_delay_us(1);
	uint8_t bit = !bit_is_clear(PINB, SDA);  // Read SDA
	while (PINB & (1 << SCL));  // Wait for SCL LOW
	_delay_us(1);
	return bit;
}

// Read a full byte from master
uint8_t read_byte() {
	uint8_t byte = 0;
	for (int i = 0; i < 8; i++) {
		byte <<= 1;
		byte |= read_bit();  // Read MSB first
	}
	return byte;
}

// Send ACK by pulling SDA LOW during 9th clock
void send_ack() {
	DDRB |= (1 << SDA);       // SDA as output
	PORTB &= ~(1 << SDA);     // Drive LOW
	while (!(PINB & (1 << SCL))); // Wait for SCL HIGH
	_delay_us(1);
	while (PINB & (1 << SCL));    // Wait for SCL LOW
	_delay_us(1);
	DDRB &= ~(1 << SDA);      // Release SDA
	PORTB |= (1 << SDA);      // Enable pull-up
}

int main(void) {
	i2c_slave_init();

	while (1) {
		if (wait_for_start_condition()) {
			uint8_t address = read_byte();  // Receive address
			send_ack();
			//if slave address is a match and least significant bit is high then master is doing a write operation and slave is reading
			if ((address >> 1) == SLAVE_ADDRESS && ((address & 1) == 0)) {
				uint8_t data = read_byte();  // Receive actual data
				send_ack();
				PORTC = data;  // Display data on slave PORT connected to the LEDs
				_delay_ms(10); // Allow LED's to be on for a sufficient amount of time so if another bit is sent the change is visible
			}
		}
	}
}