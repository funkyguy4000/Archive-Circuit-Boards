/*
 * TWI.h
 *
 * Created: 3/20/2014 10:01:39 PM
 *  Author: student
 */ 

void TWIInit(void){
	//set SCLK to 400kHz
	TWSR = 0x00;
	TWBR = 0x0C;
	//enable TWI
	TWCR = (1<<TWEN);
}

void TWIStart(void){
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while ((TWCR & (1<<TWING)) == 0 );	
}

void TWIStop(void){
	TWCR = (1<<TWINT) | (1<<TWSTO) | (1<<TWEN);
}

void TWIWrite(uint8_t u8data){
	TWDR = u8data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t TWIReadACK(void){
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

uint8_t TWIReadNACK(void){
	TWCR = (1<<TWINT) | (1<<TWEN);
	while ((TWCR & (1<<TWINT)) == 0);
	return TWDR;
}

uint8_t TWIGetStatus(void){
	uint8_t status;
	//mask status
	status = TWSR & 0xF8
	return status;
}

