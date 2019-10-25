#include <Servo.h>  //arduino library
#include <math.h>   //standard c library

Servo gripServo;  
Servo rightServo;  
Servo leftServo; 

int desiredMouth;
int gripPos;
int rightSpeed = 0;
int leftSpeed = 0;
int command;
int desiredDelay = 16;
int ready = 0;
int  robotMode = 222;

//+++++++++++++++ULTRASONIC VARIABLES++++++++++++++++++++++++++++

#define echoPin A2 // Echo Pin
#define trigPin A3 // Trigger Pin
#define buzzerPin A0 // Pin for the buzzer

bool holder = 1;
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long readDistance; // the output distance from the sensor

//+++++++++++++++FUNCTION DECLARATIONS+++++++++++++++++++++++++++
int ultraSensor(int theEchoPin, int theTrigPin);
void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo);
void speakWalter (int soundPin, int maxWords);

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void setup()
{ 
  Serial.begin(9600);
  gripServo.attach(4);
  rightServo.attach(3); //pin s9 on Meped
  leftServo.attach(2);
  
  Serial.setTimeout(50);      //ensures the the arduino does not read serial for too long
  Serial.println("started");
  
  gripServo.write(50);        //intial positions of servos
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  ready = 0;
} 

//primary arduino loop
void loop() 
{ 
  // read a usb command if available
  if (Serial.available()){
    ready = 1;
    robotMode = Serial.parseInt();
    leftSpeed = Serial.parseInt();
    rightSpeed = Serial.parseInt();
    gripPos = Serial.parseInt();
    //Serial.println(robotMode);
    //Serial.println(leftSpeed);
    //Serial.println(rightSpeed);
    if(Serial.read() == '\n'){              // if the last byte is 'd' then stop reading and execute command 'd' stands for 'done'
        Serial.flush();                     //clear all other commands piled in the buffer
        Serial.print('d');                  //send completion of the command
    }
  }

  //++++++++++++++++++Remote Mode++++++++++++++++++++++
  if (robotMode == 1){
      motorControl(-rightSpeed,-leftSpeed, rightServo, leftServo);
      servoParallelControl ( gripPos, gripServo, 5 );
    }

  // ++++++++++++++++Stopped Mode+++++++++++++++++++++
   if (robotMode == 222){  
      motorControl(2,2, rightServo, leftServo);
    }

// +++++++++++++++++++Autonomous Mode++++++++++++++++++++++++
  
  if (robotMode == 256){
    leftSpeed = -20;
    rightSpeed = -20;
   
   //read the distance read by the sensor
   desiredDelay = 3;
   int singleRead = 0;
   int allReads = 0;
   for (int i = 0; i< 25; i++){
      singleRead = ultraSensor(echoPin, trigPin); //read the distance
      allReads += singleRead;
    }
    
    // final average
    readDistance = allReads/25;
    //Serial.println(readDistance);
    ready = 1;

//++++++++++++++++++++++++++++Interaction with Wall+++++++++++++++++++++++++++++++++++++++
   
       if ( (readDistance < 20 && readDistance > 3) ){
          //speakWalter(buzzerPin, 5);
          // have random reactions from this point (stop and backup, turn, roomba reactions)
          //this is where all the fun starts
          int randomNumber = random(8);
          //Serial.println(randomNumber);
          if (randomNumber == 1){
            //Serial.println("turnright");
            //whatAction(3);
            rightSpeed = -30;    
            leftSpeed  = 30;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay(200); // delay so that the robot has time to turn far away from obstacle
          }
          if (randomNumber == 2){
            //Serial.println("turnleft");
            rightSpeed = 30;    
            leftSpeed  = -30;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay(200); // delay so that the robot has time to turn far away from obstacle
          }
          if (randomNumber == 3){
            //Serial.println("backup");
            rightSpeed = -0;    
            leftSpeed  = -0;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay(200);
            rightSpeed = 20;    
            leftSpeed  = 30;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay(800); // delay so that the robot has time to turn far away from obstacle
          }
          if (randomNumber == 4){
            //Serial.println("long right");
            rightSpeed = -30;    
            leftSpeed  = 30;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay(400); // delay so that the robot has time to turn far away from obstacle
          }      
          if (randomNumber == 5){
            //Serial.println("long left");
            rightSpeed = 30;    
            leftSpeed  = -30;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay(500); // delay so that the robot has time to turn far away from obstacle
          }      
          if (randomNumber == 6){
            //Serial.println("long left");
            rightSpeed = 30;    
            leftSpeed  = -30;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay(3000); // delay so that the robot has time to turn far away from obstacle
          }    
           if (randomNumber == 7 ){
            //Serial.println("backup angled");
            rightSpeed = -0;    
            leftSpeed  = -0;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay(200);
            rightSpeed = 30;    
            leftSpeed  = 20;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay(2000); // delay so that the robot has time to turn far away from obstacle
          }  
    
       }

//+++++++++++++++++++++++++++++++++Moving Along wihtout Detecting Obstacle++++++++++++++++++   

    // While just walking along occassionaly have a reaction   
       if ( (readDistance > 20) ){
          
          int randomNumber = random(500);
          //move the elbow and shoulder some amount
          
          if (randomNumber > 50){
            rightSpeed = -30;    //move ahead at half speed
            leftSpeed  = -30;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay(100);
          }
          if (randomNumber <= 10){
            rightSpeed = -30;    //move ahead at half speed
            leftSpeed  = 30;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay (300);
          }
          if (randomNumber > 10 && randomNumber <= 20){
            rightSpeed = 30;    //move ahead at half speed
            leftSpeed  = -30;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay (300);
          }
          if (randomNumber > 20 && randomNumber <= 30){
            rightSpeed = 30;    //move ahead at half speed
            leftSpeed  = -30;
            motorControl(rightSpeed,leftSpeed, rightServo, leftServo);
            delay (2000);
          }
    
          //insert a count to trigger random behavior
          //boredCount ++;
    
           //Serial.println("Hello2");
       }
    
     
      // if ( (readDistance < 5) ){
          // this command stopps the robot for 5 second if it get too close to something
        //  rightSpeed = 0;    //move ahead at half speed. Speed at 90 in range of 0-180 is no speed
         // leftSpeed  = 0;
    
        //  motorControl(0,0, rightServo, leftServo);
        //  delay (5000);
           //Serial.println("Stopped");
       //}
    
       
    } // end of robotMode if

} // end of primary loop

//++++++++++++++++++++++++++++++FUNCTION DEFINITIONS++++++++++++++++++++++++++++++++++++++++++

void motorControl(int rightSpeed, int leftSpeed, Servo theRightServo, Servo theLeftServo){

 
  //Set all of the servo positions
  theRightServo.write(90 - rightSpeed);  //compensate for the 0-180, -90-90 relationship
  theLeftServo.write(90 + leftSpeed);
  //mouthServo.write(desiredMouth);

}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int ultraSensor(int theEchoPin, int theTrigPin){
   //this fucntion caluclates and returns the distance in cm
    
   long duration, distance; // Duration used to calculate distance
   /* The following trigPin/echoPin cycle is used to determine the
   distance of the nearest object by bouncing soundwaves off of it. */ 
   digitalWrite(theTrigPin, LOW); 
   delayMicroseconds(2); 
  
   digitalWrite(theTrigPin, HIGH);
   delayMicroseconds(10); 
   
   digitalWrite(theTrigPin, LOW);
   duration = pulseIn(theEchoPin, HIGH);
   
   //Calculate the distance (in cm) based on the speed of sound.
   distance = duration/58.2;  
   return distance;
  
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

void speakWalter (int soundPin, int maxWords){
  int toneDuration;
  int numberOfWords;
  int toneFreq;
  
  numberOfWords = random (1,maxWords);
  //Serial.print("Number of words = ");
  //Serial.println(numberOfWords);
  
  for ( int i; i<= numberOfWords; i++){
    toneDuration = random (25, 150);
    toneFreq = random (100, 1800);
    tone(soundPin, toneFreq, toneDuration);  
    
    delay(toneDuration);

  }
  
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int servoParallelControl (int thePos, Servo theServo, int theSpeed ){
  
    int startPos = theServo.read();        //read the current pos
    int newPos = startPos;
    //int theSpeed = speed;
    
    //define where the pos is with respect to the command
    // if the current position is less that the actual move up
    if (startPos < (thePos-5)){     
       newPos = newPos + 1;
       theServo.write(newPos);
       delay(theSpeed);
       return 0;      
    }
  
   else if (newPos > (thePos + 5)){
      newPos = newPos - 1;
      theServo.write(newPos);
      delay(theSpeed);
      return 0;
    }  
    
    else {
        return 1;
    }  
    
}

