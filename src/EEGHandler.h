#ifndef EEGHANDLER_H_
#define EEGHANDLER_H_

#include "Handler.h"

#include <vector>
#include <queue>
using std::vector;
using std::queue;

class EEGData;

class EEGHandler : public Handler {

public:
	EEGHandler();
    ~EEGHandler();
	void start(int userId);
	void connect(string ip, int port);
	
private:
	
	void recordData();
	void sendEEGOsc();
	void sendFFTOsc();

	int currentUserId;
	DataHandle hData;

	boost::mutex *_mutex;
	vector<EEGData*> eeg_vec;

	static const int channels = 22;

	static const EE_DataChannel_t targetChannelList[channels];
};

#endif //EEGHANDLER_H_