//MICROSWITCH

const int MICRO_DEFAULT = HIGH;
const int MICRO_FIRE = LOW;
const int MICRO_PIN = 5;
volatile int micro_state = 0;
volatile boolean micro_ready = false;

//RELAY

const int RELAY_DEFAULT = LOW;
const int RELAY_FIRE = HIGH;
const int RELAY_PIN = 6;
const int RELAY_LENGTH = 200;

unsigned long relay_off = 0;
unsigned long relay_on = 0;

//DELAY
//Delay the firing of the relay until the desired frame 
//is at the correct position for the color change
volatile int DELAY = 400;

//TIMERS
unsigned long timer = 0;
volatile int change = 0;

const int MAX_CHANGES = 5;
const int changes[5] = {0, 0, 0, 0, 0};


void setup() {
  // put your setup code here, to run once:
  pinMode(MICRO_PIN, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  
  digitalWrite(RELAY_PIN, RELAY_DEFAULT);

  Serial.begin(57600);
  Serial.println("Started model c notch detector");
}

void loop() {
  timer = millis();
  // put your main code here, to run repeatedly:
  checkMicro();
  checkRelay();
}

/**
 * Gets the state of the microswitch and returns
 * fires relay if true
 */
void checkMicro () {
  micro_state = digitalRead(MICRO_PIN);
  if (micro_ready && relay_on == 0 && micro_state == MICRO_FIRE) {
   fireRelay();
  }
  if (!micro_ready && micro_state == MICRO_DEFAULT) {
    micro_ready = true;
  }
}

void fireRelayOld () {
   Serial.println("Firing relay");
  delay(DELAY);
  digitalWrite(RELAY_PIN, RELAY_FIRE);
  delay(RELAY_LENGTH);
  digitalWrite(RELAY_PIN, RELAY_DEFAULT); 
}
/**
 * Delays for DELAY ms and then sets the relay
 * to it's RELAY_FIRE state
 */
void fireRelay () {
    Serial.println("Firing relay");
    Serial.println(DELAY);
    relay_on = timer + DELAY;
    micro_ready = false;
    //change++;
    //if (change % 3 == 0) {
      //DELAY += 50;  
    //}
}

void checkRelay () {
  //on
  if (relay_on != 0 && timer >= relay_on) {
    digitalWrite(RELAY_PIN, RELAY_FIRE);
    relay_on = 0;
    relay_off = timer + RELAY_LENGTH;
  }
  //off
  if (relay_off != 0 && timer >= relay_off) {
    digitalWrite(RELAY_PIN, RELAY_DEFAULT); 
    relay_off = 0;
  }
}

