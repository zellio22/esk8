//radio  esk8
// lib radio
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

//lib display
#include <Adafruit_SSD1306.h>

//includ prog
#include "display.h"
#include "fonc.h"
#include "config.h"



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);// def du display

//timer
long time_rf=0;
long act_display,time_clignote_true,time_clignote_false,time_dist,last_micros;

//teste
int potar_speed = A6;
int potar_bat = A7;
int potar_thr = A8;

//variable interne
String state_bouton = "";
String last_state_bouton = "";
float speed=0; // stocage de la vitesse
float dist=0; // distance parcourue 
bool creneau = 1;


int bat_level;


void setup() {
	
	printf_begin(); 												// pour le debug
	Serial.begin(115200);
	
	if (!radio.begin()) {											//check etat radio
		Serial.println(F("Radio ne repond pas!!"));
		while (1) {} // boucle infini
	}
	else {Serial.println(F("RF24 Start"));}
	
	if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {		//check etat display
		Serial.println(F("SSD1306 allocation failed"));
		while (1) {} // boucle infini
	}
	else {Serial.println(F("Display start"));}
	
	// radio setup
	radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX 
	radio.setPayloadSize(sizeof(data)); // def de la taille à transmetre
	
	radio.openWritingPipe(address[radioNumber]);
	radio.openReadingPipe(1, address[!radioNumber]);
	


	//clear du display
	display.display();
	display.clearDisplay();
	
} //fin setup

void loop() {
	
	state_bouton = bouton_ana();
	
	if (state_bouton!=""){
		Serial.print("etat bouton : ");
		Serial.println(state_bouton);
		count_menu();
		Serial.print("num_ecran : ");
		Serial.println(menu);
		switch_ecran();			//actualise directement l'ecran pas de lag !!!
		
	}

	if(millis()-act_display > 50){	//actualisation display toute les 50ms
		act_display = millis();
		bat_level=(float)analogRead(potar_bat)/10.23;
		speed=(float)analogRead(potar_speed)/10.23*diametre_roue*pi*0.0036/gear_ratio; // 10.23 convertion max tachy 100tr/s
		payload.thr=analogRead(potar_thr);
		switch_ecran();
		
	}

	if(millis()-time_dist > 1000){
		time_dist = millis();
		dist_cal(); //calcul de la distance parcourue
		Serial.print("Km   : "); Serial.println(dist,4);
		Serial.print("Km/s : "); Serial.println(speed,4);
		Serial.print("m/s  : "); Serial.println((float)speed/3.6,4);
		Serial.print("thr  : "); Serial.println(payload.thr);
		Serial.print("lag  : "); Serial.println(millis()-last_micros-1000);
		last_micros=millis();
		f_radio(1);//envoi de la payload

		Serial.println(" ");
	}


}// fin loop


void f_radio(bool role){ 	//fonction radio

	if (role){
		// tx mode 
		radio.stopListening(); // stop lecture
		unsigned long start_timer = micros();                   // time debut
		bool report = radio.write(&payload, sizeof(data));     // transmet et save dans repport
		unsigned long end_timer = micros();                     // time fin
		
		if (report) {
			Serial.print(F("Transmission ok! "));          		// payload was delivered
			Serial.print(F(" Temps de  = "));
			Serial.print(end_timer - start_timer);              // print the timer result
			Serial.println(F(" uS"));


		} 
		
		else {
			Serial.println(F("Transmission failed or timed out")); // payload was not delivered

			//radio.printPrettyDetails();
			menu=99;
		}
		
	
		
	}
	
	else{
		// rx mode
		radio.startListening();// reprendre lecture
		uint8_t pipe;
		if (radio.available(&pipe)) {             	// is there a payload? get the pipe number that recieved it
			uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
			radio.read(&payload, sizeof(data));            // fetch payload from FIFO
			Serial.print(F("Received "));
			Serial.print(bytes);                    // print the size of the payload
			Serial.print(F(" bytes on pipe "));
			Serial.print(pipe);                     // print the pipe number
			Serial.print(F(": "));


		}		
	}
}

































