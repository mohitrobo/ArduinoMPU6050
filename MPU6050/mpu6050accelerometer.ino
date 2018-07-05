

    #include<Wire.h>  
    int accelResult[3]; 
    float timeStep = 0.02; 
    float biasAccelX,biasAccelY,biasAccelZ;  
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
        result[i] = Wire.read(); 
        i++;
      }
    } 

   
    void getAccelerometerReadings(int accelResult[]) 
    {
      byte buffer[6];
      readFrom(0x68,0x3B,6,buffer);
      accelResult[0] = (((int)buffer[0]) << 8 ) | buffer[1]; 
      accelResult[1] = (((int)buffer[2]) << 8 ) | buffer[3]; 
      accelResult[2] = (((int)buffer[4]) << 8 ) | buffer[5]; 
    }
    
    void setup() 
    {
      int totalAccelXValues = 0;
      int totalAccelYValues = 0;
      int totalAccelZValues = 0;
      int i;  
      Wire.begin();  
      Serial.begin(115200);  
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
    rollAccel = atan2((accelResult[1]-biasAccelY),(accelResult[2]-biasAccelZ)) * 57.3;   
    pitchAccel = atan2(-(accelResult[0]-biasAccelX),sqrt((accelResult[1]-biasAccelY)*(accelResult[1]-biasAccelY)+(accelResult[2]-biasAccelZ)*(accelResult[2]-biasAccelZ)))*57.3; 
    Serial.print("AccelRoll= ");
    Serial.print(rollAccel);
    Serial.print(" AccelPitch= ");
    Serial.print(pitchAccel);
    Serial.print("\n");
    delay(100);
  }



  
