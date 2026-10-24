#include "Int_SI24R1.h"

// ����һ����̬�ķ��͵�ַ  => ���͵�ַ����յ�ַ��ͬ
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x01, 0x06, 0x1E, 0x01}; // ����һ����̬���͵�ַ

// SPI��дһ���ֽ� => д����ֽ��Ǵ���Ĳ���  ��ȡ���ֽ��Ƿ���ֵ
static uint8_t SPI_RW(uint8_t byte)
{
	uint8_t rx_data = 0;
	HAL_SPI_TransmitReceive(&hspi1, &byte, &rx_data, 1, 1000);
	return rx_data;
}

/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��SI24R1_WRITE_REG��reg��
					value:�Ĵ�����ֵ
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t Int_SI24R1_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;

	CS_LOW;
	status = SPI_RW(reg);
	SPI_RW(value);
	CS_HIGH;

	return (status);
}

/********************************************************
�������ܣ�д�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��SI24R1_WRITE_REG��reg��
					pBuf:д�����׵�ַ
					bytes:д�����ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t Int_SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t size)
{
	uint8_t status, byte_ctr;

	CS_LOW;
	status = SPI_RW(reg);
	for (byte_ctr = 0; byte_ctr < size; byte_ctr++)
	{
		SPI_RW(*pBuf++);
	}

	CS_HIGH;

	return (status);
}

/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ����ʽ��SI24R1_READ_REG��reg��
����  ֵ���Ĵ���ֵ
*********************************************************/
uint8_t Int_SI24R1_Read_Reg(uint8_t reg)
{
	uint8_t value;

	CS_LOW;
	SPI_RW(reg);
	value = SPI_RW(0);
	CS_HIGH;

	return (value);
}

/********************************************************
�������ܣ���ȡ�Ĵ�����ֵ�����ֽڣ�
��ڲ�����reg:�Ĵ���ӳ���ַ��SI24R1_READ_REG��reg��
					pBuf:���ջ��������׵�ַ
					bytes:��ȡ�ֽ���
����  ֵ��״̬�Ĵ�����ֵ
*********************************************************/
uint8_t Int_SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size)
{
	uint8_t status, byte_ctr;

	CS_LOW;
	status = SPI_RW(reg);
	for (byte_ctr = 0; byte_ctr < size; byte_ctr++)
	{
		pBuf[byte_ctr] = SPI_RW(0); // ��ȡ���ݣ����ֽ���ǰ
	}
	CS_HIGH;

	return (status);
}

/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��
��ڲ�������
����  ֵ����
*********************************************************/
void Int_SI24R1_RX_Mode(void)
{
	CE_LOW;
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // �����豸����ͨ��0ʹ�úͷ����豸��ͬ�ķ��͵�ַ
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);						   // ʹ�ܽ���ͨ��0�Զ�Ӧ��
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);					   // ʹ�ܽ���ͨ��0
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, CHANNEL);					   // ѡ����Ƶͨ��40
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);			   // ����ͨ��0ѡ��ͷ���ͨ����ͬ��Ч���ݿ���
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);					   // ���ݴ�����1Mbps�����书��4dBm
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0f);						   // CRCʹ�ܣ�16λCRCУ�飬�ϵ磬����ģʽ
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, 0xff);						   // ������е��жϱ�־λ
	CE_HIGH;																	   // ����CE���������豸
}

/********************************************************
�������ܣ�SI24R1����ģʽ��ʼ��
��ڲ�������
����  ֵ����
*********************************************************/
void Int_SI24R1_TX_Mode(void)
{
	CE_LOW;
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);	   // д�뷢�͵�ַ
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // Ϊ��Ӧ������豸������ͨ��0��ַ�ͷ��͵�ַ��ͬ
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);						   // ʹ�ܽ���ͨ��0�Զ�Ӧ��
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);					   // ʹ�ܽ���ͨ��0
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + SETUP_RETR, 0x0a);					   // �Զ��ط���ʱ�ȴ�250us+86us���Զ��ط�10��
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, CHANNEL);					   // ѡ����Ƶͨ��0x40
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);					   // ���ݴ�����1Mbps�����书��4dBm
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0e);						   // CRCʹ�ܣ�16λCRCУ�飬�ϵ�
	CE_HIGH;
}

/********************************************************
�������ܣ���ȡ��������   Ӳ��ֱ�ӽ������ݱ��浽 FIFO������ => ͨ��״̬��־λ�ж϶������Ƿ�������
��ڲ�����rxbuf:�������ݴ���׵�ַ
����  ֵ��0:���յ�����
		  1:û�н��յ�����
*********************************************************/
uint8_t Int_SI24R1_RxPacket(uint8_t *rxbuf)
{
	uint8_t state;
	// ����ȡ����ֵ ԭ�ⲻ����д��״̬�Ĵ���  => ��Ϊ״̬�Ĵ����еı�־λ���Ϊд1���
	state = Int_SI24R1_Read_Reg(STATUS);					// ��ȡ״̬�Ĵ�����ֵ
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); // ���RX_DS�жϱ�־

	if (state & RX_DR) // ���յ�����
	{
		Int_SI24R1_Read_Buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH); // ��ȡ����
		Int_SI24R1_Write_Reg(FLUSH_RX, 0xff);					 // ���RX FIFO�Ĵ���
		return 0;
	}
	return 1; // û�յ��κ�����
}

/********************************************************
�������ܣ�����һ�����ݰ�
��ڲ�����txbuf:Ҫ���͵�����
����  ֵ: 0: ���ͳɹ� 1: ����ʧ��
*********************************************************/
uint8_t Int_SI24R1_TxPacket(uint8_t *txbuf)
{
	uint8_t state;
	CE_LOW;													  // CE���ͣ�ʹ��SI24R1����
	Int_SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); // д���ݵ�TX FIFO,32���ֽ�
	CE_HIGH;												  // CE�øߣ�ʹ�ܷ���

	// û��ʹ���ж��ж��Ƿ������  => ʹ����ѯ��ȡ״̬��־λ
	// while (IRQ == 1)
	// 	;										 // �ȴ��������

	state = Int_SI24R1_Read_Reg(STATUS); // ��ȡ״̬�Ĵ�����ֵ
	while (((state & TX_DS) == 0) && ((state & MAX_RT) == 0))
	{
		state = Int_SI24R1_Read_Reg(STATUS);
		vTaskDelay(1);
	}

	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); // ���TX_DS��MAX_RT�жϱ�־
	if (state & MAX_RT)										// �ﵽ����ط�����
	{
		Int_SI24R1_Write_Reg(FLUSH_TX, 0xff); // ���TX FIFO�Ĵ���
		return 1;
	}
	if (state & TX_DS) // �������
	{
		return 0;
	}
	return 1; // ����ʧ��
}

uint8_t si24r1_rx_buff[5] = {0};

/**
 * @brief SI24R1�ĳ�ʼ�����
 *
 * @return uint8_t  0:���ɹ�  1:���ʧ��
 */
uint8_t Int_SI24R1_Check(void)
{

	// 1. ����SPIͨ���ܹ�������д�Ĵ���
	// 1.0 SI24R1оƬ��Ҫ�ȶ�ȡһ��  ��֤SPI����֮����д��
	Int_SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR, si24r1_rx_buff, TX_ADR_WIDTH);

	// 1.1 д�뷢�͵�ַ
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);

	// 1.2 ��ȡͬ��������
	Int_SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR, si24r1_rx_buff, TX_ADR_WIDTH);

	for (uint8_t i = 0; i < TX_ADR_WIDTH; i++)
	{
		if (si24r1_rx_buff[i] != TX_ADDRESS[i])
		{
			return 1;
		}
	}
	return 0;
}

/**
 * @brief Ӳ���ӿڲ�SI24R1�ĳ�ʼ��
 *
 */
void Int_SI24R1_Init(void)
{
	// �ϵ�֮���оƬ�ӳ� >100ms
	HAL_Delay(200);
	// У����
	while (Int_SI24R1_Check() == 1)
	{
		// ÿ���μ����10ms
		HAL_Delay(10);
	}

	// ����Ĭ�ϵ�״̬Ϊ����ģʽ  => ÿ�η������ݵ�ʱ��  �л�������״̬ 
	Int_SI24R1_RX_Mode();
	debug_printf("SI24R1 Init Success!\r\n");
}
