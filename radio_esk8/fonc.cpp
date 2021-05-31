/*fonc.cpp*/

#include "Arduino.h"




//variable interne
extern int bouton_read;

extern String state_bouton;
extern String last_state_bouton;



String bouton_ana(){					//lecture du bouton renvoi str
	
	int analog0;
	String read;
	analog0 = analogRead(bouton_read);
	//Serial.println(analog0);
	delay(50);
	if (analog0 == 000) {read = "bas";}
	if (analog0 <= 420 && analog0 >400) {read = "droit";}
	if (analog0 <= 621 && analog0 >601) {read = "haut";}
	if (analog0 <= 754 && analog0 >734) {read = "gauche";}
	if (analog0 <= 845 && analog0 >825) {read = "mid";}
	if (read != last_state_bouton){
	//	Serial.println(analog0);	//pour debug lecture bouton
		last_state_bouton = read;
		return read;
	}
	else return "";
}

extern int menu;

void count_menu(){ //incremente ou decrement le menu 
	
	if (state_bouton == "haut"){
		menu ++;
		if (menu > 3){menu = 1;}
	}
	
	if (state_bouton == "bas"){
		menu --;
		if (menu <= 0){menu = 3;}
	}
}

extern bool creneau;
extern long time_clignote_true;
extern long time_clignote_false;

bool clignotement(){ // clignotement de la fonction avec la variable creneau //0 off 1 on display

	if(creneau)time_clignote_true=millis();
	if(!creneau)time_clignote_false=millis();

	if(millis()-time_clignote_false>500){	// temps off
		creneau = false;
		//Serial.println(creneau);
	}
	
	if(millis()-time_clignote_true >900){ // temps on
		creneau = true;
		//Serial.println(creneau);
	}	
	
	return creneau;
}


extern float dist;
extern float speed;

void dist_cal(){; // calcul de la distance par integration de la vitesse 
	dist =dist+(float)speed/3600,4;
	

}











