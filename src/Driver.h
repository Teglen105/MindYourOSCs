#ifndef DRIVER_H_
#define DRIVER_H_

#include <boost/regex.hpp>
#include <boost/signals2.hpp>

#include <sstream>

#include "Emotiv.h"
#include "AppGUI.h"

class Driver
{
public:
	Driver();
	~Driver(void);

	void start();

private:

	void run_loop();
	void show_menu();
	void handle_choice(int choice);
	void connect_epoc(string to);
	void disconnect_epoc();

	stringstream *ss_error;
	boost::shared_mutex _mutex;
	boost::shared_lock<boost::shared_mutex> _lock;
	boost::thread epoc_thread;
	boost::thread gui_thread;
	
	AppGUI gui;
	Emotiv epoc;

	string connect_to;
	bool connect;
	bool connected;
	bool running;
	char* e_ip;
	int e_port;

	char* osc_ip;
	int osc_port;

	boost::signals2::signal<void(int)> menu_sig;
};

#endif //DRIVER_H_