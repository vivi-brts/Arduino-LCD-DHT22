#include <DHT.h> 
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int lcd_key = 0;
int adc_key_in = 0;
#define DHTPIN 11 // what pin we're connected to
#define DHTTYPE DHT22 // DHT 22 (AM2302)

//States for the menu.
int currentMenuItem = 0;
int lastState = 0;
int mainstate = 0;

//sensor
DHT dht(DHTPIN, DHTTYPE);


int checkstate()
{
  int x = analogRead(0); // read the value from the keypad
 if (x < 50) {
  //right
 }
    
        else if (x < 195) 
        {
          mainstate=1; //up
        }
  
          else  if (x < 380)
            {
             mainstate=2; //down
            }
    
            else  if (x < 555)
            { 
              mainstate=3; //left
            }

              else  if (x < 790)
               {
                 mainstate=4; //select
              }
    
    return mainstate;
}  // Checks if menu buttons pressed to initiate the menu display

 
void mainMenu() {// Sets up main menu
 //Refresh the button pressed.
   //State = 0 every loop cycle.
   int state;
   state = 0;
  //Set the Row 0, Col 0 position.
  lcd.setCursor(0,0);

     int x = analogRead (0);
      //Check analog values from LCD Keypad Shield
    if (x < 50) {
     //Right
   } else if (x < 195) {
    //Up
     state = 1;
   } else if (x < 380){
    //Down
     state = 2;
    } else if (x < 555){
     //Left
     state = 3;
   } else if (x < 790){
      //Select
     state = 4;
   }

   
  //If we are out of bounds on th menu then reset it.
  if (currentMenuItem < 0 || currentMenuItem > 4) {
   currentMenuItem = 0; 
  }
 
   //If we have changed Index, saves re-draws.
   if (state != lastState) {
      if (state == 1) {
         //If Up
          currentMenuItem = currentMenuItem - 1; 
          displayMenu(currentMenuItem);
      } else if (state == 2) {
         //If Down
          currentMenuItem = currentMenuItem + 1;  
          displayMenu(currentMenuItem);
      } else if (state == 3) {
         //If Left
         currentMenuItem = 0; 
      }else if (state == 4) {
         //If Selected
         selectMenu(currentMenuItem); 
         currentMenuItem = 0; 
      }
      //Save the last State to compare.
      lastState = state;
   } 
   //Small delay
  delay(5);

}


   //Display Menu Option based on Index.
void displayMenu(int x) {// Prints menu options
     switch (x) {
      case 1:
        clearPrintTitle();
        lcd.print ("Display Temperature");
        break;
      case 2:
        clearPrintTitle();
        lcd.print ("Display Humidity");
        break;
       case 3:
        clearPrintTitle();
        lcd.print ("Set Brightness");
        break;
      case 4:
        clearPrintTitle();
        lcd.print ("Show Name&Group");
        break;
    }
}
 
//Print a basic title on Row 1.
void clearPrintTitle() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" OPTION TO");
  lcd.setCursor(0,1); 
}


//Show the selection on Screen.
void selectMenu(int x) {// Lowest menu level
  int state;
   float h = dht.readHumidity();
    float t = dht.readTemperature();

   switch (x) {
      case 1:
        lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Temperature: ");
              if (isnan(t)) {  //check if sensor works
                      lcd.setCursor(1,1);
                      lcd.println("Failed to read DHT");
                      } else {
                      lcd.setCursor(5,1);
                      lcd.print(t);
                      lcd.print(" *C");
                      delay(500); 
                      }
     break;
     
      case 2:
       lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Humidity: ");
        
        if (isnan(h)) {
              lcd.setCursor(1,1);
              lcd.println("Failed to read DHT");
            } else {
              lcd.setCursor(5,1);
              lcd.print(h);
              lcd.print(" %");   
               }
        break;
        
       case 3:
        lcd.clear();
        //lcd.setCursor(2,0);
        //lcd.print ("- Brightness + ");
        //lcd.createChar(1,b);
        brgthn();
        
        delay(500);
        break;
        
      case 4:
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print ("Your name here");
        lcd.setCursor(1,1);
        lcd.print("Group xy");
        
        delay(500);
        break;
    }
}



void callDht()
{
   float h = dht.readHumidity();
   float t = dht.readTemperature();
  lcd.setCursor(1,0);
  lcd.print("Temp: ");
  lcd.setCursor(7,0);
  lcd.print(t);
  lcd.print(" *C");
  
  lcd.setCursor(1,1);
  lcd.print("Humid: ");
  lcd.setCursor(8,1);
  lcd.print(h);
  lcd.print(" %");
}


#include <LCDKeypad.h>
#define DELTA 10
LCDKeypad lcdd;

//predefined backlight
unsigned char bckl=150;

void brgthn(){
  int btn;
 lcdd.clear();
 lcdd.setCursor(0,1);
 lcdd.print("Backlight:");
 lcdd.print(bckl);

//infinite loop for backlight
while(1){
  while ((btn=lcdd.button())==KEYPAD_NONE)
   { } //wait for button press
 delay(50);
 lcdd.noBlink(); 

 //adjust the backlight
 if (btn==KEYPAD_RIGHT)
    {
      if ((0xFF-DELTA)>=bckl)
        bckl +=DELTA;
      else bckl = 0xFF;
    }
    else if (btn==KEYPAD_LEFT)
    {
      if (DELTA<=bckl)
        bckl -= DELTA;
      else bckl = 0;
    }  
    else if(btn==KEYPAD_DOWN)
    {
       lcd.setCursor(0,1);
       lcd.print("Press RST to get back to initial state");
       lcd.scrollDisplayLeft();//builtin command to scroll left the text
       delay(150);
     }

 lcdd.print("                ");
 lcdd.setCursor(0,1);
 lcdd.print("Backlight:");
 lcdd.print(bckl);
 //set new backlight
 analogWrite(10, bckl);
  }
 }


void setup()
{
    Serial.begin(9600); //used to establish communication btw computer and arduino board
    lcd.begin(16, 2); // start the lcd display
    dht.begin();      //start sensor
}

void loop()  //main loop for the circuit to work as i planned
{
  checkstate(); //check buttons if pressed
  if (mainstate == 0) //when nothing pressed
  {
    callDht(); //as default to display temperature & humidity
  }
  else
  mainMenu(); //entering the menu to chose options
      
}
