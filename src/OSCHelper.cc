#include "OSCHelper.h"

OSCHelper::OSCHelper()
	:out(buffer, buffer_size)
{
	socket = new UdpSocket();
}

void OSCHelper::connect(string ip, int port)
{
	ipEnd = new IpEndpointName(ip.c_str(), port);
}