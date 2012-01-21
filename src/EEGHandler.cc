#include "EEGHandler.h"

struct EEGData{

	int filled;
	const int channels;
	const int samples;
	vector< vector< double > > data;

	EEGData(int c, int s)
		: channels(c), samples(s), filled(0) { }

	void push_back(double* temp)
	{
		vector<double> temp_vec;
		for(int i=0; i<samples; i++)
		{
			temp_vec.push_back(temp[i]);
		}

		data.push_back(temp_vec);
	}

	vector<double> at(int i)
	{
		return data[i];
	}

};

EEGHandler::EEGHandler()
{
	_mutex = new boost::mutex;
}

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

		
		int channels = sizeof(targetChannelList)/sizeof(EE_DataChannel_t); //number of channels to record
		double* tempdata;
		EEGData *data = new EEGData(channels, nSamplesTaken);
		for (int i = 0; i < channels; i++) 
		{
			tempdata = new double[nSamplesTaken];
			EE_DataGet(hData, targetChannelList[i], tempdata, nSamplesTaken);
			data->push_back(tempdata);
		}	

		//_mutex->lock();
		data_queue.push(data);
		//_mutex->unlock();
	}
}

void EEGHandler::sendEEGOsc()
{

}

void EEGHandler::sendFFTOsc()
{
	if(data_queue.size() > 0)
	{
		_mutex->lock();
		
		EEGData *data = data_queue.front();		
		data_queue.pop();

		for(int j=0; j<data->samples; j++)
		{
			for(int i=0; i<data->channels; i++)
			{
				cout<< j << " : " << i << " : " << data->at(i).at(j) << endl;
			}
		}
		
		_mutex->unlock();
	}
}

const EE_DataChannel_t EEGHandler::targetChannelList[22] = {
	ED_COUNTER,
	ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
	ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
	ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
	ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
};

