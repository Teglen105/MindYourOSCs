#ifndef HANDLER_H_
#define HANDLER_H_

#include <string>
using std::string;

#include <edk/edk.h>
#include <edk/edkErrorCode.h>
#include <edk/EmoStateDLL.h>

#include <boost/thread.hpp>

#include "OSCHelper.h"

class Handler {

public:
	void connect(string ip, int port);

protected:
	OSCHelper osc;

};

#endif //HANDLER_H_