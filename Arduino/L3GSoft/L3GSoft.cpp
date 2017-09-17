#include <L3GSoft.h>
#define SDA_PORT PORTC
#define SDA_PIN 2
#define SCL_PORT PORTC
#define SCL_PIN 3
const byte sdaPin = 7;  // digital pin 7 wired to 'd' on BlinkM
const byte sclPin = 6;  // digital pin 6 wired to 'c' on BlinkM
#include <SlowSoftWire.h>
extern SlowSoftWire SoftWire;
#include <math.h>


// Defines ////////////////////////////////////////////////////////////////

// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define D20_SA0_HIGH_ADDRESS      0b1101011 // also applies to D20H
#define D20_SA0_LOW_ADDRESS       0b1101010 // also applies to D20H
#define L3G4200D_SA0_HIGH_ADDRESS 0b1101001
#define L3G4200D_SA0_LOW_ADDRESS  0b1101000

#define TEST_REG_ERROR -1

#define D20H_WHO_ID     0xD7
#define D20_WHO_ID      0xD4
#define L3G4200D_WHO_ID 0xD3

// Constructors ////////////////////////////////////////////////////////////////

L3GSoft::L3GSoft(void)
{
  _device = device_auto;

  io_timeout = 0;  // 0 = no timeout
  did_timeout = false;
}

// Public Methods //////////////////////////////////////////////////////////////

// Did a timeout occur in read() since the last call to timeoutOccurred()?
bool L3GSoft::timeoutOccurred()
{
  bool tmp = did_timeout;
  did_timeout = false;
  return tmp;
}

void L3GSoft::setTimeout(unsigned int timeout)
{
  io_timeout = timeout;
}

unsigned int L3GSoft::getTimeout()
{
  return io_timeout;
}

bool L3GSoft::init(deviceType device, sa0State sa0)
{
  int id;
  
  // perform auto-detection unless device type and SA0 state were both specified
  if (device == device_auto || sa0 == sa0_auto)
  {
    // check for L3GD20H, D20 if device is unidentified or was specified to be one of these types
    if (device == device_auto || device == device_D20H || device == device_D20)
    {
      // check SA0 high address unless SA0 was specified to be low
      if (sa0 != sa0_low && (id = testReg(D20_SA0_HIGH_ADDRESS, WHO_AM_I)) != TEST_REG_ERROR)
      {
        // device responds to address 1101011; it's a D20H or D20 with SA0 high     
        sa0 = sa0_high;
        if (device == device_auto)
        {
          // use ID from WHO_AM_I register to determine device type
          device = (id == D20H_WHO_ID) ? device_D20H : device_D20;
        }
      }
      // check SA0 low address unless SA0 was specified to be high
      else if (sa0 != sa0_high && (id = testReg(D20_SA0_LOW_ADDRESS, WHO_AM_I)) != TEST_REG_ERROR)
      {
        // device responds to address 1101010; it's a D20H or D20 with SA0 low      
        sa0 = sa0_low;
        if (device == device_auto)
        {
          // use ID from WHO_AM_I register to determine device type
          device = (id == D20H_WHO_ID) ? device_D20H : device_D20;
        }
      }
    }
    
    // check for L3G4200D if device is still unidentified or was specified to be this type
    if (device == device_auto || device == device_4200D)
    {
      if (sa0 != sa0_low && testReg(L3G4200D_SA0_HIGH_ADDRESS, WHO_AM_I) == L3G4200D_WHO_ID)
      {
        // device responds to address 1101001; it's a 4200D with SA0 high
        device = device_4200D;
        sa0 = sa0_high;
      }
      else if (sa0 != sa0_high && testReg(L3G4200D_SA0_LOW_ADDRESS, WHO_AM_I) == L3G4200D_WHO_ID)
      {
        // device responds to address 1101000; it's a 4200D with SA0 low
        device = device_4200D;
        sa0 = sa0_low;
      }
    }
    
    // make sure device and SA0 were successfully detected; otherwise, indicate failure
    if (device == device_auto || sa0 == sa0_auto)
    {
      return false;
    }
  }
  
  _device = device;

  // set device address
  switch (device)
  {
    case device_D20H:
    case device_D20:
      address = (sa0 == sa0_high) ? D20_SA0_HIGH_ADDRESS : D20_SA0_LOW_ADDRESS;
      break;

    case device_4200D:
      address = (sa0 == sa0_high) ? L3G4200D_SA0_HIGH_ADDRESS : L3G4200D_SA0_LOW_ADDRESS;
      break;
  }
  
  return true;
}

/*
Enables the L3G's gyro. Also:
- Sets gyro full scale (gain) to default power-on value of +/- 250 dps
  (specified as +/- 245 dps for L3GD20H).
- Selects 200 Hz ODR (output data rate). (Exact rate is specified as 189.4 Hz
  for L3GD20H and 190 Hz for L3GD20.)
Note that this function will also reset other settings controlled by
the registers it writes to.
*/
void L3GSoft::enableDefault(void)
{
  if (_device == device_D20H)
  {
    // 0x00 = 0b00000000
    // Low_ODR = 0 (low speed ODR disabled)
    writeReg(LOW_ODR, 0x00);
  }
  
  // 0x00 = 0b00000000
  // FS = 00 (+/- 250 dps full scale)
  writeReg(CTRL_REG4, 0x00);
  
  // 0x6F = 0b01101111
  // DR = 01 (200 Hz ODR); BW = 10 (50 Hz bandwidth); PD = 1 (normal mode); Zen = Yen = Xen = 1 (all axes enabled)
  writeReg(CTRL_REG1, 0x6F);
}

// Writes a gyro register
void L3GSoft::writeReg(byte reg, byte value)
{
  SoftWire.beginTransmission(address);
  SoftWire.write(reg);
  SoftWire.write(value);
  last_status = SoftWire.endTransmission();
}

// Reads a gyro register
byte L3GSoft::readReg(byte reg)
{
  byte value;

  SoftWire.beginTransmission(address);
  SoftWire.write(reg);
  last_status = SoftWire.endTransmission();
  SoftWire.requestFrom(address, (byte)1);
  value = SoftWire.read();
  SoftWire.endTransmission();

  return value;
}

// Reads the 3 gyro channels and stores them in vector g
void L3GSoft::read()
{
  SoftWire.beginTransmission(address);
  // assert the MSB of the address to get the gyro
  // to do slave-transmit subaddress updating.
  SoftWire.write(OUT_X_L | (1 << 7));
  SoftWire.endTransmission();
  SoftWire.requestFrom(address, (byte)6);
  
  unsigned int millis_start = millis();
  while (SoftWire.available() < 6)
  {
    if (io_timeout > 0 && ((unsigned int)millis() - millis_start) > io_timeout)
    {
      did_timeout = true;
      return;
    }
  }

  uint8_t xlg = SoftWire.read();
  uint8_t xhg = SoftWire.read();
  uint8_t ylg = SoftWire.read();
  uint8_t yhg = SoftWire.read();
  uint8_t zlg = SoftWire.read();
  uint8_t zhg = SoftWire.read();

  // combine high and low bytes
  g.x = (int16_t)(xhg << 8 | xlg);
  g.y = (int16_t)(yhg << 8 | ylg);
  g.z = (int16_t)(zhg << 8 | zlg);
}

void L3GSoft::vector_normalize(vector<float> *a)
{
  float mag = sqrt(vector_dot(a,a));
  a->x /= mag;
  a->y /= mag;
  a->z /= mag;
}

// Private Methods //////////////////////////////////////////////////////////////

int L3GSoft::testReg(byte address, regAddr reg)
{
  SoftWire.beginTransmission(address);
  SoftWire.write((byte)reg);
  if (SoftWire.endTransmission() != 0)
  {
    return TEST_REG_ERROR;
  }

  SoftWire.requestFrom(address, (byte)1);
  if (SoftWire.available())
  {
    return SoftWire.read();
  }
  else
  {
    return TEST_REG_ERROR;
  }
}
