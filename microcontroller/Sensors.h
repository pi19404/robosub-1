/********************************************************
 *   File Name: Sensors.h
 *
 *   Description:
 *              header file for the sensor definitions
 *
 *
 *********************************************************/

#ifndef SENSORS_H
#define	SENSORS_H

/*************************************************************************
 Constants
 ************************************************************************/

#define ACL_1_X 0x01
#define ACL_1_Y 0x02
#define ACL_1_Z 0x03

/*************************************************************************
 Macros
 ************************************************************************/


/*************************************************************************
 Enums
 ************************************************************************/

typedef enum
{
    ACL_0,
    GYRO_0,
            
}SENSOR_ID;

/*************************************************************************
 Structure Definitions
 ************************************************************************/

/*************************************************************************
 Variables
 ************************************************************************/

/*************************************************************************
 Function Declarations
 ************************************************************************/
/********************************************************
 *   Function Name: i2c_ACL_Read()
 *
 *   Description: Sets the I2C for reading from the PMOD accelerometer
 *
 *
 *********************************************************/
 void i2c_ACL_Read(void);

 /********************************************************
 *   Function Name: i2c_ACL_Initialize()
 *
 *   Description: Sets the I2C for writing (Initialize) to the PMOD accelerometer
 *
 *
 *********************************************************/
 void i2c_ACL_Initialize(void);


#endif	/* SENSORS_H */

