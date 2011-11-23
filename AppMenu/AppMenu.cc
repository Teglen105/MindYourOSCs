#include "AppMenu.h"

#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;
using std::cin;
using std::string;

void clear();
#ifdef WIN32
    #include <conio.h>
	void clear(){ system("cls"); }
#else
	#include <stdio.h>
	#include <termios.h>
	#include <unistd.h>
char _getch();
char _getch(){
    struct termios oldt, newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}
void clear(){ system("clear"); }
#endif

AppMenu::AppMenu(void)
{
	e_ip = "127.0.0.1";
	e_port = 1726;
	osc_ip = "127.0.0.1";
	osc_port = 4200;
}


AppMenu::~AppMenu(void)
{
}

void AppMenu::start(boost::function<void()> f, boost::function<void()> f2)
{
	connect = f;
	disconnect = f2;
    running = true;
    
	while(running)
	{
        clear();
		show_menu();

        cout<<"> ";
		int choice = _getch();
		
        
		handle_choice(choice);
	}
}

void AppMenu::show_menu()
{
	cout<<"Hello! Welcome to MindYourOSCs"<<endl
			<<"Please choose an option"<<endl
			<<"1. Set Emotiv IP ("<<e_ip<<")"<<endl
			<<"2. Set Emotiv Port ("<<e_port<<")"<<endl
			<<"3. Set OSC IP ("<<osc_ip<<")"<<endl
			<<"4. Set OSC Port ("<<osc_port<<")"<<endl
			<<"5. Connect"<<endl
			<<"6. Disconnect"<<endl
			<<"7. Quit"<<endl;
}

enum CHOICES{ SET_EIP=49, SET_EPORT, SET_OSCIP, SET_OSCPORT, CONNECT, DISC, QUIT };

void AppMenu::handle_choice(int choice)
{
	switch(choice)
	{
		case SET_EIP:
			get_config<string&>("Enter Emotiv IP", e_ip);
			break;
		case SET_EPORT:
			get_config<int&>("Enter Emotiv Port (Composer=1726, Control Panel=3008)",e_port);
			break;
		case SET_OSCIP:
			get_config<string&>("Enter OSC IP", osc_ip);
			break;
		case SET_OSCPORT:
			get_config<int&>("Enter OSC Port", osc_port);
			break;
		case CONNECT:
			boost::this_thread::sleep(boost::posix_time::milliseconds(75));
			connect();
			break;
		case DISC:
			boost::this_thread::sleep(boost::posix_time::milliseconds(100));
			disconnect();
			break;
		case QUIT:
			running = false;
		default:
			break;
	}
}

template<class T>
T AppMenu::get_config(string text, T data){
	clear();
	cout<<text<<endl<<" > ";
	cin>>data;
	clear();
	return data;
}
