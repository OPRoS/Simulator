#pragma	once

struct Vdata{
	int		DeviceID;
	int		RobotID;
	CString	RobotName;
	CString	DeviceName;
};

struct	RobotInfo{
	int		rCount;
	int		dCount;
	int		subCount[4];
};

typedef struct
{
	float ForwardVelocity;
	float AngularVelocity;
}	ShareJoystickStruct;

typedef	struct
{
	float	Link1;
	float	Link2;
	float	Link3;
	float	Link4;
	float	Link5;
	float	Link6;
}	ShareManipulatorStruct;

typedef struct	{
	char	command;
	char	RobotName[30];
	char	deviceCnt;
	char	DeviceName[16][30];
}	initDevice;

typedef struct	{
	char command;
	char cnt;
	unsigned int deviceID[16];
}	findDeviceID;

typedef	struct {
	char	command;
	short	cnt;
	unsigned int	RobotID[3];
	char	RobotName[3][30];
}	InfoRobot;

typedef	struct{
	char			command;
	short			cnt;
	unsigned int	DeviceID[128];
	char			DeviceName[128][30];
}	InfoDevice;


typedef	struct{
	char	command;
	short	cnt;
	int		ID;
	float	value[128];
}	DeviceValue;

typedef struct {
	char command;
	int cnt;
	int ID;
	unsigned char Image[230400];
}	ImageData;

typedef	struct{
	char	command;
	int		ID;
}	Reqmsg;