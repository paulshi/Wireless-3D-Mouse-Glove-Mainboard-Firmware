#include "nrf24l01p.h"
#include <avr32/io.h>
#include <compiler.h>
#include <power_clocks_lib.h>

//#include "print_funcs.h"
#include <intc.h>
#include <pm.h>
#include <delay.h>
#include <stdint.h>
#include <flashc.h>


int main(void)
{
	uint8_t i;
	uint8_t temp;
	uint32_t temp32;
	//const char st[]="01234567890123456789012345678901abcd\r\n";

	static const gpio_map_t NRF24L01P_SPI_GPIO_MAP =
	  {
		{AVR32_PIN_PA15, AVR32_SPI_SCK_0_0_FUNCTION},
	    {AVR32_PIN_PA28, AVR32_SPI_MISO_0_2_FUNCTION},
	    {AVR32_PIN_PA14, AVR32_SPI_MOSI_0_0_FUNCTION},
	    {AVR32_PIN_PA16, AVR32_SPI_NPCS_0_0_FUNCTION}
	  };


	 // Switch the CPU main clock to oscillator 0

//	 pm_enable_osc0_crystal(&AVR32_PM, FOSC0); //start OSC0
//	 pm_enable_clk0(&AVR32_PM, 3); //Enable CLK0
//
//	 pm_pll_setup(&AVR32_PM, 0, 6, 1, 0, 16); // Set PPL0, Let VCO be 84Mhz
//	 pm_pll_set_option(&AVR32_PM, 0, 0, 0, 0); // set PPL Option, Let Fvco=84Mhz
//	 pm_pll_enable(&AVR32_PM, 0);
//	 pm_wait_for_pll0_locked(&AVR32_PM);
//	 flashc_set_wait_state(1);
//	 pm_cksel(&AVR32_PM, 1, 0, 1, 0, 0, 0); // Set Clock for each bus
//	 pm_switch_to_clock(&AVR32_PM, AVR32_PM_MCSEL_PLL0); //main clock to be PLL0

	 //INTC_init_interrupts();
	  pm_switch_to_osc0(&AVR32_PM, FOSC0, 60);

		gpio_enable_gpio_pin(AVR32_PIN_PA23);
		   gpio_set_gpio_pin(AVR32_PIN_PA23);	//enabled
		   //gpio_set_gpio_pin(AVR32_PIN_PA23); //100 set for channel 4


	  // add the spi options driver structure for NRF24L01P
	  spi_options_t spiOptions =
	  {
	    .reg          = 0, //Channel, unsure, should be 0
	    .baudrate     = 8000000,
	    .bits         = 8,
	    .spck_delay   = 0,
	    .trans_delay  = 0,
	    .stay_act     = 1,
	    .spi_mode     = 0,
	    .modfdis      = 1
	  };


	  Disable_global_interrupt();
	  INTC_init_interrupts();
	  Enable_global_interrupt();


	  // Assign I/Os to SPI
	  gpio_enable_module(NRF24L01P_SPI_GPIO_MAP,
	                     sizeof(NRF24L01P_SPI_GPIO_MAP) / sizeof(NRF24L01P_SPI_GPIO_MAP[0]));

	  // Initialize as master
	  spi_initMaster(&AVR32_SPI, &spiOptions);

	  // Set selection mode: variable_ps, pcs_decode, delay
	  spi_selectionMode(&AVR32_SPI, 0, 0, 0);



	  // setup chip registers
	  spi_setupChipReg(&AVR32_SPI, &spiOptions, FOSC0);

	  // Enable SPI
	  spi_enable(&AVR32_SPI);








//	  dip204_example_configure_push_buttons_IT();
//
//	  // configure local joystick
//	  dip204_example_configure_joystick_IT();

	  // initialize delay driver
	  delay_init( FOSC0);

	  // initialize NRF24L01P
	  //dip204_init(backlight_PWM, TRUE);

	  // reset marker
	  //current_char = 0x10;

	  // Display default message.
//	  dip204_set_cursor_position(8,1);
//	  dip204_write_string("ATMEL");
//	  dip204_set_cursor_position(7,2);
//	  dip204_write_string("EVK1100");
//	  dip204_set_cursor_position(6,3);
//	  dip204_write_string("AVR32 UC3");
//	  dip204_set_cursor_position(3,4);
//	  dip204_write_string("AT32UC3A Series");
//	  dip204_hide_cursor();

	  //Initializing NRF24L01P
	  NRF24L01P_Init(NRF24L01P_mProperty);
//
//	  SPI_PutChar('h');
//	  	SPI_PutChar('e');
//	  	SPI_PutChar('l');
//	  	SPI_PutChar('l');
//	  	SPI_PutChar('o');
//	  	SPI_PutChar('1');
//	  	SPI_PutChar('2');
//	  	SPI_PutChar('3');
//	  	SPI_PutChar('4');
//	  	SPI_PutChar('5');
//	  	SPI_PutChar('6');

while(1)
{


	//SPI_Puts("dddasdfasdfsadfsadf Hello!!! :D :D :D \n\r");
	SPI_Puts("01234567890123456789012345678901abcd\r\n");
	delay_ms(10);
	//SPI_Puts("helloworldhelloworldhelloworldhel");
	//SPI_Puts("hello world! :D \r\n");
	//delay_ms(5000);
	//SPI_PutChar('a');
	//SPI_PutChar('b');
//	SPI_Puts("a\r\n");
//	SPI_Puts("b\r\n");


	//delay_ms(1000);
	//SPI_PutChar('1');
////	SPI_PutChar(temp);


//	CEDown();
//	Start_SS();
//	CMD(FLUSH_TX);
//	Stop_SS();
//	Start_SS();
//while (!AVR32_SPI.SR.tdre);
//
//	AVR32_SPI.TDR.td=W_TX_PAYLOAD;
////	spi_write(&AVR32_SPI,  W_TX_PAYLOAD);
//
//	for (i=0; i<32; i++)
//	{
//		while (!AVR32_SPI.SR.tdre);
//		AVR32_SPI.TDR.td=i;
//	}
//
//	Stop_SS();
//	CEUp();

	//delay_ms(100);

////
//	gpio_tgl_gpio_pin(AVR32_PIN_PA23);
//	delay_ms(1000);
//

//	AVR32_SPI.IER.tdre=1;
//	temp='C';
//	Start_SS(); // pull down the CSN pin
//	TransChar(W_TX_PAYLOAD); // write the command and receive the value in the status register
//	TransChar(temp);
//	Stop_SS();
	//delay_ms(10);
	//temp="hello damn world!  ";


	//SPI_Puts("hello");

//
//		Start_SS(); // pull down the CSN pin
//		CEDown();
//		TransChar(W_TX_PAYLOAD); // write the command and receive the value in the status register
//		TransChar('1');
//		//TransBlock(SPI_TxBuf, SPI_Length , NULL); // write and receive a block of data with length equals to ucLen
//		//SPI_Length--;
//		CEUp();
//		Stop_SS();
//		delay_ms(10);
//		Start_SS(); // pull down the CSN pin
//		CEDown();
//		TransChar(W_TX_PAYLOAD); // write the command and receive the value in the status register
//		TransChar('2');
//		//TransBlock(SPI_TxBuf, SPI_Length , NULL); // write and receive a block of data with length equals to ucLen
//		//SPI_Length--;
//		CEUp();
//		Stop_SS();
//		delay_ms(10);
//		Start_SS(); // pull down the CSN pin
//		CEDown();
//		TransChar(W_TX_PAYLOAD); // write the command and receive the value in the status register
//		TransChar('3');
//		//TransBlock(SPI_TxBuf, SPI_Length , NULL); // write and receive a block of data with length equals to ucLen
//		//SPI_Length--;
//		CEUp();
//		Stop_SS();
//		delay_ms(10);
//		Start_SS(); // pull down the CSN pin
//		CEDown();
//		TransChar(W_TX_PAYLOAD); // write the command and receive the value in the status register
//		TransChar('4');
//		//TransBlock(SPI_TxBuf, SPI_Length , NULL); // write and receive a block of data with length equals to ucLen
//		//SPI_Length--;
//		CEUp();
//		Stop_SS();
//		delay_ms(10);
//		Start_SS(); // pull down the CSN pin
//		CEDown();
//		TransChar(W_TX_PAYLOAD); // write the command and receive the value in the status register
//		TransChar('5');
//		//TransBlock(SPI_TxBuf, SPI_Length , NULL); // write and receive a block of data with length equals to ucLen
//		//SPI_Length--;
//		CEUp();
//		Stop_SS();
//		delay_ms(10);
//		Start_SS(); // pull down the CSN pin
//		CEDown();
//		TransChar(W_TX_PAYLOAD); // write the command and receive the value in the status register
//		TransChar('6');
//		//TransBlock(SPI_TxBuf, SPI_Length , NULL); // write and receive a block of data with length equals to ucLen
//		//SPI_Length--;
//		CEUp();
//		Stop_SS();
}




	return 0;
}
