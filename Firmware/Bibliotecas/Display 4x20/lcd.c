#include "main.h"
#include "lcd.h"

#define DISPLAY_PORT GPIOA->ODR
#define RS 0x00000010
#define E  0x00000020


void busy(void)
{
 HAL_Delay(1);
}


void lcd_wr (char datos)
{
	  datos >>= 4;         
    DISPLAY_PORT &= ~0x0000003F;	
    DISPLAY_PORT |= datos;    
	  DISPLAY_PORT &= ~RS;
    DISPLAY_PORT |= E;	
    HAL_Delay(1);
		DISPLAY_PORT &= ~E;
    busy();      

}

void lcd_wr_instr4bits (char datos)
{
	char aux;
	
    aux = datos;
    aux >>= 4;
    aux &= 0x0f;
    DISPLAY_PORT &= ~0x0000003F;
    DISPLAY_PORT |= aux;
	  DISPLAY_PORT &= ~RS;
    DISPLAY_PORT |= E;	
    HAL_Delay(1);
		DISPLAY_PORT &= ~E;
   	busy();     

   	aux = datos;
    aux &= 0x0f;
    DISPLAY_PORT &= ~0x0000003F;
    DISPLAY_PORT |= aux;
	  DISPLAY_PORT &= ~RS;
    DISPLAY_PORT |= E;	
    HAL_Delay(1);
		DISPLAY_PORT &= ~E;
    busy();     
}

void lcd_wr_data4bits (char datos)
{
	unsigned char aux;
	
    aux = datos;
    aux >>= 4;
    aux &= 0x0f;
    DISPLAY_PORT &= ~0x0000003F;
    DISPLAY_PORT |= aux;
	  DISPLAY_PORT |= RS;
	  DISPLAY_PORT |= E;
    HAL_Delay(1);
		DISPLAY_PORT &= ~E;
    busy();     

   	aux = datos;
    aux &= 0x0f;
    DISPLAY_PORT &= ~0x0000003F;
    DISPLAY_PORT |= aux;
	  DISPLAY_PORT |= RS; 
    DISPLAY_PORT |= E;	
    HAL_Delay(1);
		DISPLAY_PORT &= ~E; 
    busy();   	
}

void init_lcd()
{
	HAL_Delay(200); 
	HAL_Delay(200); 
  lcd_wr(0x30);//30
  HAL_Delay(30);
  lcd_wr(0x30);//30
  HAL_Delay(30);
  lcd_wr(0x30);//30
  HAL_Delay(30);
  lcd_wr(0x20);
  busy();
  lcd_wr_instr4bits(0x28);//2 lineas-4 bits-caracteres 5*8
  busy();
  lcd_wr_instr4bits(0x06);
  busy();
  lcd_wr_instr4bits(0x0c);
  busy();
  lcd_wr_instr4bits(0x01);
  busy();
}

void cls_lcd(void)
{
     busy();   
     lcd_wr_instr4bits(0x01);

}
   

void posic_lcd(char adress)
{
     busy();
     lcd_wr_instr4bits(adress | 0x80);
     
}


void wrchar(unsigned char dato)
{
  lcd_wr_data4bits(dato);  
}


void print (char cadena[]){

  char i;    

  i = 0;
  while (cadena[i]!='\0'){
    wrchar(cadena[i]);
        i++;}  
}

void print1 (char cadena[]){
  char i;    

  posic_lcd(0x00);
  i = 0;
  while (cadena[i]!='\0'){
    wrchar(cadena[i]);
        i++;}  

}

void print2 (char cadena[]){
  char i;    

  posic_lcd(0x40);
  i = 0;
  while (cadena[i]!='\0'){
    wrchar(cadena[i]);
        i++;}  

}


void print3 (char cadena[]){
  char i;    

  posic_lcd(0x14);
  i = 0;
  while (cadena[i]!='\0'){
    wrchar(cadena[i]);
        i++;}  

}

void print4 (char cadena[]){
  char i;    

  posic_lcd(0x54);
  i = 0;
  while (cadena[i]!='\0'){
    wrchar(cadena[i]);
        i++;}  

}

void cursor_normal(void)
{
  lcd_wr_instr4bits(0x0c);  //cursor normal
}

void cursor_titilante(void)
{
  lcd_wr_instr4bits(0x0d);  //cursor titilante
}


void print_int_dec (unsigned value) //de "00" a "65535" (int)
{
unsigned char dmil=0,umil=0,centena=0,decena=0,unidad=0;

unidad = value % 0x0a;
decena = (value/10) % 0x0a;
centena = (value/100) % 0x0a;
umil = (value/1000) % 0x0a;
dmil = (value/10000) % 0x0a;

if (dmil) wrchar(dmil | 0x30);
if (umil) wrchar(umil | 0x30);
if (centena) wrchar(centena | 0x30);
 wrchar(decena | 0x30);
 wrchar(unidad | 0x30);
}


void print_hex(unsigned char dato)
{
   unsigned char aux;

   aux = dato;
   aux &= 0xf0;
   aux >>= 4;
   if (aux > 9) aux += 0x37;
     else aux += 0x30;
   wrchar(aux);

   aux = dato;
   aux &= 0x0f;
   if (aux > 9) aux += 0x37;
     else aux += 0x30;
   wrchar(aux);

}

void print_int_hex(unsigned int dato)
{
   print_hex(dato >> 8);
   print_hex(dato & 0xff);
}

