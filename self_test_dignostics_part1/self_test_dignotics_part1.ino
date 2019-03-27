#include <Adafruit_NeoPixel.h>
#include "U8glib.h"


#include <IRremote.h>
#define delay_val 2000
#define delay_val_1k 750
#define interval 15000
#define PIN 6

#define x1 0
#define y1 17
//#define x2 0
#define y2 45
#define pin_Count 12   // the number of pins (i.e. the length of the array)
//#define BT_Test_code "555"


U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

//const String TEST="Test ";
//const char FINISHED[8]="Finished";
//const char STARTED[7]="Started";
//String FILENAME="sdTest.txt";

const char *const string_table[] PROGMEM = {"Self Test", "Diagnostics", "Test Started",
                                            "Test Finished", "OK", "LED 13", "LED 0",
                                            "LED RING", "WS2812", "BUZZER", "BUTTON", 
                                            "TOUCHPAD", "LDR", "Potentiometer", "LM35",
                                            "IR RECEIVER", "Press Button", "3 TImes",
                                            "Press Touchpad", "Button tested", 
                                            "Touchpad tested", "RED", "GREEN", "BLUE",
                                            "Color", "Rainbow", "Effect", "Fade In Fade Out",
                                            "Shine Light On", "Turn knob of", "Press IR Remote",
                                            "Code1", "Code2", "Code3", "Code4", "Received",
                                            "Touchpad touched", "Button pressed", "IR tested",
                                            "BLUETOOTH","send data", "from phone", "tested OK","Finished"
                                            };
                                         
unsigned long currentMillis = 0;

#define RECV_PIN 3
IRrecv irrecv(RECV_PIN);
decode_results results;

void showMessageOnLcd(const char *str1, const char *str2)
    {
     
      u8g.firstPage();
      do { //u8g.setFont(u8g_font_timB14);
          // Serial.println(str1);
           //(buffer, str1);  // Necessary casts and dereferencing, just copy.
           //Serial.println(buffer);
           u8g.drawStr( x1, y1,str1);
           //Serial.println(str2);
           //strcpy_P(buffer, str2);  // Necessary casts and dereferencing, just copy.
           u8g.drawStr( x1, y2, str2);    
         } while( u8g.nextPage() );
      delay(delay_val);   
    }
    
void setup() {
  // put your setup code here, to run once:
 // Serial.begin(9600);
  u8g.setFont(u8g_font_timR14);
 // u8g.setColorIndex(1); // Instructs the display to draw with a pixel on. 
        
  strip.begin();
  //strip.show(); // Initialize all pixels to 'off'                                       //
  showMessageOnLcd((char *)pgm_read_word(&(string_table[0])),(char *)pgm_read_word(&(string_table[1])));
  //delay(delay_val);

      
   
    LED13_TEST();
    LED0_TEST();
    LED_RING_TEST();
    buzzer_TEST();
    WS2812_TEST();
    button1_TEST();
    touchPad_TEST();
    ldr_TEST();
    potentiometer_TEST();
    LM35_TEST(); 
    IR_TEST();
    //sdCard_TEST();   
}



void LED13_TEST()
    { 
      DDRB |= (1<<PB5);  
      showMessageOnLcd((char *)pgm_read_word(&(string_table[5])),(char *)pgm_read_word(&(string_table[2])));
      //delay(delay_val);   
      for(byte a=0;a<10;a++)
         {
          PORTB ^= (1<<PB5);
          delay(delay_val_1k);
         }  
      showMessageOnLcd((char *)pgm_read_word(&(string_table[5])) ,(char *)pgm_read_word(&(string_table[3])));
      //delay(delay_val);
      PORTB &= 0b00000000; 
    }
  
void LED0_TEST()
    { 
      DDRD |= (1<<PD0);
      PORTD &= 0b00000000;  
      showMessageOnLcd((char *)pgm_read_word(&(string_table[6])),(char *)pgm_read_word(&(string_table[2])));
      //delay(delay_val);   
      for(byte a=0;a<10;a++)
         {
          PORTD ^= (1<<PD0);
          delay(delay_val_1k);
         }  
      showMessageOnLcd((char *)pgm_read_word(&(string_table[6])) ,(char *)pgm_read_word(&(string_table[3])));
     // delay(delay_val);
      PORTD &= 0b00000000; 
    }  

void LED_RING_TEST()
    {     
      // int led_Pins[] = {1,2,3,4,5,6,7,8,9,10,11,12};   //an array of pin numbers to which LEDs are attached
     
      DDRD = 0xFF;
      DDRB = 0xFF;
      showMessageOnLcd((char *)pgm_read_word(&(string_table[7])),(char *)pgm_read_word(&(string_table[2])));
      //delay(delay_val);   
      for(byte i=1; i<=pin_Count; i++)
         {
           digitalWrite(i,HIGH);
           //showMessageOnLcd(10, 30,"LED "+ String(i),90,30,"ON");   
           delay(500);
         }
      delay(delay_val);
      for(byte j=pin_Count; j>0; j--)
         {
           digitalWrite(j,LOW);       
           //showMessageOnLcd(10, 30,"LED "+String(j),90,30,"OFF");    
           delay(500);
           if(j==0) break;
         }
      delay(delay_val);   
      showMessageOnLcd((char *)pgm_read_word(&(string_table[7])),(char *)pgm_read_word(&(string_table[3])));
      //delay(delay_val); 
    }
  
void buzzer_TEST()
    { 
      showMessageOnLcd((char *)pgm_read_word(&(string_table[9])),(char *)pgm_read_word(&(string_table[2])));
      //delay(delay_val);
     // notes in the melody:
     int melody[]= {196, 196, 220, 196, 262, 247, 196, 196, 220, 196, 294, 262, 
                    196, 196, 392, 330, 262, 247, 220, 349, 349, 330, 262, 294, 262 };
     // note durations: 4 = quarter note, 8 = eighth note, etc.:
     int noteDurations[] = { 8,8,4,4,4,2,8,8,4,4,4,2,8,8,4,4,4,4,3,8,8,4,4,4,2 };

     // iterate over the notes of the melody: 
     for(byte thisNote = 0; thisNote < 26; thisNote++) 
        {
         // to calculate the note duration, take one second 
         // divided by the note type.
         //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
         int noteDuration = 1000/noteDurations[thisNote];
         tone(A1, melody[thisNote],noteDuration);
         // to distinguish the notes, set a minimum time between them.
         // the note's duration + 30% seems to work well:
         int pauseBetweenNotes = noteDuration * 1.60;
         delay(pauseBetweenNotes);
         // stop the tone playing:
         noTone(8);
        }
      showMessageOnLcd((char *)pgm_read_word(&(string_table[9])),(char *)pgm_read_word(&(string_table[3])));
      //delay(delay_val); 
    }

void WS2812_TEST()
   { 
     showMessageOnLcd((char *)pgm_read_word(&(string_table[8])),(char *)pgm_read_word(&(string_table[2])));
     //delay(delay_val);
     for(byte i=0; i<3; i++)
        {
          switch(i)
               { 
                case 0: strip.setPixelColor(0,255,0,0);strip.show();
                showMessageOnLcd((char *)pgm_read_word(&(string_table[21])),(char *)pgm_read_word(&(string_table[24])));
                delay(delay_val);break;
                case 1: strip.setPixelColor(0,0,255,0);strip.show();
                showMessageOnLcd((char *)pgm_read_word(&(string_table[22])),(char *)pgm_read_word(&(string_table[24])));
                delay(delay_val);break;
                case 2: strip.setPixelColor(0,0,0,255);strip.show();
                showMessageOnLcd((char *)pgm_read_word(&(string_table[23])),(char *)pgm_read_word(&(string_table[24])));
                delay(delay_val); break;
               }
         } 
    //delay(delay_val);
    showMessageOnLcd((char *)pgm_read_word(&(string_table[25])),(char *)pgm_read_word(&(string_table[26])));
   // delay(delay_val); 

     for(byte j=0; j<255; j++) 
        {
          for(byte i=0; i<strip.numPixels(); i++) 
             {
               strip.setPixelColor(i, Wheel((i+j) & 255));
             }
          strip.show();
          delay(20);
        } 
     // delay(delay_val);  
    showMessageOnLcd((char *)pgm_read_word(&(string_table[27])),(char *)pgm_read_word(&(string_table[26])));
    //delay(delay_val);
 
    for(byte j=0; j<3; j++) 
       { // Fade IN
       for(byte k = 0; k < 255; k++)
          { 
           switch(j)  
                 { 
                   case 0: strip.setPixelColor(0,k,0,0);strip.show(); break;
                   case 1: strip.setPixelColor(0,0,k,0);strip.show(); break;
                   case 2: strip.setPixelColor(0,0,0,k);strip.show(); break; 
                  }
           delay(10);
          }
         // Fade OUT
       for(byte k=255; k>=0; k--) 
          { 
           switch(j) 
                 { 
                   case 0: strip.setPixelColor(0,k,0,0);strip.show(); break;
                   case 1: strip.setPixelColor(0,0,k,0);strip.show(); break;
                   case 2: strip.setPixelColor(0,0,0,k);strip.show(); break;
                  }
           delay(3);
           if(k==0)break;  
           }

       }  
     showMessageOnLcd((char *)pgm_read_word(&(string_table[8])),(char *)pgm_read_word(&(string_table[3])));
     //delay(delay_val); 
   } 
   
uint32_t Wheel(byte WheelPos) 
       {
         if(WheelPos < 85) 
           {
             return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
           } 
         else if(WheelPos < 170) 
               {
                 WheelPos -= 85;
                 return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
               } 
         else 
             {
               WheelPos -= 170;
               return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
             }
        }

void button1_TEST()
    { 
      byte Button1Pressed = 0;
      byte Button1LastState = LOW; 
      DDRD &= 0x00000000;
      showMessageOnLcd((char *)pgm_read_word(&(string_table[10])),(char *)pgm_read_word(&(string_table[2])));
      //delay(delay_val);
      showMessageOnLcd((char *)pgm_read_word(&(string_table[16])),(char *)pgm_read_word(&(string_table[17])));
      //delay(delay_val);
      currentMillis = millis();
      while(((millis() - currentMillis) < interval) && Button1Pressed < 3)
           {
            byte Button1State = ((PIND & (1<<5)) >> 5);
            if(Button1State==HIGH && Button1LastState==LOW)
              {
               Button1Pressed++;
               u8g.firstPage();
           do {  
                u8g.drawStr( x1, y1, (char *)pgm_read_word(&(string_table[37])));
                u8g.setPrintPos(x1, y2);
                u8g.print(Button1Pressed);
              } while( u8g.nextPage() );
              } 
            if(Button1Pressed == 3)
              {  
              // Button1Pressed =0;
               showMessageOnLcd((char *)pgm_read_word(&(string_table[19])),(char *)pgm_read_word(&(string_table[4])));
               //delay(delay_val);
              }
        
            Button1LastState=Button1State;

             Button1State = ((PIND & (1<<5)) >> 5);    
          } 
        showMessageOnLcd((char *)pgm_read_word(&(string_table[10])),(char *)pgm_read_word(&(string_table[3])));
        //delay(delay_val);        
    } 

void touchPad_TEST()
    { 
      byte touched = 0;
      byte touchPadLastState=LOW;
      showMessageOnLcd((char *)pgm_read_word(&(string_table[11])),(char *)pgm_read_word(&(string_table[2])));
      //delay(delay_val);
      showMessageOnLcd((char *)pgm_read_word(&(string_table[18])),(char *)pgm_read_word(&(string_table[17])));
      //delay(delay_val);
      currentMillis = millis();
      while(((millis() - currentMillis) < interval) && touched < 3)
           {
             //int touchPadState = digitalRead(touchpadPin);
             byte touchPadState = ((PIND & (1<<4)) >> 4);
             if(touchPadState == HIGH && touchPadLastState == LOW)
               { 
                 touched++;
                 u8g.firstPage();
                 do {  
                     u8g.drawStr(x1, y1, (char *)pgm_read_word(&(string_table[36])));
                     u8g.setPrintPos(x1, y2);
                     u8g.print(touched);
                    } while( u8g.nextPage() );
          }
              if(touched == 3)
                {
                 // touched = 0;
                  showMessageOnLcd((char *)pgm_read_word(&(string_table[20])),(char *)pgm_read_word(&(string_table[4])));
                  //delay(delay_val);
                 } 
        
               touchPadLastState=touchPadState;
               touchPadState = ((PIND & (1<<4)) >> 4);    
              } 
       showMessageOnLcd((char *)pgm_read_word(&(string_table[11])),(char *)pgm_read_word(&(string_table[3])));
       //delay(delay_val);
    } 

void ldr_TEST()
    { 
      showMessageOnLcd((char *)pgm_read_word(&(string_table[12])),(char *)pgm_read_word(&(string_table[2])));
      //delay(2000);
      showMessageOnLcd((char *)pgm_read_word(&(string_table[28])),(char *)pgm_read_word(&(string_table[12])));
      //delay(2000);
      currentMillis = millis();
      while(((millis() - currentMillis) < interval))
           {
             int ldr_Value = analogRead(A3);
             
             u8g.firstPage();
           do {  
                u8g.drawStr( x1, y1, "LDR value:");
                u8g.setPrintPos(x1, y2);
                u8g.print(ldr_Value);
              } while( u8g.nextPage() );
            delay(500); 
           }
      showMessageOnLcd((char *)pgm_read_word(&(string_table[12])),(char *)pgm_read_word(&(string_table[3])));
      //delay(2000);
     
    } 
void potentiometer_TEST()
    {  
      showMessageOnLcd((char *)pgm_read_word(&(string_table[13])),(char *)pgm_read_word(&(string_table[2])));
      //delay(2000);  
      showMessageOnLcd((char *)pgm_read_word(&(string_table[29])),(char *)pgm_read_word(&(string_table[13])));
      //delay(2000);
      currentMillis = millis();
      while(((millis() - currentMillis) < interval))
          {
            int pot_Value = analogRead(A2);
            u8g.firstPage();
           do {  
                u8g.drawStr( x1, y1, "Pot value:");
                u8g.setPrintPos(x1, y2);
                u8g.print(pot_Value);
              } while( u8g.nextPage() );
            delay(500);
           }
       showMessageOnLcd((char *)pgm_read_word(&(string_table[13])),(char *)pgm_read_word(&(string_table[3])));
       //delay(2000);
      
     
    } 
    
 void LM35_TEST()
      { 
        showMessageOnLcd((char *)pgm_read_word(&(string_table[14])),(char *)pgm_read_word(&(string_table[2])));
        //delay(2000);
        //showMessageOnLcd(1,17,"increase or",5,45,"decrease temp");
        //delay(2000);
        currentMillis = millis();
        while(((millis() - currentMillis) < interval)) 
             {
               byte temperature =( 5 * analogRead(A0) * 100) / 1024;
               u8g.firstPage();
               do {  
                    u8g.drawStr(x1, y1, "Temp:");
                    u8g.setPrintPos(x1, y2);
                    u8g.print(temperature);
                    u8g.setPrintPos(50, y2);
                    u8g.print(char(176));
                    u8g.drawStr( 55, y2, "C");
                  } while( u8g.nextPage() );
               delay(500);
             }
         showMessageOnLcd((char *)pgm_read_word(&(string_table[14])),(char *)pgm_read_word(&(string_table[3])));
         //delay(2000);
      }

void IR_TEST() 
    { 
      byte count=0;
      irrecv.enableIRIn(); // Start the receiver
      showMessageOnLcd((char *)pgm_read_word(&(string_table[15])),(char *)pgm_read_word(&(string_table[2])));
      //delay(2000);
      showMessageOnLcd((char *)pgm_read_word(&(string_table[30])),(char *)pgm_read_word(&(string_table[17])));
      //delay(2000);
      currentMillis = millis();
      while((millis()-currentMillis < interval) && count < 3) 
           {
             if(irrecv.decode(&results)) 
               {
                 //translateIR();
                 switch(count)
                       {
                         case 0: showMessageOnLcd((char *)pgm_read_word(&(string_table[31])),(char *)pgm_read_word(&(string_table[35])));count++; break;
                         case 1: showMessageOnLcd((char *)pgm_read_word(&(string_table[32])),(char *)pgm_read_word(&(string_table[35])));count++; break;
                         case 2: showMessageOnLcd((char *)pgm_read_word(&(string_table[33])),(char *)pgm_read_word(&(string_table[35])));count++; break;
                         //case 3: showMessageOnLcd((char *)pgm_read_word(&(string_table[34])),(char *)pgm_read_word(&(string_table[35])));count++; break;
                         delay(500);
                       }
                 irrecv.resume(); // receive the next value      
                 if(count>=3)
                   {  
                     showMessageOnLcd((char *)pgm_read_word(&(string_table[38])),(char *)pgm_read_word(&(string_table[4])));
                     //delay(2000);
                   }
                   
                 }
             }
      showMessageOnLcd((char *)pgm_read_word(&(string_table[15])),(char *)pgm_read_word(&(string_table[3])));
      showMessageOnLcd((char *)pgm_read_word(&(string_table[0])),(char *)pgm_read_word(&(string_table[43])));
      //delay(2000);
    }

void loop()
{

    }







        
