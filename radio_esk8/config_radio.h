/*config_radio.h*/

// mode skate par defaut 
const int mode_esc = 1; // 1 souple 2 normale 3 no brain 


//constante skate
const float pi=3.1414159; // pi reste une constante !!!
const float diametre_roue = 55; // dimetre de la roue en mm 
const float gear_ratio = 1.5; // 10 trs moteur / 1 tour roue // 1 pour test

// menu par defaut
int menu = 1; //1 defaut,2 etc...




//************************************************//
//parametre de l'ecran LCD

#define SCREEN_WIDTH 128	
#define SCREEN_HEIGHT 64 
#define OLED_RESET     4 
#define SCREEN_ADDRESS 0x3C 

//structure rf

struct data_out { 		// de la radio vers le skate
	int thr; 			// throttle
	bool mode = 0; 		// true demmande de data 

}payload_out;

struct data_in { 		// du skate vers radio 
	float tension_bat; 		// v_in
	float courant;			//current_in
	float amp_hours;
	float watt_hours;
	int thachy;				//tachometer_abs
}payload_in;

//config radio
RF24 radio(7, 8); // 7 pour CE 8 pour CSN
uint64_t address[] = {0xF9F9F9F9F9F9,0xE9E8F0F0E1LL}; // adresse tx / rx


// set radio
bool radioNumber = 0; //0 telecommmande 1 vesc
bool radio_role = 0; // 0 rx 1 tx



//lecture ana
int bouton_read = A0; // affectation du bouton selecteur 






























