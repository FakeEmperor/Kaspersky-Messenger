#pragma once
#include "mymessenger.h"
#include <memory>

messenger::MessengerSettings MyMessenger::MyMessenger::DEFAULT_SERVER_SETTINGS = { "localhost", 122 };
std::shared_ptr<MyMessenger::MyMessenger> MyMessenger::MyMessenger::instance = nullptr;