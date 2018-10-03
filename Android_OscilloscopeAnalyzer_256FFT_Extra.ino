/*
Audio Range Oscilloscope and Spectrum Analyzer with Bluetooth to Android Phone
Hany Fame
*/

#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the FFT library

//int y[380];
int y;
int sensor[191]; //Main Array
int j=0;
int buttonState = 0;
int state = 0;
const int analogInPin = A0;
const int buttonPin = 4;



//////////////////Setup Function/////////////////////

void setup() 
{  
  pinMode(buttonPin, INPUT);
  pinMode(12, OUTPUT);
  Serial.begin(115200); // use the serial port
}



///////////////Main Loop Function/////////////////////

void loop() {
  
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH)
  {
    state=!state;
  }
  
  
  /////////////Run Spectrum Analyzer if Pin 4 = high/////////////////////////
  if (state == 1 && buttonState == LOW) 
  {
    digitalWrite(12, HIGH);
    for (int i = 0 ; i < 512 ; i += 2) // save 256 samples
    {
      int k = analogRead(analogInPin);; // form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
    }
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    //sei();
    for (j=0; j<=127; ++j)
    {
      sensor[j]=map(fft_log_out[j], 0, 200, 400, 250);
      //Serial.print(y);
      //Serial.print("z");
    }
        for (j=0; j<=127; ++j)
    {
      Serial.print(sensor[j]);
      Serial.print("z"); //ASCII Character delimiter between each byte
    }
    
  }
  
  
  ////////////////Run Oscilloscope if Pin 4 = low///////////////////////// 
  else if (state == 0 && buttonState == LOW) 
  {
    digitalWrite(12, LOW);
    for (int i=0; i <191; i++)
    {
    sensor[i] = analogRead(analogInPin);
    //y=map(sensor, 0, 1023, 450, 150);
    //Serial.print(y);
    //Serial.print("z");
    }
      for (int i=0; i <191; i++)
    {
      y=map(sensor[i], 0, 1023, 350, 50);
      Serial.print(y);
      Serial.print("z");
    }
    
  }

    ////////////Delimiter for Android app to read data array from bluetooth module, before reading new data array
    Serial.println("A"); 
}
    
