#include "Driver.h"

Driver::Driver() 
{
	ss_error = new stringstream();
}

Driver::~Driver(void)
{
	
}

void Driver::start()
{
	boost::function<void(string)> connect = boost::bind(&Driver::connect_epoc, this, _1);
	boost::function<void()> disconnect = boost::bind(&Driver::disconnect_epoc, this);
	
	Fl::lock();
	gui.run(connect, disconnect);
}

void Driver::connect_epoc(string to)
{
	connect_to = to;

	try
	{       
		string e_ip, osc_ip;
		int e_port, osc_port;
		gui.getValues(e_ip, e_port, osc_ip, osc_port);
		if ( epoc.connect(connect_to, e_ip, e_port) )
		{
			epoc.connectOsc(osc_ip, osc_port);
		}
		gui.setEmotivStatus("Connected");
		epoc_thread = boost::thread(boost::bind(&Emotiv::run, epoc));		
	}catch(int code){
		*ss_error<<"Problem connecting to epoc ("<<code<<"). Check connections\n and try again";
		gui.setEmotivStatus(ss_error->str());
		ss_error->str("");
	}catch(...){
		*ss_error<<"Problem connecting to epoc (unknown error). Check connections\n and try again";
		gui.setEmotivStatus(ss_error->str());
		ss_error->str("");
	}
}

void Driver::disconnect_epoc()
{
	epoc.disconnect();
	gui.setEmotivStatus("Disconnected");
}
