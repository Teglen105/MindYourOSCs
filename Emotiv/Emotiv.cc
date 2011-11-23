#include "Emotiv.h"


Emotiv::Emotiv(void)
{
	eEvent = EE_EmoEngineEventCreate();
	eState = EE_EmoStateCreate();
	connected = false;
	_mutex = new boost::mutex();
}


Emotiv::~Emotiv(void)
{
	//EE_EmoEngineEventFree(eEvent);
	//EE_EmoStateFree(eState);
}

int Emotiv::connect(int loc, char* ip, int port)
{
	if(connected)
		disconnect();

	int code = ( (loc == remote) 
		? EE_EngineRemoteConnect(ip, port) 
		: EE_EngineConnect()
		);

#ifdef EEG
	hData = EE_DataCreate();
#endif

	if(code == EDK_OK)
		connected = true;
	else
		throw code;

	return connected;
}

void Emotiv::disconnect()
{
	_mutex->lock();
	EE_EngineDisconnect();
	connected = false;
	_mutex->unlock();
}

void Emotiv::connectOsc(char* ip, int port)
{
	handler.connect(ip, port);
}

void Emotiv::run()
{
	while(connected)
	{
		_mutex->lock();
		getEvent(eEvent);
		_mutex->unlock();
	}
}

void Emotiv::getEvent(EmoEngineEventHandle)
{
	engineStatusCode = EE_EngineGetNextEvent(eEvent);
		if(engineStatusCode == EDK_OK)
		{
			EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
			EE_EmoEngineEventGetUserId(eEvent, &userId);
			EE_EngineGetNumUser(&numberOfUsers);

			switch((int)eventType)
			{
			case EE_UserAdded:
				//EE_DataAcquisitionEnable(userId,true);
				break;
			case EE_EmoStateUpdated:
				EE_EmoEngineEventGetEmoState(eEvent, eState);
				handler.start(eState, userId);
				break;
			}
		}

#ifdef EEG
		EE_DataUpdateHandle(0, hData);

		unsigned int nSamplesTaken=0;
		EE_DataGetNumberOfSample(hData,&nSamplesTaken);
#endif
}
