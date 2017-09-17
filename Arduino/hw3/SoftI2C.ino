/*

MinIMU-9-Arduino-AHRS
Pololu MinIMU-9 + Arduino AHRS (Attitude and Heading Reference System)

Copyright (c) 2011-2016 Pololu Corporation.
http://www.pololu.com/

MinIMU-9-Arduino-AHRS is based on sf9domahrs by Doug Weibel and Jose Julio:
http://code.google.com/p/sf9domahrs/

sf9domahrs is based on ArduIMU v1.5 by Jordi Munoz and William Premerlani, Jose
Julio and Doug Weibel:
http://code.google.com/p/ardu-imu/

MinIMU-9-Arduino-AHRS is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your option)
any later version.

MinIMU-9-Arduino-AHRS is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License along
with MinIMU-9-Arduino-AHRS. If not, see <http://www.gnu.org/licenses/>.

*/

#ifdef IMU_V5

#include <LSM6.h>
#include <LIS3MDL.h>

LSM6 gyro_acc;
LIS3MDL mag;

#else // older IMUs through v4

#define SDA_PORT PORTC
#define SDA_PIN 2
#define SCL_PORT PORTC
#define SCL_PIN 3
const byte sdaPin = 7;  // digital pin 7 wired to 'd' on BlinkM
const byte sclPin = 6;  // digital pin 6 wired to 'c' on BlinkM
#include <SlowSoftWire.h>
SlowSoftWire SoftWire = SlowSoftWire(sdaPin,sclPin,true);
#include <L3GSoft.h>
#include <LSM303Soft.h>


L3GSoft soft_gyro;
LSM303Soft soft_compass;

#endif


void SoftI2C_Init()
{
  SoftWire.begin();
}

void SoftGyro_Init()
{
#ifdef IMU_V5
  // Accel_Init() should have already called gyro_acc.init() and enableDefault()
  gyro_acc.writeReg(LSM6::CTRL2_G, 0x4C); // 104 Hz, 2000 dps full scale
#else
  soft_gyro.init();
  soft_gyro.enableDefault();
  soft_gyro.writeReg(L3GSoft::CTRL_REG4, 0x20); // 2000 dps full scale
  soft_gyro.writeReg(L3GSoft::CTRL_REG1, 0x0F); // normal power mode, all axes enabled, 100 Hz
#endif
}

void SoftRead_Gyro()
{
#ifdef IMU_V5
  gyro_acc.readGyro();

  SAN[0] = gyro_acc.g.x;
  SAN[1] = gyro_acc.g.y;
  SAN[2] = gyro_acc.g.z;
#else
  soft_gyro.read();

  SAN[0] = soft_gyro.g.x;
  SAN[1] = soft_gyro.g.y;
  SAN[2] = soft_gyro.g.z;
#endif

  soft_gyro_x = SENSOR_SIGN[0] * (SAN[0] - AN_OFFSET[0]);
  soft_gyro_y = SENSOR_SIGN[1] * (SAN[1] - AN_OFFSET[1]);
  soft_gyro_z = SENSOR_SIGN[2] * (SAN[2] - AN_OFFSET[2]);
}

void SoftAccel_Init()
{
#ifdef IMU_V5
  gyro_acc.init();
  gyro_acc.enableDefault();
  gyro_acc.writeReg(LSM6::CTRL1_XL, 0x3C); // 52 Hz, 8 g full scale
#else
  soft_compass.init();
  soft_compass.enableDefault();
  switch (compass.getDeviceType())
  {
    case LSM303Soft::device_D:
      soft_compass.writeReg(LSM303Soft::CTRL2, 0x18); // 8 g full scale: AFS = 011
      break;
    case LSM303Soft::device_DLHC:
      soft_compass.writeReg(LSM303Soft::CTRL_REG4_A, 0x28); // 8 g full scale: FS = 10; high resolution output mode
      break;
    default: // DLM, DLH
      soft_compass.writeReg(LSM303Soft::CTRL_REG4_A, 0x30); // 8 g full scale: FS = 11
  }
#endif
}

// Reads x,y and z accelerometer registers
void SoftRead_Accel()
{
#ifdef IMU_V5
  gyro_acc.readAcc();

  AN[3] = gyro_acc.a.x >> 4; // shift left 4 bits to use 12-bit representation (1 g = 256)
  AN[4] = gyro_acc.a.y >> 4;
  AN[5] = gyro_acc.a.z >> 4;
#else
  soft_compass.readAcc();

  SAN[3] = soft_compass.a.x >> 4; // shift left 4 bits to use 12-bit representation (1 g = 256)
  SAN[4] = soft_compass.a.y >> 4;
  SAN[5] = soft_compass.a.z >> 4;
#endif
  soft_accel_x = SENSOR_SIGN[3] * (SAN[3] - AN_OFFSET[3]);
  soft_accel_y = SENSOR_SIGN[4] * (SAN[4] - AN_OFFSET[4]);
  soft_accel_z = SENSOR_SIGN[5] * (SAN[5] - AN_OFFSET[5]);
}

void SoftCompass_Init()
{
#ifdef IMU_V5
  mag.init();
  mag.enableDefault();
#else
  // LSM303: doesn't need to do anything because Accel_Init() should have already called compass.enableDefault()
#endif
}

void SoftRead_Compass()
{
#ifdef IMU_V5
  mag.read();

  magnetom_x = SENSOR_SIGN[6] * mag.m.x;
  magnetom_y = SENSOR_SIGN[7] * mag.m.y;
  magnetom_z = SENSOR_SIGN[8] * mag.m.z;
#else
  soft_compass.readMag();

  soft_magnetom_x = SENSOR_SIGN[6] * soft_compass.m.x;
  soft_magnetom_y = SENSOR_SIGN[7] * soft_compass.m.y;
  soft_magnetom_z = SENSOR_SIGN[8] * soft_compass.m.z;
#endif
}

