//@CarmelitoA 23-Nov-2015 Toy Truck Controller (part 4 -WiFi controller Web App for Monster truck)
//using LinkIt One(http://www.seeedstudio.com/depot/LinkIt-ONE-p-2017.html) and Sparkfun Motor driver(https://www.sparkfun.com/products/9457)

#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiServer.h>
#include <LWiFiClient.h>
#define WIFI_AP "XXXXXXXXX" //WiFi  name
#define WIFI_PASSWORD "XXXXXXXXXX" //WiFi password
#define WIFI_AUTH LWIFI_WEP  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP according to your WiFi AP configuration

LWiFiServer server(80);
String HTTP_req;          // stores the HTTP request
String readString;

//Motor driver and LinkIt One connections
int STBY = 10; //standby

//Motor A -Front -- thought this motor is a DC motor, this kind off acts like sero horn movement.
int PWMA = 3; //PWM pin for Speed control 
int AIN1 = 7; //Direction
int AIN2 = 8; //Direction

//Motor B - Back
int PWMB = 9; //PWM pin for Speed control
int BIN1 = 5; //Direction
int BIN2 = 6; //Direction

char movement ='F';

void setup(){
  LWiFi.begin();
  //Serial.begin(115200); //comment out after debuing is complete using the Serial Monitor
  pinMode(STBY, OUTPUT);

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);


  Serial.println("Connecting to AP");
  LWiFi.connectWPA(WIFI_AP, WIFI_PASSWORD);//CJA using WPA connection per LinkIt One user guide
  printWifiStatus();
  Serial.println("Start Server");
  server.begin();
  Serial.println("Server Started");
}

void loop(){
delay(500);
//Wifi Controller
LWiFiClient client = server.available();
if (client)
  {
   while (client.connected()) {  
      if (client.available()) {
        char c = client.read();
        //read  one by one char HTTP request
        if (readString.length() < 100) {
          //storing characters to string
          readString += c;
         }
         //if HTTP request has ended
         if (c == '\n') {          
           //Serial.println(readString);//comment out for deployment of the project
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println("Connection: close"); 
           client.println();  
           client.println("<!DOCTYPE HTML>");   
           client.println("<html>");
           client.println("<head>");
           client.println("<title>LinkIt ONE Monster Truck Controller</title>");
           client.println("<link href='https://rawgit.com/CJAndrade/LinkIt-ONE-WiFi-Robot/master/robostyle.css' rel='stylesheet' type='text/css'>");
           client.println("</head>");
           client.println("<body>");
           client.println("<h1>LinkIt ONE Toy Truck Controller</h1>");
           client.println("<br/><table><tr><td></td>");
           client.println("<td><a href=\"/?moveFoward\"\"><button>Foward</button></a></td>");  
           client.println("<td></td></tr><tr>");
           client.println("<td><a href=\"/?left\"\"><button>Left</button></a></td>");  
           client.println("<td><a href=\"/?back\"\"><button> &nbsp;Back&nbsp;</button></a></td>");
           client.println("<td><a href=\"/?right\"\"><button>Right</button></a></td>");
           client.println("</tr><tr><td></td><td>");  
           client.println("<a href=\"/?stopAll\"\"><button>&nbsp;stop&nbsp;</button></a>");     
           client.println("</td><td></td></table>");
           client.println("<br/>"); 
           client.println("</body>");
           client.println("</html>");
     
           delay(5);
           //stopping client
           client.stop();
           if (readString.indexOf("?left") >0){
            stop();
            delay(100);
            movement ='L';
            turn(255,'L');
            Serial.println(movement);
           }
           if (readString.indexOf("?back") >0){
            movement ='B';
            Serial.println(movement);
            stop();
            delay(100);
            move(240,'B');
           }
           if (readString.indexOf("?right") >0){
            stop();
            delay(100);
            movement ='R';
            turn(255,'R');
            Serial.println(movement);
           }
           if (readString.indexOf("?moveFoward") >0){
            movement ='F';
            Serial.println(movement);
            stop();
            delay(100);
            move(250,'F');
            }
           if (readString.indexOf("?stopAll") >0){
            movement ='S'; 
            Serial.println(movement); 
            stop();
            //delay(250);      
           }
            //clearing string for next read
            readString="";      
         }
       }
    }
  }
  
}


void turn(int speed,char side){ 
  digitalWrite(STBY, HIGH);//enable the standby pin 
  if(side == 'L'){ //left turn
    digitalWrite(BIN1, LOW); 
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, speed);   
    delay(100);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, speed);
  }else{ //Right turn
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, speed);
    delay(50);
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, speed);
  }
  
}

void move(int speed,char moveDirection){
digitalWrite(STBY, HIGH);//enable the standby pin
  if(moveDirection =='B'){
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
      analogWrite(PWMA, speed);
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
      analogWrite(PWMB, speed); 
      
  }else{
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
      analogWrite(PWMA, speed);
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);
      analogWrite(PWMB, speed);    
  } 
}

void stop(){
//enable standby  
  digitalWrite(STBY, LOW); 
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(LWiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = LWiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.print("subnet mask: ");
  Serial.println(LWiFi.subnetMask());

  Serial.print("gateway IP: ");
  Serial.println(LWiFi.gatewayIP());

  // print the received signal strength:
  long rssi = LWiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
