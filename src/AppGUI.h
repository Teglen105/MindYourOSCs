#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Output.H>

#include<sstream>
#include<iostream>
using std::cout; 
using std::endl;

void choice_event(Fl_Widget *choice, void *gui);
void button_event(Fl_Widget *button, void *gui);

class AppGUI {

public:
	AppGUI(int c, char **v);
	void run();
	
	void choice_event_i(Fl_Widget *choice);
	void button_event_i(Fl_Widget *button);
	
private:
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
	Fl_Output *status;
	
	std::stringstream int_converter;
};