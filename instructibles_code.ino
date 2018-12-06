

/*
Liquid flow rate sensor -DIYhacking.com Arvind Sanjeev

Measure the liquid/water flow rate using this code. 
Connect Vcc and Gnd of sensor to arduino, and the 
signal line to arduino digital pin 2.
 
 */


#include <Time.h>

#define TIME_MSG_LEN 11 // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER 'T' // Header tag for serial time sync message
#define TIME_REQUEST 7 // ASCII bell character requests a time sync message 

// T1262347200 //noon Jan 1 2010


#define sample_time 1000
byte statusLed    = 13;

byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;
byte sensorInterrupt2 = 1;  // 1 = digital pin 3
byte sensorPin2       = 3;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;

float calibrationFactor2 = 4.5;
volatile byte pulseCount2;  
float flowRate2;
unsigned int flowMilliLitres2;
unsigned long totalMilliLitres2;
unsigned long oldTime2;
unsigned long SystemTime;

float totalflow;

float pf1;
float pf2;

void setup()
{
  
  // Initialize a serial connection for reporting values to the host
  Serial.begin(115200);
   
  // Set up the status LED line as an output
  pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, HIGH);  // We have an active-low LED attached
  
  pinMode(sensorPin, INPUT);
   pinMode(sensorPin2, INPUT);
 // digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  pulseCount2        = 0;
  flowRate2          = 0.0;
  flowMilliLitres2   = 0;
  totalMilliLitres2  = 0;
  oldTime2           = 0;

  totalflow = 0.0;
  pf1 = 0;
  pf2 = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  attachInterrupt(sensorInterrupt2, pulseCounter2, FALLING);
 
}

/**
 * Main program loop
 */
void loop()
{
   
   if((millis() - oldTime) > sample_time)    // Only process counters once per second
  { 

    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    noInterrupts();
   // detachInterrupt(sensorInterrupt);
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((float(sample_time) / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    flowRate2 = ((float(sample_time) / (millis() - oldTime)) * pulseCount2) / calibrationFactor2;
    oldTime = millis();
    totalflow = flowRate + flowRate2;
    pf1 = (flowRate/totalflow)*100 ;
    pf2 = (flowRate2/totalflow)*100;

    
    unsigned int frac;
    
    // Print the flow rate for this second in litres / minute

    // Serial.print(millis());
    Serial.print(","); 
    Serial.print(flowRate);  // Print the integer part of the variable
    //Serial.print(".");             // Print the decimal point
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    //frac = (flowRate - int(flowRate)) * 10;
    //Serial.print(frac, DEC) ;      // Print the fractional part of the variable
    // Print the number of litres flowed in this second
    Serial.print(",");             // Output separator
    //SECOND SENSOR===================================================================================
    Serial.print(flowRate2);  // Print the integer part of the variable
    // Serial.print(".");             // Print the decimal point
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    //frac = (flowRate2 - int(flowRate2)) * 10;
    // Serial.println(frac, DEC) ;      // Print the fractional part of the variable with a newline character
    // Reset the pulse counter so we can start incrementing again
    Serial.print(",");
    Serial.print(pf1);
    Serial.print(",");
    Serial.println(pf2);
    Serial.print(" ");
    //Serial.print(flowRate);
    //Serial.print(flowRate2);
    
    
    pulseCount = 0;
    pulseCount2 = 0;
    // Enable the interrupt again now that we've finished sending output
    //attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    interrupts();
  }
}

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
}
void pulseCounter2()
{
  // Increment the pulse counter
  pulseCount2++;
}
