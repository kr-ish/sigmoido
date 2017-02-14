// Werkstatt
// White: Semitone
// Blue: Octave
// Yellow: Param1
// Green: Param2

const int vcaPin = 11;
const int vcfPin = 10;
const int vcoLinPin = 9;
const int vcoPin = 6;
const int lfoPin = 5;

// Playing Notes
int OCTAVE = 0; // (0 - 2) * 60
int FIFTH = 0; // (0 - 1) * 35
int SEMI = 0; // (0 - 14)* 5
int NOTE = 0;
int VCA_LEVEL;
int ATTACK;
int DECAY;
int desiredAttack = 0;
int desiredDecay = 0;

int PAR1_VAL;
int PAR1_OCT;
int PAR1_FIFTH;
int PAR2_VAL;
int PAR2_OCT;
int PAR2_FIFTH;

// Ohmmeter
const int semiString = A0; 
const int octaveString = A1;  
const int paramString1 = A2;  
const int paramString2 = A3;
const int attPotPin = A4;
const int decPotPin = A5;

// Vars
int rawSemi = 0;
int rawOct = 0;
int rawP1 = 0;
int rawP2 = 0;

int Vin = 5;

float VoutSemi = 0;
float VoutOct = 0;
float VoutP1 = 0;
float VoutP2 = 0;

float ref_resSemi = 330000;
float ref_resRest = 100000;

float measured_resSemi = 0;
float measured_resOct = 0;
float measured_resP1 = 0;
float measured_resP2 = 0;

float bufferSemi = 0;
float bufferOct = 0;
float bufferP1 = 0;
float bufferP2 = 0;

void noteOn(int note){
//  Serial.print("Note On: ");
//  Serial.println(NOTE);
  VCA_LEVEL = 255;
  DECAY = desiredDecay;
}

void noteOff(){
//  Serial.println("Note Off");
  VCA_LEVEL = 0;
  ATTACK = desiredAttack;
}

void attackNote(int rate){
//  Serial.println("Attack Note");
  if(VCA_LEVEL < 255){
    VCA_LEVEL += (255 - rate) + 1;
    delay(2);
  }
  if(VCA_LEVEL >= 255){
    ATTACK = 0;
  }
}  

void decayNote(int rate){
//  Serial.println("Decay Note");
  if(VCA_LEVEL > 0){
    VCA_LEVEL -= (255 - rate) + 1;
    delay(2);
  }
  if(VCA_LEVEL <= 0){
    DECAY = 0;
  }
}  

void setup() {
  // put your setup code here, to run once:
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  ATTACK = desiredAttack;
  DECAY = desiredDecay;
}

void loop() {
  // put your main code here, to run repeatedly:
  
   desiredAttack = map(analogRead(attPotPin), 0, 1023, 255, 0);
   desiredDecay = map(analogRead(decPotPin), 0, 1023, 255, 0);

// semitone resistances (highest to lowest per string): 
// Highest: 0/100, 10k, 20k, 30k, 40k
// Middle: 100k, 110k, 120k, 130k, 140k
// Lowest: 200k, 210k, 220k, 230k, 240k

// Lowest - Highest
// 240k, 230k, 220k, 210k, 200k, 140k, 130k, 120k, 110k, 100k, 40k, 30k, 20k, 20k, 0

// param resistances: 100, 22k, 44k, 66k, 88k

// **** SEMI ****
   rawSemi = analogRead(semiString);
   bufferSemi = rawSemi * Vin;
   VoutSemi = (bufferSemi)/1024.0;
   bufferSemi = (Vin/VoutSemi) - 1;
   measured_resSemi = ref_resSemi * bufferSemi;
   
//   Serial.print("Res: ");
//   Serial.println(measured_res);

  if(measured_resSemi < 300000.0) {  // if playing
    
    if(measured_resSemi > 235000.0) {  // if 240k
      SEMI = 1;
    }
    else if(measured_resSemi > 225000.0){  // if 230k
      SEMI = 2;
    }
    else if(measured_resSemi > 215000.0){  // if 220k
      SEMI = 3;
    }
    else if(measured_resSemi > 205000.0){  // if 210k
      SEMI = 4;
    }
    else if(measured_resSemi > 180000.0){  // if 200k
      SEMI = 5;
    }
    else if(measured_resSemi > 130000.0){  // if 140k
      SEMI = 6;
    }        
    else if(measured_resSemi > 110000.0){  // if 130k
      SEMI = 7;
    }        
    else if(measured_resSemi > 105000.0){  // if 120k
      SEMI = 8;
    }        
    else if(measured_resSemi > 93000.0){  // if 110k
      SEMI = 9;
    }        
    else if(measured_resSemi > 75000.0){  // if 100k
      SEMI = 10;
    }        
    else if(measured_resSemi > 34000.0){  // if 40k
      SEMI = 11;
    }        
    else if(measured_resSemi > 25000.0){  // if 30k
      SEMI = 12;
    }        
    else if(measured_resSemi > 15000.0){  // if 20k
      SEMI = 13;
    }        
    else if(measured_resSemi > 1000.0){  // if 10k
      SEMI = 14;
    }        
    else if(measured_resSemi > 0.0){  // if 0/100
      SEMI = 15;
    }        
    
//   NOTE = OCTAVE*60 + SEMI*5;
//   analogWrite(vcoPin, NOTE);    
   
   if(ATTACK > 0){
    attackNote(ATTACK);
   }
   else{
    noteOn(NOTE);
   }
  }

  else{  // if not playing

   if(DECAY > 0){
    decayNote(DECAY);
   }
   else{
     noteOff();  
   }
  }
// **** END SEMI ****

// **** OCT ****
   rawOct = analogRead(octaveString);
   bufferOct = rawOct * Vin;
   VoutOct = (bufferOct)/1024.0;
   bufferOct = (Vin/VoutOct) - 1;
   measured_resOct = ref_resRest * bufferOct;
   
//   Serial.print("Res: ");
//   Serial.println(measured_resOct);

  if(measured_resOct < 100000.0) {  // if playing
    
    if(measured_resOct > 76000.0) {  // if 88k
      OCTAVE = 0;
      FIFTH = 0;
    }
    
    else if(measured_resOct > 54000.0){  // if 66k
      Serial.println("oct 0 fifth 1");
      OCTAVE = 0;
      FIFTH = 1;
    }
    
    else if(measured_resOct > 32000.0){  // if 44k
      OCTAVE = 1;
      FIFTH = 0;
    }
    else if(measured_resOct > 1000.0){  // if 22k
      OCTAVE = 1;
      FIFTH = 1;
    }
    else if(measured_resOct > 10.0){  // if 100
      OCTAVE = 2;
      FIFTH = 0;
    }    
//   NOTE = OCTAVE*60 + FIFTH*35 + SEMI*5;
//   analogWrite(vcoPin, NOTE);    
   
//   if(ATTACK > 0){
//    attackNote(ATTACK);
//   }
//   else{
//    noteOn(NOTE);
//   }
  }

  else{  // if not playing

  }
// **** END OCT ****
//
// **** PARAM 1 ****
   rawP1 = analogRead(paramString1);
   bufferP1 = rawP1 * Vin;
   VoutP1 = (bufferP1)/1024.0;
   bufferP1 = (Vin/VoutP1) - 1;
   measured_resP1 = ref_resRest * bufferP1;
   
//   Serial.print("Res: ");
//   Serial.println(measured_res);

  if(measured_resP1 < 100000.0) {  // if playing
    
    if(measured_resP1 > 76000.0) {  // if 88k
      PAR1_OCT = 0;
      PAR1_FIFTH = 0;
    }    
    else if(measured_resP1 > 54000.0){  // if 66k
      PAR1_OCT = 0;
      PAR1_FIFTH = 1;
    }    
    else if(measured_resP1 > 32000.0){  // if 44k
      PAR1_OCT = 1;
      PAR1_FIFTH = 0;
    }
    else if(measured_resP1 > 1000.0){  // if 22k
      PAR1_OCT = 1;
      PAR1_FIFTH = 1;
    }
    else if(measured_resP1 > 10.0){  // if 100
      PAR1_OCT = 2;
      PAR1_FIFTH = 0;
    }    
   PAR1_VAL = PAR1_OCT*60 + PAR1_FIFTH*35;    
   
//   if(ATTACK > 0){
//    attackNote(ATTACK);
//   }
//   else{
//    noteOn(NOTE);
//   }
  }

  else{  // if not playing

  }
// **** END PARAM1 ****
//
//// **** PARAM 2 ****
   rawP2 = analogRead(paramString2);
   bufferP2 = rawP2 * Vin;
   VoutP2 = (bufferP2)/1024.0;
   bufferP2 = (Vin/VoutP2) - 1;
   measured_resP2 = ref_resRest * bufferP2;
   
//   Serial.print("Res: ");
//   Serial.println(measured_res);

  if(measured_resP2 < 100000.0) {  // if playing
    
    if(measured_resP2 > 76000.0) {  // if 88k
      PAR2_OCT = 0;
      PAR2_FIFTH = 0;
    }
    else if(measured_resP2 > 54000.0){  // if 66k
      PAR2_OCT = 0;
      PAR2_FIFTH = 1;
    }    
    else if(measured_resP2 > 32000.0){  // if 44k
      PAR2_OCT = 1;
      PAR2_FIFTH = 0;
    }
    else if(measured_resP2 > 1000.0){  // if 22k
      PAR2_OCT = 1;
      PAR2_FIFTH = 1;
    }
    else if(measured_resP2 > 10.0){  // if 100
      PAR2_OCT = 2;
      PAR2_FIFTH = 0;      
    }    
   PAR2_VAL = PAR2_OCT*60 + PAR2_FIFTH*35;
   
//   if(ATTACK > 0){
//    attackNote(ATTACK);
//   }
//   else{
//    noteOn(NOTE);
//   }
  }

  else{  // if not playing

  }

//// **** END PARAM2 ****

  if(VCA_LEVEL > 255){
    VCA_LEVEL = 255;
  }
  else if(VCA_LEVEL < 0){
    VCA_LEVEL = 0;
  }

//  if(VCO_VAL > 255){
//   VCO_VAL = 255;
//  }
//  else if(VCA_LEVEL < 0){
//    VCO_VAL = 0;
//  }
//  Serial.print("VCA_LEVEL = ");
//  Serial.println(VCA_LEVEL);
  analogWrite(vcaPin, VCA_LEVEL);
  NOTE = OCTAVE*60 + FIFTH*35 + SEMI*5;

  Serial.print("RES OCT: ");
  Serial.print(measured_resOct);
  Serial.print(" ,RES P1: ");
  Serial.print(measured_resP1);
  Serial.print(" ,RES P2: ");
  Serial.print(measured_resP2);
  Serial.print(" ,OCT: ");
  Serial.print(OCTAVE);
  Serial.print(" ,FIFTH: ");
  Serial.print(FIFTH);
  Serial.print(" ,SEMI: ");
  Serial.print(SEMI);
  Serial.print(" ,PAR1_OCT: ");
  Serial.print(PAR1_OCT);
  Serial.print(" ,PAR1_FIFTH: ");
  Serial.print(PAR1_FIFTH);
  Serial.print(" ,PAR1_VAL: ");
  Serial.println(PAR1_VAL);
//  delay(10);
  analogWrite(vcoPin, NOTE);
  analogWrite(lfoPin, PAR1_VAL);
  analogWrite(vcfPin, PAR2_VAL);
}

