#include <IRremote.h>    //Infrared sensor function library

const int RECV_PIN = 8;
const int ECHO_PIN = 7;
const int LED_PIN = 3;
const int EN_PIN = 4;
const int PWM_PIN_1 = 5;
const int PWM_PIN_2 = 6;
int status = 2;             //The status of distance
int dir = 0;			//Rotation direction
double distance = 0;
IRrecv irrecv(RECV_PIN);  //Create receiver object pin
decode_results results;	  //Create decode address variable

void setup()
{
  pinMode(ECHO_PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  pinMode(EN_PIN,OUTPUT);
  Serial.begin(9600);      //Start the serial port at the connection speed of 9600
  irrecv.enableIRIn();     //Start the infrared receiver
  irrecv.blink13(true);   //Start the LED (on-board) flashing feedback of the receiver
}

void loop()
{
  if(irrecv.decode(&results))  //Receive signal and decode
  {
    switch(results.value)
    {
      case 16582903:   //Remote control button 1
      digitalWrite(EN_PIN,HIGH);
      analogWrite(PWM_PIN_1,0);
      analogWrite(PWM_PIN_2,255);
      dir = 2;
      break;
      
      case 16615543:   //Remote control button 2
      digitalWrite(EN_PIN,HIGH);
      analogWrite(PWM_PIN_1,0);
      analogWrite(PWM_PIN_2,144);
      dir = 1;
      break;
      
      case 16599223:   //Remote control button 3
      digitalWrite(EN_PIN,HIGH);
      analogWrite(PWM_PIN_1,255);
      analogWrite(PWM_PIN_2,0);
      dir = 0;
      break;
      
      case 16591063:   //Remote control button 4
      digitalWrite(EN_PIN,HIGH);
      analogWrite(PWM_PIN_1,144);
      analogWrite(PWM_PIN_2,0);
      dir = 0;
      break;
      
      case 16623703:   //Remote control button 5
      digitalWrite(EN_PIN,LOW);
      dir = 0;
      break ;
      
      default:
      break;
    }
    irrecv.resume();  //Resume infrared receiver
  }
  dis(&status);     //Measured distance
  if(status<2)		//If the distance is less than 100cm
  {
    digitalWrite(LED_PIN,HIGH);  //Warning LED on
    if(dir)		//If the direction is positive
    {
      if(status == 1)	  //If the distance is 50 ~ 100cm
      {
        if(dir == 2)	  //If the speed is high
        {
          for(int i = 255;i > 144;i--)
          {
            analogWrite(PWM_PIN_2, i); //Slow down
            delay(2);
          }
          dir = 1;		//Change speed status to low speed
        }
      }    
      else
      {
        digitalWrite(EN_PIN, LOW);  //Brake
      }
  	}
  }
  else
  {
    digitalWrite(LED_PIN,LOW);  //Warning LED off
  }
}

void dis(int* status)  //Function of measuring distance
{
  digitalWrite(ECHO_PIN,HIGH);
  delayMicroseconds(2);
  digitalWrite(ECHO_PIN,LOW);
  pinMode(ECHO_PIN,INPUT);
  distance = pulseIn(ECHO_PIN,HIGH)*0.022265;  //Calculate distance
  pinMode(ECHO_PIN,OUTPUT);
  if(distance > 100)		//Write distance status
  {
    *status = 2;
  }
  else
  {
    if(distance > 50)
    {
      *status = 1;
    }
    else
    {
      *status = 0; 
    }
  }
}
