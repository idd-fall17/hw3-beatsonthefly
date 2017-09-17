



#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi

int gyro_x;
int gyro_y;
int gyro_z;
int accel_x;
int accel_y;
int accel_z;
int magnetom_x;
int magnetom_y;
int magnetom_z;
float c_magnetom_x;
float c_magnetom_y;
float c_magnetom_z;
int soft_gyro_x;
int soft_gyro_y;
int soft_gyro_z;
int soft_accel_x;
int soft_accel_y;
int soft_accel_z;
int soft_magnetom_x;
int soft_magnetom_y;
int soft_magnetom_z;
float soft_c_magnetom_x;
float soft_c_magnetom_y;
float soft_c_magnetom_z;
float MAG_Heading;
int SENSOR_SIGN[9] = {1,1,1,-1,-1,-1,1,1,1}; //Correct directions x,y,z - gyro, accelerometer, magnetometer
int AN_OFFSET[6]={0,0,0,0,0,0}; //Array that stores the Offset of the sensors

int AN[6]; //array that stores the gyro and accelerometer data
int SAN[6]; //array that stores the gyro and accelerometer data
int pA[6];
int pSA[6];

long long da;
long long dsa;

int leftFlag,rightFlag;

void setup() {
  Serial.begin(115200);
  
  I2C_Init();
  SoftI2C_Init();

  delay(1500);
  
  Accel_Init();
  Gyro_Init();
  SoftAccel_Init();
  SoftGyro_Init();

  delay(20);

  leftFlag=0;
  rightFlag =0;

  da=0;
  dsa=0;
  
}

void loop() {
  // put your main code here, to run repeatedly:

    Read_Gyro();   // This read gyro data
    Read_Accel();     // Read I2C accelerometer
    SoftRead_Gyro();   // This read gyro data
    SoftRead_Accel();     // Read I2C accelerometer

    if(abs(AN[0])>2000 && AN[0]>-AN[2] && leftFlag==0){
      //Serial.println('q');
      leftFlag=1;
    }else if(abs(-AN[2])>2000 && leftFlag==0){
      //Serial.println('w');
      leftFlag=2;
    }else if(leftFlag==1 && da>14000){
      Serial.println('q');
      leftFlag=3;
    }else if(leftFlag==2 && da>14000){
      Serial.println('w');
      leftFlag=3;
    }else if(-AN[2]<200 && AN[0] <200){
      leftFlag=0;
    }

    if(SAN[0]>2000 && SAN[0]>SAN[2] && rightFlag==0){
      //Serial.println('e');
      rightFlag=1;
    }else if(SAN[2]>2000 && rightFlag==0){
      //Serial.println('r');
      rightFlag=2;
    }else if(rightFlag==1 && dsa>14000){
      Serial.println('e');
      rightFlag=3;
    }else if(rightFlag==2 && dsa>14000){
      Serial.println('r');
      rightFlag=3;
    }else if(SAN[2]<200 && SAN[0] <200){
      rightFlag=0;
    }
    //Serial.println((double)da/1000.0);
    da = (long)(pA[3] - AN[3])*(pA[3] - AN[3])+(long)(pA[4] - AN[4])*(pA[4] - AN[4])+(long)(pA[5] - AN[5])*(pA[5] - AN[5]);
    dsa = (long)(pSA[3] - SAN[3])*(pSA[3] - SAN[3])+(long)(pSA[4] - SAN[4])*(pSA[4] - SAN[4])+(long)(pSA[5] - SAN[5])*(pSA[5] - SAN[5]);
    pSA[3] = SAN[3];
    pSA[4] = SAN[4];
    pSA[5] = SAN[5];
    pA[3] = AN[3];
    pA[4] = AN[4];
    pA[5] = AN[5];
    delay(10);
    /*Serial.print(AN[0]);  //(int)read_adc(0)
    Serial.print("\t");
    Serial.print(AN[2]);
    Serial.print("\t");
    Serial.print(SAN[0]);
    Serial.print("\t");
    Serial.print(SAN[2]);
    Serial.println();*/

}
