#include "EEGHandler.h"


void EEGHandler::connect(string ip, int port)
{
	Handler::connect(ip, port);
	hData = EE_DataCreate();
	EE_DataSetBufferSizeInSec(1);
}

void EEGHandler::start(int userId)
{
	currentUserId = userId;
	recordData();

#ifdef FFT
	boost::thread(boost::bind(&EEGHandler::sendFFTOsc, this));
#elif defined EEG
	boost::thread(boost::bind(&EEGHandler::sendEEGOsc, this));
#endif

}

void EEGHandler::recordData()
{
	EE_DataUpdateHandle(0, hData);

	unsigned int nSamplesTaken=0;
	EE_DataGetNumberOfSample(hData,&nSamplesTaken);				

	if (nSamplesTaken != 0  ) {

		double* data = new double[nSamplesTaken];
		int channels = sizeof(targetChannelList)/sizeof(EE_DataChannel_t); //number of channels to record
		for (int sampleIdx=0 ; sampleIdx<(int)nSamplesTaken ; ++ sampleIdx) {
			for (int i = 0; i < channels; i++) {
				EE_DataGet(hData, targetChannelList[i], data, nSamplesTaken);
				if(targetChannelList[i] == ED_TIMESTAMP)
					cout << i <<": "<<data[sampleIdx] << endl;
			}
		}
		delete[] data;					
	}
}

void EEGHandler::sendEEGOsc()
{

}

void EEGHandler::sendFFTOsc()
{

}

const EE_DataChannel_t EEGHandler::targetChannelList[22] = {
	ED_COUNTER,
	ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
	ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
	ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
	ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
};

