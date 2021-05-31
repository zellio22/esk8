
#include <SPI.h>
#include "printf.h"
#include "RF24.h"

RF24 radio(7, 8); // 7 pour CE 8 pour CSN
uint64_t address[] = {0xF9F9F9F9F9F9,0xE9E8F0F0E1LL}; // adresse tx / rx

bool radioNumber = 1; //0 telecommmande 1 vesc
bool role = false; // true tx, false rx


struct data {
	int axe1;
	int axe2;
	int axe3;
	int thr;

}payload;

int iteration=0;
uint8_t pipe;

void setup() {
	
	printf_begin(); // pour le debug
	Serial.begin(115200);
	
	if (!radio.begin()) {
		Serial.println(F("Radio ne repond pas!!"));
		while (1) {} // boucle infini
	}
	else {Serial.println(F("RF24 Start"));}
	
	radio.setPALevel(RF24_PA_LOW); // RF24_PA_MAX 
	radio.setPayloadSize(sizeof(data)); // def de la taille a transmetre
	
	radio.openWritingPipe(address[radioNumber]);
	radio.openReadingPipe(1, address[!radioNumber]);

	radio.startListening();// passage en lecture

} //fin setup

void loop() {

	if (radio.available(&pipe)){
		uint8_t bytes = radio.getPayloadSize();
		radio.read(&payload, sizeof(data));
		iteration++;
		Serial.print("Iteration : ");Serial.println(iteration);
		Serial.print(F("Received "));
		Serial.print(bytes);                    // print the size of the payload
		Serial.print(F(" bytes on pipe "));
		Serial.print(pipe);                     // print the pipe number
		Serial.println(F(": "));
		Serial.print("varable Thr a : ");Serial.println(payload.thr);
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



