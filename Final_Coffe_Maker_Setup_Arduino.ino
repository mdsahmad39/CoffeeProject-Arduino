/*
  COFFEE PERCOLATOR

  With this code uploaded to arduino board along with Coffee Machine(modified with sensors).
  So, Coffee machine can be monitored and controlled from anywhere around the world.
  The main idea is to make Coffee machine controllable from anywhere. 
  
  

  The circuit:
  
  Input components
  ----------------------------
  Photoresistor 1 is connected to pin A0;
  Photoresistor 2 is connected to pin A1;
  Photoresistor 3 is connected to pin A2;  
  Temperature sensor is connected to pin A3;
  Current sensor is connected to pin A4;
  Buttonpin is connected to pin 4;

  Output components
  -----------------------------
  Relaysignal is connected to pin 8;
  LED 1 is connected to pin 10;
  LED 2 is connected to pin 11;
  LED 3 is connected to pin 12;

  Created 05-02-2020
  By
    Devi Priya Battina
    Sarfaraz Ahmad Mahammad
    Sushma Vendrapu
  Modified 07-02-2020
  By
    Devi Priya Battina
    Sarfaraz Ahmad Mahammad
    Sushma Vendrapu
    
 For functioning please visit:
  http://www.connectivetechs.com

 For Coding please visit:
  

*/

//including required libraries for WiFi module.

#include <ArduinoJson.h>
#include <SoftwareSerial.h>

const int buttonPin =4;    // The push button that sends information if the relay will turn on and off.
const int relaySignal=8;   //The pin sends the activation or deactivation command to the relay. 
int state=0;               //The variable is keeping track of the current status.
int lightLevel=0;          //Initialising Light level with 0(which indicates the state of machine)
const int lightSensor1=A0; // Photoresistor at Arduino analog pin A0
const int lightSensor2=A1; // Photoresistor at Arduino analog pin A1
const int lightSensor3=A2; // Photoresistor at Arduino analog pin A2
const int tempSensor=A3;   // Temperature sensor
const int currentSensor=A4;// Assigning analog pin A5 to variable 'sensor'
float tempOut;        //temporary variable to hold temperature sensor reading
int value1;        // Store value from photoresistor1 (0-1023)
int value2;        // Store value from photoresistor2 (0-1023)
int value3;        //Store value from photoresistor3 (0-1023)
float Var1=0;      //Initialising the variable which is to be sent to Web-Interface
float Var2=0       //Initialising the variable which is to be sent to Web-Interface
float Var3=0;      //Initialising the variable which is to be sent to Web-Interface
float Var4=0;      //Initialising the variable which is to be sent to Web-Interface
float Var5=0;      //Initialising the variable which is to be sent to Web-Interface
float Var11=0;     //Initialising the variable which is to be sent to Web-Interface
float Var12=0;     //Initialising the variable which is to be sent to Web-Interface
float Var13=0;     //Initialising the variable which is to be sent to Web-Interface
float Var14=0;     //Initialising the variable which is to be sent to Web-Interface
float Var15=0;     //Initialising the variable which is to be sent to Web-Interface
float Var6=0;      //Initialising the variable which is to be sent to Web-Interface
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;     // Defining string status initially
int Message="";    //Initializing the message to power ON-Off of coffee machine
int seconds ;      //Store value of seconds
int minutes ;      //Store value of minutes
int hours ;        //Store value of hours
unsigned long presentMillis=0;          //Initialising time for calculating time for each session
unsigned long currentMillis=millis();  //Defining start-time and to store it for calculating different sessions


SoftwareSerial mySerial(2, 3);         // RX, TX(connecting to Wi-Fi module)

void setup() { 
  mySerial.begin(9600);
  pinMode(buttonPin,INPUT);       // Configuring sensor pin as input
  pinMode(tempSensor,INPUT);      // Configuring sensor pin as input
  pinMode(currentSensor, INPUT);  // Configuring sensor pin as input
  pinMode(lightSensor1, INPUT);   // Configuring sensor pin as input
  pinMode(lightSensor2, INPUT);   // Configuring sensor pin as input
  pinMode(lightSensor3, INPUT);   // Configuring sensor pin as input
  pinMode(relaySignal,OUTPUT);    // Configuring sensor pin as ouput
  for(int pinNumber = 10; pinNumber<13; pinNumber++){
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, LOW);
    }                             // Configuring three led's as output
  Serial.begin(9600);             //Initialising baud data for serial monitor
}

//Function for calculating power consumption of Coffee Maker.
//This function will be called for each loop.
float getVoltage() {
  float result;
  float maxValue;
  float currentSens;
  unsigned long int startMillis1 = millis();
  while((millis()-startMillis1<1000)){
    currentSens = analogRead(currentSensor);
    if(maxValue < currentSens){
      maxValue=currentSens;
      }
  }
    result = (230*(currentSens*(5.0/1023))*1.4);
  
    if(result<1){
      result=0;
    }
    return result;
}

//Function for calculating Coffee Level present in pot of Coffee Maker.
//This function will be called in each loop.
float getLevel(){
  float result1;
  value1 = analogRead(lightSensor1);
  value2 = analogRead(lightSensor2);
  if(value1<250){
    result1 = 0;
    }
   else if(251<value1<380){
    result1 = 10;
    }
   else if(381<value1<600){
    result1 = 25;
   }
   else if(601<value1<740){
    result1 = 50;
    }
   else if(741<value1<790){
    result1 = 80;
    }
   else if(value1>791){
    result1 = 100;
    }
  return result1;
}

//Function for determining if pot is present in desired location of coffee maker.
//This function will be called in each loop.
float potPresence(){
  float result2;
  value3 = analogRead(lightSensor3);
  if(value3<920){
    result2 = 0;
    }
   else if(value3>921){
    result2 = 1;
    }
  return result2;
  }

void loop() {

    //if loop runs if button is pressed.
    //changes the state to opposite state(ON to OFF or OFF to ON) after receiving the message.
    if(digitalRead(buttonPin)==HIGH){
      delay(300);
      if(state==0){
        digitalWrite(relaySignal,HIGH);
        Serial.println("Machine is Powered On");
        presentMillis=millis();
        state=1;
       }
      else if(state==1){
        digitalWrite(relaySignal,LOW);
        Serial.println("Machine is Powered Off");
        state=0;
       }
       delay(200);
      }

    //if loop runs if message is received from web-interface.
    //changes the state to opposite state(ON to OFF or OFF to ON) after receiving the message.  
    if(Message){
      if(state==0){
        digitalWrite(relaySignal,HIGH);
        Serial.println("Machine is Powered On");
        presentMillis=millis();
        state=1;
       }
      else if(state==1){
        digitalWrite(relaySignal,LOW);
        Serial.println("Machine is Powered Off");
        state=0;
       }
     }


    //This if condition runs if Coffee Maker is powered ON  
    if(state==1){
        digitalWrite(10, HIGH);   //changing LED to high
        digitalWrite(11, HIGH);   //changing LED to high
        digitalWrite(12, HIGH);   //changing LED to high
        currentMillis=millis()-presentMillis; //Time for each session after powering ON
        seconds = currentMillis/1000;
        minutes = seconds/60;
        hours = minutes/60;
        
        tempOut=analogRead(tempSensor); //Reading the value from Temperature sensor
        
        tempOut=tempOut*0.48828125; //converting to Degree Celsius

        Var1=(minutes);       // Storing Time
        
        Var2=tempOut;         // Storing value in Degree Celsius

        Var3=getLevel();      // Storing Coffee level present in pot
        
        Var4=getVoltage();    // Storing value of power

        Var5=potPresence();   // Storing value corresponding to pot presence

        // Verifying data with Web-Interface(only used once as it uses space and causes problem)
        Serial.print ("Time = ");
        Serial.print (hours);
        Serial.print (":");
        Serial.print (minutes);
        Serial.print (":");
        Serial.println(seconds);
        Serial.print("Temperature in DegreeC = " );
        Serial.println(Var2);
        Serial.print("Power Consumption = " );
        Serial.println(Var4);
        Serial.print("Coffee Level = " );
        Serial.println(Var3);
        
        delay(1000); //Delay of 1 second for ease of viewing

        // Sending values to Web-Interface
        StaticJsonDocument<400> doc;
        
          String Payload = "{""Parameter1"":"+(String)Var1+","+
                              "Parameter2"":"+(String)Var2+","+   
                               "Parameter3"":"+(String)Var3+","+    
                               "Parameter4"":"+(String)Var4+","+    
                               "Parameter5"":"+(String)Var5+","+                   
                              "Parameter6"":"+(String)Var6+"}"; 
        
          deserializeJson(doc, Payload);
          serializeJson(doc,  mySerial);
     
    
        while (mySerial.available()) {
          // get the new byte:
          inputString = mySerial.readString();    
            stringComplete = true;
            Serial.println(inputString);
      }
    }


    //This else if condition runs if Coffee Maker is powered OFF
    else if(state==0){
      digitalWrite(10, LOW);   //changing LED to low
      digitalWrite(11, LOW);   //changing LED to low
      digitalWrite(12, LOW);   //changing LED to low
      tempOut=analogRead(tempSensor); //Reading the value from Temperature sensor
          
      tempOut=tempOut*0.48828125;   //converting to Degree Celsius
      
      Var2=tempOut;                // Storing value in Degree Celsius
  
      Var5=potPresence();          // Storing value corresponding to pot presence
  
      // Verifying data with Web-Interface(only used once as it uses space and causes problem)
      Serial.print("Temperature in DegreeC = " );
      Serial.println(Var2);
      Serial.print("Pot Presence= ");
      Serial.println(Var5);    
      StaticJsonDocument<400> doc;
    
      String Payload = "{""Parameter1"":"+(String)Var11+","+
                          "Parameter2"":"+(String)Var2+","+   
                           "Parameter3"":"+(String)Var13+","+    
                           "Parameter4"":"+(String)Var14+","+    
                           "Parameter5"":"+(String)Var5+","+                   
                          "Parameter6"":"+(String)Var6+"}"; 
    
      deserializeJson(doc, Payload);
      serializeJson(doc,  mySerial);
     
    
      while (mySerial.available()) {
        // get the new byte:
        inputString = mySerial.readString();    
          stringComplete = true;
          Serial.println(inputString);  
      }
     }

StaticJsonDocument<200> mes;

//if loop for getting message(turn ON-OFF of Coffee Machine)
if (stringComplete) {
    deserializeJson(mes,inputString);
    inputString = "";
    stringComplete = false;
    Message=mes["VirButt"]; 
       }  
    delay(1000);  //delay of getting value from web-interface
    
   
}
