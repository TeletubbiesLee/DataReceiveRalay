/**
  *             Copyright (C) SOJO Electric CO., Ltd. 2019-2020. All right reserved.
  * @file:      CC1101.c
  * @brief:     CC1101���ߴ���ģ�������ļ�
  * @version:   V1.0.0 
  * @author:    Lei
  * @date:      2019-10-11
  * @update:    [2019-10-11][Lei][creat]
  */


#include "DriverSPI.h"
#include "sys.h"
#include "CC1101.h"


//99.9756kBand
const RF_SETTINGS rfSettings = 
{
    0x00,
    0x08,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.
    0x5B,   // MDMCFG4   Modem configuration.
    0xF8,   // MDMCFG3   Modem configuration.
    0x03,   // MDMCFG2   Modem configuration.
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.

    0x01,   // CHANNR    Channel number.
    0x47,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0xB6,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x1C,   // BSCFG     Bit synchronization Configuration.
    0xC7,   // AGCCTRL2  AGC control.
    0x00,   // AGCCTRL1  AGC control.
    0xB2,   // AGCCTRL0  AGC control.

    0xEA,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x09,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0   GDO0 output pin configuration. Refer to SmartRF Studio User Manual for detailed pseudo register explanation.

    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0x50    // PKTLEN    Packet length.
};


static void CC1101_IO_Init(void);
static void SPI_WriteReg(uint8_t addr, uint8_t value);
static void SPI_WriteBrustReg(uint8_t addr, uint8_t *buffer, uint8_t count);
static void SPI_Strobe(uint8_t strobe);

static void SPI_ReadBrustReg(uint8_t addr, uint8_t *buffer, uint8_t count);
static uint8_t SPI_ReadStatus(uint8_t addr);



static void CC1101_IO_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIOBʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();           //����GPIODʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();           //����GPIOIʱ��

    GPIO_Initure.Pin=GPIO_PIN_12; //PB12
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);	//PB12��1 
	
    GPIO_Initure.Pin=GPIO_PIN_3; //PD3
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOD,&GPIO_Initure);
	
    GPIO_Initure.Pin=GPIO_PIN_4; //PI4
    GPIO_Initure.Mode=GPIO_MODE_INPUT;  //����
    GPIO_Initure.Pull=GPIO_PULLDOWN;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
}


void CC1101_Reset(void)
{
    CSn = 0;			//Ƭѡ����0
    while(MISO);
    SPI2_ReadWriteByte(CCxxx0_SRES);
    while(MISO);
    CSn = 1;			//Ƭѡ����1
}


void CC1101_PowerUp(void)
{
	SPI2_Init();
	CC1101_IO_Init();
	
	CSn = 1;			//Ƭѡ����1
	delay_us(4);
	CSn = 0;			//Ƭѡ����1
	delay_us(4);
	CSn = 1;			//Ƭѡ����1
	delay_us(100);
	CC1101_Reset();
}

void CC1101_SettingsReg(void)
{
	uint8_t paTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};
	
	SPI_WriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);//���Ѽӵ�
    // Write register settings
    SPI_WriteReg(CCxxx0_FSCTRL1,  rfSettings.FSCTRL1);
    SPI_WriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL0);
    SPI_WriteReg(CCxxx0_FREQ2,    rfSettings.FREQ2);
    SPI_WriteReg(CCxxx0_FREQ1,    rfSettings.FREQ1);
    SPI_WriteReg(CCxxx0_FREQ0,    rfSettings.FREQ0);
    SPI_WriteReg(CCxxx0_MDMCFG4,  rfSettings.MDMCFG4);
    SPI_WriteReg(CCxxx0_MDMCFG3,  rfSettings.MDMCFG3);
    SPI_WriteReg(CCxxx0_MDMCFG2,  rfSettings.MDMCFG2);
    SPI_WriteReg(CCxxx0_MDMCFG1,  rfSettings.MDMCFG1);
    SPI_WriteReg(CCxxx0_MDMCFG0,  rfSettings.MDMCFG0);
    SPI_WriteReg(CCxxx0_CHANNR,   rfSettings.CHANNR);
    SPI_WriteReg(CCxxx0_DEVIATN,  rfSettings.DEVIATN);
    SPI_WriteReg(CCxxx0_FREND1,   rfSettings.FREND1);
    SPI_WriteReg(CCxxx0_FREND0,   rfSettings.FREND0);
    SPI_WriteReg(CCxxx0_MCSM0 ,   rfSettings.MCSM0 );
    SPI_WriteReg(CCxxx0_FOCCFG,   rfSettings.FOCCFG);
    SPI_WriteReg(CCxxx0_BSCFG,    rfSettings.BSCFG);
    SPI_WriteReg(CCxxx0_AGCCTRL2, rfSettings.AGCCTRL2);
	SPI_WriteReg(CCxxx0_AGCCTRL1, rfSettings.AGCCTRL1);
    SPI_WriteReg(CCxxx0_AGCCTRL0, rfSettings.AGCCTRL0);
    SPI_WriteReg(CCxxx0_FSCAL3,   rfSettings.FSCAL3);
	SPI_WriteReg(CCxxx0_FSCAL2,   rfSettings.FSCAL2);
	SPI_WriteReg(CCxxx0_FSCAL1,   rfSettings.FSCAL1);
    SPI_WriteReg(CCxxx0_FSCAL0,   rfSettings.FSCAL0);
    SPI_WriteReg(CCxxx0_FSTEST,   rfSettings.FSTEST);
    SPI_WriteReg(CCxxx0_TEST2,    rfSettings.TEST2);
    SPI_WriteReg(CCxxx0_TEST1,    rfSettings.TEST1);
    SPI_WriteReg(CCxxx0_TEST0,    rfSettings.TEST0);
    SPI_WriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2);
    SPI_WriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0);
    SPI_WriteReg(CCxxx0_PKTCTRL1, rfSettings.PKTCTRL1);
    SPI_WriteReg(CCxxx0_PKTCTRL0, rfSettings.PKTCTRL0);
    SPI_WriteReg(CCxxx0_ADDR,     rfSettings.ADDR);
    SPI_WriteReg(CCxxx0_PKTLEN,   rfSettings.PKTLEN);
	
	SPI_WriteBrustReg(CCxxx0_PATABLE, paTabel, 8);
	
}


void CC1101_SendPacket(uint8_t* txBuffer, uint8_t size)
{
	SPI_WriteReg(CCxxx0_TXFIFO, size);
    SPI_WriteBrustReg(CCxxx0_TXFIFO, txBuffer, size);	//д��Ҫ���͵�����

    SPI_Strobe(CCxxx0_STX);		//���뷢��ģʽ��������

    // Wait for GDO0 to be set -> sync transmitted
    while (!GDO0);
    // Wait for GDO0 to be cleared -> end of packet
    while (GDO0);
	SPI_Strobe(CCxxx0_SFTX);
}


void CC1101_SetRxMode(void)
{
	SPI_Strobe(CCxxx0_SRX);			//C1101���ý���ģʽ
}


uint8_t CC1101_ReceivePacket(uint8_t* rxBuffer, uint8_t* length)
{
	  uint8_t status[2];
	  uint8_t packetLength;
	  uint8_t i = (*length) * 4;		//�������Ҫ����datarate��length������
	
	  SPI_Strobe(CCxxx0_SRX);			//�������״̬
	  delay_us(4);
	  while(GDO0)
	  {
			 delay_us(4);
			 --i;
			 if(i < 1)
			 return 0;
    }
	  if(SPI_ReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)	//����ӵ��ֽ�����Ϊ0
	  {
		 packetLength = SPI_ReadReg(CCxxx0_RXFIFO);//������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
         if (packetLength <= *length) 		//�����Ҫ����Ч����С�ڵ��ڽ��յ������ݰ��ĳ���
		     {
            SPI_ReadBrustReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //�������н��յ�������
            *length = packetLength;				//�ѽ������ݳ����޸ĵ��޸�Ϊ��ǰ���ݵĳ���
        
            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            SPI_ReadBrustReg(CCxxx0_RXFIFO, status, 2); 	//����CRCУ��λ
						SPI_Strobe(CCxxx0_SFRX);		//��ϴ���ջ�����
            return (status[1] & CRC_OK);			//���У��ɹ��򷵻ؽ��ճɹ�
         }
		 else 
		{
            *length = packetLength;
            SPI_Strobe(CCxxx0_SFRX);		//��ϴ���ջ�����
            return 0;
        }
	}
	else
		return 0;
}



static void SPI_WriteReg(uint8_t addr, uint8_t value)
{
	CSn = 0;										//Ƭѡ����0
	while(MISO);
	SPI2_ReadWriteByte(addr);
	SPI2_ReadWriteByte(value);
	CSn = 1;										//Ƭѡ����1
}


static void SPI_WriteBrustReg(uint8_t addr, uint8_t *buffer, uint8_t count)
{
	uint8_t i = 0, temp = 0;
	
	temp = addr | WRITE_BURST;				//����д����
	CSn = 0;													//Ƭѡ����0
	while(MISO);
	SPI2_ReadWriteByte(temp);
	
	for(i = 0; i < count; i++)
	{
		SPI2_ReadWriteByte(buffer[i]);
	}
	CSn = 1;													//Ƭѡ����1
}


static void SPI_Strobe(uint8_t strobe)
{
	CSn = 0;			
	while(MISO);
	SPI2_ReadWriteByte(strobe);
	CSn = 1;			
}


uint8_t SPI_ReadReg(uint8_t addr)
{
	uint8_t temp = 0, value= 0;
	
	temp = addr | READ_SINGLE;		
	CSn = 0;			
	while(MISO);
	SPI2_ReadWriteByte(temp);
	value = SPI2_ReadWriteByte(0);
	CSn = 1;			
	return value;
}


static void SPI_ReadBrustReg(uint8_t addr, uint8_t *buffer, uint8_t count)
{
	uint8_t i = 0, temp = 0;
	
	temp = addr | READ_BURST;				
	CSn = 0;			
	while(MISO);
	SPI2_ReadWriteByte(temp);
	
	for(i = 0; i < count; i++)
	{
		buffer[i] = SPI2_ReadWriteByte(0);
	}
	CSn = 1;
}


static uint8_t SPI_ReadStatus(uint8_t addr)
{
	uint8_t temp = 0, value= 0;
	
	temp = addr | READ_BURST;		
	CSn = 0;			
	while(MISO);
	SPI2_ReadWriteByte(temp);
	value = SPI2_ReadWriteByte(0);
	CSn = 1;			
	
	return value;
}

