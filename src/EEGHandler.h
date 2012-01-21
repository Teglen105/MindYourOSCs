#ifndef EEGHANDLER_H_
#define EEGHANDLER_H_

#include "Handler.h"

class EEGHandler : public Handler {

public:
	void start(int userId);
	void connect(string ip, int port);

private:
	
	void recordData();
	void sendEEGOsc();
	void sendFFTOsc();

	int currentUserId;
	DataHandle hData;

	static const EE_DataChannel_t targetChannelList[22];
};

#endif //EEGHANDLER_H_