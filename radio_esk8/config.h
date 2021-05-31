/*config.h*/

// mode skate par defaut 
const int modeesc = 1; // 1 souple 2 normal 3 no brain 


//constante skate
const float pi=3.1414159; // pi reste une constante !!!
const float diametre_roue = 55; // dimetre de la roue en mm 
const float gear_ratio = 10.0; // 10 trs moteur / 1 tour roue // 1 pour test

// menu par defaut
int menu = 1; //1 defaut,2 etc...




//************************************************//
//parametre de l'ecran LCD

#define SCREEN_WIDTH 128	
#define SCREEN_HEIGHT 64 
#define OLED_RESET     4 
#define SCREEN_ADDRESS 0x3C 

//structure rf

struct data {
	int axe1;
	int axe2;
	int axe3;
	int thr;

}payload;


//config radio
RF24 radio(7, 8); // 7 pour CE 8 pour CSN
uint64_t address[] = {0xF9F9F9F9F9F9,0xE9E8F0F0E1LL}; // adresse tx / rx


// set radio
bool radioNumber = 0; //0 telecommmande 1 vesc
bool radio_role =0;



//lecture ana
int bouton_read = A0; // affectation du bouton selecteur 


