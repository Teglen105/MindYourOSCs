#include "Handler.h"

void Handler::connect(string ip, int port)
{
	osc.connect(ip, port);
}