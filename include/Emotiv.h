#ifndef EMOTIV_H_
#define EMOTIV_H_

#include "EEGHandler.h"
#include "EmotivHandler.h"

const int COMP_PORT = 1726;
const int CTRL_PORT = 3008;

class Emotiv
{

public:
	Emotiv(void);
	~Emotiv(void);

	typedef enum TYPES{
		engine,
		remote
	} connection_types;

	int connect(string to, string ip = "127.0.0.1" , int port = COMP_PORT);
	void connectOsc(string ip = "127.0.0.1", int port = 4200);

	void run();
	void disconnect();

private:	

	void getEvent(EmoEngineEventHandle);

	EmoEngineEventHandle eEvent;
	EmoStateHandle eState;
	

#if defined EEG || defined FFT
	EEGHandler handler;
#else
	EmotivHandler handler;
#endif
	
	char* ip;
	int port;
	int engineStatusCode;
	unsigned int numberOfUsers;
	unsigned int userId;
	bool connected;
	boost::mutex *_mutex;


};

#endif //EMOTIV_H_