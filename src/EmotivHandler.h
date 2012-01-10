#ifndef EMOTIV_HANDLER_H_
#define EMOTIV_HANDLER_H_

#include <edk/edk.h>
#include <edk/edkErrorCode.h>
#include <edk/EmoStateDLL.h>

#include <ip/UdpSocket.h>
#include <osc/OscOutboundPacketStream.h>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/assign.hpp>
using namespace boost::assign;

#include <map>
using std::map;

#include <string>
using std::string;

#include <sstream>
using std::stringstream;

class EmotivHandler
{
	
	UdpSocket* socket;
	IpEndpointName* ipEnd;

public:
	EmotivHandler();
	EmotivHandler(EmoStateHandle&, int);
	~EmotivHandler(void);

	
	void connect(string ip, int port);
	void start(EmoStateHandle eState, int usernum);
	
private:

	void run();
	void sendOsc();
	void stateHandle();	

	EmoStateHandle eState;
	stringstream *user_ss;
	//Epoc Status
	int usernum;
	int batteryLevel;
	int maxBatteryLevel;
	static const int contactQualitySize = EE_CHAN_FP2+1;
	EE_EEG_ContactQuality_t contactQuality[contactQualitySize];

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

	//EEG
	static map<int, string> sensorLabels;
};

#endif //EMOTIV_HANDLER_H_