/*
 * SPI.c
 *
 * Created: 05.06.2020 15:23:18
 *  Author: GaD_Bogdan
 */ 

#include "SPI.h"

#define delay_between_bytes_us 100
//===================================================================================================================
void SPI_MS_init(void)
{
	DDRB |= (1<<PORTB3)|(1<<PORTB5);					// MOSI, SCK
	PORTB &= ~((1<<PORTB3)|(1<<PORTB5));				// Низкий уровень на MOSI, SCK
	DDR(SPI_slaves_port) |= (1<<SPI_slave1_pin)|(1<<SPI_slave2_pin)|(1<<SPI_slave3_pin)|(1<<SPI_slave4_pin);
	// На выход ножки, к которым подключены пин SS ведомых устройств
	SPI_slaves_port |= (1<<SPI_slave1_pin)|(1<<SPI_slave2_pin)|(1<<SPI_slave3_pin)|(1<<SPI_slave4_pin);
	// Высокий уровень на всех ножках с SS	
	ClearBit(DDRB, 4);									// MISO на вход
	SetBit(SPSR, SPI2X);								// Включаем удвоение частоты тактирования SPI
	SPCR = (1<<SPE)|(1<<MSTR);							// Включаем SPI выбираем контроллер в качетсве мастера
}
//===================================================================================================================
void SPI_SL_init(void)
{
	DDRB &= ~((1<<PORTB2)|(1<<PORTB3)|(1<<PORTB5));		// MOSI, SCK, SS на вход
	SetBit(DDRB, 4);									// MISO на выход
	ClearBit(PORTB, 4);									// 0 на MISO
	SetBit(SPSR, SPI2X);								// Включаем удвоение частоты тактирования SPI
	SPCR = (1<<SPIE)|(1<<SPE)|(0<<MSTR);				// Включаем SPI, прерывания, выбираем контроллер в качетсве ведомого
}
//===================================================================================================================
uint8_t SPI_MS_send_byte(uint8_t byte)
{
	SPDR = byte;										// Начинаем передачу данных
	while(BitIsClear(SPSR, SPIF));						// Ждем пока завершится передача данных
	return SPDR;										// Возвращаем полученный байт
}
//===================================================================================================================
void SPI_MS_send_uint32_t(uint8_t * p8_data32, uint8_t slaves_pin)
{
	ClearBit(SPI_slaves_port, slaves_pin);			// Говорим, с каким устройством мы хотим общаться
	SPI_MS_send_byte(*p8_data32++);					// отправляем первый байт
	_delay_us(delay_between_bytes_us);									// Задрежка, чтобы контроллер успел обработать данные
	SPI_MS_send_byte(*(p8_data32++));				// отправляем второй байт
	_delay_us(delay_between_bytes_us);									// Задрежка, чтобы контроллер успел обработать данные
	SPI_MS_send_byte(*(p8_data32++));				// отправляем третий байт
	_delay_us(delay_between_bytes_us);									// Задрежка, чтобы контроллер успел обработать данные
	SPI_MS_send_byte(*(p8_data32));	// отправляем четвертый байт
	SetBit(SPI_slaves_port, slaves_pin);			// Возвращаем высокий уровень на ножку SS
}