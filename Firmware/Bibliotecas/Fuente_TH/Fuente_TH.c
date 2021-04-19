/*
 * Fuente_TH.c
 *
 *      Author: Federico Noto
 */
#include "stdio.h"
#include "stdint.h"
#include "main.h"
#include "Fuente_TH.h"
#include "lcd.h"
#define I2C_ADDRESS 0x40
#define VUELTAS 10
#define LIM_T_MAX 40
extern I2C_HandleTypeDef hi2c1;
uint8_t wbufhum[1]=  {0xE5};
uint8_t wbuftemp[1]= {0xE3};
uint8_t rbuf[3] = {0,0,0};
int hyst=0;

float Humidity()
{
	//////////////////////////////////////////////
 //						Medicion de Humedad						 //
//////////////////////////////////////////////
	uint8_t MSB,LSB;
	float promedio=0,hum;
	int contador=VUELTAS; 
	
	while(contador<=VUELTAS)
	{
		HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t*)wbufhum, 1, 10000);
		HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t*)rbuf, 3, 10000);
    MSB=rbuf[0];
    LSB=rbuf[1];
    hum=((((MSB<<8)|LSB)*125)/65536.00)-6;
 		promedio=hum+promedio;
	}	
		hum=promedio/VUELTAS;
		if(hum>=100)
        hum=99.9;
    return hum; //devuelve el valor de la humedad medida
}

float Temperature()
{
  ///////////////////////////////////////////////
 //						Medicion de Temperatura					//
///////////////////////////////////////////////
	uint8_t MSB,LSB;
	float promedio=0,temp; 
	int contador=VUELTAS; 
	
while(contador<=VUELTAS)
	{
		HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t*)wbuftemp, 1, 10000);
		HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)I2C_ADDRESS, (uint8_t*)rbuf, 3, 10000);
    MSB=rbuf[0];
    LSB=rbuf[1];
    temp=(((((MSB<<8)|LSB))*175.72)/65536)-46.85;
		promedio=temp+promedio;
	}
		temp=promedio/VUELTAS;
    return temp; //devuelve el valor de la temperatura medida en grados centigrados
}

void TH_Update() // Muestra las magnitudes de humedad y temperatura en pantalla y por ahora tambien activa/desactiva el cooler.
{
	char  aux[6]; float temp; 
	
	//Temperatura
		sprintf(aux,"%.4f ",Temperature());
	  posic_lcd(0x00+9);
		print(aux);
	
	//Humedad
	  sprintf(aux,"%.4f ",Humidity());
	  posic_lcd(0x14+9);
		print(aux);
}

void TH_Cooler()
{
	float  temp = Temperature();
	// Quizas esta porcion de codigo, estaria bueno ponerlo en alguna rutina de interrupcion
		if((temp+hyst)>=LIM_T_MAX)
				{
				//estado alto de señal para activar el cooler
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET); //pin elegido al azar (ver)
				//"histeresis", cantidad de grados que se agregan para que el cooler no conmute innecesariamente
				//alrrededor del valor maximo, cuando desciendan la x cant de grados de hyst, se apaga el cooler.	
				hyst=3;	 
				}				
				else
						{
						//estado bajo de señal para desactivar el cooler
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
						hyst=0;
						}					
	
}

