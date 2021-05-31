/*display.h*/

#include "Arduino.h"
#include "fonc.h"
#include <Adafruit_SSD1306.h>



extern int menu;
extern int bat_level;
extern float dist;
extern float speed;
extern Adafruit_SSD1306 display;
extern uint64_t address[];

void switch_ecran(){
	display.clearDisplay();
	display.setTextColor(SSD1306_WHITE);
	display.cp437(true);         
	display.setTextSize(2);	
	
	switch (menu){
	case 1://
		display.setCursor(00,00);
		display.print(bat_level);
		display.setCursor(112,00);
		display.print("1");
		
		
		if (bat_level <= 25 && clignotement()){ //ajouter un truk pour faire clignoter la batterie 
			display.drawRect(6,16,8,2, BLACK); // teton petite batterie		
			display.drawRect(0,18,20,46, BLACK); // rectangle petite batterie
			display.fillRect(1,int((1-(float)bat_level/100)*44+19),18,int((float)bat_level/100*60),BLACK);	//60 car fait de la merde en bas a cause des float
		}
		else{
			display.drawRect(6,16,8,2, WHITE); // teton petite batterie		
			display.drawRect(0,18,20,46, WHITE); // rectangle petite batterie
			display.fillRect(1,int((1-(float)bat_level/100)*44+19),18,int((float)bat_level/100*60),WHITE); // rectangle plein 	

			
			
		}
		display.setCursor(28,16);
		display.print(speed,1);//,1 pour 1 seul carac apres la virgule
		display.setTextSize(1);
		display.print(" Km/h");
		display.setTextSize(2);
		display.setCursor(28,36);
		display.print(dist,2);
		display.setTextSize(1);
		display.print(" m");
		display.setCursor(28,55);
		display.print("3500 mA/h");
		display.display();

		break;
	case 2:
		display.setCursor(00,00);
		display.print("   Ecran 2");
		display.display();

		break;
	case 3:
		display.setCursor(00,00);
		display.print("   Ecran 3");
		display.display();

		break;
		
	case 99:
		display.setTextSize(2); 
		display.setCursor(00,00);
		display.println("  TX fail");
		display.setTextSize(1);
		display.println("Merci de bien allumer le Skate");

		display.display();

		break;
	}
	
	
}