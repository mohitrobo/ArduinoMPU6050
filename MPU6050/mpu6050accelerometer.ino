

    #include<Wire.h>  //I2C library
    int accelResult[3]; 
    float timeStep = 0.02;  //20ms. need a time step value for integration of gyro angle from angle/sec
    float biasAccelX,biasAccelY,biasAccelZ;  //for zero bias
    float rollAccel = 0;
    float pitchAccel = 0;
    void writeTo(byte device, byte toAddress, byte val)
    {
      Wire.beginTransmission(device);  
      Wire.write(toAddress);
      Wire.write(val);
      Wire.endTransmission();
    }
    void readFrom(byte device, byte fromAddress, int num, byte result[]) 
    {
      Wire.beginTransmission(device); 
      Wire.write(fromAddress);  
      Wire.endTransmission();  
      Wire.requestFrom((int)device, num);  
      int i = 0; 
      while(Wire.available()) 
      {
        result[i] = Wire.read(); //result stores 1 byte of data
        i++;
      }
    } 

    //Function for reading the accelerometers
    void getAccelerometerReadings(int accelResult[])   //Accelerometer data is in 2's complement form
    {
      byte buffer[6];
      readFrom(0x68,0x3B,6,buffer);
      accelResult[0] = (((int)buffer[0]) << 8 ) | buffer[1]; // X value
      accelResult[1] = (((int)buffer[2]) << 8 ) | buffer[3]; // Y value
      accelResult[2] = (((int)buffer[4]) << 8 ) | buffer[5]; // Z value
    }
    
    void setup() 
    {
      int totalAccelXValues = 0;
      int totalAccelYValues = 0;
      int totalAccelZValues = 0;
      int i;  
      Wire.begin();  //open I2C communication as master
      Serial.begin(115200);  //open serial communication to PC
      writeTo(0x68,0x6B,0x09); 
      writeTo(0x68,0x1C,0x00); 
      writeTo(0x68,0x1A,0x02); 
      writeTo(0x68,0x19,0x09); 
      delay(100);
      for (i=0; i<50; i++)
      {
        getAccelerometerReadings(accelResult);
        totalAccelXValues += accelResult[0];
        totalAccelYValues += accelResult[1];
        totalAccelZValues += accelResult[2];
      }
      biasAccelX = totalAccelXValues / 50;
      biasAccelY = totalAccelYValues / 50;
      biasAccelZ = (totalAccelZValues / 50) - 256; 
    }
  
  void loop() 
  {
    getAccelerometerReadings(accelResult);
    //calculate the roll in degrees as measured by the accelerometer
    rollAccel = atan2((accelResult[1]-biasAccelY),(accelResult[2]-biasAccelZ)) * 57.3;  //angle calculated using inverse of tan  
    //calculate the pitch in degrees as measured by the accelerometer
    pitchAccel = atan2(-(accelResult[0]-biasAccelX),sqrt((accelResult[1]-biasAccelY)*(accelResult[1]-biasAccelY)+(accelResult[2]-biasAccelZ)*(accelResult[2]-biasAccelZ)))*57.3; 
    //display roll data from accelerometer
    Serial.print("AccelRoll= ");
    Serial.print(rollAccel);
    //display pitch data from accelerometer
    Serial.print(" AccelPitch= ");
    Serial.print(pitchAccel);
    Serial.print("\n");
    delay(100);
  }



  
