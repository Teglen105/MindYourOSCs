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

int Emotiv::connect(string to, string ip, int port)
{    
	if(connected)
		disconnect();

	_mutex->lock();
	int code = ( (to == "remote") 
		? EE_EngineRemoteConnect(ip.c_str(), port)
		: EE_EngineConnect()
		);
	_mutex->unlock();

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

void Emotiv::connectOsc(string ip, int port)
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
#if defined EEG || defined FFT        
		case EE_UserAdded:
			EE_DataAcquisitionEnable(userId,true);
			break;
#else
		case EE_EmoStateUpdated:
			EE_EmoEngineEventGetEmoState(eEvent, eState);
			handler.start(eState, userId);
			break;
#endif
		}

#if defined EEG || defined FFT
		handler.start(userId);
#endif

	}
}
