#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Multiline_Output.H>

#include <boost/function.hpp>

#include<sstream>
#include<iostream>
using std::cout; 
using std::endl;
#include<string>
using std::string;

void choice_event(Fl_Widget *choice, void *gui);
void button_event(Fl_Widget *button, void *gui);

class AppGUI {

public:
	AppGUI();
	void run(boost::function<void(string)> connect, boost::function<void()> disconnect);
	
	void choice_event_i(Fl_Widget *choice);
	void button_event_i(Fl_Widget *button);
	
	void getValues(string &emotiv_ip, int &emotiv_port, string &osc_ip, int &osc_port);
	void setEmotivStatus(string status);
	
private:
	enum { 
		EMO_IP, EMO_PORT, OSC_IP, OSC_PORT
	};

	void init_window();
	void init_buttons();
	void init_inputs();
	void init_choice();
	
	void set_emotiv(const char *ip, const char *port);
	void set_osc(const char *ip, const char *port);
	
	int argc;
	char **argv;
	
	int window_width;
	int window_height;
	
	Fl_Window *window;
	Fl_Button *connect_button;
	Fl_Button *disconnect_button;
	Fl_Input *inputs[4];
	Fl_Label *labels[4];
	Fl_Choice *choice;
	Fl_Multiline_Output *status;
	
	std::stringstream int_converter;

	boost::function<void(string)> connect;
	boost::function<void()> disconnect;
};