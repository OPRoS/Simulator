#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define SIMULATOR_CONTROL_PROTOCOL ((unsigned short)0x01)

#define KP_REQ_CONNECT ((unsigned short)(SIMULATOR_CONTROL_PROTOCOL+0x01))
/*
no body
*/

#define KP_ANS_CONNECT ((unsigned short)(SIMULATOR_CONTROL_PROTOCOL+0x02))
/*
no body
*/

#define KP_REQ_START_SIMULATION ((unsigned short)(SIMULATOR_CONTROL_PROTOCOL+0x03))
/*
no body
*/

#define KP_ACK_START_SIMULATION ((unsigned short)(SIMULATOR_CONTROL_PROTOCOL+0x04))
/*
no body
*/

#define KP_REQ_STOP_SIMULATION ((unsigned short)(SIMULATOR_CONTROL_PROTOCOL+0x05))
/*
no body
*/

#define KP_ACK_STOP_SIMULATION ((unsigned short)(SIMULATOR_CONTROL_PROTOCOL+0x06))
/*
no body
*/

#define KP_REQ_LOAD_MODEL ((unsigned short)(SIMULATOR_CONTROL_PROTOCOL+0x07))
/*
string : file name(.aml or .eml)
string : name
*/

#define KP_ACK_LOAD_MODEL ((unsigned short)(SIMULATOR_CONTROL_PROTOCOL+0x08))
#define SUCCESS_LOAD_MODEL ((char)0x01)
#define FAIL_LOAD_MODEL ((char)(0x00))
/*
byte : success code (success or fail)
*/

#define KP_REQ_UNLOAD_MODEL ((unsigned short)(SIMULATOR_CONTROL_PROTOCOL+0x09))
/*
string : file name(.aml or .eml)
string : name
*/

#define KP_ACK_UNLOAD_MODEL ((unsigned short)(SIMULATOR_CONTROL_PROTOCOL+0x0a))
#define SUCCESS_UNLOAD_MODEL ((char)0x01)
#define FAIL_UNLOAD_MODEL ((char)(0x00))
/*
byte : success code (success or fail)
*/

#define ROBOT_CONTROL_PROTOCOL ((unsigned short)0x0100)

#define KP_NOTI_ROBOT_INFOES ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x01))
/*
short : robot count 
{
int: robot id
string : robot name
}
*/

#define KP_NOTI_SENSOR_VALUE ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x02))
/*
short : sensor count
{
int: device id
int: device type
int: sensor data number
float[] : data
}
*/

#define  KP_CMD_MOTOR_VALUE ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x03))
/*
short: motor number
{
int : device id
int : data number
float[] : motor input value 
}
*/

#define KP_REQ_SENSOR_VALUE ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x04))
/*
int : device id
*/

#define KP_ACK_SENSOR_VALUE ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x05))
/*
short 
{
int: device id
int: device type
int: sensor data number
float[] : data
}
*/

#define KP_REQ_DEVICE_LIST ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x06))
/*
int : robot id
*/

#define KP_ACK_DEVICE_LIST ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x07))
/*
short : number of device
{
int		: device id
string	: device name
int		: device type 
int		: data number (float)
}
*/

#define KP_CMD_RESET_DEVICE ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x08))
/*
int		: device id
*/

#define KP_REQ_BODY_LIST ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x09))
#define KP_ACK_BODY_LIST ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x0a))

#define KP_REQ_BODY_DATA ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x0b))
#define KP_ACK_BODY_DATA ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x0c))

#define KP_REQ_JOINT_LIST ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x0d))
#define KP_ACK_JOINT_LIST ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x0e))

#define KP_REQ_JOINT_DATA ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x0f))
#define KP_ACK_JOINT_DATA ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x10))

#define KP_REQ_DEVICE_DATA ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x11))
#define KP_ACK_DEVICE_DATA ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x12))

// °­¿ø´ë
#define KP_MOVE_UP ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x13))
#define KP_MOVE_DOWN ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x14))
#define KP_ACK_MOVE_UP ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x015))
#define KP_ACK_MOVE_DOWN ((unsigned short)(ROBOT_CONTROL_PROTOCOL+0x16))

const short SIMULATION_CONTROL_PORT = 5350;
const short ROBOT_CONTROL_PORT = 5250;
const short SIMULATOR_DATA_PORT = 5450;


#endif