#include "nrf24l01p.h"


static volatile uint8_t SPI_TxBuf[SPI_TX_BUFFER_SIZE];
static volatile uint8_t SPI_TxHead; //writing --last location written
static volatile uint8_t SPI_TxTail; //read    --last location read   8 bit can represent maximu 256 byte
static volatile uint16_t SPI_Length;


//Sending Interrupt

void Wireless_SPI_INT_handler(void)
{
//	unsigned char tmptail;
	uint8_t tmpStatus;
	//uint8_t *pucTmp;
	uint8_t i;
	uint8_t imax;
	uint8_t pucTmp2[32];

	CEDown(); // standby
	tmpStatus=CMD(NOP);

	if (tmpStatus & (1<<MAX_RT))
	{
		CMD(FLUSH_TX);
		WriteReg(STATUS, 0x7E);
	}

	//while (tmpStatus & (1<<TX_FULL));
	//Write SPI_TxBuf
	/*************************************************************************
	Function: SPI Data Register Empty interrupt
	Purpose:  called when the SPI is ready to transmit the next byte
	**************************************************************************/



	if ( SPI_TxHead != SPI_TxTail ) //sending soft buffer has stuff
	{
		/* calculate and store new buffer index */
		//tmptail = (SPI_TxTail + 1) & (SPI_TX_BUFFER_SIZE-1);
		SPI_TxTail = ((SPI_TxTail + 1) & (SPI_TX_BUFFER_SIZE-1));
		/* get one byte from buffer and write it to UART */
		//UDR = SPI_TxBuf[tmptail];  /* start transmission */

	//	portENTER_CRITICAL();

//
//		if (SPI_Length>=32)
//		{
//			//pucTmp = (uint8_t*) alloca(sizeof(uint8_t[32])); // allocate enough space from the stack
//
//			for (i=0;i<32;i++)
//			{
//				//pucTmp[31-i]=SPI_TxBuf[SPI_TxTail+i];
//				//pucTmp[i]=SPI_TxBuf[SPI_TxTail+i];
//				pucTmp2[i]=SPI_TxBuf[(SPI_TxTail+i)&(SPI_TX_BUFFER_SIZE-1)];
//			}
//
//			//while (!spi_writeRegisterEmptyCheck(&AVR32_SPI));
//
//			CMD_2(W_TX_PAYLOAD, pucTmp2 , 32, NULL);
//
//
//			SPI_Length=SPI_Length-32;
//			SPI_TxTail=((SPI_TxTail+31)&(SPI_TX_BUFFER_SIZE-1));
//
//		}
//
//		else
//		{
//			CMD_2(W_TX_PAYLOAD, (uint8_t*)(&(SPI_TxBuf[SPI_TxTail])), 1, NULL);
//			SPI_Length--;
//		}



					//pucTmp = (uint8_t*) alloca(sizeof(uint8_t[32])); // allocate enough space from the stack

		if (SPI_Length<32)
			imax=SPI_Length;
		else
			imax=32;

		for (i=0;i<imax;i++)
		{
			//pucTmp[31-i]=SPI_TxBuf[SPI_TxTail+i];
			//pucTmp[i]=SPI_TxBuf[SPI_TxTail+i];
			pucTmp2[i]=SPI_TxBuf[(SPI_TxTail+i)&(SPI_TX_BUFFER_SIZE-1)];
		}

		//while (!spi_writeRegisterEmptyCheck(&AVR32_SPI));

		CMD_2(W_TX_PAYLOAD, pucTmp2 , imax, NULL);


		SPI_Length=SPI_Length-imax;
		SPI_TxTail=((SPI_TxTail+imax-1)&(SPI_TX_BUFFER_SIZE-1));



		//CMD(W_TX_PAYLOAD, SPI_TxBuf,  ); // if the the payload buffer is empty, then fill it

		//uint8_t ucTmp;
		//CEDown(); // pull up the CSN pin
		//CMD_2(W_TX_PAYLOAD, pucBuff, ucLen);

//		while (tmpStatus & (1<<TX_FULL));

//		TransChar(W_TX_PAYLOAD); // write the command and receive the value in the status register
	//	TransChar(SPI_TxBuf[tmptail]);
		//TransBlock(SPI_TxBuf, SPI_Length , NULL); // write and receive a block of data with length equals to ucLen
		//SPI_Length--;
		 // pull down the CSN pin


	}

	else{
		/* tx buffer empty, disable TDRE interrupt */
		  AVR32_SPI.IDR.tdre=1;
//		 AVR32_SPI.idr=1<<AVR32_SPI_IDR_TDRE_OFFSET;

	}


	WriteReg(STATUS, 0x7E);


	CEUp(); //transmit



//	uint8_t temp;
//	temp='C';
//	Start_SS(); // pull down the CSN pin
//	TransChar(W_TX_PAYLOAD); // write the command and receive the value in the status register
//	TransChar(temp);
//	Stop_SS();
//	delay_ms(10);
//	AVR32_SPI.IDR.txempty=1;


}


void NRF24L01P_Init(NRF24L01P_Property_t NRF24L01P_mProperty)
{
	SPI_TxHead=0;
	SPI_TxTail=0;
	SPI_Length=0;

	NRF24L01P_mProperty.ucCH=50; // default channel
	NRF24L01P_mProperty.pucAddr[0]=0xE7;
	NRF24L01P_mProperty.pucAddr[1]=0xE7;
	NRF24L01P_mProperty.pucAddr[2]=0xE7;
	NRF24L01P_mProperty.pucAddr[3]=0xE7;
	NRF24L01P_mProperty.pucAddr[4]=0xC1;
	//NRF24L01P_mProperty.ucMaxTxQueueLength=40;
	//NRF24L01P_mProperty.ucMaxRxQueueLength=255;

//
//	NRF24L01P_IRQ_PORT |= _BV(NRF24L01P_IRQ_PIN); // set the IRQ pin to input mode with internal pull-up resister
//	NRF24L01P_IRQ_DDR &= ~_BV(NRF24L01P_IRQ_PIN);
//
//	NRF24L01P_CE_PORT &= ~_BV(NRF24L01P_CE_PIN); // set the CE to output and set it to 0 for now
//	NRF24L01P_CE_DDR  |= _BV(NRF24L01P_CE_PIN);


	gpio_enable_gpio_pin(NRF24L01P_CE_PIN);

	CEDown(); // make sure the RF chip is in standby or power down mode
//	portENTER_CRITICAL();
	WriteReg(CONFIG, (1<<EN_CRC) | (1<<CRCO)); // power down
	WriteReg(CONFIG, (1<<PWR_UP) | (1<<EN_CRC) | (1<<CRCO) | (1<<MASK_RX_DR) | (1<<MASK_TX_DS) | (1<<MASK_MAX_RT)); // power up in pTX mode, diable all interrupt except max_rt
	//in standby-I
	CMD(FLUSH_TX); // flush both RX and TX FIFOs
	CMD(FLUSH_RX);
	WriteReg(STATUS, 0x70); // clear the interrupt

	WriteReg(RF_SETUP, 0x26); // set the data rate to 250kbps with 0dBm output power
	WriteReg(EN_AA, 0x01); // enable auto acknowledgment for pipe0
	WriteReg(EN_RXADDR, 0x01); // enable pipe0
	WriteReg(DYNPD, 0x01); // enable dynamic payload length in pipe0
	WriteReg(FEATURE, (1<<EN_DPL) | (1<<EN_ACK_PAY)); // enable global dynamic payload length and payload with ACK
	WriteReg(SETUP_AW, 0x03); // set address width to 5
	WriteReg(SETUP_RETR, 0xFF); // wait 4ms for each retransmit, maximum 15 retransmits allowed
	// enable 2 bytes CRC
	delay_ms(5); // wait for power up //Page 24 Datasheet Timing Information
	SetProperty(&NRF24L01P_mProperty);
//	EICRB |= _BV(ISC71);
//	EIMSK |= _BV(INT7); // external interrupt on PE7, falling edge trigger
//	portEXIT_CRITICAL()	;
	  //Configure Interrupt Handler
	  Disable_global_interrupt();
	    // init the interrupts
	  //

	  //Disable ALL SPI Interrupt
	  AVR32_SPI.IDR.txempty=1;
	  AVR32_SPI.IDR.nssr=1;
	  AVR32_SPI.IDR.ovres=1;
	  AVR32_SPI.IDR.modf=1;
	  AVR32_SPI.IDR.rdrf=1;
	  AVR32_SPI.IDR.tdre=1;


	  /* register PB0 handler on level 1 */
	  INTC_register_interrupt(&Wireless_SPI_INT_handler, AVR32_SPI_IRQ, AVR32_INTC_INT0);

	  //INTC_register_interrupt( &dip204_example_PB_int_handler, AVR32_GPIO_IRQ_0 + (GPIO_BACKLIGHT_MINUS/8), AVR32_INTC_INT1);
	  //INTC_register_interrupt( &dip204_example_PB_int_handler, AVR32_GPIO_IRQ_0 + (GPIO_CHARSET/8), AVR32_INTC_INT1);

	  /* Enable all interrupts */
	  Enable_global_interrupt();



}

uint8_t WriteReg(uint8_t ucAddr, uint8_t ucData) {
	//portENTER_CRITICAL();
	Start_SS();// pull down the CSN pin
	uint8_t ucTmp = TransChar(W_REGISTER | (REGISTER_MASK & ucAddr));
	// write the command to write a register and return the value in the status register
	TransChar(ucData); // write the data to the register
	Stop_SS(); // pull up the CSN pin
	//portEXIT_CRITICAL();
	return ucTmp;
}

uint8_t CMD_2(uint8_t ucCMD, uint8_t *pucSend, uint8_t ucLen,
		uint8_t *pucReceive) {
	uint8_t ucTmp;
	Start_SS(); // pull down the CSN pin
	ucTmp = TransChar(ucCMD); // write the command and receive the value in the status register
	TransBlock(pucSend, ucLen, pucReceive); // write and receive a block of data with length equals to ucLen
	Stop_SS(); // pull up the CSN pin
	return ucTmp; // return the value in the status register
}

uint8_t CMD(uint8_t ucCMD) {
	return CMD_2(ucCMD,NULL,0,NULL);
}

//CWireless::~CWireless(void) {
//	CEDown(); // standby
//	WriteReg(CONFIG, _BV(EN_CRC) | _BV(CRCO)); // power down
//	FlushRxQueue();
//}

//void CWireless::ChangePins(IOSet *psCSN, IOSet *psCE) {
//	portENTER_CRITICAL();
//	CSPIDevice::Init(psCSN); // initialize the SPI device with CSN pin as chip select
//	m_ucPinCE = _BV(psCE->ucPIN); // set CE pin to output
//	*(psCE->pucDDR) |= m_ucPinCE;
//	m_pucPORTCE = psCE->pucPORT;
//	*m_pucPORTCE &= ~m_ucPinCE;
//	portEXIT_CRITICAL()
//	;
//}

//uint16_t CWireless::AddToQueue(uint8_t *pucBuff, uint8_t ucLen,
//		Flag *peIfDone) {
//	if (ucLen) {
//		uint8_t ucTmp = ucLen; // save the information for later use
//		Flag *peTmp = peIfDone;
//		if (ucLen > 32) { // if a recursion will happen later, then redefine the data
//			ucLen = 32; // resize the data to 32 bytes
//			peIfDone = NULL; // ignore the flag
//		}
//		if (m_ucTxQueueLength == m_sProperty.ucMaxTxQueueLength) {
//			// if the number of packages in the TX queue exceeds the limit, then we should abandon one package
//			// according to the FIFO rule
//			if (((CChildData*) m_pTxQueueHead->m_pNext)->m_peIfDone)
//				*((CChildData*) m_pTxQueueHead->m_pNext)->m_peIfDone = Failed;
//			// set the flag to 'failed'
//			CWireless::m_ucTxQueueLength--; // decrease the counter
//			CWireless::CChildData *pTmp = m_pTxQueueHead; // change the m_pTxQueueHead to the next package
//			CWireless::m_pTxQueueHead = (CChildData*) (pTmp->m_pNext);
//			CDataControl::GetInstance()->AddToFreeList(pTmp); // will free the previous m_pTxQueueHead later
//		}
//		portENTER_CRITICAL();
//		if (!m_ucTxQueueLength) {
//			CEDown();
//			CMD(W_TX_PAYLOAD, pucBuff, ucLen); // if the the payload buffer is empty, then fill it
//			CEUp(); // start transmission
//		}
//		m_pTxQueueTail->m_pNext = new CChildData(pucBuff, ucLen, peIfDone, 0);
//		// allocate the space for a new package from the heap
//		m_pTxQueueTail = (CChildData*) (m_pTxQueueTail->m_pNext);
//		// add the new package to the tail of the TX queue
//		m_ucTxQueueLength++; // increase the counter
//		if (ucTmp > 32)
//			AddToQueue(pucBuff + 32, ucTmp - 32, peTmp); // a recursion here to handle data block larger than 32bytes
//		portEXIT_CRITICAL()
//		;
//	}
//	return m_ucTxQueueLength; // return the number of data blocks currently in the queue
//}

//void CWireless::PushString(const char *pcStr) {
//	uint8_t ucCount1 = 0, ucCount2 = 0;
//	for (char *pcPtr = (char*) pcStr; *pcPtr; pcPtr++) {
//		ucCount1++;
//		if ((*pcPtr) == '\n') {
//			AddToQueue((uint8_t*) (pcStr + ucCount2), ucCount1 - 1);
//			AddToQueue((uint8_t*) ("\r\n"), 2);
//			ucCount2 += ucCount1;
//			ucCount1 = 0;
//		}
//	}
//	AddToQueue((uint8_t*) (pcStr + ucCount2), ucCount1);
//}

//uint8_t CWireless::GetFromQueue(uint8_t* &pucBuff) {
//	if (m_ucRxQueueLength) {
//		portENTER_CRITICAL();
//		pucBuff = ((CChildData*) m_pRxQueueHead->m_pNext)->m_pucBuff;
//		// set the pointer to the data block
//		// the useful data is actually from m_pRxQueueHead->m_pDataNext
//		// the m_pRxQueueHead is dummy
//		uint8_t ucTmp = ((CChildData*) m_pRxQueueHead->m_pNext)->m_ucLen;
//		// get the length of the block
//		CChildData* pTmp = m_pRxQueueHead; // change the m_pRxQueueHead
//		m_pRxQueueHead = (CChildData*) (pTmp->m_pNext);
//		m_ucRxQueueLength--; // decrease the counter
//		CDataControl::GetInstance()->AddToFreeList(pTmp); // will free the previous m_pRxQueueHead later
//		portEXIT_CRITICAL();
//		return ucTmp; // return the length of the data block
//	}
//	pucBuff = NULL;
//	return 0;
//}

//uint8_t CWireless::FlushRxQueue(void) {
//	uint8_t ucTmp = m_ucRxQueueLength;
//	CChildData *pTmp1 = (CChildData*) (m_pRxQueueHead->m_pNext);
//	// initialize the pointer to the first package
//	while (ucTmp) { // use the temporary counter to ensure only those data already in the RX queue before
//		// this function is called will be deleted. New data arrived during the execution of the function
//		// will not be affected
//		m_pRxQueueHead = (CChildData*) (pTmp1->m_pNext); // change the head of the queue
//		pTmp1->AddSubBlockToFreeList(); // will free the allocated data block later
//		CChildData *pTmp2 = pTmp1;
//		pTmp1 = (CChildData*) (pTmp1->m_pNext); // set the pointer to the next package
//		CDataControl::GetInstance()->AddToFreeList(pTmp2); // will free the previous m_pRxQueueHead later
//		ucTmp--; // decrease the temporary counter
//		m_ucRxQueueLength--; // decrease the global counter
//	}
//	return m_ucRxQueueLength; // return the number of package remaining in the RX queue, usually 0
//}

void SetProperty(NRF24L01P_Property_t *psProperty) {
	//portENTER_CRITICAL();
	CEDown(); // to ensure being at standby mode
	WriteReg(RF_CH, (psProperty->ucCH) & 0x7F); // set the channel
	ChangeAddr(psProperty->pucAddr);
	CEUp(); // go to standby II mode
//	memcpy((void*) &m_sProperty, (const void*) psProperty, sizeof(NRF24L01P_Property_t)); // copy data
//	if (psProperty->ucMaxTxQueueLength > 20)
//		m_sProperty.ucMaxTxQueueLength = 20; // the TX queue size should be less than 20, or the receiver may crash
//	//portEXIT_CRITICAL();

}

uint8_t ReadReg(uint8_t ucAddr, uint8_t *pucData) {
//	portENTER_CRITICAL();
	Start_SS(); // pull down the CSN pin
	uint8_t ucTmp = TransChar(R_REGISTER | (REGISTER_MASK & ucAddr));
	// write the command to read a register and return the value in the status register
	*pucData = TransChar(NOP); // get the data from the register
	Stop_SS(); // pull up the CSN pin
//	portEXIT_CRITICAL();
	return ucTmp; // return the value in the status register
}




//void INT7_vect(void) {
//	uint8_t ucTmp1 = CMD(NOP); // read the status register
//	CEDown(); // go to standby mode
//	if (ucTmp1 & _BV(RX_DR)) { // new data available in the RX FIFO
//		if (m_ucRxQueueLength == NRF24L01P_mProperty.ucMaxRxQueueLength) {
//			// if the number of packages in the RX queue exceeds the limit, then we should abandon one package
//			// according to the FIFO rule ??
//			m_ucRxQueueLength--; // decrease the counter
//			CWireless::CChildData *pTmp2 = CWireless::m_pRxQueueHead; // change the m_pRxQueueHead to the next package
//			CWireless::m_pRxQueueHead
//					= (CWireless::CChildData*) (pTmp2->m_pNext);
//			pTmp2->AddSubBlockToFreeList(); // free the data block in that package
//			CDataControl::GetInstance()->AddToFreeList(pTmp2); // will free the previous m_pRxQueueHead later
//		}
//		uint8_t ucTmp2 = (ucTmp1 & RX_P_NO) >> 1, ucTmp3 = NOP; // put the pipe ID of the latest package in ucTmp2
//		pTmp->CMD(R_RX_PL_WID, &ucTmp3, 1, &ucTmp3); // read the length of the package
//		CWireless::m_pRxQueueTail->m_pNext = new CWireless::CChildData(NULL,
//				ucTmp3, NULL, ucTmp2);
//		// allocate the space for a new package from the heap
//		CWireless::m_pRxQueueTail
//				= (CWireless::CChildData*) (CWireless::m_pRxQueueTail->m_pNext);
//		// add the new package to the tail of the RX queue
//		CWireless::m_pRxQueueTail->m_pucBuff = (uint8_t*) malloc(
//				ucTmp3 * sizeof(uint8_t));
//		// allocate the space for the received data
//		pTmp->CMD(R_RX_PAYLOAD, CWireless::m_pRxQueueTail->m_pucBuff, ucTmp3,
//				CWireless::m_pRxQueueTail->m_pucBuff); // read the received data from the RX FIFO
//		CWireless::m_ucRxQueueLength++; // increase the counter
//	}
//	if (ucTmp1 & _BV(TX_DS)) { // a transmission is successful
//		if (((CWireless::CChildData*) CWireless::m_pTxQueueHead->m_pNext)->m_peIfDone)
//			*((CWireless::CChildData*) CWireless::m_pTxQueueHead->m_pNext)->m_peIfDone
//					= Done; // set the flag
//		CWireless::m_ucTxQueueLength--; // decrease the counter
//		CWireless::CChildData *pTmp2 = CWireless::m_pTxQueueHead; // change the m_pTxQueueHead to the next package
//		CWireless::m_pTxQueueHead = (CWireless::CChildData*) (pTmp2->m_pNext);
//		CDataControl::GetInstance()->AddToFreeList(pTmp2); // will free the previous m_pTxQueueHead later
//		if (CWireless::m_ucTxQueueLength) // if more packages are in the TX queue
//			pTmp->CMD(
//					W_TX_PAYLOAD,
//					((CWireless::CChildData*) CWireless::m_pTxQueueHead->m_pNext)->m_pucBuff,
//					((CWireless::CChildData*) CWireless::m_pTxQueueHead->m_pNext)->m_ucLen);
//		// write a new package to the TX FIFO
//	}
//	if (ucTmp1 & _BV(MAX_RT)) { // a transmission reaches the maximum re-transmission count
//		pTmp->CMD(FLUSH_TX); // flush the TX FIFO
//		pTmp->CMD(
//				W_TX_PAYLOAD,
//				((CWireless::CChildData*) CWireless::m_pTxQueueHead->m_pNext)->m_pucBuff,
//				((CWireless::CChildData*) CWireless::m_pTxQueueHead->m_pNext)->m_ucLen);
//		// rewrite the package to the TX FIFO
//		if (CWireless::m_ucFailureCount + 1) // increase the failure counter
//			CWireless::m_ucFailureCount++;
//	}
//	pTmp->WriteReg(STATUS, 0x7E); // clear the interrupt bits
//	pTmp->CEUp(); // return to PRX mode
//}
//

void CEDown(void){
	gpio_clr_gpio_pin(NRF24L01P_CE_PIN);	// Chip Disabled

}

void CEUp(void) {
	   gpio_set_gpio_pin(NRF24L01P_CE_PIN);  //Chip Enabled
}

void Start_SS(void)
{
	//NRF24L01P_CSN_PORT &= ~_BV(NRF24L01P_CSN_PIN);
	spi_selectChip(&AVR32_SPI,0);
	//delay_ms(1);


}

void Stop_SS(void)
{
	spi_unselectChip(&AVR32_SPI,0);
//	NRF24L01P_CSN_PORT |= _BV(NRF24L01P_CSN_PIN);
}

uint8_t WriteReg_2(uint8_t ucAddr, uint8_t *pucBuff, uint8_t ucLen) {
		return CMD_2(W_REGISTER | (REGISTER_MASK & ucAddr), pucBuff, ucLen, NULL);
	}

void ChangeAddr(uint8_t *pucAddr) {
		uint8_t *pucTmp;
		uint8_t i;
		pucTmp = (uint8_t*) alloca(sizeof(uint8_t[5])); // allocate enough space from the stack
		for (i = 5; i; i--) {
			*(pucTmp + i - 1) = pucAddr[5 - i]; // reverse the pipe0 address
			WriteReg_2(RX_ADDR_P0, pucTmp, 5); // set the pipe0 address
			WriteReg_2(TX_ADDR, pucTmp, 5); // set the TX address the same as the pipe0 address   --For PTX device only!
		}
	}

uint8_t TransChar(uint8_t ucData) {
	uint16_t tmpRead;

	while (!spi_writeRegisterEmptyCheck(&AVR32_SPI));
	spi_write(&AVR32_SPI,  ucData);

	//AVR32_SPI.sr & AVR32_SPI_SR_TDRE_MASK))

	while(! (spi_writeEndCheck(&AVR32_SPI))); //wait until previous transfer compolete

	while(! (spi_readRegisterFullCheck(&AVR32_SPI)));
	spi_read(&AVR32_SPI, &tmpRead);
	return (uint8_t)tmpRead;
}

void TransBlock(uint8_t *pucSend, uint8_t ucLen, uint8_t *pucReceive) {
	uint8_t i;
	uint8_t ucTmp;
	for (i = 0; i < ucLen; i++) {
		ucTmp = TransChar(*(pucSend + i));
		if (pucReceive)
			*(pucReceive + i) = ucTmp;
	}
}

void SPI_PutChar(uint8_t data)
{
	SPI_PutChar2(data,0);
}

void SPI_PutChar2(uint8_t data, uint8_t string)
{
	uint8_t tmpStatus;



	  //AVR32_SPI.IDR.tdre=1;
	volatile unsigned char tmpHEAD;
	tmpHEAD=(SPI_TxHead+1) & (SPI_TX_BUFFER_SIZE-1); //buffer about to full, refuse to write
	while (tmpHEAD==SPI_TxTail) ; //wait for freespace
	if (!string)
		{
			Disable_global_interrupt();
			AVR32_SPI.IDR.tdre=1;
//			tmpStatus=CMD(NOP);
			//while (!(tmpStatus & (1<<tdre)));

		}

	SPI_TxHead=tmpHEAD;
	SPI_TxBuf[SPI_TxHead]=data;
	SPI_Length++;

	if (!string)
		{
			AVR32_SPI.IER.tdre=1;
			Enable_global_interrupt();
		}

	//enable TX interrupt
	  /* setup SPI interrupt on TDRE Transmit Data Register Empty*/


// AVR32_SPI.ier=1<<AVR32_SPI_IER_TDRE_OFFSET;




}





void SPI_Puts(const char *s )
{
	Disable_global_interrupt();
	AVR32_SPI.IDR.tdre=1;
    while (*s!='\0')
    {
    	SPI_PutChar2(*s++, 1);
    }
    AVR32_SPI.IER.tdre=1;
	Enable_global_interrupt();


}/* uart_puts */
