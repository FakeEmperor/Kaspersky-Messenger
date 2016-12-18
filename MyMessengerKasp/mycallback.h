#pragma once
#include <functional>
#include "messenger.h"

using namespace messenger;

class MessageCallback : public IMessagesObserver
{
public:
	typedef std::function<void(const MessageId& msgId, message_status::Type status)> status_callback_type;
	typedef std::function<void(const UserId& senderId, const Message& msg)> recv_callback_type;
private:

	status_callback_type callback_status_;
	recv_callback_type callback_received_;

public:

	MessageCallback(const status_callback_type &callback_status,
						const recv_callback_type &callback_received) :
		callback_received_(callback_received), callback_status_(callback_status)
	{
	}

	virtual void OnMessageStatusChanged(const MessageId& msgId, message_status::Type status)
	{
		callback_status_(msgId, status);
	}
	virtual void OnMessageReceived(const UserId& senderId, const Message& msg)
	{
		callback_received_(senderId, msg);
	}
};

class DynamicAsyncCallback : public messenger::IAsyncOperationCallback {
	std::function<void(messenger::operation_result::Type)> callback;
public:
	virtual void OnOperationResult(messenger::operation_result::Type t) {
		callback(t);
	}

	DynamicAsyncCallback(const std::function<void(messenger::operation_result::Type)> &fn) :callback(fn) {

	}

	virtual bool callable() const
	{
		return (bool)this->callback;
	}

};


template<typename T>
class DynamicAsyncParametrizedCallback : public messenger::IAsyncOperationCallback
{
	std::function<void(messenger::operation_result::Type, const T&)> callback;

public:
	virtual void OnOperationResult(messenger::operation_result::Type t, const T& value) {
		callback(t, value);
	}

	DynamicAsyncParametrizedCallback(const std::function<void(messenger::operation_result::Type, const T&)> &fn) :callback(fn) {

	}

	virtual bool callable() const
	{
		return (bool)this->callback;
	}
};