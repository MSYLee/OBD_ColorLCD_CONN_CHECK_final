


byte SPDinData;
byte RPMinData;
byte TempinData;
byte DTCinData;
byte TPSinData;
byte initinData;

char SPDinChar;
char RPMinChar;
char DTCinChar;
char TempinChar;
char TPSinChar;
char initinChar;

String BuildINRPM = "";
String BuildINSPD = "";
String BuildINTemp = "";
String BuildINDTC = "";
String BuildINTPS = "";
String BuildINinit = "";

String DisplaySPD = "";
String DisplayRPM = "";
String DisplayTemp = "";
String DisplayDTC = "";
String DisplayTPS = "";

String SPDString = "";
String TempString = "";
String RPMString1 = "";
String RPMString2 = "";
String DTCString00="";
String DTCString0 = "";
String DTCString1 = "";
String DTCString2 = "";
String TPSString = "";
String initString = "";

long DisplayVRPM;
long DisplayVSPD;
long DisplayVTemp;
long DisplayVDTC;
long DisplayVTPS;

long TPSA;
long SPDA;
long RPMA;
long RPMB;
long TempA;
long DTCA;
long DTCB;
int tmp=0;

void obdinit(){
    Serial.println("01 05 ");
    Serial.flush();
    delay(130);
    while (Serial.available() > 0)
  {
    initinData = 0;
    initinChar = 0;
    initinData = Serial.read();
    initinChar = char(TempinData);
    BuildINinit = BuildINinit + initinChar;
  }
  initString = BuildINinit.substring(13, 19);

    
}

void calcrpm(){
   Serial.println("01 0C ");
  
   delay(130);
    Serial.flush();
   BuildINRPM = "";
   while (Serial.available() > 0)
  {
    RPMinData = 0;
    RPMinChar = 0;
    RPMinData = Serial.read();
    RPMinChar = char(RPMinData);
    BuildINRPM = BuildINRPM + RPMinChar;
  }
  RPMString1 = BuildINRPM.substring(16, 18);
  RPMString2 = BuildINRPM.substring(19, 21);
  RPMA = strtol(RPMString1.c_str(), 0, 16); 
  RPMB = strtol(RPMString2.c_str(), 0, 16);
  DisplayVRPM = (RPMA * 256 + RPMB) / 4;
  DisplayRPM = String(DisplayVRPM - 0) + "RPM";  

  

}

void checkdtc(){
  Serial.println("03 ");
 delay(130);
  Serial.flush();

  BuildINDTC = "";
  while (Serial.available() > 0)
  {
    DTCinData = 0;
    DTCinChar = 0;
    DTCinData = Serial.read();
    DTCinChar = char(DTCinData);
    BuildINDTC = BuildINDTC + DTCinChar;
  }
  DTCString00 = BuildINDTC.substring(7, 9);
  DTCString0 = BuildINDTC.substring(10, 12);
  DTCString1 = BuildINDTC.substring(10, 12);
  DTCString2 = BuildINDTC.substring(13, 15);


   if (DTCString0 != "DA"){
     DisplayDTC =  "[P" + DTCString1+DTCString2+"]";
    
  }
  
    else{
    DisplayDTC = "CLEAR!";
  }


}
void cleardtc(){
  Serial.println("04 ");
}

void calctemp(){
  Serial.println("01 05 ");
  delay(130);
  Serial.flush();
 
  BuildINTemp = "";
  while (Serial.available() > 0)
  {
    TempinData = 0;
    TempinChar = 0;
    TempinData = Serial.read();
    TempinChar = char(TempinData);
    BuildINTemp = BuildINTemp + TempinChar;
  }
  TempString = BuildINTemp.substring(16, 19);
  TempA = strtol(TempString.c_str(), 0, 16); 
  DisplayVTemp = TempA;
  DisplayTemp = String(DisplayVTemp - 40) + "C";

}

void calctps(){
  Serial.println("01 11 ");
  delay(130);
  Serial.flush();
  
  BuildINTPS = "";
  while (Serial.available() > 0)
  {
    TPSinData = 0;
    TPSinChar = 0;
    TPSinData = Serial.read();
    TPSinChar = char(TPSinData);
    BuildINTPS = BuildINTPS + TPSinChar;
  }
  TPSString = BuildINTPS.substring(16, 19);
  TPSA = strtol(TPSString.c_str(), 0, 16); 
  DisplayVTPS = (TPSA * 100)/255;
  DisplayTPS = String(DisplayVTPS)+ "%";

}

void calcspd(){
  Serial.println("01 0D ");
  Serial.flush();
  delay(130);
  BuildINSPD = "";
  while (Serial.available() > 0)
  {
    SPDinData = 0;
    SPDinChar = 0;
    SPDinData = Serial.read();
    SPDinChar = char(SPDinData);
    BuildINSPD  = BuildINSPD  + SPDinChar;
  }
  SPDString = BuildINSPD.substring(16, 19);
  SPDA = strtol(SPDString.c_str(), 0, 16);
  DisplayVSPD = SPDA;
  DisplaySPD = String(DisplayVSPD) + "Km/h";

}
