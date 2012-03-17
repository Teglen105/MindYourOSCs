#ifndef EMOTIV_HANDLER_H_
#define EMOTIV_HANDLER_H_

#include <map>
using std::map;

#include <sstream>
using std::stringstream;

#include "Handler.h"

class EmotivHandler : public Handler {

public:
	EmotivHandler();
	EmotivHandler(EmoStateHandle&, int);
	~EmotivHandler(void);

	void connect(string ip, int port);
	void start(EmoStateHandle eState, int usernum);
	
private:

	void init();
	void stateHandle();
	void sendOsc();

	OSCHelper osc;
	EmoStateHandle eState;
	stringstream *user_ss;
	int usernum;
	int batteryLevel;
	int maxBatteryLevel;
	static const int contactQualitySize = EE_CHAN_FP2+1;
	EE_EEG_ContactQuality_t contactQuality[contactQualitySize];
	static EE_DataChannel_t targetChannelList[22];

	//Gyro
	int gyroX, gyroY;

	//Affectiv
	static map<int, string> AffectivLabels;
	map<int, float> AffectivValues;

	typedef enum AffectivTypes
	{
		ENGAGED_BORED, 
		EXCITEMENT,
		EXCITEMENTLT, 
		MEDITATION, 
		FRUSTRATION
	} AffectivTypes_t;

	//Expressiv
	static map<int, string> ExpressivLabels;
	map<int, float> ExpressivValues;
	EE_ExpressivAlgo_t actionLow;
	EE_ExpressivAlgo_t actionHigh;
	float x, y, leftLid, rightLid;
	float powerLow;
	float powerHigh;

	enum ExpressivTypes
	{
		//---- EYES ----
		WINK_LEFT, 
		WINK_RIGHT,
		BLINK,
		LEFT_LID,
		RIGHT_LID,
		HORIEYE,
		LOOK_LEFT,
		LOOK_RIGHT,
		VERTEYE,
		LOOK_UP,
		LOOK_DOWN, 

		//---- LOWER FACE ----
		SMILE,    
		CLENCH,
		LAUGH,
		SMIRK_LEFT,
		SMIRK_RIGHT, 

		 //---- UPPER FACE ----
		FURROW,  
		EYEBROW
	};
	
	//Cognitiv
	static map<int, string> CognitivLabels;
	EE_CognitivAction_t currentCogType;
	float cog_power;

	//Sensor quality
	static map<int, string> sensorLabels;
};

#endif //EMOTIV_HANDLER_H_