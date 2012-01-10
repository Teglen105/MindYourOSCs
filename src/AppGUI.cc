#include "AppGUI.h"

AppGUI::AppGUI(int c, char **v)
:argc(c), argv(v)
{
	init_window();
	init_buttons();
	init_inputs();
	init_choice();
}

void AppGUI::run()
{
	window->end();
  window->show(argc, argv);
  Fl::run();
}

void AppGUI::init_window()
{
	 window_width = 450;
	window_height = 225;
	
	int w = 110;
	int h = 25;
	int x = window_width/2 - w/2 + 20;
	int y = 15;
	
	window = new Fl_Window(window_width,window_height, "MindYourOSCs");
	status = new Fl_Output(x, y, w, h, "Status:");
	status->value("Not Connected");
}

void AppGUI::init_buttons()
{
	int w = 80;
  int h = 25;
  int x = window_width/2 - w/2;
  int y = 80;
  
     connect_button = new Fl_Button(x,   y, w, h, "Connect");
  disconnect_button = new Fl_Button(x, y+h, w, h, "Disconnect");
  
  connect_button->callback(button_event, this);
  disconnect_button->callback(button_event, this);
}

enum { 
	EMO_IP, EMO_PORT, OSC_IP, OSC_PORT
};
void AppGUI::init_inputs()
{
	int x = 10;
	int y = 50;
	int w = 130;
	int h = 25;
	
	inputs[EMO_IP] = new Fl_Input(x, y+h, w, h, "Emotiv IP");
	inputs[EMO_PORT] = new Fl_Input(x, y+h*3, w, h, "Emotiv Port");
	
	x = 310;
	
	inputs[OSC_IP] = new Fl_Input(x, y+h, w, h, "OSC IP");
	inputs[OSC_PORT] = new Fl_Input(x, y+h*3, w, h, "OSC Port");
	
	inputs[EMO_IP]->align(FL_ALIGN_TOP_LEFT);
	inputs[EMO_PORT]->align(FL_ALIGN_TOP_LEFT);
	inputs[OSC_IP]->align(FL_ALIGN_TOP_LEFT);
	inputs[OSC_PORT]->align(FL_ALIGN_TOP_LEFT);
}

void AppGUI::init_choice()
{
	int w = 150;
	int h = 25;
	int x = window_width/2 - w/2;
	int y = 175;
	
	choice = new Fl_Choice(x, y, w, h, "Connect To:");
	choice->align(FL_ALIGN_TOP_LEFT);
		
	choice->add("Control Panel", 0, choice_event, this);
	choice->add("EmoComposer", 0, choice_event, this);
	choice->add("Custom", 0, choice_event, this);
	choice->add("Engine", 0, choice_event, this);
	choice->value(0);
	
	set_emotiv((char*)"127.0.0.1", (char*)"3008");
	set_osc((char*)"127.0.0.1", (char*)"4200");
}

void choice_event(Fl_Widget *choice, void *gui)
{
	((AppGUI*)gui)->choice_event_i(choice);
}

void button_event(Fl_Widget *button, void *gui)
{
	((AppGUI*)gui)->button_event_i(button);
}

enum { CONTROL_PANEL, COMPOSER, CUSTOM, ENGINE };
void AppGUI::choice_event_i(Fl_Widget *choice)
{
	switch(((Fl_Choice*)choice)->value())
	{
		case CONTROL_PANEL:
			set_emotiv((char*)"127.0.0.1", (char*)"3008");
			break;
		case COMPOSER:
			set_emotiv((char*)"127.0.0.1", (char*)"1726");
			break;
		case CUSTOM: case ENGINE:
			set_emotiv((char*)"", (char*)"");
			break;
	}
}

void AppGUI::button_event_i(Fl_Widget *button)
{
	cout<<button->label()<<endl;
}

void AppGUI::set_emotiv(const char *ip, const char *port)
{
	inputs[EMO_IP]->value(ip);
	inputs[EMO_PORT]->value(port);
}

void AppGUI::set_osc(const char *ip, const char *port)
{
	inputs[OSC_IP]->value(ip);
	inputs[OSC_PORT]->value(port);
}



