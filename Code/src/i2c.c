#include "i2c.h"

/********************************************************************/
void I2C1_Init(void){
	
	RCC->CCIPR |= (1<<12);						// I2C1 Clock SRC - SYSCLK
	RCC->IOPENR |= RCC_IOPENR_GPIOBEN;			// Clock GPIOB
	RCC->APBENR1 |= RCC_APBENR1_I2C1EN;			// Clock I2C1
	
	GPIOB->AFR[1] |= (6<<0);
	GPIOB->AFR[1] |= (6<<4);					// PB8, PB9 - GPIO AF6
	
	// PB8 SCL
	GPIOB->MODER &= ~GPIO_MODER_MODE8;
	GPIOB->MODER |= GPIO_MODER_MODE8_1;			// AF OD
	GPIOB->OTYPER |= GPIO_OTYPER_OT8;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED8;		// Full speed
	
	// PB9 SDA
	GPIOB->MODER &= ~GPIO_MODER_MODE9;
	GPIOB->MODER |= GPIO_MODER_MODE9_1;			// AF OD
	GPIOB->OTYPER |= GPIO_OTYPER_OT9;
	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED9;		// Full speed
	
	I2C1->TIMINGR = 0x10707DBC;					// 100KHz 
	//I2C1->TIMINGR = 0x00602173;				// 400KHz 
	
	I2C1->CR1 |= I2C_CR1_TXIE; 
	I2C1->CR1 |= I2C_CR1_RXIE; 
	I2C1->CR1 |= I2C_CR1_STOPIE;   
	I2C1->CR1 |= I2C_CR1_ERRIE; 
	I2C1->CR1 |= I2C_CR1_PE;                    // EnableI2C1

}
/********************************************************************/
void i2c_Start_addr(I2C_TypeDef* I2Cx, uint8_t addr, uint8_t dir){
	
	switch(dir){
		
		case 0:
    I2Cx->CR2 &= ~I2C_CR2_RD_WRN;
		I2Cx->CR2 |= (uint8_t)(addr<<1)+0; 
			break;
		case 1:
    I2Cx->CR2 |= I2C_CR2_RD_WRN;
		I2Cx->CR2 |= (uint8_t)(addr<<1)+1; 
			break;
		
  }
  
}
/******************************************************************************/
void i2c_Write(I2C_TypeDef* I2Cx, uint8_t mode, uint8_t data){
	
	switch(mode){
		
		case _I2C_MASTER_SINGLE_SEND:	
			default:
      I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_AUTOEND | I2C_CR2_START ;        
      while (!(I2Cx->ISR & I2C_ISR_TXIS)){}
      I2C1->TXDR = (uint8_t)data;   
      while (!(I2Cx->ISR & I2C_ISR_STOPF)){}         
      I2Cx->ICR = I2C_ICR_STOPCF; 
			break;
				
		case _I2C_MASTER_BURST_SEND_START:
			I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_RELOAD | I2C_CR2_START ;  
      while (!(I2Cx->ISR & I2C_ISR_TXIS)){}
      I2Cx->TXDR = (uint8_t)(data);  
      while (!(I2Cx->ISR & I2C_ISR_TCR)){} 
			break;
				
		case _I2C_MASTER_BURST_SEND_CONT:
			I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_RELOAD;  
      I2Cx->TXDR = (uint8_t)(data);     
      while (!(I2Cx->ISR & I2C_ISR_TCR)){} 
			break;
				
		case _I2C_MASTER_BURST_SEND_FINISH:
			I2Cx->CR2 |=(uint32_t)(1 << 16) | I2C_CR2_AUTOEND;    
      while (!(I2Cx->ISR & I2C_ISR_TXIS)){} 
      I2Cx->TXDR = (uint8_t)data; 
      I2Cx->CR2 &= ~I2C_CR2_RELOAD;   
      while (!(I2Cx->ISR & I2C_ISR_STOPF)){}   	
			break;
		
  }
	I2Cx->CR2&= ~I2C_CR2_NBYTES;
	
}
/******************************************************************************/
void i2c_Read(I2C_TypeDef* I2Cx, uint8_t *data ,uint8_t mode){
	
	switch(mode) {
		
    case _I2C_MASTER_SINGLE_RECEIVE :
    default:
      I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_AUTOEND | I2C_CR2_START ;        
      while (!(I2Cx->ISR & I2C_ISR_RXNE)){}	
      *data = (uint8_t)I2Cx->RXDR;   
      while (!(I2Cx->ISR & I2C_ISR_STOPF)){}         
      I2Cx->ICR = I2C_ICR_STOPCF; // Clear STOPF flag 
       
      break;
      
    case _I2C_MASTER_BURST_RECEIVE_START :
      I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_RELOAD | I2C_CR2_START ;        
      while (!(I2Cx->ISR & I2C_ISR_RXNE)){}
      *data = (uint8_t)I2Cx->RXDR;     
      while (!(I2Cx->ISR & I2C_ISR_TCR)) {} 
				
      break;
				
    case _I2C_MASTER_BURST_RECEIVE_CONT :
      I2Cx->CR2 |= (uint32_t)(1 << 16) | I2C_CR2_RELOAD;  
      *data = (uint8_t)I2Cx->RXDR;   
       while (!(I2Cx->ISR & I2C_ISR_TCR)) {}  
				 
      break;
				 
    case _I2C_MASTER_BURST_RECEIVE_FINISH:
      I2Cx->CR2 |=(uint32_t)(1 << 16)| I2C_CR2_AUTOEND;
      *data = (uint8_t)I2Cx->RXDR;   
       while (!(I2Cx->ISR & I2C_ISR_TCR)){}                                  
      I2Cx->CR2 &= ~I2C_CR2_RELOAD;     
      while (!(I2Cx->ISR & I2C_ISR_STOPF))  {}   
      I2Cx->ICR = I2C_ICR_STOPCF;  // Clear STOPF flag
      
      break;
  }

  I2Cx->CR2&= ~I2C_CR2_NBYTES; //Clear NBYTES
	
}
/******************************************************************************/
void i2c_BUSY(I2C_TypeDef* I2Cx){
	
	while(I2Cx->ISR & 0x8000){};
	
}
/******************************************************************************/

