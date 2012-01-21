#ifndef OSCHELPER_H_
#define OSCHELPER_H_

#include<string>
using std::string;

#include <ip/UdpSocket.h>
#include <osc/OscOutboundPacketStream.h>

#include<iostream>
using std::cout; using std::endl;

class OSCHelper {

public:
	OSCHelper();

	template<typename T> void sendMsg(string msg, T val)
	{
		out.Clear();
		out << osc::BeginBundleImmediate
			<< osc::BeginMessage(msg.c_str())
			<< (T)val
			<< osc::EndMessage
			<< osc::EndBundle;
		socket->SendTo( *ipEnd, out.Data(), out.Size() );
	}
	void connect(string ip, int port);

private:
	UdpSocket* socket;
	IpEndpointName* ipEnd;
	osc::OutboundPacketStream out;

	static const int buffer_size = 64;
	char buffer[buffer_size];	
};

#endif //OSCHELPER_H_