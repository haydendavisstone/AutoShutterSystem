//CDA 4630-002
//Final Project
//Hayden Stone and Tanis Anderson

#define RS P2_0
#define EN P2_1
#define D4 P2_2
#define D5 P2_3
#define D6 P2_4
#define D7 P2_5

#include <LiquidCrystal.h> //default library
#include <Keypad.h>
#include <string.h>
#include <Servo.h>

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7); //Let the library know how we have connected the LCD

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'0','1','2','3'},
  {'4','5','6','7'},
  {'8','9','A','B'},
  {'C','D','E','F'}
};
byte rowPins[ROWS] = {11, 12, 13, 14}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 8, 9, 10}; //connect to the column pinouts of the keypad

int insidetemp = 0;
int insidelight = 0;
int outsidetemp = 0;
int outsidelight = 0;
int shutterangle = 0;
int initialtemp = 0;

Servo myservo;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {

  Serial.begin(9600);
  lcd.begin(16, 2); //16*2 LCD display
  lcd.setCursor (0,0); //Place the cursor at 1st row 1st column
  myservo.attach(P1_3);  // attaches the servo on pin 3 to the servo object
  lcd.print("Final Project"); //Display intro message

   delay(3000);//Wait for display to show info
   lcd.clear();

}

void loop() {
    String userinput = "";
    String usermode = "";

    insidetemp = analogRead(P1_5);
    initialtemp = analogRead(P1_5);
    insidelight = 1023 - analogRead(P1_1);
    outsidetemp = analogRead(P1_4);
    outsidelight = 1023 - analogRead(P1_7);

    myservo.write(0);

    while(true){
        usermode = GetMode();
        if(usermode == "1"){

            lcd.clear();
            lcd.print("automatic mode");
            delay(1000);

            while(true){
                //get values at beginning of loop
                insidetemp = analogRead(P1_5);
                outsidetemp = analogRead(P1_4);
                insidelight = 1023 - analogRead(P1_1);
                outsidelight = 1023 - analogRead(P1_7);

                //night time
            if (outsidelight <= 300){
                if(shutterangle == 0){
                    delay(1000); // 30 min in reality
                }
                else{
                    shutterangle = 0;
                    myservo.write(shutterangle);
                    delay(1000);
                }

            }   //outside temp is 1.08 times greater than indoor temp or indoor temp has risen by 1.08
            else if (outsidetemp >= insidetemp*1.08 || insidetemp > initialtemp*1.08){
                if(shutterangle == 0){
                    delay(1000);
                }
                else{
                    shutterangle = shutterangle - 22;
                    myservo.write(shutterangle);
                    delay(1000);
                }
            }   //if not hot outside and outside light is greater than indoor light
            else if (outsidelight > insidelight*1.1){
                if(shutterangle == 88){
                    delay(1000);
                }
                else{
                    shutterangle = shutterangle + 22;
                    myservo.write(shutterangle);
                    delay(1000);
                }
            }   //not hot and light levels normal
            else{
                delay(1000);
            }
        }
        }
        else if(usermode == "2"){

            lcd.clear();
            lcd.print("user mode");
            delay(1000);

            while(true){
            userinput = GetInput();
            if(userinput == "1"){
                myservo.write(0);
                delay(1000);
            }
            else if(userinput == "2"){
                myservo.write(22);
                delay(1000);
            }
            else if(userinput == "3"){
                myservo.write(44);
                delay(1000);
            }
            else if(userinput == "4"){
                myservo.write(66);
                delay(1000);
            }
            else if(userinput == "5"){
                myservo.write(88);
                delay(1000);

            }
            else if(userinput == "*"){
                lcd.clear();
                lcd.print("exiting usermode");
                delay(1000);
                break;
            }
            }

        }
        else{
            lcd.clear();
            lcd.print("error try again");
            delay(1000);
    }
}
}

String GetInput(){
    int adc_val;
    while(true){
      adc_val = analogRead(A6); /* Read input from keypad */
      if (adc_val>850)
      {
              lcd.clear();
              lcd.print("1");
              delay(1000);
              return "1";
      }
      else if ( adc_val>550  && adc_val<750)
      {
          lcd.clear();
          lcd.print("2");
          delay(1000);
          return "2";
      }
      else if ( adc_val>400  && adc_val<550)
      {
          lcd.clear();
          lcd.print("3");
          delay(1000);
          return "3";
      }
      else if ( adc_val>250  && adc_val<300)
      {
          lcd.clear();
          lcd.print("4");
          delay(1000);
          return "4";
      }
      else if ( adc_val>210  && adc_val<240)
      {
          lcd.clear();
          lcd.print("5");
          delay(1000);
          return "5";
      }
      else if ( adc_val>117  && adc_val<200)
      {
          lcd.clear();
          lcd.print(String("invalid input"));
          delay(1000);
      }
      else if ( adc_val>=96  && adc_val<=110)
      {
          lcd.clear();
          lcd.print("*");
          delay(1000);
          return "*";
      }
      else if ( adc_val>=91  && adc_val<=95)
      {
              lcd.clear();
              lcd.print(String("invalid input"));
              delay(1000);
      }
      else if ( adc_val>=50  && adc_val<=90)
      {
          lcd.clear();
          lcd.print(String("invalid input"));
          delay(1000);
      }
      else
      {
          lcd.clear();
          lcd.print("enter value 1-5:");
          lcd.setCursor(0, 1);
          lcd.print("closed-open");
          delay(50);
    }
    }
}

String GetMode(){
    int adc_val;
    String tempmode;

    while(true){
      adc_val = analogRead(A6); /* Read input from keypad */
      if (adc_val>850)
      {
              lcd.clear();
              tempmode = "1";
              lcd.print(String(tempmode));
              delay(1000);
      }
      else if ( adc_val>550  && adc_val<750)
      {
              lcd.clear();
              tempmode = "2";
              lcd.print(String(tempmode));
              delay(1000);
      }
      else if ( adc_val>117  && adc_val<550)
      {
          lcd.clear();
          lcd.print(String("invalid input"));
          delay(1000);
      }
      else if ( adc_val>=96  && adc_val<=110)
      {
          lcd.clear();
          lcd.print("* pressed");
          delay(1000);
      }
      else if ( adc_val>=91  && adc_val<=95)
      {
              lcd.clear();
              lcd.print(String("invalid input"));
              delay(1000);
      }
      else if ( adc_val>=50  && adc_val<=90)
      {
          lcd.clear();
          lcd.print("# pressed");
          delay(1000);
      }
      else
      {
          lcd.clear();
          lcd.print("1: auto, 2: user");
          delay(50);
          if (tempmode.length() >= 1) {
              return tempmode;
          }
    }
    }
}



