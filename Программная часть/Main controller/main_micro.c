/*
 * main_microcontroller.c
 *
 * Created: 05.06.2020 14:03:35
 * Author : GaD_Bogdan
 */ 

#include "Libraries/main.h"
uint8_t byte_counter = 0;						// ������� �������� ������
uint32_t data32;								// ���������� ��� ������
uint32_t coordinates[4];						// ������ ��� ������, ���������� �� UART
uint8_t * p_coordinate_1;						// ��������� �� ������ ���� ������ ����������
//===================================================================================================================
void wait_while_devices_busy(void);
//===================================================================================================================
int main(void)
{
	p_coordinate_1 = (uint8_t *)coordinates;	// �������������� ��������� �� 
	SPI_MS_init();								// �������������� SPI ��� ������� ���������
	USART_Init(16);								// �������������� USART, �������� 115200
	USART_Transmit(120);						// ������ � ��������� ���������� ��� ��������
	wait_while_devices_busy();					// ���� ���� ���������� �����������
    while (1) 
    {
		if (byte_counter == 16)
		{
			byte_counter = 0;
			SPI_MS_send_uint32_t(p_coordinate_1, SPI_slave1_pin);				// �������� ���������� ������� ����������
			SPI_MS_send_uint32_t((p_coordinate_1 + 4), SPI_slave2_pin);			// �������� ���������� ������� ����������
			SPI_MS_send_uint32_t((p_coordinate_1 + 8), SPI_slave3_pin);			// �������� ���������� �������� ����������
			SPI_MS_send_uint32_t((p_coordinate_1 + 12), SPI_slave4_pin);		// �������� ���������� ���������� ����������
			USART_Transmit(120);					// ������� ����������, ��� ������ �������� ��������� ������
			wait_while_devices_busy();				// ����, ���� ��� ���������� �������� ���� ������
			_delay_us(20);
		}
    }
}
//===================================================================================================================
ISR(USART_RX_vect)
{
	*(p_coordinate_1 + byte_counter) = UDR0;
	byte_counter++;
}
//===================================================================================================================
void wait_while_devices_busy(void)
{
	DDRD = 0b00001111;							// ����� 4-7 �� ����
	PORTD |= 0b11110000;						// �������� � 1 �� ������ 4-7
	while(1)									// ���� ���� ��� ���������� ����� ������
	{
		_delay_us(20);
		if (((PIND & 0b11110000) == 0b11110000))
		{
			break;
		}
	}
}
//===================================================================================================================



