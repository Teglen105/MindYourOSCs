#include "EmotivHandler.h"
#include <iostream>
using namespace std;


EmotivHandler::EmotivHandler()
{
	socket = new UdpSocket();
	
}

EmotivHandler::EmotivHandler(EmoStateHandle& e, int u)
	:eState(e), usernum(u)
{
}


EmotivHandler::~EmotivHandler(void)
{
}

void EmotivHandler::start(EmoStateHandle e, int u){
	eState = e;
	usernum = u;

	run();
	stateHandle();
	boost::thread(boost::bind(&EmotivHandler::sendOsc, this));
}

void EmotivHandler::connect(char* ip, int port)
{
	ipEnd = new IpEndpointName(ip, port);
}

void EmotivHandler::run()
{
	for(int i=0; i<FRUSTRATION; i++)
		AffectivValues[i] = 0;

	for(int i=0; i<EYEBROW; i++)
		ExpressivValues[i] = 0;

	currentCogType = COG_NEUTRAL;
}

void EmotivHandler::sendOsc()
{
	map<int, string>::iterator iter;
	stringstream ss;
	ss<<usernum;
	string user;
	ss>>user;

	const int buffer_size = 64;
	char buffer[buffer_size];
	string exp_string = "/EXP/";
	string aff_string = "/AFF/";

//-----GYRO----------------------------------
	osc::OutboundPacketStream out(buffer, buffer_size);
	out << osc::BeginBundleImmediate
		<< osc::BeginMessage(string("/GYRO/X/"+user).c_str())
		<< (int)gyroX
	    << osc::EndMessage 
		<< osc::BeginMessage(string("/GYRO/Y/"+user).c_str())
		<< (int)gyroY
		<< osc::EndMessage
		<< osc::EndBundle;
	socket->SendTo( *ipEnd, out.Data(), out.Size() );

//-----COGNITIV----------------------------------
	out = osc::OutboundPacketStream(buffer, buffer_size);
	out << osc::BeginBundleImmediate
		<< osc::BeginMessage(string("/COG/"+CognitivLabels[currentCogType]+"/"+user).c_str())
		<< (float)cog_power
	    << osc::EndMessage 
		<< osc::EndBundle;
	socket->SendTo( *ipEnd, out.Data(), out.Size() );

//-----EXPRESSIV----------------------------------
	iter = ExpressivLabels.begin();
	do{
		float val = ((ExpressivValues[iter->first] >= .001)
			? ExpressivValues[iter->first] 
			: 0
			);

		out = osc::OutboundPacketStream(buffer, buffer_size);
		out << osc::BeginBundleImmediate
			<< osc::BeginMessage(string(exp_string+iter->second+"/"+user).c_str())
			<< (float)val
			<< osc::EndMessage 
			<< osc::EndBundle;
		socket->SendTo( *ipEnd, out.Data(), out.Size() );
	}while(++iter != ExpressivLabels.end());

//-----AFFECTIV----------------------------------
	iter = AffectivLabels.begin();
	do{
		float val = ((AffectivValues[iter->first] >= .001)
			? AffectivValues[iter->first] 
			: 0
			);

		out = osc::OutboundPacketStream(buffer, buffer_size);
		out << osc::BeginBundleImmediate
			<< osc::BeginMessage(string(aff_string+iter->second+"/"+user).c_str())
			<< (float)val
			<< osc::EndMessage 
			<< osc::EndBundle;
		socket->SendTo( *ipEnd, out.Data(), out.Size() );
	}while(++iter != AffectivLabels.end());
}

void EmotivHandler::stateHandle()
{

	//Gyro
	EE_HeadsetGetGyroDelta(usernum, &gyroX, &gyroY);
//-------------------------------------------------------------------------------------------
	//Affectiv
	AffectivValues[ENGAGED_BORED] = ES_AffectivGetEngagementBoredomScore(eState);
	AffectivValues[EXCITEMENT]    = ES_AffectivGetExcitementShortTermScore(eState);
	AffectivValues[EXCITEMENTLT]  = ES_AffectivGetExcitementLongTermScore(eState);
	AffectivValues[MEDITATION]    = ES_AffectivGetMeditationScore(eState);
	AffectivValues[FRUSTRATION]   = ES_AffectivGetFrustrationScore(eState);
//-------------------------------------------------------------------------------------------
	//Expressiv
	ES_ExpressivGetEyelidState(eState, &ExpressivValues[LEFT_LID], &ExpressivValues[RIGHT_LID]);
	ES_ExpressivGetEyeLocation (eState, &ExpressivValues[HORIEYE], &ExpressivValues[VERTEYE]);

	actionLow = ES_ExpressivGetLowerFaceAction      ( eState );
	powerLow  = ES_ExpressivGetLowerFaceActionPower ( eState );

	actionHigh = ES_ExpressivGetUpperFaceAction      ( eState );		
	powerHigh  = ES_ExpressivGetUpperFaceActionPower ( eState );

	ExpressivValues[WINK_LEFT] = (ExpressivValues[LEFT_LID] == 1);
	ExpressivValues[WINK_RIGHT] = (ExpressivValues[RIGHT_LID] == 1);
	ExpressivValues[BLINK] = (ExpressivValues[LEFT_LID] == ExpressivValues[RIGHT_LID] && ExpressivValues[RIGHT_LID] == 1);
		
	if((actionLow & EXP_SMILE) == EXP_SMILE){ ExpressivValues[SMILE] = powerLow; }
	if((actionLow & EXP_CLENCH) == EXP_CLENCH){ ExpressivValues[CLENCH] = powerLow; }
	if((actionLow & EXP_LAUGH) == EXP_LAUGH){ ExpressivValues[LAUGH] = powerLow; }
	if((actionLow & EXP_SMIRK_LEFT) == EXP_SMIRK_LEFT){ ExpressivValues[SMIRK_LEFT] = powerLow; }
	if((actionLow & EXP_SMIRK_RIGHT) == EXP_SMIRK_RIGHT){ ExpressivValues[SMIRK_RIGHT] = powerLow; }

	if((actionHigh & EXP_EYEBROW) == EXP_EYEBROW){ ExpressivValues[EYEBROW] = powerHigh; }
	if((actionHigh & EXP_FURROW) == EXP_FURROW){ ExpressivValues[FURROW] = powerHigh; }
//-------------------------------------------------------------------------------------------
	//Cognitiv
	currentCogType = ES_CognitivGetCurrentAction(eState);
	cog_power = ES_CognitivGetCurrentActionPower(eState);	
//-------------------------------------------------------------------------------------------
	
}

map<int, string> EmotivHandler::AffectivLabels = map_list_of 
	(ENGAGED_BORED, "Engaged")
	(EXCITEMENT, "Excitement")
	(EXCITEMENTLT, "ExcitementLT")
	(MEDITATION, "Meditation")
	(FRUSTRATION, "Frustration");

map<int, string> EmotivHandler::ExpressivLabels = map_list_of 
	(WINK_LEFT, "WINK_LEFT")
	(WINK_RIGHT, "WINK_RIGHT")
	(BLINK, "BLINK")
	(LEFT_LID, "LEFT_LID")
	(RIGHT_LID, "RIGHT_LID")
	(HORIEYE, "HORIEYE")
	(VERTEYE, "VERTEYE")
	(SMILE, "SMILE")
	(CLENCH, "CLENCH")
	(LAUGH, "LAUGH")
	(SMIRK_LEFT, "SMIRK_LEFT")
	(SMIRK_RIGHT, "SMIRK_RIGHT")
	(FURROW, "FURROW")
	(EYEBROW, "EYEBROW");

map<int, string> EmotivHandler::CognitivLabels = map_list_of 
	(COG_NEUTRAL, "NEUTRAL")
	(COG_PUSH, "PUSH")
	(COG_PULL, "PULL")
	(COG_LIFT, "LIFT")
	(COG_DROP, "DROP")
	(COG_LEFT, "LEFT")
	(COG_RIGHT, "RIGHT")
	(COG_ROTATE_LEFT, "ROTATE_LEFT")
	(COG_ROTATE_RIGHT, "ROTATE_RIGHT")
	(COG_ROTATE_CLOCKWISE, "ROTATE_CLOCKWISE")
	(COG_ROTATE_COUNTER_CLOCKWISE, "ROTATE_COUNTER_CLOCKWISE")
	(COG_ROTATE_FORWARDS, "ROTATE_FORWARDS")
	(COG_ROTATE_REVERSE, "ROTATE_REVERSE")
	(COG_DISAPPEAR, "DISAPPEAR");