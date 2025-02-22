#include <Arduino.h>
#include <digcomp.h>
#define ISensor 32
double temp =0;
double temp1 = 0;
#define Resolution 4096
//Voltage Source
#define VSourve 3.3
#define LED 2
#define VN 39

float increase = 65536/500;
int t1 =0;
int t2=0;
//Filter

float b[]={0.758546992994776, 0.758546992994776};
float a[]={1.000000000000000, 0.517093985989552};

float lp_in1[2];
float lp_out1[2];

float lp_in2[2];
float lp_out2[2];

dig_comp filterISensor(b,a, lp_in1, lp_out1, 2,2);
dig_comp filterVoltage(b,a, lp_in2, lp_out2, 2,2);

// Max possible PWM value
double pwm =0;
double error=0, KP=200, KD=5, KI=1, lastError=0, sumError=0;
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  adcAttachPin(ISensor);
  adcAttachPin(VN);
  adcStart(VN);
  adcStart(ISensor);

  analogReadResolution(11);       // Default of 12 is not very linear. Recommended to use 10 or 11 depending on needed resolution.
  analogSetAttenuation(ADC_11db);
  pinMode(LED, OUTPUT);
  //pinMode(5, OUTPUT);
  ledcSetup(0, 50000, 16);
  ledcAttachPin(5, 0);
  ledcWrite(0, 65536);
}

void loop() {
    /*t1 = millis();
    temp = analogRead(VN);
    temp1 = analogRead(ISensor);
    Serial.print(filterVoltage.calc_out(temp), 5);
    Serial.print("   ");
    Serial.println(filterISensor.calc_out(temp1),6);
    t2=millis();
    if((t2-t1)<700){
      delay(700-(t2-t1));
    }*/
 /* digitalWrite(5, HIGH);
  Serial.print(analogRead(ISensor));
  Serial.print("   ");
  Serial.print(analogRead(VN));
  Serial.print("|");
  delay(3000);
  digitalWrite(5, LOW);
  Serial.print("  ");
  Serial.print(analogRead(ISensor));
  Serial.print("   ");
  Serial.println(analogRead(VN));
  delay(3000);*/
  // put your main code here, to run repeatedly:
  //Serial.println(getZero(),7);
  //ledcWrite(0, 65536);
  //Serial.println(ReadVoltage(VN), 5);
  ledcWrite(0,0);
  delay(5000);
  for (size_t i = 0; i < 65536; i+=increase)
  { 
    t1 = millis();
    //temp = averageAnalogReading(60,VN);
    //temp1 = analogRead(ISensor);
    Serial.print(i);
    Serial.print(" ");
    Serial.print(getVoltage(), 5);
    /*Serial.print("   ");
    Serial.println(filterISensor.calc_out(temp1),6);*/

    //Serial.print(averageAnalogReading(300,VN), 7);
    Serial.print("   ");
    Serial.println(readISensor(),7);

    ledcWrite(0, i);
    t2=millis();
    if((t2-t1)<100){
      delay(100-(t2-t1));
    }
  }
  Serial.println("Finished");
  /*for (double i = 0; i < 1; i+=0.01)
  { 
    temp=readISensor();
    print(temp, i, pwm);
    while (temp<i)
    {
      temp=readISensor();
      print(temp, i,pwm);
      error = i-temp;
      pwm+=KP*error + KD * (error - lastError)+ KI*sumError;
      if(pwm<0)
        pwm=0;
      sumError+=error;
      lastError=error;
      ledcWrite(0, pwm);
      delay(1);  
    }
      
  }*/
}

float readISensor()
{
  //return filterISensor.calc_out(analogRead(ISensor)); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  //float I =ReadVoltage(ISensor)+0.116284;//*1.072;
  //I+=0.233492+0.016315-0.002122;
  //I*=14.834;
  //I-=24.992;
  //return averageAnalogReading(300, ISensor);
  //return 0.1041*averageAnalogReading(300, ISensor)-15.877;
  return 0.1041*averageAnalogReading(100, ISensor)-15.877;
}

double averageAnalogReading(double samples, int analogPin)
{
  double avg = 0;

  for (size_t i = 0; i < samples; i++)
  {
    /* code */
    avg+=analogRead(analogPin);
  }
  return avg/samples;
  //return lowpassFilter.output();
}

double ReadVoltage(double reading)
{
  if(reading < 1 || reading > 4095) return 0;
  //return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
  return -0.000000000000016 * pow(reading,4) + 0.000000000118171 * pow(reading,3)- 0.000000301211691 * pow(reading,2)+ 0.001109019271794 * reading + 0.034143524634089;
} // Added an improved polynomial, use either, comment out as requi

void print(double t, double i, double p){
    Serial.print("PWM: ");
    Serial.print(p);
    Serial.print("  Sensor: ");
    Serial.print(temp,5);
    Serial.print("  Corriente: ");
    Serial.println(i,4);
}


double getZero(){
  double temp = 0;
  for (size_t i = 0; i < 100; i++)
  {
    /* code */
    temp += ReadVoltage(ISensor);
  }
  return temp/1000;
}
double getVoltage(){
  return 0.0264*averageAnalogReading(300,VN)+2.4251;//  *19.66720169 * (3.3/4096);
}