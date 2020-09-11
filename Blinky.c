/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher and Graphic Demo
 * Note(s): 
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008-2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/
                  
#include <LPC17xx.H>                    /* NXP LPC17xx definitions            */
#include "string.h"
#include "LED.h"  
#include "GLCD.h"
#include "ADC.h"
#include "KBD.h" 
 
#include "grug.c" 
#include "hank.c" 
#include "lory.c"  
#include "music_note.c"

#define __FI        1                      /* Font index 16x24               */
#define __USE_LCD   1										/* Uncomment to use the LCD, uses a lot of CPU power, therefore simulations, leave commented */ 

extern unsigned char ClockLEDOn;
extern unsigned char ClockLEDOff;
extern unsigned char ClockANI;
extern unsigned char Bg_16bpp_t[];
extern unsigned char Bg_16bpp_l[];
extern unsigned char Bg_16bpp_r[];
extern unsigned char Bg_16bpp_b[];
extern unsigned char ARM_Ani_16bpp[];

uint16_t ADC_Dbg;
uint16_t kbd_actual_value = 0;  
uint32_t ad_avg = 0;
uint16_t ad_val = 0, ad_val_ = 0xFFFF;
unsigned int dir_cnt = 1; 
unsigned int menu_counter = 1; 
unsigned int selection_enable = 0; 
unsigned int buffer; 
unsigned volatile int incr = 0; 
unsigned volatile int down_gal = 0; 
unsigned volatile int down_mp3 = 0;
void delay (unsigned long input){ //gives delay in increment
unsigned long int1;  
unsigned long int2;	 
	for (int1 = input; int1 >= 1; int1 -- ){
		for (int2 = 1; int2 <= 786; int2 ++); 
} 
} 

//******************************************************

void mp3 (void){ 

	GLCD_SetTextColor(Black);
	GLCD_DisplayString(0, 0, __FI, "Adj. Vol w/ Pot"); 
	GLCD_DisplayString(8, 0, __FI, "Press down to exit"); 
	
	GLCD_Bitmap (  75, 300, 150,  150,  MUSIC_NOTE_C_PIXEL_DATA);   
  
	if (kbd_actual_value == 0x20){  
		selection_enable = 0; 
				incr = 0; 
				GLCD_Clear(Purple); 
				down_mp3 = 0; 		
	}
}


//******************************************************

//******************************************************
void gallery (void){   
	GLCD_SetTextColor(Black);
	GLCD_DisplayString(0, 0, __FI, "L/R Stick to shift"); 
	GLCD_DisplayString(8, 0, __FI, "Press down to exit");
	if (kbd_actual_value == 0x10){ 
				dir_cnt++;
	} 
	else if (kbd_actual_value == 0x40){ 
				dir_cnt--;
	} 
	if ( (dir_cnt < 1)  ){ 
				dir_cnt = 3;
	}else if (dir_cnt > 3){ 
				dir_cnt = 1;
	}
	switch (dir_cnt){  
		case 1:  
			GLCD_Bitmap (  75, 300, 150,  150, LORY_C_PIXEL_DATA);  
			break;
		case 2:    	  
			GLCD_Bitmap (  75, 300, 150,  150, GRUG_C_PIXEL_DATA);  
			break; 
		case 3:       	  
			GLCD_Bitmap (  75, 300, 150,  150, HANK_C_PIXEL_DATA);  
			break;  
	}  
	if (kbd_actual_value == 0x20){    
				selection_enable = 0; 
				incr = 0; 
				GLCD_Clear(Purple); 
				down_gal = 0;
	}

}
//************************************************************* 

void menu_selection(){ 
	switch (menu_counter){  
	case 1:   
		GLCD_SetBackColor(Cyan);	
		GLCD_DisplayString(4, 0, __FI, "Gallery");    
		GLCD_SetBackColor(White); 
		GLCD_DisplayString(6, 0, __FI, "Music");    
		GLCD_DisplayString(8, 0, __FI, "Game");
		break;
	case 2:   
		GLCD_SetBackColor(Cyan);	
		GLCD_DisplayString(6, 0, __FI, "Music");  
		GLCD_SetBackColor(White);   
		GLCD_DisplayString(8, 0, __FI, "Game"); 
		GLCD_DisplayString(4, 0, __FI, "Gallery"); 
		break;
	case 3:   
		GLCD_SetBackColor(Cyan);	
		GLCD_DisplayString(8, 0, __FI, "Game"); 
		GLCD_SetBackColor(White);	 
		GLCD_DisplayString(6, 0, __FI, "Music");  
		GLCD_DisplayString(4, 0, __FI, "Gallery"); 
		break;
	}

}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {                       /* Main Program                       */
  LED_Init ();
  GLCD_Init();  
	KBD_Init();    
	ADC_Init();  
GLCD_Clear  (Purple); 		
GLCD_SetTextColor(Black);
GLCD_DisplayString(4, 0, __FI, "Gallery");   
GLCD_DisplayString(6, 0, __FI, "Music");    
GLCD_DisplayString(8, 0, __FI, "Game");
 

//unsigned int incr = 0;
for (;;) {                            /* Loop forever                       */  

	
	delay(5000);
	
  kbd_actual_value = get_button(); 
if (kbd_actual_value == 0x20){  
			menu_counter ++;
} 
else if (kbd_actual_value == 0x08){ 
			menu_counter --;
} 

if (kbd_actual_value == 0x01){  
	selection_enable = 1;
} 


// Ensuring a circular menu style for both menu and gallery
 if ( (menu_counter < 1)  ){ 
			menu_counter = 3;
}else if (menu_counter > 3){ 
			menu_counter = 1;
} 
//////////////////////////////////////////////////////////////////// 
if (selection_enable == 0){
	menu_selection();
}  
else {   
		if (incr == 0){ 
			GLCD_Clear(Purple); 	
		}  
		incr = 1;
}


if ( selection_enable == 1 && menu_counter == 1){ 
	gallery();
}  
if ( selection_enable == 1 && menu_counter == 2){  
	
	mp3();
} 


		
/*	
#define KBD_SELECT 0x01
#define KBD_UP 0x08
#define KBD_RIGHT 0x10
#define KBD_DOWN 0x20
#define KBD_LEFT 0x40
#define KBD_MASK 0x79 
*/
  }
}
