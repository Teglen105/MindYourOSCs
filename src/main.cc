#ifdef WIN32
    #pragma comment(lib, "WINMM.lib")
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "wsock32.lib")
#endif

#include "AppGUI.h"

int main(int argc, char **argv) {
  
  AppGUI gui(argc, argv);
  
  try
  {
  	
  	gui.run();
  	
  }catch(...){
  	
  }
  
  return 0;
}