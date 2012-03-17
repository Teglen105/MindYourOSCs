#include "EEGHandler.h"

vector<double> hann_vec;

EEGData::EEGData(int c)
		:channel(c)
{
	data = new vector<double>();
	count = 0;

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SAMPLES_SIZE);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SAMPLES_SIZE);
    plan = fftw_plan_dft_1d(SAMPLES_SIZE-20, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	sums.push_back(0);
}

EEGData::~EEGData()
{
	
}

void EEGData::record(double *tempdata, int samples)
{
	for(int i=0; i<samples; i++)
	{
		++count;
		data->push_back(tempdata[i]);

		int size = data->size();
		if(size % (SAMPLES_SIZE/8) == 0)
		{
			sums.push_back(0);
		}

		sums.back() += tempdata[i];

		if(size >= SAMPLES_SIZE)
		{
			fftData();
			data->erase(data->begin());
		}

		if(sums.size() > 8)
		{
			sums.erase(sums.begin());
		}
	}

	delete[] tempdata;
}

void EEGData::fftData()
{
	int j=0;
	for( int i=0; i<SAMPLES_SIZE; ++i)
	{
		if(i != 0 && i % (SAMPLES_SIZE/8) == 0)
		{
			++j;
		}

		in[i][0] = data->at(i) - ( data->at(i) - ( sums[j] / (SAMPLES_SIZE/8)) );
	}

	hannWin(in, SAMPLES_SIZE);
	fftw_execute(plan);
}

void EEGData::hannWin(fftw_complex *in, int size)
{
	for(int i = 0; i < size; i++)
	{
		in[i][0] *= hann_vec[i];
	}
}

//----------------------------------------------------------------------
//**********************************************************************
//----------------------------------------------------------------------

EEGHandler::EEGHandler()
{
	_mutex = new boost::mutex;

	for(int i=0; i<CHANNELS; i++)
	{
		eeg_vec.push_back(new EEGData(i));
	}

	
	calc_hann_win();
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

	unsigned int samples=0;
	EE_DataGetNumberOfSample(hData,&samples);		
	
	if (samples != 0  ) {
		
		double* tempdata;
		
		for (int i = 0; i < CHANNELS; i++) 
		{
			tempdata = new double[samples];
			EE_DataGet(hData, targetChannelList[i], tempdata, samples);

			eeg_vec[i]->record(tempdata, samples);
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

void EEGHandler::calc_hann_win()
{
	for(int i = 0; i < SAMPLES_SIZE; i++)
	{
		double A = i;

		A = 2 * PI * A;

		A /= (SAMPLES_SIZE - 1);

		A = cos(A);

		A = ((i == 0) 
			? 1 + A
			: 1 - A
			);

		A*=.5;

		hann_vec.push_back(A);
	}
}

//----------------------------------------------------------------------



