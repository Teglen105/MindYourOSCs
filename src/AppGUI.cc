#include "AppGUI.h"

AppGUI::AppGUI()
{
	init_window();
	init_buttons();
	init_inputs();
	init_choice();
}

void AppGUI::run(boost::function<void(string)> connect, boost::function<void()> disconnect)
{
	this->connect = connect;
	this->disconnect = disconnect;
	window->end();
	window->show();
	Fl::run();
}

void AppGUI::getValues(string &emotiv_ip, int &emotiv_port, string &osc_ip, int &osc_port)
{
	emotiv_ip = string(inputs[EMO_IP]->value());
	emotiv_port = atoi(inputs[EMO_PORT]->value());
	osc_ip = string(inputs[OSC_IP]->value());
	osc_port = atoi(inputs[OSC_PORT]->value());
}

void AppGUI::setEmotivStatus(string status_msg)
{
	status->value(status_msg.c_str());
}

void AppGUI::init_window()
{
	 window_width = 450;
	window_height = 245;
	
	int w = 380;
	int h = 45;
	int x = 55;
	int y = 15;
	
	window = new Fl_Window(window_width,window_height, "MindYourOSCs");
	status = new Fl_Multiline_Output(x, y, w, h, "Status:");
	status->value("Not Connected");
}

void AppGUI::init_buttons()
{
	int w = 80;
	int h = 25;
	int x = window_width/2 - w/2;
	int y = 100;
  
     connect_button = new Fl_Button(x,   y, w, h, "Connect");
  disconnect_button = new Fl_Button(x, y+h, w, h, "Disconnect");
  
  connect_button->callback(button_event, this);
  disconnect_button->callback(button_event, this);
}


void AppGUI::init_inputs()
{
	int x = 10;
	int y = 65;
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
	int y = 200;
	
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
	string label = string(button->label());

	if(label.compare("Connect") == 0)
	{
		string connect_to = "remote";
		if(((Fl_Choice*)choice)->value() == ENGINE)
		{
			connect_to = "engine";
		}

		connect(connect_to);
	}
	else if(label.compare("Disconnect") == 0)
	{
		disconnect();
	}
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



