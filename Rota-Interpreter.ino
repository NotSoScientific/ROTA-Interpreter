#include <Servo.h>
//define the pins, servo, variables
int ENA = 11;
int SRV = 10;
Servo steer;
int M1 = 12;
int M2 = 13;

int count = 0; // the current byte being read.
float dir, spd; // the direction and speed.

void setup() {
  Serial.begin(9600); //serial baud rate
  pinMode(ENA, OUTPUT); //speed control for our motor
  steer.attach(SRV);

  dir = 98; // init dir to center.
  steer.write(dir);

  spd = 0; //set motorspeed to 0

  pinMode(M1, OUTPUT); // motor controller control channels.
  pinMode(M2, OUTPUT);
}

/* The looping mainbody of our program. The loop has been made in such a way, that
 *  for each loop, the program gets a byte from the serial connection, this is
 *  identified with count.
 *  The 3rd byte is the left stick y, which is the throttle.
 *  The 4th byte is the right stick x, which is the steering.
 *  Our system receives more input, but this was unnecessary for our final program.
 *  We chose to leave it in as it provides more customisability.
 */
void loop() {

  int dat = Serial.read(); // read byte.

  if(dat != -1){ // check there is a byte.

    if(count == 3){ // steer.
      dir = 15*(dat-127)/127;
      steer.write(97+dir);
    }

    if(count == 2){ // throttle.
      spd = 255*(dat-127)/127;

      if(spd < 0.0){ // set direction.
        spd *= -1;
        digitalWrite(M1, HIGH);
        digitalWrite(M2, LOW);
      }else{
        digitalWrite(M2, HIGH);
        digitalWrite(M1, LOW);
      }

      analogWrite(ENA, spd); // update motorspeed.
    }
    count = (count+1)%5; // count loops back at 5, we are now reading the first byte again.
  }
}
