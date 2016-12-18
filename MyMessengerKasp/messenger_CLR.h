#pragma once
#include "stdafx.h"

namespace ManagedMessenger
{

	std::string marshal_to_string(System::String^ str);

	template <typename T>
	std::vector<T> marshal_to_vector(System::Collections::Generic::ICollection<T>^ list)
	{
		if (list == nullptr) throw gcnew ArgumentNullException(L"list or no list, that is the question.");
		std::vector<T> result(list->Count);
		for each (S& elem in list)
			result.push_back(marshal_as<T>(elem));
		return result;
	}


	public ref struct SecurityPolicy {
		messenger::encryption_algorithm::Type type;
		cliext::vector<Byte>^ key;
	internal:

		SecurityPolicy(messenger::SecurityPolicy& sp);
		

		operator messenger::SecurityPolicy();
	public:
		SecurityPolicy(): type(messenger::encryption_algorithm::None) {}

		SecurityPolicy(messenger::encryption_algorithm::Type type): type(type)
		{
			
		}


	};

	public ref class User {
	internal:
		//TODO: scoped_ptr
		MyUser* user;
		SecurityPolicy^ _cached;

		User(MyUser& u);
		~User();

		MyUser ToNative()
		{
			return *user;
		}

	protected:

	public:

		


		User(System::String^ username, System::String^ password, SecurityPolicy^ policy);

		//define properties
		property System::String^ Username {
			System::String^ get() {
				return gcnew System::String(user->get_username().c_str());
			}
		}

		property System::String^ Password {
			System::String^ get() {
				return gcnew System::String((this->user->get_password().data()));
			}
		}

		property System::String^ UserId {
			System::String^ get() {
				return gcnew System::String(user->get_uid().c_str());
			}
		}

		property SecurityPolicy^ Security {
			SecurityPolicy^ get() {
				return this->_cached;
			}
			void set(SecurityPolicy^ policy)
			{
				this->_cached = policy;
				this->user->get_secpol() = static_cast<messenger::SecurityPolicy>(policy);
			}
		}


	};

	generic <typename TData>
	public ref class DataEventArgs : public EventArgs {
	public:
		TData Data;
	
		DataEventArgs(TData data)
		{
			this->Data = data;
		}
	};
	
	generic <typename TData1, typename TData2>
	public ref class DataTwoEventArgs : public EventArgs {
	public:
		TData1 Data1;
		TData2 Data2;
		
		DataTwoEventArgs(TData1 data1, TData2 data2)
		{
			this->Data1 = data1;
			this->Data2 = data2;
		}
	};

	public ref class Times {
	public:
		static DateTime time_t2DateTime(std::time_t date) {
			double msec = static_cast<double>(date);
			return DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind::Utc).AddMilliseconds(msec);

		}
		static std::time_t DateTime2time_t(DateTime date) {
			TimeSpan diff = date.ToUniversalTime() - DateTime(1970, 1, 1);
			return static_cast<std::time_t>(diff.TotalMilliseconds);

		}
	};

	public ref class ManagedMessage
	{
	internal:
		::Message *u_msg;

		ManagedMessage(Message msg) : u_msg(new Message(msg)) {};

		~ManagedMessage()
		{
			delete u_msg;
		}
	public:
		ManagedMessage() :u_msg(new Message()) {};
		

		property String^ Text {
			String^ get() {
				return gcnew String(std::string((char*)u_msg->content.data.data(), u_msg->content.data.size()).c_str());
			}
			void set(System::String^ msg)
			{
				auto str = marshal_to_string(msg);
				u_msg->content.data.assign(str.data(), str.data() + str.size());
			}
		};
		property String^ Id {
			String^ get()
			{
				return gcnew String(u_msg->identifier.c_str());
			}
			void set(System::String^ msg)
			{
				u_msg->identifier = marshal_to_string(msg);
			}
		}

		property Boolean Encrypted {
			Boolean get()
			{
				return u_msg->content.encrypted;
			}
			void set(Boolean encrypted)
			{
				u_msg->content.encrypted = encrypted;
			}
		}

		property DateTime Time {
			DateTime get()
			{
				return Times::time_t2DateTime(u_msg->time);
			}
			void set(DateTime time)
			{
				u_msg->time = Times::DateTime2time_t(time);
			}
		}
	};

	public enum class ResultStatus
	{
		Ok,
		AuthError,
		NetworkError,
		InternalError
	};

	public enum class MessageStatus
	{
		Sending,
		Sent,
		FailedToSend,
		Delivered,
		Seen
	};

	public ref class ManagedMessenger {
	public:
		typedef EventHandler<DataEventArgs<ResultStatus>^> StatusDelegate;
		typedef EventHandler<DataTwoEventArgs<ResultStatus, List<User^>^>^> UserRequestDelegate;
		typedef EventHandler<DataTwoEventArgs<String^, ManagedMessage^>^> MessageReceptionDelegate;
		typedef EventHandler<DataTwoEventArgs<String^, MessageStatus>^> MessageStatusDelegate;

	private:
	internal:
		delegate void ManagedLoginHandler(messenger::operation_result::Type);
		delegate void ManagedUserRequestHandler(messenger::operation_result::Type, const std::vector<messenger::User>&);
		delegate void ManagedMessageReceivedHandler(const UserId& senderId, const Message& msg);
		delegate void ManagedMessageStatusHandler(const MessageId& msgId, message_status::Type status);



		std::shared_ptr<::MyMessenger::MyMessenger>* u_mgr;
		System::Delegate^ memberNativeLoginHandler;
		System::Delegate^ memberUserRequestHandler;
		System::Delegate^ memberMessageReceivedHandler;
		System::Delegate^ memberMessageStatusHandler;
				
		messenger::ILoginCallback *login_callback;
		messenger::IRequestUsersCallback *request_user_callback;
		messenger::IMessagesObserver *message_callback;
				
		String ^url;
		int port;

		void NativeLoginHandler(messenger::operation_result::Type type);
		void NativeUserRequestHandler(messenger::operation_result::Type type, const std::vector<messenger::User> &users);
		void NativeMessageReceptionHandler(const UserId& senderId, const Message& msg);
		void NativeMessageStatusHandler(const MessageId& msgId, message_status::Type status);

		////// EVENT QUEUES
		UserRequestDelegate^ user_request__event_queue_ = { nullptr };
		StatusDelegate^ login__event_queue_ = { nullptr };
		MessageReceptionDelegate^ message_reception__event_queue_ = { nullptr };
		MessageStatusDelegate^ message_status__event_queue_ = { nullptr };


		ManagedMessenger(const std::shared_ptr<::MyMessenger::MyMessenger> &msngr);
		~ManagedMessenger();
	public:
				

		ManagedMessenger(String^ url, Int32 port): ManagedMessenger(MyMessenger::MyMessenger::GetInstance())
		{
			this->url = url;
			this->port = port;
		}

		void BeginRequestUsers()
		{
			(*u_mgr)->RequestActiveUsers(this->request_user_callback);
		}

		void BeginLogin(User^ user)
		{
			(*u_mgr)->Login(user->ToNative(), this->login_callback);
		}

		ManagedMessage^ BeginSend(User^ user, String^ message)
		{
			MessageContent cnt;
			auto str = marshal_to_string(message);
			cnt.data.assign(str.begin(), str.end());
			return gcnew ManagedMessage((*u_mgr)->Send(user->ToNative(), cnt));
		}

		void Connect()
		{
			try
			{
				(*u_mgr)->Connect(this->message_callback, messenger::MessengerSettings(marshal_to_string(url), port));
			}
			catch (const std::exception &e) {
				System::String^ what = gcnew System::String((std::string("Unmanaged code throws error on ManagedMessenger::Connect(): ") + e.what()).c_str());
				System::Exception^ ex = gcnew System::Exception(what);
				throw ex;
			}
		}

		void Disconnect()
		{
			(*u_mgr)->Disconnect();
		}




		event UserRequestDelegate^ OnUsersRequested {
			void add(UserRequestDelegate ^ d) {
				this->user_request__event_queue_ += d;
			}
			void remove(UserRequestDelegate ^ d) {
				this->user_request__event_queue_ -= d;
			}
	internal:
			void raise(Object^ target, DataTwoEventArgs<ResultStatus, List<User^>^>^ args) {
				UserRequestDelegate^ tmp = this->user_request__event_queue_;
				if (tmp) {
					tmp->Invoke(target, args);
				}
			}
		};

		event StatusDelegate^ OnLogin {
			void add(StatusDelegate^ d) {
				this->login__event_queue_ += d;
			}
			void remove(StatusDelegate^ d) {
				this->login__event_queue_ -= d;
			}
	internal:
			void raise(Object^ target, DataEventArgs<ResultStatus>^ args) {
				StatusDelegate^ tmp = this->login__event_queue_;
				if (tmp) {
					tmp->Invoke(target, args);
				}
			}
		};
		


		event MessageReceptionDelegate^ OnReceivedMessage {
			void add(MessageReceptionDelegate^ d) {
				this->message_reception__event_queue_ += d;
			}
			void remove(MessageReceptionDelegate^ d) {
				this->message_reception__event_queue_ -= d;
			}
		internal:
			void raise(Object^ target, DataTwoEventArgs<String^, ManagedMessage^>^ args) {
				MessageReceptionDelegate^ tmp = this->message_reception__event_queue_;
				if (tmp) {
					tmp->Invoke(target, args);
				}
			}
		};


		event MessageStatusDelegate^ OnStatusMessage {
			void add(MessageStatusDelegate^ d) {
				this->message_status__event_queue_ += d;
			}
			void remove(MessageStatusDelegate^ d) {
				this->message_status__event_queue_ -= d;
			}
		internal:
			void raise(Object^ target, DataTwoEventArgs<String^, MessageStatus>^ args) {
				MessageStatusDelegate^ tmp = this->message_status__event_queue_;
				if (tmp) {
					tmp->Invoke(target, args);
				}
			}
		};
		/////////////////////// PROPERTY ////////////////////

	};

}