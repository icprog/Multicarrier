    
#include <machine.h>
#include <alarm.h>
#include <utility/ostream.h>
#include <i2c.h>
#include <gpio.h>
#include <cpu.h>
//#include <machine/cortex_m/emote3_gptm.h>

using namespace EPOS;

OStream cout;

class LSM330
{
private:
    //Addresses
    enum {
        //Slave adresses
        I2C_ACC_ADDR        = 0x1D, //  The 7-bit base slave address for the linear aceleration sensor
        I2C_GYRO_ADDR       = 0x6B, //  The 7-bit base slave address for the angular rate sensor (0x6B)
        
        //Accelerometer registers
        CTRL_REG5_A         = 0x20, //Register to select power mode for the acc
        CTRL_REG7_A         = 0x25, //Register to select general configuration for the acc
        FIFO_CTRL_REG_A     = 0x2E, //Control register for FIFO
        
        //Configurations for the accelerometer (To do: include all possible commands)
        POWER_MODE_100      = 0x67,  //Enable X, Y and Z axis, power on mode and output data rate 100 Hz
        FIFO_ENABLE         = 0x40,  //Enable FIFO setting the FIFO_EN bit on register CTRL_REG7_A
        STREAM_MODE         = 0x40,  //Enable Strean Mode for FIFO on register FIFO_CTRL_REG_A
        
        //Gyroscope registers
        CTRL_REG1_G         = 0x20,  //Register to select power mode for the gyro
        CTRL_REG5_G         = 0x24,  //Register to enable FIFO
        FIFO_CTRL_REG_G     = 0x2E, //Control register for FIFO

        //Configurations for the gyroscope
        POWER_MODE_95       = 0x0F, //Output data rate of 95MHz and Bandwidth of  12.5Hz

        //Data registers
        REG_OUT_X_L         = 0x28, // OUT_X_L_A
        REG_OUT_X_H         = 0x29, // OUT_X_H_A
        REG_OUT_Y_L         = 0x2A, // OUT_Y_L_A
        REG_OUT_Y_H         = 0x2B, // OUT_Y_H_A
        REG_OUT_Z_L         = 0x2C, // OUT_Z_L_A
        REG_OUT_Z_H         = 0x2D,  // OUT_Z_H_A

    };


public:


    LSM330() {
        _i2c =  new I2C(0, 'b', 1, 'b', 0); //master, SDA = PB1, SCL = PB0
    }

    void beginAcc(){
        
        char data[6];
        char config[2];
        char regData[2];
        int regValue;

        /*************** Configurations for the accelerometer ***************/
        //Configure power mode
        config[0] = CTRL_REG5_A;
        config[1] = POWER_MODE_100;
        _i2c->put(I2C_ACC_ADDR, config, 2); 
        eMote3_GPTM::delay(50);
        //Read CTRL_REG5_A register
        _i2c->put(I2C_ACC_ADDR, CTRL_REG5_A);
        eMote3_GPTM::delay(50);
        _i2c->get(I2C_ACC_ADDR, regData,2);
        eMote3_GPTM::delay(50);
        regValue = (int)regData[0];
        //cout << "CTRL_REG5_A: " << regValue << endl;

        //Enable FIFO
        config[0] = CTRL_REG7_A;
        config[1] = FIFO_ENABLE;
        _i2c->put(I2C_ACC_ADDR, config, 2); 
        eMote3_GPTM::delay(50);
        //Read CTRL_REG7_A register
        _i2c->put(I2C_ACC_ADDR, CTRL_REG7_A);
        eMote3_GPTM::delay(50);
        /*
        _i2c->get(I2C_ACC_ADDR, regData,2);
        eMote3_GPTM::delay(50);
        regValue = (int)regData[0];
        cout << "CTRL_REG7_A: " << regValue << endl;
        */

        //Enable FIFO Stream Mode
        config[0] = FIFO_CTRL_REG_A;
        config[1] = STREAM_MODE;
        _i2c->put(I2C_ACC_ADDR, config, 2); 
        eMote3_GPTM::delay(50);
        //Read FIFO_CTRL_REG_A register
        _i2c->put(I2C_ACC_ADDR, FIFO_CTRL_REG_A);
        eMote3_GPTM::delay(50);
        /*
        _i2c->get(I2C_ACC_ADDR, regData,2);
        eMote3_GPTM::delay(50);
        regValue = (int)regData[0];
        cout << "FIFO_CTRL_REG_A: " << regValue << endl;
        */
    }

    void beginGyro(){

        char data[6];
        char config[2];
        char regData[2];
        int regValue;

        /************ Configurations for the gyroscope **************/
        //Configure power mode
        config[0] = CTRL_REG1_G;
        config[1] = POWER_MODE_95;
        _i2c->put(I2C_GYRO_ADDR, config, 2); 
        eMote3_GPTM::delay(50);
        //Read CTRL_REG5_A register
        _i2c->put(I2C_GYRO_ADDR,CTRL_REG1_G);
        eMote3_GPTM::delay(50);
        /*
        _i2c->get(I2C_GYRO_ADDR, regData,2);
        eMote3_GPTM::delay(50);
        regValue = (int)regData[0];
        cout << "CTRL_REG1_G: " << regValue << endl;
        */

        //Enable FIFO
        config[0] = CTRL_REG5_G;
        config[1] = FIFO_ENABLE;
        _i2c->put(I2C_GYRO_ADDR, config, 2); 
        eMote3_GPTM::delay(50);
        //Read CTRL_REG7_A register
        _i2c->put(I2C_GYRO_ADDR, CTRL_REG5_G);
        eMote3_GPTM::delay(50);
        /*
        _i2c->get(I2C_GYRO_ADDR, regData,2);
        eMote3_GPTM::delay(50);
        regValue = (int)regData[0];
        cout << "CTRL_REG5_G : " << regValue << endl;
        */

        //Enable FIFO Stream Mode
        config[0] = FIFO_CTRL_REG_G;
        config[1] = STREAM_MODE;
        _i2c->put(I2C_GYRO_ADDR, config, 2); 
        eMote3_GPTM::delay(50);
        //Read FIFO_CTRL_REG_A register
        _i2c->put(I2C_GYRO_ADDR,  FIFO_CTRL_REG_G);
        eMote3_GPTM::delay(50);
        /*
        _i2c->get(I2C_GYRO_ADDR, regData,2);
        eMote3_GPTM::delay(50);
        regValue = (int)regData[0];
        cout << "FIFO_CTRL_REG_G: " << regValue << endl;
        */
    }


    int getAceleration(float aceleration[]) {

        char dataXLSB[2];
        char dataYLSB[2];
        char dataZLSB[2];
        char dataXMSB[2];
        char dataYMSB[2];
        char dataZMSB[2];

        int ax, ay, az;
        
        /*Reads 16 bits of data, 2 for each axel (the value is expressed as two's complement)*/
        //lsb X axel - reg (0x28)
        _i2c->put(I2C_ACC_ADDR, REG_OUT_X_L);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_ACC_ADDR, dataXLSB);               //i2c->get(I2C_ACC_ADDR, dataXLSB, 2);
        eMote3_GPTM::delay(5);

        //msb X axel - reg(0x29)
        _i2c->put(I2C_ACC_ADDR, REG_OUT_X_H);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_ACC_ADDR, dataXMSB);              //_i2c->get(I2C_ACC_ADDR, dataXMSB, 2);
        eMote3_GPTM::delay(5);

        //lsb Y axel - reg (0x2A)
        _i2c->put(I2C_ACC_ADDR, REG_OUT_Y_L);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_ACC_ADDR, dataYLSB);             //_i2c->get(I2C_ACC_ADDR, dataYLSB, 2);
        eMote3_GPTM::delay(5);
        
        //msb Y axel - reg (0x2B)
        _i2c->put(I2C_ACC_ADDR, REG_OUT_Y_H);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_ACC_ADDR, dataYMSB);             //_i2c->get(I2C_ACC_ADDR, dataYMSB, 2);
        eMote3_GPTM::delay(5);
        
        //lsb Z axel - reg (0x2C)
        _i2c->put(I2C_ACC_ADDR, REG_OUT_Z_L);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_ACC_ADDR, dataZLSB);            //_i2c->get(I2C_ACC_ADDR, dataZLSB, 2);
        eMote3_GPTM::delay(5);
        
        //msb Z axel - reg (0x2D)
        _i2c->put(I2C_ACC_ADDR, REG_OUT_Z_H);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_ACC_ADDR, dataZMSB);            //_i2c->get(I2C_ACC_ADDR, dataZMSB, 2);
        eMote3_GPTM::delay(5);


        ax = ((int)dataXMSB[0] << 8) | (int)dataXLSB[0];
        ay = ((int)dataYMSB[0] << 8) | (int)dataYLSB[0];
        az = ((int)dataZMSB[0] << 8) | (int)dataZLSB[0];

        /*Convert the two's complement value*/
        if(ax > 32767){
        ax-= 65536;
        }
        if(ay > 32767){
        ay -= 65536;
        }
        if(az > 32767){
        az -= 65536;
        }


        /* Data conversion: 
            -Aceleration range +-2g
            -Linear aceleration sensitivity 0.061 mg/digit
        */
        aceleration[0] = (float)ax*0.061;
        aceleration[1] = (float)ay*0.061;
        aceleration[2] = (float)az*0.061;
    }

    void getAngle(float angle[]) {

        char dataXLSB[2];
        char dataYLSB[2];
        char dataZLSB[2];
        char dataXMSB[2];
        char dataYMSB[2];
        char dataZMSB[2];

        int gx, gy, gz;
        
        /*Reads 6 bits of data, 2 for each axel (the value is expressed as two's complement)*/
        //lsb X axel - reg (0x28)
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_X_L);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_GYRO_ADDR, dataXLSB);                  
        eMote3_GPTM::delay(5);

        //msb X axel - reg(0x29)
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_X_H);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_GYRO_ADDR, dataXMSB);                
        eMote3_GPTM::delay(5);

        //lsb Y axel - reg (0x2A)
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_Y_L);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_GYRO_ADDR, dataYLSB);                 
        eMote3_GPTM::delay(5);
        
        //msb Y axel - reg (0x2B)
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_Y_H);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_GYRO_ADDR, dataYMSB);                  
        eMote3_GPTM::delay(5);
        
        //lsb Z axel - reg (0x2C)
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_Z_L);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_GYRO_ADDR, dataZLSB);                 
        eMote3_GPTM::delay(5);
        
        //msb Z axel - reg (0x2D)
        _i2c->put(I2C_GYRO_ADDR, REG_OUT_Z_H);
        eMote3_GPTM::delay(5);
        _i2c->get(I2C_GYRO_ADDR, dataZMSB);               
        eMote3_GPTM::delay(5);

    
        gx = ((int)dataXMSB[0] << 8) | (int)dataXLSB[0];
        gy = ((int)dataYMSB[0] << 8) | (int)dataYLSB[0];
        gz = ((int)dataZMSB[0] << 8) | (int)dataZLSB[0];

        /*Convert the two's complement value*/
        if(gx > 32767){
        gx -= 65536;
        }
        if(gy > 32767){
        gy -= 65536;
        }
        if(gz > 32767){
        gz -= 65536;
        }

         /* Data conversion: 
            -Angular rate range +-250dps
            -Sensitivity 8.75 mdps/digit
        */
        angle[0] = (float)gx*8.75;
        angle[1] = (float)gy*8.75;
        angle[2] = (float)gz*8.75;

    }
		
private:
    I2C * _i2c;
};



int main()
{
    LSM330 si;
    float aceleration[3] = {0,0,0}, angles[3] = {0,0,0};
    si.beginAcc();
    si.beginGyro();

    while(1)
    {
        si.getAceleration(aceleration);
        cout << "Aceleration X = " << aceleration[0]<< " Aceleration Y = "<< aceleration[1] << " Aceleration Z = " << aceleration[2] << endl;     
        
        si.getAngle(angles);
        cout << "Angle X = " << angles[0]<< " Angle Y = "<< angles[1] << " Angle Z = " << angles[2] << endl;
        
        cout << " " << endl;
        eMote3_GPTM::delay(1000000);
    }

    return 0;
}



