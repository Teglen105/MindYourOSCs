#include "Driver.h"


Driver::Driver(void)
{
	connect = false;
	
}


Driver::~Driver(void)
{
}



void Driver::start()
{
	boost::function<void()> connect = boost::bind(&Driver::connect_epoc, this);
	boost::function<void()> disconnect = boost::bind(&Driver::disconnect_epoc, this);
	
    menu_thread = boost::thread(boost::bind(&AppMenu::start, &menu, connect, disconnect));
	boost::thread main_thread(boost::bind(&Driver::run_loop, this));
	
	menu_thread.join();
	main_thread.join();
}

void Driver::connect_epoc()
{
	connect = true;
}

void Driver::disconnect_epoc()
{
	epoc.disconnect();
}

void Driver::run_loop()
{
	while(menu.running)
	{
		if(connect)
		{
			try{
				epoc.connectOsc();
				epoc.connect();
				boost::this_thread::sleep(boost::posix_time::milliseconds(20));
				cout<<"Connected"<<endl;
				epoc_thread = boost::thread(boost::bind(&Emotiv::run, epoc));		
			}catch(int code){
				cout<<"Problem connecting to epoc ("<<code<<"). Check connections and try again"<<endl<<endl;
			}catch(...){
                cout<<"Problem connecting to epoc ("<<0<<"). Check connections and try again"<<endl<<endl;
            }
		connect = false;
		}
	}
}
