volatile byte seqA = 0;
volatile byte seqB = 0;
volatile float cnt1 = 88.0;
volatile byte cnt2 = 0;
volatile boolean right = false;
volatile boolean left = false;
volatile boolean button = false;
float outputValue = 0;
float x = 88.0;
float y = 108.0;

#include <Wire.h>                                                        
#include <LiquidCrystal_I2C.h>
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  
  // Enable internal pull-up resistors
  digitalWrite(A0, HIGH);
  digitalWrite(A1, HIGH);
  digitalWrite(A2, HIGH);
 
  PCICR =  0b00000010; // 1. PCIE1: Pin Change Interrupt Enable 1
  PCMSK1 = 0b00000111; // Enable Pin Change Interrupt for A0, A1, A2
  Serial.begin (9600);
  Serial.println ("Channel Frequency");
  lcd.begin();
  lcd.backlight(); 
  
}

void loop() {

      // MAIN LOOP 
      if ( left ) { 
        //Serial.print (" Right Turn :");
        //Serial.print ( outputValue,1 );
        //Serial.print("     ");
        Serial.print( cnt1, 1 );
        Serial.println(" MHz");        
        left = false; 
      }
      if ( right ) { 
        // Serial.print (" Left Turn :");
       // Serial.print ( outputValue,1 );
        
        Serial.print( cnt1, 1);
        Serial.println(" MHz");
        right = false;
      } 
      if( button ) {
        Serial.print ( "Left: ");
        Serial.print ( cnt1 );
        Serial.print ( "Right: ");
        Serial.println ( cnt1  );

        button = false;
      }
      delay ( 50);
      lcd.setCursor(0, 0); 
      lcd.print(cnt1);
      delay(500);
}

ISR (PCINT1_vect) {

// If interrupt is triggered by the button
  if (!digitalRead(A0)) {
    
    button = true;
    }

// Else if interrupt is triggered by encoder signals
  else {
    
    // Read A and B signals
    boolean A_val = digitalRead(A1);
    boolean B_val = digitalRead(A2);
    
    // Record the A and B signals in seperate sequences
    seqA <<= 1;
    seqA |= A_val;
    
    seqB <<= 1;
    seqB |= B_val;
    
    // Mask the MSB four bits
    seqA &= 0b00001111;
    seqB &= 0b00001111;
    
    // Compare the recorded sequence with the expected sequence
    if (seqA == 0b00001001 && seqB == 0b00000011) {
      cnt1 = cnt1 + 0.1;
      if (cnt1 > 108.0){
        cnt1 = 88.1;
      }
      left = true;
      }
     
    if (seqA == 0b00000011 && seqB == 0b00001001) {
      cnt1 = cnt1 - 0.1;
    if (cnt1 < 88.1){
      cnt1 = 108.0;
    }
      right = true;
    }         
    }
}  
