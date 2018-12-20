int input_table[16] = { };
const uint8_t  left_motor_pin1=9,left_motor_pin2=10,left_motor_pin3=12,left_motor_pin4=11,right_motor_pin1=3,right_motor_pin2=2,right_motor_pin3=4,right_motor_pin4=5;
int setup1=48;
const uint8_t pwmpin1 = 8,pwmpin2 = 13,pwmpin3 = 7,pwmpin4 = 6;
unsigned long lastTime;
double Input, Output, Setpoint=0;
double errSum, lastErr;
double kp=39, ki=0.2, kd=0;
int SampleTime=100;
 
double ITerm,lastInput;
struct PWMVALUE
{
  int Left = 0;
  int Right = 0;
 int base = 100;//速度基量
  int inc = 0; //速度增量
};
PWMVALUE PWM;
int x = 0,x1=0,x2=0,x3=0,x4=0,count=0,b=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
    pinMode(A1,INPUT);
pinMode(A0,INPUT);
pinMode(A2,INPUT);
pinMode(A3,INPUT);
pinMode(A4,INPUT);
pinMode(A5,INPUT);
pinMode(A6,INPUT);
pinMode(A7,INPUT);
pinMode(A8,INPUT);
pinMode(A9,INPUT);
pinMode(A10,INPUT);
pinMode(A11,INPUT);
pinMode(A12,INPUT);
pinMode(A13,INPUT);
pinMode(A14,INPUT);
pinMode(A15,INPUT);
for(int i=0;i<14;i++)
{
  pinMode(i,OUTPUT);
}
digitalWrite(22,HIGH);
digitalWrite(40,HIGH);
}

void loop() {
  int a=0;
   if(b==8)
  {
    analogWrite(8,63);
    analogWrite(13,63);
    analogWrite(6,63);
    analogWrite(7,63);
    delay(600);
  analogWrite(8,0);//you1
    analogWrite(13,0);//zuo2
    analogWrite(6,0);//you2
    analogWrite(7,0);//zuo1
    delay(10000);
  }
  input_table[0]=digitalRead(A1);
  input_table[1]=digitalRead(A2);
  input_table[2]=digitalRead(A3);
  input_table[3]=digitalRead(A4);
  input_table[4]=digitalRead(A5);
  input_table[5]=digitalRead(A0);
  input_table[6]=digitalRead(A7);
  input_table[7]=digitalRead(A6);
  input_table[8]=digitalRead(A8);
  input_table[9]=digitalRead(A9);
  input_table[10]=digitalRead(A10);
  input_table[11]=digitalRead(A11);
  input_table[12]=digitalRead(A12);
  input_table[13]=digitalRead(A13);
  input_table[14]=digitalRead(A14);
  input_table[15]=digitalRead(A15);
    //找黑线
    for (int i=0;i<15;i++)
    {
      if ( input_table[i]==1)
      { 
        //找到黑线x即误差=>用于比例计算
        x1= i - 8;
        count++;
      }
      //break;
    }
   /*  if(count>=3&&count<=5)
    {
      for(int i=0;i<500;i++)
      {
   for(int k=15;k>=0;k--)
    {
     if ( input_table[k]==1)
      {
        //找到黑线x即误差=>用于比例计算
        x2= k - 8;
      }
      //break;
    }
    }
 
    }*/
    if(count>=8)
    {
    b++;

    delay(100);
    }
    
// x3=abs(x1);
  //  x4=abs(x2);
    //if(x3<x4)
      //x=x2;
      //else
      x=x1;
      //if(x_last==1&&)
  if(x>2||x<-2)
   {
      //PWM.base=180;
      Input=x;
      Compute();
      PWM.inc=Output;
        }
   if(x>=-2&&x<=2)
    {
      //PWM.base=100;
       Input=x;
       Compute();
       PWM.inc=Output;
      }
    PWM.Right = PWM.base + PWM.inc;
    PWM.Left = PWM.base - PWM.inc;
     if(PWM.Left>255)
        PWM.Left=255;
    if(PWM.Right>255)
        PWM.Right=255;
         if(PWM.Left<-255)
        PWM.Left=-255;
    if(PWM.Right<-255)
        PWM.Right=-255;
    if(PWM.Left<0)
    {
         motor2(PWM.Left,left_motor_pin1,left_motor_pin2,pwmpin1);
  motor2(PWM.Left,left_motor_pin3,left_motor_pin4,pwmpin2);
    }
   else
         {
          motor1(PWM.Left,left_motor_pin1,left_motor_pin2,pwmpin1);
         motor1(PWM.Left,left_motor_pin3,left_motor_pin4,pwmpin2);
         }
    if(PWM.Right<0)
       {
        motor2(PWM.Right,right_motor_pin1,right_motor_pin2,pwmpin3);
        motor2(PWM.Right,right_motor_pin3,right_motor_pin4,pwmpin4);
       }
  else
        {
          motor1(PWM.Right,right_motor_pin1,right_motor_pin2,pwmpin3);
        motor1(PWM.Right,right_motor_pin3,right_motor_pin4,pwmpin4);
        }
    Serial.print("X:");
    Serial.print(x);
    Serial.print("left_motor:");
    Serial.print(PWM.Left);
    Serial.print("right_motor:");
    Serial.print(PWM.Right);  
    Serial.print("PWM.INC=");
  Serial.println(PWM.inc);
  count=0;
    }
/*working variables*/
void Compute()
{
   /*How long since we last calculated*/
   unsigned long now = millis();
   if(now%10==0)
    ITerm=0;
    if(now%500==0)
    lastInput=Input;
   double timeChange = (double)(now - lastTime);
   double error =Input;
   ITerm+=(ki*error);
   double dInput  = (Input - lastInput);
   /*Compute PID Output*/
   Output = kp * error + ITerm - kd * dInput;
   /*Remember some variables for next time*/
   lastInput = Input;
   lastTime = now;
   SampleTime-=10;
   if(SampleTime<=0)
    ITerm=0;
}
void motor1(int _speed,int in1,int in2,int PWMpin)
{
  digitalWrite(in1,0);
  digitalWrite(in2,1);
  analogWrite(PWMpin,_speed);
}
void motor2(int _speed,int in1,int in2,int PWMpin)
{
  digitalWrite(in1,1);
  digitalWrite(in2,0);
  analogWrite(PWMpin,_speed);
}
 
