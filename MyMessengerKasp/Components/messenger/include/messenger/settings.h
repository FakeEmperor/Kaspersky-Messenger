#ifndef MESSENGER_SETTINGS_H
#define MESSENGER_SETTINGS_H

#include "types.h"

namespace messenger
{
  
struct MessengerSettings
{
    std::string    serverUrl;
    unsigned short serverPort;
    
    MessengerSettings() : serverPort(0) {}
	MessengerSettings(const std::string &url, unsigned short port) : serverUrl(url), serverPort(port) {};
};
    
}

#endif /* MESSENGER_SETTINGS_H */
