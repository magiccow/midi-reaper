#include <SoftwareSerial.h>


SoftwareSerial mySerial(10, 11); // RX, TX

void setOutput(int on){
  if(on){
    digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(3, HIGH);  
    digitalWrite(4, LOW);  
  }else{
    digitalWrite(LED_BUILTIN, LOW);      
    digitalWrite(3, LOW);  
    digitalWrite(4, HIGH);      
  }
}


int state=0;
unsigned char buffer[5];
int buflen=0;

void add(unsigned char ch){
  if(state==0){
    unsigned char op = ch & 0xF0;
    if(op == 0xB0){   
      buffer[buflen++]=ch;
      state=1;
    }else if(op==0x80 || op==0x90 || op==0xC0 || op==0xD0){       
      buffer[buflen++]=ch;
      state=5;
    }
  }else if(state==1){
    if(buflen<3){
      buffer[buflen++]=ch;
      if(buflen==3){
        for(int i=0;i<3;i++){
          Serial.print(buffer[i],HEX);      
          Serial.print(' ');      
        }
        Serial.println();
        if(buffer[0]==0xB0 && buffer[1]==0x2C){
          int on = (buffer[2]==0x45);
          if(on)
            Serial.println("Output on");
          else
            Serial.println("Output off");
          setOutput(on);
        }
        state=0;
        buflen=0;
      }
    }
  }else if(state==5){
    if(buflen<3){
      buffer[buflen++]=ch;
      if(buflen==3){
        for(int i=0;i<3;i++){
          Serial.print(buffer[i],HEX);      
          Serial.print(" ");      
        }
        Serial.println();
        if(buffer[0]==0x90 && buffer[1]==0x5F){
          int on = (buffer[2]==0x7F);
          if(on)
            Serial.println("Note on: Output on");
          else
            Serial.println("Note off: Output off");   
          setOutput(on);
        }
        state=0;
        buflen=0;
      }
    }    
  }else if(ch==0xFA || ch==0xFC){
    if(ch==0xFA){
      Serial.println("Start");
    }else{
      Serial.println("Stop");      
    }
  }
}


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  setOutput(0);  

  // set the data rate for the SoftwareSerial port
  mySerial.begin(31250);
  Serial.println("Running");   
}


void loop() { // run over and over
  if (mySerial.available()) {
    unsigned char c = mySerial.read();
    add(c);
  }
  
}
