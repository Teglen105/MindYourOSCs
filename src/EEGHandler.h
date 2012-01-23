#ifndef EEGHANDLER_H_
#define EEGHANDLER_H_

#include "Handler.h"

#include <vector>
#include <queue>
using std::vector;
using std::queue;

struct EEGQueue;
struct EEGData;

class EEGHandler : public Handler {

public:
	EEGHandler();
	void start(int userId);
	void connect(string ip, int port);
	
private:
	
	void recordData();
	void sendEEGOsc();
	void sendFFTOsc();

	int currentUserId;
	DataHandle hData;

	boost::mutex *_mutex;
	EEGQueue *data_queue;

	static const EE_DataChannel_t targetChannelList[22];
};

#endif //EEGHANDLER_H_