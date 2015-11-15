//@CarmelitoA 11-Nov-2015 basic Obstacle Avoiding Robot (part 2 -Making Obstacle Avoiding by connecting Ultrasonic sensor)
//using LinkIt One(http://www.seeedstudio.com/depot/LinkIt-ONE-p-2017.html) and Sparkfun Motor driver(https://www.sparkfun.com/products/9457)

//Motor driver and LinkIt One connections
int STBY = 10; //standby

//Motor A - left
int PWMA = 3; //PWM pin for Speed control 
int AIN1 = 7; //Direction
int AIN2 = 8; //Direction

//Motor B - Right
int PWMB = 9; //PWM pin for Speed control
int BIN1 = 5; //Direction
int BIN2 = 6; //Direction

//Ultrasonic sensor connections HC-SR04
int trigPin = 2;
int echoPin = 4;
long duration, cm, inches;

void setup(){
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //Serial.begin(9600); //enable for debugging in the Serial Monitor
}

void loop(){
  //moving foward move(speed,direction) - F - foward 
  move(255,'F');
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(4);
  digitalWrite(trigPin, LOW);

  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  // converts the time to a distance
  cm = (duration / 2) / 29.1;
  //inches = (duration / 2) / 74;
  delay(250);
  //Serial.println("Distance in cms");
 // Serial.print(cm);
  //Sets object detection distance to 4.5
  if (cm < 10) {
   // Serial.println("obstacle detected");
    stop();
    delay(250);
    move(255,'B'); //moving back for 2 seconds
    delay(2000);
    turn(220,'L'); //move left , you can also choose to move it right by using R instead of L
    delay(3000);
  }
}



void turn(int speed,char side){ //CJATODO also try and add foward and back direction for the turn
  digitalWrite(STBY, HIGH);//enable the standby pin 
  if(side == 'L'){ //left turn
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, speed);
    digitalWrite(BIN1, LOW); //Stop motor B
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, speed);   
  }else{ //Right turn
    digitalWrite(AIN1, LOW);//Stop motor A
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, speed);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, speed);
  }
  
}

void move(int speed,char moveDirection){
digitalWrite(STBY, HIGH);//enable the standby pin
  if(moveDirection =='B'){
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      analogWrite(PWMA, speed);
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);
      analogWrite(PWMB, speed); 
      
  }else{
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      analogWrite(PWMA, speed);
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
      analogWrite(PWMB, speed);    
  } 
}

void stop(){
//enable standby  
  digitalWrite(STBY, LOW); 
}
