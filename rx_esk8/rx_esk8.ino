
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

RF24 radio(7, 8); // 7 pour CE 8 pour CSN
uint64_t address[] = {0xF9F9F9F9F9F9,0xE9E8F0F0E1LL}; // adresse tx / rx

bool radioNumber = 1; //0 telecommmande 1 vesc




struct data_in { // de la radio vers le skate
	int thr; // throttle
	bool mode = 0; // true demmande de data 

}payload_in;

struct data_out { // du skate vers radio 
	float tension_bat; 										// v_in
	float courant = (float)100*payload_in.thr/1023;			//current_in
	float amp_hours = 3.54;
	float watt_hours = 18.25;
	int thachy = payload_in.thr;			//tachometer_abs = payload_in.thr pour teste
}payload_out;

int iteration=0;
uint8_t pipe;
long time_rf;

void setup() {
	
	printf_begin(); // pour le debug
	Serial.begin(115200);
	
	if (!radio.begin()) {
		Serial.println(F("Radio ne repond pas!!"));
		while (1) {} // boucle infini
	}
	else {Serial.println(F("RF24 Start"));}
	
	radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX 	
	radio.setPayloadSize(sizeof(data_in)); // def de la taille a recevoir	
	radio.openWritingPipe(address[radioNumber]);
	radio.openReadingPipe(1, address[!radioNumber]);
	radio.startListening();// passage en lecture

} //fin setup

void loop() {

	if(millis()-time_rf > 1000){
		time_rf=millis();
		payload_out.tension_bat=50.2;
		payload_out.courant = (float)100*payload_in.thr/1023;
		payload_out.amp_hours = 3.54;
		payload_out.watt_hours = 18.25;
		payload_out.thachy = 1023;
		//Serial.println(payload_in.mode);
		f_radio(1);		// appel de la fonction radio 
	}
	
}// fin loop	


void f_radio(bool role){ 		//fonction radio

	if (role){
		// tx mode 
		//delay(50);
		radio.stopListening(); // stop lecture
		radio.setPayloadSize(sizeof(data_out)); // def de la taille a recevoir
		unsigned long start_timer = micros();                   // time debut
		bool report = radio.write(&payload_out, sizeof(data_out));      // transmet et save dans repport
		unsigned long end_timer = micros();                     // time fin
		
		if (report) {
			Serial.print(F("Transmission ok! "));          		// payload was delivered
			Serial.print(F(" Temps de  = "));
			Serial.print(end_timer - start_timer);              // print the timer result
			Serial.println(F(" uS"));
			//payload_in.mode = 0; // repassage en mode listening 
			//role=payload_in.mode; //fg
		} 
		
		else {
			Serial.println(F("Transmission failed or timed out")); // payload was not delivered

			//radio.printPrettyDetails();
			//delay(800);
			//payload_in.mode = 0;
		}
			
	}
	
	else{
		// rx mode
		
		radio.startListening();// reprendre lecture
		uint8_t pipe;
		if (radio.available(&pipe)) {             	// is there a payload? get the pipe number that recieved it
			radio.setPayloadSize(sizeof(data_in)); // def de la taille a recevoir
			uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
			radio.read(&payload_in, sizeof(data_in));            // fetch payload from FIFO
			Serial.print(F("Received "));
			Serial.print(bytes);                    // print the size of the payload
			Serial.print(F(" bytes on pipe "));
			Serial.print(pipe);                     // print the pipe number
			Serial.println(F(": "));
			Serial.print("varable Thr à : ");Serial.println(payload_in.thr);
			Serial.print("Mode radio    : ");Serial.println(payload_in.mode);

		}		
	}
	
	
	
}



