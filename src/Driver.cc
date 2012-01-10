#include "Driver.h"
//#include<gtkmm/main.h>

Driver::Driver(void)
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
	
  //menu_thread = boost::thread(boost::bind(&AppMenu::start, &menu, connect, disconnect));
	boost::thread main_thread(boost::bind(&Driver::run_loop, this));
	
	//menu_thread.join();
	main_thread.join();
}

void Driver::connect_epoc(string to)
{
  connect_to = to;
	connect = true;
}

void Driver::disconnect_epoc()
{
	epoc.disconnect();
}

void Driver::run_loop()
{
	/*while(menu.running)
	{
		if(connect)
		{
			try
			{       
				epoc.connectOsc(menu.osc_ip, menu.osc_port);
				epoc.connect(connect_to, menu.e_ip, menu.e_port);
				boost::this_thread::sleep(boost::posix_time::milliseconds(20));
				//menu.setEmotivStatus("Connected");
				epoc_thread = boost::thread(boost::bind(&Emotiv::run, epoc));		
			}catch(int code){
				*ss_error<<"Problem connecting to epoc ("<<code<<"). Check connections and try again";
				//menu.setEmotivStatus(ss_error->str());
				ss_error->str("");
			}catch(...){
        *ss_error<<"Problem connecting to epoc (unknown error). Check connections and try again";
				//menu.setEmotivStatus(ss_error->str());
				ss_error->str("");
      }
			connect = false;
			menu.refresh();
		}
	}*/
}
