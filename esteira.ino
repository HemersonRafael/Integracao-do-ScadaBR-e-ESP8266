#include <ESP8266WiFi.h>
#include <Modbus.h>
#include <ModbusIP_ESP8266.h>
#include <Servo.h>

//Modbus Registers Offsets (0-9999)
const int SENSOR_IREG = 0;

//Modbus Registers Offsets (0-9999)
const int SERVO_COIL = 100;
const int ESTEIRA_COIL = 101;

//Used Pins
const int EsteiraPin = 12;
const int ServoPin = 14;
const int AtivaPin = 13;


//ModbusIP object
ModbusIP mb;
long ts;

//Servo object
Servo myservo;

void setup() {
    Serial.begin(115200);
 
    //Config Modbus IP
    mb.config("tbrc", "tbrc12345");
  
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    //Define mode the pin
    pinMode(EsteiraPin, OUTPUT);
    
    // Add SENSOR_IREG register - Use addIreg() for analog Inputs
    mb.addIreg(SENSOR_IREG);
    mb.addCoil(ESTEIRA_COIL);
    mb.addCoil(SERVO_COIL);
    ts = millis();
    
    //Define pin servo
    myservo.attach(ServoPin);
}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();

   //Read each two seconds
   if (millis() > ts + 2000) {
       ts = millis();
       //Setting raw value (0-1024)
       mb.Ireg(SENSOR_IREG, analogRead(A0));
       //Attach EsteiraPin to ESTEIRA_COIL register
       digitalWrite(EsteiraPin, mb.Coil(ESTEIRA_COIL));
       digitalWrite(AtivaPin, mb.Coil(SERVO_COIL));
      }
   
   if(mb.Coil(SERVO_COIL)){
    myservo.write(90);
   }
   else{
    myservo.write(180);
   }
   
}
