volatile byte seqA = 0;
volatile byte seqB = 0;
volatile byte cnt1 = 0;
volatile byte cnt2 = 0;
volatile boolean right = false;
volatile boolean left = false;
volatile boolean button = false;


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
  Serial.println ("Hello world");
  
}

void loop() {

      // MAIN LOOP 
      if ( left ) { 
        Serial.print (" Left Turn :");
        Serial.println ( cnt1 );
        left = false; 
      }
      if ( right ) { 
        Serial.print (" Right Turn :");
        Serial.println ( cnt2 );
        right = false;
      } 
      if( button ) {
        Serial.print ( "Left: "    );
        Serial.print ( cnt1 );
        Serial.print ( "Right: " );
        Serial.println ( cnt2  );

        button = false;
      }
      delay ( 50);
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
      cnt1++;
      left = true;
      }
     
    if (seqA == 0b00000011 && seqB == 0b00001001) {
      cnt2++;
      right = true;
      }
  }
}  
