 // include the library code:
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

#define M0 A0 // Device 1
#define M1 A1 // Device 2
#define M2 A2 // Device 3
#define M3 A3 // Device 4
#define pir1 10  // pir 1
#define pir2 11  // pir 2
char cmd_arr1[100];
int cmd_count1;
int pir1_flag=0,pir2_flag=0;
char d1=0,f1=0,d2=0,f2=0;
unsigned long pir1time=0,pir2time=0;
void print_relay_status()
{
  
  if(d1 == 0) // Light1 OFF
  {
    lcd.setCursor(7, 0); 
    lcd.print("X");
  }
  else
  {
    lcd.setCursor(7, 0); 
    lcd.print("1");
  }
  
  if(f1 == 0) // Fan1
 
  {
    lcd.setCursor(9, 0); 
    lcd.print("X");
  }
  else
  {
    lcd.setCursor(9, 0); 
    lcd.print("1");
  }

  if(d2 == 0) // Light2
  {
    lcd.setCursor(8, 0);
    lcd.print("X");
  }
  else
  {
    lcd.setCursor(8, 0);
    lcd.print("1");
  }
  if(f2 == 0) // Fan2
  {
    lcd.setCursor(10, 0);
    lcd.print("X");
  }
  else
  {
    lcd.setCursor(10, 0);
    lcd.print("1");
  }  
}
//*************************************************************
void setup() 
{                
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(pir1, INPUT);pinMode(pir2, INPUT);
  //digitalWrite(pir1,LOW);digitalWrite(pir2,LOW);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("    IoT Home    ");//Arduino IoT Home Energy Sensor
  lcd.setCursor(0, 1);    
  lcd.print(" Energy Sensor  ");
  delay(2000);
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("DEV ON:XXXX     ");    
  //lcd.print("D1:OFF    D2:OFF");
  lcd.setCursor(0, 1);    
  lcd.print("PIR STAT:XX     ");    
  //lcd.print("D3:OFF    D4:OFF");
  pinMode(M0, OUTPUT);  pinMode(M2, OUTPUT);
  pinMode(M1, OUTPUT);  pinMode(M3, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(M0, HIGH);   digitalWrite(M1, HIGH);   
  digitalWrite(M2, HIGH);   digitalWrite(M3, HIGH);
  pir1_flag=0;pir2_flag=0;
  d1=0;f1=0;d2=0;f2=0;
  //print_relay_status();
}
//**************************************************
void get_pir_status()
{
  char state1 = digitalRead(pir1);
  char state2 = digitalRead(pir2);
  if(state1 == HIGH)
  {
    lcd.setCursor(9,1);
    lcd.print("1");
    //lcd.print("PIR1:DETECTED   ");
    Serial.print("<ISENSOR1 ACTIVATED>");
    delay(2000);  
  }
  else
  {
    lcd.setCursor(9,1);
    lcd.print("X");
    digitalWrite(13, HIGH);
    //lcd.print("PIR1:NOTHING    ");
  }

  if(state2 == HIGH)
  {
    lcd.setCursor(10,1);
    lcd.print("1");
    digitalWrite(13, LOW);
    //lcd.print("PIR2:DETECTED   ");
    Serial.print("<ISENSOR2 ACTIVATED>");
    delay(2000);  
  }
  else
  {
    lcd.setCursor(10,1);
    lcd.print("X");
    digitalWrite(13, HIGH);
    //lcd.print("PIR2:NOTHING    ");
  }

  if(pir1_flag == 1 && state1 != HIGH)
  {
    if(millis()-pir1time>=30000)
    {
      pir1time = millis();
      lcd.clear();
      lcd.print("POWER SAVER R1  ");
      digitalWrite(M0, HIGH);   digitalWrite(M1, HIGH);
      delay(2000);
       lcd.setCursor(0, 0);
      lcd.print("DEV ON:         ");    
      lcd.setCursor(0, 1);    
      lcd.print("PIR STAT:XX     ");
      d1 = 0; d2 = 0;
      print_relay_status();
    }
  }
  else
  {
    pir1time = millis();  
  }

  if(pir2_flag == 1 && state2 != HIGH)
  {
    if(millis()-pir2time>=30000)
    {
      pir2time = millis();
      lcd.clear();
      lcd.print("POWER SAVER R2  ");
      digitalWrite(M2, HIGH);   digitalWrite(M3, HIGH);
      delay(2000);
      lcd.setCursor(0, 0);
      lcd.print("DEV ON:         ");    
      lcd.setCursor(0, 1);    
      lcd.print("PIR STAT:XX     ");
      f1 = 0; f2 = 0;
      print_relay_status();
    }
  }
  else
  {
    pir2time = millis();  
  }
}
//**************************************************
void serial_get_command()
{
  char inchar=0;
  if(Serial.available() > 0)
  {
    inchar = Serial.read();    
    if(inchar == '<')
    {
      cmd_count1=0;
      while(inchar != '>' && cmd_count1<15)
      {
        if(Serial.available() > 0)
        {
          inchar = Serial.read();
          cmd_arr1[cmd_count1++] = inchar;
          cmd_arr1[cmd_count1] = '\0';
        }        
      }
      if(inchar == '>')
      {
        print_relay_status();
        cmd_count1--;
        cmd_arr1[cmd_count1] = '\0';
        print_relay_status();
        if(cmd_arr1[0]=='A')
        {
          cmd_arr1[0]='0';digitalWrite(M0, LOW);d1=1;pir1_flag=1;
          pir1time = millis();
        }
        else if(cmd_arr1[0]=='B')
        {
          cmd_arr1[0]='0';digitalWrite(M0, HIGH);d1=0;pir1_flag=0;
        }
        else if(cmd_arr1[0]=='C')
        {
          cmd_arr1[0]='0';digitalWrite(M1, LOW);d2=1;pir1_flag=1;
          pir1time = millis();
        }
        else if(cmd_arr1[0]=='D')
        {
          cmd_arr1[0]='0';digitalWrite(M1, HIGH);d2=0;pir1_flag=0;
        }
        else if(cmd_arr1[0]=='E')
        {
          cmd_arr1[0]='0';digitalWrite(M2, LOW);f1=1;pir2_flag=1;
          pir2time = millis();
        }
        else if(cmd_arr1[0]=='F')
        {
          cmd_arr1[0]='0';digitalWrite(M2, HIGH);f1=0;pir2_flag=0;
        }
        else if(cmd_arr1[0]=='G')
        {
          cmd_arr1[0]='0';digitalWrite(M3, LOW);f2=1;pir2_flag=1;
          pir2time = millis();
        }
        else if(cmd_arr1[0]=='H')
        {
          cmd_arr1[0]='0';digitalWrite(M3, HIGH);f2=0;pir2_flag=0;
        }
      }
    }
  }
}
//******************************************************
void loop() 
{
   serial_get_command();
   get_pir_status();
   print_relay_status();    
}
