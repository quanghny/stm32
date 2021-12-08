
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <string.h>
BlynkTimer timer;
WidgetRTC rtc;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "xKTCe-OQm1LSlz0lnMOzwoPmb-Hkh8jX";

// Your WiFi credentials.
// Set password to "" for open networks
char ssid[] = "autorace_2.4G";
char pass[] = "autoraceonly";
byte moc, moc1, moc2;
int Time=0, check = 0, v1=0, CountStart1=0, CountStop1=0, f=0;
int CountStart2=0, CountStop2=0;
String Connect, Rx_data, Tx_data, Tx_data2, pinData1, pinData2, buf1, buf2, bu, a, b, c, d;

WidgetTerminal terminal(V1);
void setup()
{
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);  
  terminal.clear(); 
  Blynk.virtualWrite(V1,"Connecting .....");
  Blynk.virtualWrite(V0,0);
  Connect = "connecting";
  check =1;
  timer.setInterval(1000,sensorvalue1);
  setSyncInterval(10 * 60);
}

   BLYNK_WRITE(V0) //Button Widget is writing Led1
{
   v1 = param.asInt(); 
   if(v1==1)  pinData1 = "led1on";
   if(v1==0)  pinData1 = "led1off";
   check =1;
}
 BLYNK_WRITE(V4) //Button Widget is writing Led2
{
   v1 = param.asInt(); 
   if(v1==1)  pinData2 = "led2on";
   if(v1==0)  pinData2 = "led2off";
   check =2;
}
 BLYNK_WRITE(V2) //Button Widget is writing input timer Led1
{
    int startTimeInSecs;
    int stopTimeInSecs;
    startTimeInSecs = param[0].asLong(); 
    stopTimeInSecs = param[1].asLong(); 
    if(startTimeInSecs >= Time )
    {
      CountStart1 = startTimeInSecs - Time;
    }
    if(startTimeInSecs < Time )
    {
      CountStart1 = 86400 - (Time - startTimeInSecs);
    }
    if(stopTimeInSecs >= Time )
    {
      CountStop1 = stopTimeInSecs - Time;
    }
    if(stopTimeInSecs < Time )
    {
      CountStop1 = 86400 - (Time - stopTimeInSecs);
    }
    check = 1;
}

 BLYNK_WRITE(V5) //Button Widget is writing input timer Led2
{
    int startTimeInSecs;
    int stopTimeInSecs;
    startTimeInSecs = param[0].asLong(); 
    stopTimeInSecs = param[1].asLong(); 
    if(startTimeInSecs >= Time )
    {
      CountStart2 = startTimeInSecs - Time;
    }
    if(startTimeInSecs < Time )
    {
      CountStart2 = 86400 - (Time - startTimeInSecs);
    }
    if(stopTimeInSecs >= Time )
    {
      CountStop2 = stopTimeInSecs - Time;
    }
    if(stopTimeInSecs < Time )
    {
      CountStop2 = 86400 - (Time - stopTimeInSecs);
    }
    check = 2;
}
BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}
void loop()
{
   Blynk.run();
   timer.run();
   
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
   if(check==1)
  {
    Tx_data = Connect+","+pinData1+","+CountStart1+","+CountStop1;
    Serial.println(Tx_data);
    CountStart1 = 0;
    CountStop1 = 0;
    Connect="";
    Tx_data="";
  }
  if(check==2)
  {
    Tx_data = pinData2+":"+CountStart2+":"+CountStop2;
    Serial.println(Tx_data); 
    CountStart2 = 0;
    CountStop2 = 0;    
    Tx_data="";
  }
  check =0;
  if(Serial.available()>0)
 {
   Rx_data = Serial.readStringUntil('\n');
   a= getValue(Rx_data, ',',1);
   b= getValue(Rx_data, ',',2);
   c= getValue(Rx_data, ',',3);
   d= getValue(Rx_data, ',',4);
      if(buf1!=c)
      {
        buf1=c;
      if(buf1=="On") 
        Blynk.virtualWrite(V0,1);   
      if(buf1=="Off") 
        Blynk.virtualWrite(V0,0);
      }  
      if(buf2!=d)
      {
        buf2=d;
      if(buf2=="On2") Blynk.virtualWrite(V4,1);
      if(buf2=="Off2") Blynk.virtualWrite(V4,0);
      }  
        if(a=="Sconnect") Blynk.virtualWrite(V1,"Successful Connection !!!!"); 
      Rx_data="";
 } 
 Time = hour()*3600 + minute()*60 + second();
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length();
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] =  i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void sensorvalue1()
{ 
  int gas =b.toInt();
  Blynk.virtualWrite(V3,gas);
  if(gas >3000) Blynk.notify("Ro Ri Khi Gas  !!!!!");
}
