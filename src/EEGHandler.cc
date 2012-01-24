#include "EEGHandler.h"

class EEGData {

	int channel;
	vector<double> data;

public:

	EEGData(int c)
		:channel(c) {}

	void record(double *tempdata, int samples)
	{
		for(int i=0; i<samples; i++)
		{
			data.push_back(tempdata[i]);
		}

		delete[] tempdata;
	}
};

//----------------------------------------------------------------------

EEGHandler::EEGHandler()
{
	_mutex = new boost::mutex;

	for(int i=0; i<channels; i++)
	{
		eeg_vec.push_back(new EEGData(i));
	}
}

EEGHandler::~EEGHandler()
{

}

//----------------------------------------------------------------------

void EEGHandler::connect(string ip, int port)
{
	Handler::connect(ip, port);
	hData = EE_DataCreate();
	EE_DataSetBufferSizeInSec(1);
}

//----------------------------------------------------------------------

void EEGHandler::start(int userId)
{
	currentUserId = userId;
	recordData();
}

//----------------------------------------------------------------------

void EEGHandler::recordData()
{
	EE_DataUpdateHandle(0, hData);

	unsigned int nSamplesTaken=0;
	EE_DataGetNumberOfSample(hData,&nSamplesTaken);		
	
	if (nSamplesTaken != 0  ) {
		
		double* tempdata;
		
		for (int i = 0; i < channels; i++) 
		{
			
			tempdata = new double[nSamplesTaken];
			EE_DataGet(hData, targetChannelList[i], tempdata, nSamplesTaken);

			boost::thread(boost::bind(&EEGData::record, eeg_vec[i], tempdata, nSamplesTaken));
			
			/*#ifdef FFT
				
			#elif defined EEG
				run_thread = new boost::thread(boost::bind(&EEGHandler::sendEEGOsc, this));
			#endif*/
		}	
	}
}

//----------------------------------------------------------------------

void EEGHandler::sendEEGOsc()
{

}

//----------------------------------------------------------------------

void EEGHandler::sendFFTOsc()
{

}

//----------------------------------------------------------------------

const EE_DataChannel_t EEGHandler::targetChannelList[channels] = {
	ED_COUNTER,
	ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
	ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
	ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
	ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
};

