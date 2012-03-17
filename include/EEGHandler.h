#ifndef EEGHANDLER_H_
#define EEGHANDLER_H_

#include "Handler.h"
#include <fftw3.h>
#include <vector>
#include <queue>
using std::vector;
using std::queue;

#define PI 3.14159265
#define SAMPLES_SIZE 1024
#define CHANNELS 22
const EE_DataChannel_t targetChannelList[CHANNELS] = {
	ED_COUNTER,
	ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
	ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
	ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
	ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
};
//==========================================================

typedef queue< fftw_complex* > FFTData;

class EEGData
{
public:
	EEGData(int c);
	~EEGData();
	void record(double *tempdata, int samples);
	
private:
	void fftData();
	void hannWin(fftw_complex *in, int size);
	int channel;
	int count;
	vector<double> sums;
	vector<double> *data;

	fftw_complex* in;
	fftw_complex* out;
	fftw_plan plan;
};

//==========================================================

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
	void fftData(int index);
	void hannWin(fftw_complex *in, int size);
	void calc_hann_win();

	int currentUserId;
	DataHandle hData;

	boost::mutex *_mutex;
	vector<EEGData*> eeg_vec;
};

#endif //EEGHANDLER_H_