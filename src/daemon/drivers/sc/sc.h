#pragma once
#include "scc/driver.h"

typedef struct SCController SCController;
typedef struct SCInput SCInput;
typedef struct SDInput SDInput;

typedef enum {
	SC_WIRED = 1,
	SC_WIRELESS = 2,
	SC_BT = 3,
} SCControllerType;

typedef enum {
	SS_NOT_CONFIGURED = 0,
	SS_READY = 1,
	SS_FAILED = 2
} SCControllerState;

#define MAX_SERIAL_LEN	16
/** MAX_DESC_LEN has to fit "<SCByCable SERIAL>" */
#define MAX_DESC_LEN (15 + MAX_SERIAL_LEN)
/** MAX_DESC_LEN has to fit "scSERIAL" */
#define MAX_ID_LEN (3 + MAX_SERIAL_LEN)

struct SDInput {
	uint8_t			ptype;  		//0x00
	uint8_t			_a1[3]; 		//0x01 
	uint32_t		seq;			//0x03 
	uint16_t		buttons0;		//0x09 
	uint8_t			buttons1;		//0x0A
	uint8_t			buttons3;		//0x0C
	uint8_t			buttons4;		//0x0D
	uint8_t			buttons5;		//0x0E
	uint8_t			buttons6;		//0x0E
	int16_t			lpad_x;			//0x10
	int16_t			lpad_y;			//0x12
	int16_t			rpad_x;			//0x13
	int16_t			rpad_y;			//0x16
	int16_t			accel_x;		//0x18
	int16_t			accel_y;		//0x1A
	int16_t			accel_z;		//0x1C
	int16_t			gpitch;			//0x1E
	int16_t			gyaw;			//0x20
	int16_t			groll;			//0x22
	int16_t			q1;				//0x24
	int16_t			q2;				//0x26
	int16_t			q3;				//0x28
	int16_t			q4;				//0x2A
	int16_t			ltrig;			//0x2C
	int16_t			rtrig;			//0x2E
	int16_t			lthumb_x;		//0x30
	int16_t			lthumb_y;		//0x32
	int16_t			rthumb_x;		//0x34
	int16_t			rthumb_y;		//0x36
	int16_t			lpad_pressure;	//0x38
	int16_t			rpad_pressure;	//0x3A
	// uint8_t		_a4[16];
};

struct SCInput {
	uint8_t			_a1[2];
	uint8_t			ptype;
	uint32_t		seq;
	uint16_t		buttons0;
	uint8_t			buttons1;
	uint8_t			ltrig;
	uint8_t			rtrig;
	uint8_t			_a2[3];
	int16_t			lpad_x;
	int16_t			lpad_y;
	int16_t			rpad_x;
	int16_t			rpad_y;
	uint8_t			_a3[4];
	int16_t			accel_x;
	int16_t			accel_y;
	int16_t			accel_z;
	int16_t			gpitch;
	int16_t			groll;
	int16_t			gyaw;
	int16_t			q1;
	int16_t			q2;
	int16_t			q3;
	int16_t			q4;
	// uint8_t		_a4[16];
};


struct SCController {
	Controller			controller;
	Daemon*				daemon;
	Mapper*				mapper;
	SCControllerType	type;
	SCControllerState	state;
	InputDevice*		dev;
	char				serial[MAX_SERIAL_LEN];
	char				desc[MAX_DESC_LEN];
	char				id[MAX_ID_LEN];
	uint16_t			idx;
	bool				gyro_enabled;
	uint64_t			auto_id;
	bool				auto_id_used;
	uint16_t			idle_timeout;	// in seconds
	uint8_t				led_level;		// 1 to 100
	HapticData			hdata[2];
	ControllerInput		input;
};

typedef enum {
	PT_INPUT = 0x01,
	PT_HOTPLUG = 0x03,
	PT_IDLE = 0x04,
	PT_OFF = 0x9f,
	PT_AUDIO = 0xb6,
	PT_CLEAR_MAPPINGS = 0x81,
	PT_CONFIGURE = 0x87,
	PT_LED = 0x87,
	PT_CALIBRATE_JOYSTICK = 0xbf,
	PT_CALIBRATE_TRACKPAD = 0xa7,
	PT_SET_AUDIO_INDICES = 0xc1,
	PT_LIZARD_BUTTONS = 0x85,
	PT_LIZARD_MOUSE = 0x8e,
	PT_FEEDBACK = 0x8f,
	PT_RESET = 0x95,
	PT_GET_SERIAL = 0xAE,
} SCPacketType;

typedef enum {
	PL_LED = 0x03,
	PL_OFF = 0x04,
	PL_FEEDBACK = 0x07,
	PL_CONFIGURE = 0x15,
	PL_CONFIGURE_BT = 0x0f,
	PL_GET_SERIAL = 0x15,
} SCPacketLength;

typedef enum {
	CT_LED = 0x2d,
	CT_CONFIGURE = 0x32,
	CONFIGURE_BT = 0x18,
} SCConfigType;

/** Returns NULL on failure */
SCController* create_usb_controller(Daemon* daemon, InputDevice* dev, SCControllerType type, uint16_t idx);
/** Common for wired and wireless controller */
void handle_input(SCController* sc, SCInput* i);
/** Returns false on failure */
bool read_serial(SCController* sc);
/** Returns false on failure */
bool clear_mappings(SCController* sc);
/** This is effectively reverse of clear_mappings. Returns false on failure */
bool lizard_mode(SCController* sc);
/** Returns false on failure */
bool configure(SCController* sc);
/**
 * Called to inform daemon & mapper about controller being disconnected.
 * Does not deallocate 'sc'.
 */
void disconnected(SCController* sc);

