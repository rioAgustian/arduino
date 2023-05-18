// Jadi, ada piezoelektrik dan LED
// Kalau keluaran analog dari piezo >= nilai ambang
// Maka LED nyala

int ledoutput = 13; // pin connected to LED
int THRESHOLD = 100;

void setup() {
pinMode(ledoutput, OUTPUT);   // this function is used to declare led connected pin as output
Serial.begin(9600); //Open the serial to set the baud rate for 9600bps
}

void loop() {
int val;
val=analogRead(0);  // function to read analog voltage from sensor
Serial.print("Vibration is ");
Serial.println(val, DEC); //Print the analog value read via serial port
delay(50);

if (val >= THRESHOLD)                    // function to check voltage level from sensor
{
digitalWrite(ledoutput, HIGH);
delay(50); // to make the LED visible
Serial.print("LED ON");
}
else
digitalWrite(ledoutput, LOW);
}