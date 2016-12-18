#pragma once
#include "stdafx.h"
#include "MyUser.h"
#include "mycallback.h"
#include <functional>

namespace MyMessenger {

	enum Messenger_status
	{ CONNECTED, DISCONNECTED, SHIT_HAPPENS};

	class MyMessenger : public std::enable_shared_from_this<MyMessenger>
	{
		static messenger::MessengerSettings			DEFAULT_SERVER_SETTINGS;
		std::shared_ptr<messenger::IMessenger>		msg_i;		// pointer to main IMessenger class
		static std::shared_ptr<MyMessenger>			instance;
		Messenger_status							state = { DISCONNECTED };
		std::shared_ptr<MyUser>						active_user;
		messenger::MessengerSettings				cur_settings;

		ILoginCallback* login_callback_ = { nullptr };
		IMessagesObserver* message_callback_ = { nullptr };

		std::shared_ptr<DynamicAsyncCallback>		connected_callback_;

		void connected_callback_handler(messenger::operation_result::Type code)
		{
			if (code == decltype(code)::Ok)
				this->state = CONNECTED;
			else
				this->state = DISCONNECTED;
			if (this->login_callback_)
				this->login_callback_->OnOperationResult(code);
			this->login_callback_ = nullptr;
			if (message_callback_)
				msg_i->RegisterObserver(message_callback_);
		}
		
		MyMessenger()
		{
			cur_settings = DEFAULT_SERVER_SETTINGS;
			connected_callback_ = std::make_shared<DynamicAsyncCallback>(std::bind(&MyMessenger::connected_callback_handler, this, std::placeholders::_1));
			this->msg_i = GetMessengerInstance(cur_settings);
		}

	public:
		MyMessenger(const MyMessenger&) = delete;

		virtual ~MyMessenger() {

		}

		static std::shared_ptr<MyMessenger> GetInstance() throw() {
			if (!MyMessenger::instance)
				MyMessenger::instance.reset(new MyMessenger());
			return MyMessenger::instance;
		}

		//void SetUserCred(string login, string pass)
		//{
		//	std::shared_ptr<MyUser> new_user(new MyUser(login, pass));
		//	this->active_user = new_user;
		//}


		void Connect(messenger::IMessagesObserver *message_callback, messenger::MessengerSettings settings = MyMessenger::DEFAULT_SERVER_SETTINGS, const bool reconnect = false) throw() {
			bool connect = reconnect; // connect - true, reconnect in any way. connect == false - connect, only if not connected or new connection
			if (!connect && (CONNECTED == state)) {
				if (settings.serverUrl != cur_settings.serverUrl || settings.serverPort != cur_settings.serverPort) {
					try {
						
						this->Disconnect();
					}
					catch (...) {}
				}
			}
			else
				connect = true;
			////////////////////////
			cur_settings = settings;
			////////////////////////
			if (connect)
			{
				msg_i = messenger::GetMessengerInstance(settings);
				//else - all is ok
				this->message_callback_ = message_callback;
				if (message_callback)
					msg_i->RegisterObserver(message_callback);
			}

		}

		Message Send(const MyUser &user, const MessageContent& msg_content)
		{
			return msg_i->SendMessage(user.get_username(), msg_content);
		}

		void Login(const MyUser &user, messenger::ILoginCallback *login_callback)
		{
			if (!this->active_user || *(this->active_user) != user ) {
				this->active_user.reset(new MyUser(user));
				if (state == CONNECTED)
					this->Disconnect();
				if (this->login_callback_)
					throw std::exception("Login in progress!");
				else
					this->login_callback_ = login_callback;

				this->msg_i->Login(active_user->get_username()+"@localhost", active_user->get_password(),
									this->active_user->get_secpol(), reinterpret_cast<ILoginCallback*>(this->connected_callback_.get()));
				
			}
		}

		void Disconnect()
		{
			if (state == CONNECTED) {
				this->msg_i->Disconnect();
				this->state = DISCONNECTED;
			}
		}

		void RequestActiveUsers(IRequestUsersCallback *callback)
		{
			this->msg_i->RequestActiveUsers(callback);
		}

	};

	
}