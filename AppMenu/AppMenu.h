#ifndef APPMENU_H_
#define APPMENU_H_

#include<boost/bind.hpp>
#include<boost/thread.hpp>
static boost::mutex mutex;

#include<string>
using std::string;

class AppMenu
{
	
public:
	AppMenu(void);
	~AppMenu(void);

	void start(boost::function<void()>, boost::function<void()>);
	string e_ip; 
	string osc_ip;
	int e_port;
	int osc_port;

	bool running;

private:
	template<class T>
	T get_config(string text, T data);

	void show_menu();
	void handle_choice(int choice);

	
	boost::function<void()> connect;
	boost::function<void()> disconnect;
};

#endif //APPMENU_H_