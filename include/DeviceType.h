#ifndef __DEVICETYPE_H__
#define __DEVICETYPE_H__

enum eDeviceType
{
	eDeviceType_Encoder = 1,
	eDeviceType_Tachometer,
	eDeviceType_TorqueMotor = 4,
	eDeviceType_VelocityMotor,
	eDeviceType_GPS,
	eDeviceType_GYRO = 8,
	eDeviceType_FT,
	eDeviceType_Sonar = 11,
	eDeviceType_IR,
	eDeviceType_LRF,
	eDeviceType_Camera = 19,
	eDeviceType_ServoMotor,
	eDeviceType_VelocityServo,
};



#endif

