#include <MsTimer2.h>
#include <Wire.h>  //NEW!!!!!!!!!!!!
#include <stdio.h>
#include "RSM_LOGO.c"
#include "SE_OBD_SIM.h"

int rpmt=0;
int tempt=5;
int spdt=0;
int dtct=5;
int dtctmp =0;


int buzz = 11;
int tmp2=0;
bool DISP=true;
#include <Adafruit_GFX.h>    
#include <Adafruit_TFTLCD.h>

#define LCD_CD A2 
#define LCD_WR A1 
#define LCD_CS A3 
#define LCD_RD A0 

#define LCD_RESET A4 

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

String TmpDisplayTPS="";
String TmpDisplayRPM="";
String TmpDisplayTemp="";
String TmpDisplaySPD="";
String TmpDisplayDTC="";
long TmpDisplayVRPM=0;
int shift=0;
int shift_ver=0;
int redd=0;
int drwt=0;
int tpst=0;
int engtmp=0;





void setup() { 
  pinMode(11,OUTPUT);
  pinMode(10, INPUT_PULLUP);
  Serial.begin(9600);
  tft.reset();
  uint16_t identifier = tft.readID();
  tft.begin(identifier);
;
}




 




void loop() {
  // put your main code here, to run repeatedly:
 if(tmp == 0){
    bootscreen();
    delay(2000);
	  drawframe();
	  tmp++;
 }

 if(tmp == 3){
  
  Reconnect();
  drawframe();
  TmpDisplayRPM="";
  tmp++;
 }

  drawdata();

int clearbtn = digitalRead(10);
if (clearbtn == LOW){
  clearscreen();
  cleardtc();
  tempt = 5;
  dtct=5;
  drawframe();
  TmpDisplayRPM="";
  ShiftInd();
  
}
if(DisplayVRPM >= 3500 && DisplayVTPS >= 40){
  
  digitalWrite(11, HIGH);
  
}
else{
  
  if (engtmp >= 6){
  digitalWrite(11, LOW);
  engtmp=0;
  }
  engtmp++;
}
  
}


void drawframe(){

  tft.setRotation(3);
  tft.fillScreen(BLACK);

  tft.drawRect(0, 0, 319, 239, WHITE);
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(2);
  tft.setCursor(5, 38);
  tft.println("ENGINE RPM");
  tft.setCursor(5,110);
  tft.println("COOLANT");
  tft.setCursor(160,38);
  tft.println("SPEED");
  tft.setCursor(160,110);
  tft.println("THROTTLE");

 }




void drawdata() {
  


  

        
    delay(30);


		TmpDisplayTPS=DisplayTPS;
		calctps();
		
		if(TmpDisplayTPS!=DisplayTPS){
			//  tft.fillRect (9, 100, 120, 20, BLACK);
			tft.setTextSize(3);
			tft.setCursor(160,130);
			if(DisplayVTPS<10){
				tft.print(DisplayTPS+"  ");
			}
			if(DisplayVTPS<100 && DisplayVTPS > 9){
				tft.print(DisplayTPS+"  ");
			}
	
		}
		else{
			tft.setTextSize(3);
			tft.setCursor(160,130);
			tft.print(DisplayTPS);
		}
  
    
 



      delay(30);

		TmpDisplayRPM=DisplayRPM;
		calcrpm();
		
			if(TmpDisplayRPM!=DisplayRPM){
			tft.setTextSize(3);
      ShiftInd();  
			tft.setCursor (9, 60);
				if(DisplayVRPM<10){
					tft.print(DisplayRPM+"    ");
				}

			}
		else{
		tft.setTextSize(3);
		tft.setCursor (9, 60);
		tft.print(DisplayRPM);   
    
		}
  
    
    
    

 
  delay(30);
    
    if (tempt == 5){
		TmpDisplayTemp=DisplayTemp;
		calctemp();
      tft.setTextSize(3);
      tft.setCursor(9,130);
		if(TmpDisplayTemp!=DisplayTemp){


			if(DisplayVTemp<100){
				tft.print(DisplayTemp+"   ");
			}

		}
		
		else {

			tft.print(DisplayTemp);
		}
	    tempt=0;
    }
    tempt++;
  





    

		TmpDisplaySPD=DisplaySPD;
		calcspd();
      tft.setTextSize(3);
      tft.setCursor(161,60);
		if(TmpDisplaySPD!=DisplaySPD){
		
			if(DisplayVSPD<10){
			tft.print(DisplaySPD+"  ");
			}
		
	
		}
		else{

			tft.print(DisplaySPD);
		}
    
    
  






    if (dtct == 5){

		TmpDisplayDTC=DisplayDTC;
		checkdtc();
		tft.setTextSize(2);
  
    if(TmpDisplayDTC=!DisplayDTC){
        tft.setTextColor(WHITE, BLACK);
        if (DisplayDTC == "CLEAR!"){
          dtctmp =0;
			tft.fillRect (150, 165, 150, 66, BLACK);
			tft.setTextSize(2);
			tft.setCursor(5, 175);
			tft.println("DTC CODE");
			tft.setTextSize(3);
			tft.setCursor(5, 200);
			tft.print(DisplayDTC+" ");
        }
        else if(DTCString00 == "NO" || DTCString00 == "43") {
          while(dtctmp <= 1 && DisplayDTC != "CLEAR!"){
          tft.setTextColor(RED, BLACK);
          tft.setCursor(5, 175);
          tft.println("DTC CODE");
          tft.setTextSize(3);
          tft.setCursor(5, 200);
          tft.print(DisplayDTC);
          tft.setTextColor(BLACK, YELLOW);

          drawBitmap(190, 165, DTCIMG, 90, 66,YELLOW);

          tft.setTextColor(WHITE, BLACK);
          tft.setTextSize(2);
          dtctmp++;
        }
        
        }
        else{
          tmp=3;
        }
        
		tft.drawRect(0, 0, 319, 239, WHITE);    
    }
     dtct=0;
    }
 

  dtct ++;
}
    


  

void clearscreen(){
  tft.setRotation(3);
  tft.fillScreen(YELLOW);
  tft.drawRect(0, 0, 319, 239, YELLOW);
  tft.setTextColor(BLACK);
  tft.setTextSize(3);
 
  tft.setCursor(30, 70);
  tft.println("CLEAR DTC CODE");
  tft.setCursor(30, 115);
  tft.println("CHECK YOUR CAR");
  delay(2500);
  checkdtc();
}

void bootscreen(){
  

  tmp=1;

  tft.setRotation(3);
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 319, 239, WHITE);
  //write tekst

  
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);
  drawBitmap(93, 25, RSM_LOGO, 135, 81,WHITE);
  tft.setCursor(15, 150);
  tft.println("OBD DATA DISPLAY");
  tft.setCursor(5, 200);
  tft.println("Welcome SeongYeon");
  
  Retry:
  delay(3000);
  
  tft.setCursor(5, 200);
  tft.println("Connecting.......");
  
  checkdtc();

  delay(1000);
  if(DTCString00 == "NO" || DTCString00 == "43"){
    tft.setCursor(5, 200);
    tft.println("  Connection OK  ");
    
  }
  
  else  {
    tft.setCursor(15, 150);
    tft.println("     Error      ");
    tft.setCursor(5, 200);
    tft.println ("  No Connection! ");
    delay(1500);
    goto Retry;
  }
  
 
}

void Reconnect(){
  tft.setRotation(3);
  tft.fillScreen(BLACK);
  tft.drawRect(0, 0, 319, 239, WHITE);
  //write tekst


  tft.setTextColor(RED, BLACK);
  tft.setTextSize(7);
  tft.setCursor(35, 45);
  tft.println("ERROR!");
  
  tft.setTextColor(WHITE, BLACK);
  tft.setTextSize(3);

  
  tft.setCursor(25, 150);
  tft.println("Connection Fail ");

  Recon:
  delay(3000);
  
  tft.setCursor(10, 200);
  tft.println("ReConnecting.....");
  
  checkdtc();

  delay(1500);
  if(DTCString00 == "NO" || DTCString00 == "43"){
    tft.setCursor(10, 200);
    tft.println("  Connection OK ");
    
  }
  
  else  {

    tft.setCursor(10, 200);
    tft.println (" No Connection!  ");
    delay(1000);
    goto Recon;
  }
}

void drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++) {
      if(i & 7) byte <<= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if(byte & 0x80) tft.drawPixel(x+i, y+j, color);
    }
  }
}
void ShiftInd(){


  if(DisplayVRPM <= 800){
    shift=1;
    if(shift<shift_ver){
      tft.fillRect (6, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
    }
  }
  
  
  if(DisplayVRPM >= 1000){
    shift=2;
    if(shift<shift_ver){
      tft.fillRect (21, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
    }
  }
  
  if(DisplayVRPM >= 1500){
    shift=3;
    if(shift<shift_ver){
      tft.fillRect (36, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
    }
  }
  
  if(DisplayVRPM >= 1700){
    shift=4;
    if(shift<shift_ver){
      tft.fillRect (51, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
    }
  }
  
  if(DisplayVRPM >= 2000){
    shift=5;
    if(shift<shift_ver){
      tft.fillRect (66, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
    }
  }
  if(DisplayVRPM >= 2400){
    shift=6;
    if(shift<shift_ver){
     tft.fillRect (81, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
    }
  }
  if(DisplayVRPM >= 2700){
    shift=7;
    if(shift<shift_ver){
      tft.fillRect (96, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
    }
  }
  if(DisplayVRPM >= 3000){
    shift=8;
    if(shift<shift_ver){
       tft.fillRect (111, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
      tft.fillRect (110, 5, 15, 25, YELLOW);
    }
  }
  if(DisplayVRPM >= 3300){
    shift=9;
    if(shift<shift_ver){
       tft.fillRect (131, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
      tft.fillRect (110, 5, 15, 25, YELLOW);
      tft.fillRect (130, 5, 15, 25, YELLOW);
    }
  }
  if(DisplayVRPM >= 3600){
    shift=10;
    if(shift<shift_ver){
       tft.fillRect (151, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
      tft.fillRect (110, 5, 15, 25, YELLOW);
      tft.fillRect (130, 5, 15, 25, YELLOW);
      tft.fillRect (150, 5, 15, 25, YELLOW);
    }
  }
  if(DisplayVRPM >= 3900){
    shift=11;
    if(shift<shift_ver){
       tft.fillRect (171, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
      tft.fillRect (110, 5, 15, 25, YELLOW);
      tft.fillRect (130, 5, 15, 25, YELLOW);
      tft.fillRect (150, 5, 15, 25, YELLOW);
      tft.fillRect (170, 5, 15, 25, YELLOW);
    }
  }
  if(DisplayVRPM >= 4200){
    shift=12;
    if(shift<shift_ver){
      tft.fillRect (191, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
      tft.fillRect (110, 5, 15, 25, YELLOW);
      tft.fillRect (130, 5, 15, 25, YELLOW);
      tft.fillRect (150, 5, 15, 25, YELLOW);
      tft.fillRect (170, 5, 15, 25, YELLOW);
      tft.fillRect (190, 5, 15, 25, YELLOW);
    }
  }
  if(DisplayVRPM >= 4500){
    shift=13;
    if(shift<shift_ver){
      tft.fillRect (211, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
      tft.fillRect (110, 5, 15, 25, YELLOW);
      tft.fillRect (130, 5, 15, 25, YELLOW);
      tft.fillRect (150, 5, 15, 25, YELLOW);
      tft.fillRect (170, 5, 15, 25, YELLOW);
      tft.fillRect (190, 5, 15, 25, YELLOW);
      tft.fillRect (210, 5, 15, 25, YELLOW);
    }
  }
  if(DisplayVRPM >= 4800){
    shift=14;
    if(shift<shift_ver){
       tft.fillRect (231, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
      tft.fillRect (110, 5, 15, 25, YELLOW);
      tft.fillRect (130, 5, 15, 25, YELLOW);
      tft.fillRect (150, 5, 15, 25, YELLOW);
      tft.fillRect (170, 5, 15, 25, YELLOW);
      tft.fillRect (190, 5, 15, 25, YELLOW);
      tft.fillRect (210, 5, 15, 25, YELLOW);
      tft.fillRect (230, 5, 15, 25, YELLOW);
    }
  } 
  if(DisplayVRPM >= 5200){
    shift=15;
    if(shift<shift_ver){
       tft.fillRect (251, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
      tft.fillRect (110, 5, 15, 25, YELLOW);
      tft.fillRect (130, 5, 15, 25, YELLOW);
      tft.fillRect (150, 5, 15, 25, YELLOW);
      tft.fillRect (170, 5, 15, 25, YELLOW);
      tft.fillRect (190, 5, 15, 25, YELLOW);
      tft.fillRect (210, 5, 15, 25, YELLOW);
      tft.fillRect (230, 5, 15, 25, YELLOW);
      tft.fillRect (250, 5, 15, 25, YELLOW);
    }
  } 
  if(DisplayVRPM >= 5500){
    shift=16;
    if(shift<shift_ver){
       tft.fillRect (271, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
      tft.fillRect (110, 5, 15, 25, YELLOW);
      tft.fillRect (130, 5, 15, 25, YELLOW);
      tft.fillRect (150, 5, 15, 25, YELLOW);
      tft.fillRect (170, 5, 15, 25, YELLOW);
      tft.fillRect (190, 5, 15, 25, YELLOW);
      tft.fillRect (210, 5, 15, 25, YELLOW);
      tft.fillRect (230, 5, 15, 25, YELLOW);
      tft.fillRect (250, 5, 15, 25, YELLOW);
      tft.fillRect (270, 5, 20, 25, RED); //6000
    }
  }  
  if(DisplayVRPM >= 5900){
    shift=17;
    if(shift<shift_ver){
       tft.fillRect (296, 5, 320, 25, BLACK);
    }
    if (shift_ver!=shift){
      shift_ver=shift;
      tft.fillRect (5, 5, 10, 25, GREEN); //1000
      tft.fillRect (20, 5, 10, 25, GREEN); //1500
      tft.fillRect (35, 5, 10, 25, GREEN); //1500
      tft.fillRect (50, 5, 10, 25, GREEN); //1500
      tft.fillRect (65, 5, 10, 25, GREEN); //1500
      tft.fillRect (80, 5, 10, 25, GREEN);
      tft.fillRect (95, 5, 10, 25, GREEN);
      tft.fillRect (110, 5, 15, 25, YELLOW);
      tft.fillRect (130, 5, 15, 25, YELLOW);
      tft.fillRect (150, 5, 15, 25, YELLOW);
      tft.fillRect (170, 5, 15, 25, YELLOW);
      tft.fillRect (190, 5, 15, 25, YELLOW);
      tft.fillRect (210, 5, 15, 25, YELLOW);
      tft.fillRect (230, 5, 15, 25, YELLOW);
      tft.fillRect (250, 5, 15, 25, YELLOW);
      tft.fillRect (270, 5, 18, 25, RED); //6000
      tft.fillRect (292, 5, 18, 25, RED); //6000
    }
  } 
  }
  
  
  
  
  
  
  
  
  
