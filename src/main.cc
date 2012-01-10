#ifdef WIN32
    #pragma comment(lib, "WINMM.lib")
    #pragma comment(lib, "ws2_32.lib")
    #pragma comment(lib, "wsock32.lib")
#endif

#include "Driver.h"

int main(int argc, char **argv) {
  
  Driver driver;
  driver.start();
  
  return 0;
}