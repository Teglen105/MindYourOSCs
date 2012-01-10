#include "Driver.h"

Driver::Driver() 
{
	connect = false;
	ss_error = new stringstream();
}

Driver::~Driver(void)
{
	
}

void Driver::start()
{
	boost::function<void(string)> connect = boost::bind(&Driver::connect_epoc, this, _1);
	boost::function<void()> disconnect = boost::bind(&Driver::disconnect_epoc, this);
	
	gui_thread = boost::thread(boost::bind(&AppGUI::run, &gui, connect, disconnect));
	gui_thread.join();	
}

void Driver::connect_epoc(string to)
{
	cout<<"connect"<<endl;
	connect_to = to;
	connect = true;

	try
	{       
		string e_ip, osc_ip;
		int e_port, osc_port;
		gui.getValues(e_ip, e_port, osc_ip, osc_port);
		epoc.connectOsc(osc_ip, osc_port);
		epoc.connect(connect_to, e_ip, e_port);
		//boost::this_thread::sleep(boost::posix_time::milliseconds(20));
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
}
