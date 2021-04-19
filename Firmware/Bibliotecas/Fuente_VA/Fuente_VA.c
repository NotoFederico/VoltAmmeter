#include "stdio.h"
#include "stdint.h"
#include "math.h"
#include "main.h"
#include "Fuente_VA.h"
#include "Fuente_TH.h"
#include "lcd.h"

#define I2C_ADDRESS 0x90
extern I2C_HandleTypeDef hi2c1;

float tension=0.0000, corriente=0.0000, consumo=0.0000,suma=0,resultado=0,ca2=0;
uint8_t reset[1]=  	{0x06};
uint8_t conv[1] =   {0x00}; //Inicia la conversion
uint8_t tbuf_v[3] = {0x01,0xE3,0xA3};
uint8_t rbuf_v[3] = {0,0,0};
uint8_t tbuf_a[3] = {0x01,0x87,0xA3};
uint8_t rbuf_a[3] = {0,0,0};
uint8_t MSB,LSB;
char  aux[6];
int contador=0, flag_neg=0;

void  ADS1115_Update	()
{

	//Corriente
		ADS1115_Write_A();
		HAL_Delay(50);
		corriente=ADS1115_Read_A();
	  sprintf(aux,"%.4f ",corriente);
	  posic_lcd(0x40+9);
		print(aux);

	//Tension
		ADS1115_Write_V();
		tension=ADS1115_Read_V();
		sprintf(aux,"%.4f ",tension);
	  posic_lcd(0x00+9);
		print(aux);

	//Consumo
		consumo=tension*corriente;
	  sprintf(aux,"%.4f ",consumo);
	  posic_lcd(0x14+9);
		print(aux);


}


void ADS1115_Write_V()
{

	  //////////////////////////////////////////////
	 //					Escritura				 //
	//////////////////////////////////////////////

	//1. Se apunta al registro de configuracion
		HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t*)tbuf_v, 3, 10000);

	//4. Se escribe en el registro de la direccion del puntero
	//	 para iniciar la conversion

		HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t*)conv, 1, 10000);


}


float ADS1115_Read_V()
{

	  //////////////////////////////////////////////
	 //									Lectura						     	 //
	//////////////////////////////////////////////
	//6. Se leen los dos bytes del registro de conversion

	while(contador<=500)
	{
		contador++;
		HAL_I2C_Master_Receive(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)rbuf_v, 3, 10000);
		MSB=rbuf_v[0];
		LSB=rbuf_v[1];

		resultado=((((MSB<<8)|LSB)*FSR_V)/32767);
		suma=resultado+suma;
	}
		resultado=(suma/500);

//Si la medicion de corriente arroja un valor negativo, la medicion de tension debe ajustarse		
	
	if(flag_neg==1)
			resultado=resultado*-1;
		
		
// Es posible que la fuente entregue valores negativos al limitar por corriente o que al estar muy cerca 
// del cero, por ello se pone en 0.0000 V de forma forzada si ocurre cualquiera de estos dos escenarios
		if(MSB>=128) 
			resultado=0.0000;
		
		contador=0;
		suma=0;

			return resultado;

}

void ADS1115_Write_A()
{

	  //////////////////////////////////////////////
	 //					Escritura				 //
	//////////////////////////////////////////////

	//1. Se apunta al registro de configuracion

		HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t*)tbuf_a, 3, 10000);


	//4. Se escribe en el registro de la direccion del puntero
	//	 para iniciar la conversion

		HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t*)conv, 1, 10000);


	}

float ADS1115_Read_A()
{
		  //////////////////////////////////////////////
		 //					Lectura			     	 //
		//////////////////////////////////////////////
		//6. Se leen los dos bytes del registro de conversion

		while(contador<=500)
		{
			contador++;
			HAL_I2C_Master_Receive(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t *)rbuf_a, 3, 10000);

			MSB=rbuf_a[0];
			LSB=rbuf_a[1];

			if(MSB>0x80)

				{
				ca2=pow(2,16)-((MSB<<8)|LSB);
				resultado=((ca2*FSR_A)/32767);
				suma=resultado+suma;
				flag_neg=1;
				}
				else
					{
					resultado=((((MSB<<8)|LSB)*FSR_A)/32767);
					suma=resultado+suma;
					flag_neg=0;
					}
		}
			resultado=((suma/500)*2.9296875);
			if(flag_neg==1)
				resultado=resultado*-1;

			contador=0;
			suma=0;

				return resultado;

}
