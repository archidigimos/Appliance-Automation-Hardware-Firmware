#define light_pin 4
#define switchFeedback 3

#define AC_LOAD 10   // Output to Opto Triac pin
int dimming = 0; 
#define REGULATOR_PIN_UP A0 
#define REGULATOR_PIN_DN A1

#define regindi1 12
#define regindi2 11
#define regindi3 9
#define regindi4 8
#define regindi5 7

#define writeControl0 5
#define writeControl1 6
#define espCheck 13
#define hardwareSerial Serial

int aval = 0;

int TL=0;
int TL_bt = 1;
int prev_state = 0;
int state= 0;

int regulator_value_up=1020;
int regulator_value_dn=1020;
int regulator_press=0;

String tubelight = "TL_OFF-";
String fan = "FAN_OFF";

int connectionState = 0;

int sample1 = 0;
int sample2 = 0;

int dimtime = 0;
float propTime = 5;

void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  dimtime = (77 * dimming);
  //if(dimming==50)digitalWrite(AC_LOAD, HIGH);
  if(dimming==0)digitalWrite(AC_LOAD, LOW);
  else{
  digitalWrite(AC_LOAD, LOW);
  delayMicroseconds(dimtime);    // Off cycle
  digitalWrite(AC_LOAD, HIGH);   // triac firing
  delayMicroseconds(propTime);         // triac On propogation delay
  digitalWrite(AC_LOAD, LOW);    // triac Off
  }
}

void switchingTL(void)
{
  digitalWrite(light_pin, LOW);
  delay(50);
  digitalWrite(light_pin, HIGH);
}

void regulatorIndicator(int regulator_state)
{
  if(regulator_state==0){digitalWrite(regindi1, LOW);digitalWrite(regindi2, LOW);digitalWrite(regindi3, LOW);digitalWrite(regindi4, LOW);digitalWrite(regindi5, LOW);}
  else if(regulator_state==1){digitalWrite(regindi1, HIGH);digitalWrite(regindi2, LOW);digitalWrite(regindi3, LOW);digitalWrite(regindi4, LOW);digitalWrite(regindi5, LOW);}
  else if(regulator_state==2){digitalWrite(regindi1, HIGH);digitalWrite(regindi2, HIGH);digitalWrite(regindi3, LOW);digitalWrite(regindi4, LOW);digitalWrite(regindi5, LOW);}
  else if(regulator_state==3){digitalWrite(regindi1, HIGH);digitalWrite(regindi2, HIGH);digitalWrite(regindi3, HIGH);digitalWrite(regindi4, LOW);digitalWrite(regindi5, LOW);}
  else if(regulator_state==4){digitalWrite(regindi1, HIGH);digitalWrite(regindi2, HIGH);digitalWrite(regindi3, HIGH);digitalWrite(regindi4, HIGH);digitalWrite(regindi5, LOW);}
  else if(regulator_state==5){digitalWrite(regindi1, HIGH);digitalWrite(regindi2, HIGH);digitalWrite(regindi3, HIGH);digitalWrite(regindi4, HIGH);digitalWrite(regindi5, HIGH);}
}

void statusDataCheckESP(String data)
{
        digitalWrite(writeControl0, HIGH);
        digitalWrite(writeControl1, HIGH);
        if(data == "TL_OFF-FAN_OFF"){hardwareSerial.println('n');hardwareSerial.flush();}
        else if(data == "TL_OFF-FAN_ON_1"){hardwareSerial.println('o');hardwareSerial.flush();}
        else if(data == "TL_OFF-FAN_ON_2"){hardwareSerial.println('p');hardwareSerial.flush();}
        else if(data == "TL_OFF-FAN_ON_3"){hardwareSerial.println('q');hardwareSerial.flush();}
        else if(data == "TL_OFF-FAN_ON_4"){hardwareSerial.println('r');hardwareSerial.flush();}
        else if(data == "TL_OFF-FAN_ON_5"){hardwareSerial.println('s');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_OFF"){hardwareSerial.println('t');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_ON_1"){hardwareSerial.println('u');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_ON_2"){hardwareSerial.println('v');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_ON_3"){hardwareSerial.println('w');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_ON_4"){hardwareSerial.println('x');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_ON_5"){hardwareSerial.println('y');hardwareSerial.flush();}
        digitalWrite(writeControl0, HIGH);
        digitalWrite(writeControl1, LOW);
}

void statusDataCheckBLE(String data)
{
        digitalWrite(writeControl0, HIGH);
        digitalWrite(writeControl1, HIGH);
        if(data == "TL_OFF-FAN_OFF"){hardwareSerial.println('N');hardwareSerial.flush();}
        else if(data == "TL_OFF-FAN_ON_1"){hardwareSerial.println('O');hardwareSerial.flush();}
        else if(data == "TL_OFF-FAN_ON_2"){hardwareSerial.println('P');hardwareSerial.flush();}
        else if(data == "TL_OFF-FAN_ON_3"){hardwareSerial.println('Q');hardwareSerial.flush();}
        else if(data == "TL_OFF-FAN_ON_4"){hardwareSerial.println('R');hardwareSerial.flush();}
        else if(data == "TL_OFF-FAN_ON_5"){hardwareSerial.println('S');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_OFF"){hardwareSerial.println('T');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_ON_1"){hardwareSerial.println('U');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_ON_2"){hardwareSerial.println('V');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_ON_3"){hardwareSerial.println('W');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_ON_4"){hardwareSerial.println('X');hardwareSerial.flush();}
        else if(data == "TL_ON-FAN_ON_5"){hardwareSerial.println('Y');hardwareSerial.flush();}
        digitalWrite(writeControl0, HIGH);
        digitalWrite(writeControl1, LOW);
}
void sendStatus(int connectionStatus) {
  if (TL == 0) tubelight = "TL_OFF-";
  else if(TL == 1) tubelight = "TL_ON-";

  if (dimming == 0) fan= "FAN_OFF";
  else if (dimming == 80) fan= "FAN_ON_1";
  else if (dimming == 70) fan= "FAN_ON_2";
  else if (dimming == 60) fan= "FAN_ON_3";
  else if (dimming == 50) fan= "FAN_ON_4";
  else if (dimming == 30) fan= "FAN_ON_5";

  if (connectionStatus == 0) statusDataCheckESP(tubelight+fan);
  else if (connectionStatus == 1)statusDataCheckBLE(tubelight+fan);  //else if(connectionState ==1) statusDataCheckBLE(tubelight+fan);
}

void fanRegulatorManual(void)
{
  regulator_value_up = analogRead(REGULATOR_PIN_UP);
  if((regulator_value_up<=800)&&(regulator_value_up>=150)&&(regulator_press<5))
  {
    regulator_press++;
    delay(200);
    if(regulator_press==0){dimming = 0;regulatorIndicator(0);}
    else if(regulator_press==1){dimming = 80;regulatorIndicator(1);}
    else if(regulator_press==2){dimming = 70;regulatorIndicator(2);}
    else if(regulator_press==3){dimming = 60;regulatorIndicator(3);}
    else if(regulator_press==4){dimming = 50;regulatorIndicator(4);}
    else if(regulator_press==5){dimming = 30;regulatorIndicator(5);}
    sendStatus(0);
    sendStatus(1);
  }
  regulator_value_dn = analogRead(REGULATOR_PIN_DN);
  if ((regulator_value_dn<=100)&&(regulator_press>0))
  {
   regulator_press--;
  delay(200);
    if(regulator_press==0){dimming = 0;regulatorIndicator(0);}
    else if(regulator_press==1){dimming = 80;regulatorIndicator(1);}
    else if(regulator_press==2){dimming = 70;regulatorIndicator(2);}
    else if(regulator_press==3){dimming = 60;regulatorIndicator(3);}
    else if(regulator_press==4){dimming = 50;regulatorIndicator(4);}
    else if(regulator_press==5){dimming = 30;regulatorIndicator(5);}
    sendStatus(0);
    sendStatus(1);
  }
}

void sendSwitchStatus_update()
{
  state = digitalRead(switchFeedback);
  if(state == 1 && prev_state == 0)
  {
    prev_state = 1; 
    TL=1;
    sendStatus(0);
    sendStatus(1);
  }
  else if(state == 0 && prev_state == 1)
  {
    prev_state = 0;
    TL=0;
    sendStatus(0);
    sendStatus(1);
  }
  else{}
}

void esp_receive(void)
{
  int val = 0;
  val = digitalRead(espCheck);
  if(val == 1) {
    digitalWrite(writeControl0, LOW);
    digitalWrite(writeControl1, HIGH);
    while(hardwareSerial.available() <= 0) {delay(1);}
  
  char data;
  if (hardwareSerial.available() > 0) {
    data = (char)hardwareSerial.read();
    if(data == 'a'){if(TL==1){switchingTL();TL = 0;}dimming = 0;regulator_press=0;regulatorIndicator(0);sendStatus(0);}
    else if(data == 'b'){if(TL==1){switchingTL();TL = 0;}dimming = 80;regulator_press=1;regulatorIndicator(1);sendStatus(0);}
    else if(data == 'c'){if(TL==1){switchingTL();TL = 0;}dimming = 70;regulator_press=2;regulatorIndicator(2);sendStatus(0);}
    else if(data == 'd'){if(TL==1){switchingTL();TL = 0;}dimming = 60;regulator_press=3;regulatorIndicator(3);sendStatus(0);}
    else if(data == 'e'){if(TL==1){switchingTL();TL = 0;}dimming = 50;regulator_press=4;regulatorIndicator(4);sendStatus(0);}
    else if(data == 'f'){if(TL==1){switchingTL();TL = 0;}dimming = 30;regulator_press=5;regulatorIndicator(5);sendStatus(0);}

    else if(data == 'g'){if(TL==0){switchingTL();TL = 1;}dimming = 0;regulator_press=0;regulatorIndicator(0);sendStatus(0);}
    else if(data == 'h'){if(TL==0){switchingTL();TL = 1;}dimming = 80;regulator_press=1;regulatorIndicator(1);sendStatus(0);}
    else if(data == 'i'){if(TL==0){switchingTL();TL = 1;}dimming = 70;regulator_press=2;regulatorIndicator(2);sendStatus(0);}
    else if(data == 'j'){if(TL==0){switchingTL();TL = 1;}dimming = 60;regulator_press=3;regulatorIndicator(3);sendStatus(0);}
    else if(data == 'k'){if(TL==0){switchingTL();TL = 1;}dimming = 50;regulator_press=4;regulatorIndicator(4);sendStatus(0);}
    else if(data == 'l'){if(TL==0){switchingTL();TL = 1;}dimming = 30;regulator_press=5;regulatorIndicator(5);sendStatus(0);} 
    else if(data == 'm')sendStatus(0);
    else if(data == 'x')connectionState=0;//ble disconnected
    else if(data == 'y')connectionState=1;//ble connected
  }
  digitalWrite(writeControl0, HIGH);
  digitalWrite(writeControl1, LOW);  
  }
}


void ble_receive(void)
{
  char data;
   if (hardwareSerial.available() > 0) 
   {
   data = (char)hardwareSerial.read();
   
   if(data == 'a'){if(TL==1){switchingTL();TL = 0;}dimming = 0;regulator_press=0;regulatorIndicator(0);sendStatus(1);}
    else if(data == 'b'){if(TL==1){switchingTL();TL = 0;}dimming = 80;regulator_press=1;regulatorIndicator(1);sendStatus(1);}
    else if(data == 'c'){if(TL==1){switchingTL();TL = 0;}dimming = 70;regulator_press=2;regulatorIndicator(2);sendStatus(1);}
    else if(data == 'd'){if(TL==1){switchingTL();TL = 0;}dimming = 60;regulator_press=3;regulatorIndicator(3);sendStatus(1);}
    else if(data == 'e'){if(TL==1){switchingTL();TL = 0;}dimming = 50;regulator_press=4;regulatorIndicator(4);sendStatus(1);}
    else if(data == 'f'){if(TL==1){switchingTL();TL = 0;}dimming = 30;regulator_press=5;regulatorIndicator(5);sendStatus(1);}

    else if(data == 'g'){if(TL==0){switchingTL();TL = 1;}dimming = 0;regulator_press=0;regulatorIndicator(0);sendStatus(1);}
    else if(data == 'h'){if(TL==0){switchingTL();TL = 1;}dimming = 80;regulator_press=1;regulatorIndicator(1);sendStatus(1);}
    else if(data == 'i'){if(TL==0){switchingTL();TL = 1;}dimming = 70;regulator_press=2;regulatorIndicator(2);sendStatus(1);}
    else if(data == 'j'){if(TL==0){switchingTL();TL = 1;}dimming = 60;regulator_press=3;regulatorIndicator(3);sendStatus(1);}
    else if(data == 'k'){if(TL==0){switchingTL();TL = 1;}dimming = 50;regulator_press=4;regulatorIndicator(4);sendStatus(1);}
    else if(data == 'l'){if(TL==0){switchingTL();TL = 1;}dimming = 30;regulator_press=5;regulatorIndicator(5);sendStatus(1);}
    else if(data== 'q')sendStatus(1);
}
}

void setup()
{
  hardwareSerial.begin(115200);

  pinMode(light_pin, OUTPUT);
  digitalWrite(light_pin, HIGH);

  pinMode(regindi1, OUTPUT);
  pinMode(regindi2, OUTPUT);
  pinMode(regindi3, OUTPUT);
  pinMode(regindi4, OUTPUT);
  pinMode(regindi5, OUTPUT);

  digitalWrite(regindi1, LOW);
  digitalWrite(regindi2, LOW);
  digitalWrite(regindi3, LOW);
  digitalWrite(regindi4, LOW);
  digitalWrite(regindi5, LOW);

  attachInterrupt(0, zero_crosss_int, CHANGE);  // Choose the zero cross interrupt # from the table above
  pinMode(AC_LOAD, OUTPUT);
  digitalWrite(AC_LOAD, LOW);

  pinMode(switchFeedback,INPUT);
  pinMode(writeControl0, OUTPUT);
  pinMode(writeControl1, OUTPUT);
  pinMode(espCheck, INPUT);
  digitalWrite(writeControl0, HIGH);
  digitalWrite(writeControl1, LOW);
  //sei();
}

void loop()
{
  esp_receive();
  ble_receive();
  sendSwitchStatus_update();
  //digitalWrite(AC_LOAD, LOW);
  fanRegulatorManual();
}
