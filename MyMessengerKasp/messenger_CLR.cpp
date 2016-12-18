#pragma once
#pragma warning(disable:4075)
#include "stdafx.h"
#include "messenger_CLR.h"

namespace ManagedMessenger
{

	


	std::string marshal_to_string(String^ str)
	{
		msclr::interop::marshal_context context;
		return context.marshal_as<std::string>(str);
	}


	SecurityPolicy::SecurityPolicy(messenger::SecurityPolicy& sp)
	{
		Byte const* p = reinterpret_cast<Byte const*>(sp.encryptionPubKey.data());
		std::size_t size = sp.encryptionPubKey.size() * sizeof(sp.encryptionPubKey.front());
		//INIT
		this->type = sp.encryptionAlgo;
		this->key = gcnew cliext::vector<Byte>(p, p + size);
	}

	SecurityPolicy::operator messenger::SecurityPolicy()
	{
		messenger::SecurityPolicy sec_pol = messenger::SecurityPolicy();
		sec_pol.encryptionAlgo = encryption_algorithm::None;
		return sec_pol;
	}


	User::User(MyUser& u)
	{
		this->user = new MyUser(u);
		this->_cached = gcnew SecurityPolicy(this->user->get_secpol());
	}

	User::~User()
	{
		delete this->user;
		this->user = nullptr;
	}

	User::User(String^ username, String^ password, SecurityPolicy^ policy)
	{
		msclr::interop::marshal_context context;
		std::string native_username = context.marshal_as<std::string>(username);
		std::string native_password = context.marshal_as<std::string>(password);
		this->user = new MyUser(native_username, native_password, policy);
	}

	void ManagedMessenger::NativeLoginHandler(messenger::operation_result::Type type)
	{
		this->OnLogin(this, gcnew DataEventArgs<ResultStatus>(ResultStatus(type)));
	}

	void ManagedMessenger::NativeUserRequestHandler(messenger::operation_result::Type type, const std::vector<messenger::User> &users)
	{
		List<User^>^ li = gcnew List<User^>();
		for (auto u : users)
		{
			li->Add(gcnew User(MyUser(u.identifier, "", u.securityPolicy)));
		}
		this->OnUsersRequested(this, gcnew DataTwoEventArgs<ResultStatus, List<User^>^>(ResultStatus(type), li));
	}

	void ManagedMessenger::NativeMessageReceptionHandler(const UserId& senderId, const Message& msg)
	{
		this->OnReceivedMessage(this, gcnew DataTwoEventArgs<String^, ManagedMessage^>(gcnew String(senderId.c_str()), gcnew ManagedMessage(msg)) );
	}

	void ManagedMessenger::NativeMessageStatusHandler(const MessageId& msgId, message_status::Type status)
	{
		this->OnStatusMessage(this, gcnew DataTwoEventArgs<String^, MessageStatus>(gcnew String(msgId.c_str()), MessageStatus(status)));
	}

	ManagedMessenger::ManagedMessenger(const std::shared_ptr<::MyMessenger::MyMessenger> &msngr) : u_mgr(new std::shared_ptr<::MyMessenger::MyMessenger>(msngr))
	{

		this->memberNativeLoginHandler = gcnew ManagedLoginHandler(this, &ManagedMessenger::NativeLoginHandler);
		this->memberUserRequestHandler = gcnew ManagedUserRequestHandler(this, &ManagedMessenger::NativeUserRequestHandler);
		this->memberMessageReceivedHandler = gcnew ManagedMessageReceivedHandler(this, &ManagedMessenger::NativeMessageReceptionHandler);
		this->memberMessageStatusHandler = gcnew ManagedMessageStatusHandler(this, &ManagedMessenger::NativeMessageStatusHandler);

		auto raw_login_ptr = static_cast<void(*)(int)>(Marshal::GetFunctionPointerForDelegate(memberNativeLoginHandler).ToPointer());
		auto raw_requset_ptr = static_cast<void(*)(int, const std::vector<MyUser>&)>(Marshal::GetFunctionPointerForDelegate(memberUserRequestHandler).ToPointer());
		auto raw_msg_received_ptr = static_cast<void(*)(const UserId& senderId, const Message& msg)>(Marshal::GetFunctionPointerForDelegate(memberMessageReceivedHandler).ToPointer());
		auto raw_msg_status_ptr = static_cast<void(*)(const MessageId& msgId, message_status::Type status)>(Marshal::GetFunctionPointerForDelegate(memberMessageStatusHandler).ToPointer());

		login_callback = reinterpret_cast<messenger::ILoginCallback*>(
			new DynamicAsyncCallback(std::function<void(int)>(raw_login_ptr) )
			);

		request_user_callback = reinterpret_cast<messenger::IRequestUsersCallback*>(
			new DynamicAsyncParametrizedCallback<const std::vector<MyUser>&>(
				std::function<void(int, const std::vector<MyUser> &users)>(raw_requset_ptr))
		);

		message_callback = reinterpret_cast<messenger::IMessagesObserver*>(
			new MessageCallback( MessageCallback::status_callback_type(raw_msg_status_ptr), MessageCallback::recv_callback_type(raw_msg_received_ptr) )
		);
	}

	ManagedMessenger::~ManagedMessenger() {
		delete u_mgr;

		delete request_user_callback;
		delete login_callback;
		delete message_callback;
	}



}
