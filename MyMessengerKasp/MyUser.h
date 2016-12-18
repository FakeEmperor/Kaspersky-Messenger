#pragma once
#include "stdafx.h"
#include <string>
#include "messenger.h"
#include "utils\utils.h"
#include "utils\rand.h"

using namespace messenger;

class MyUser : public std::enable_shared_from_this<MyUser>
{
	std::string		_username;
	UserId			_uid;
	std::string		_password;
	SecurityPolicy	_securityPolicy;
	
	static messenger::UserId RandomUid()
	{
		auto vec = utils::rand::getrandbytes(16);
		return "uid"+ utils::str2hex(std::string(vec.begin(), vec.end()));
	}

public:
	MyUser(std::string usrname, std::string pass, SecurityPolicy sp)
	{
		this->_securityPolicy = sp;
		this->_username = usrname;
		this->_password = pass;
		//this->_uid = this->RandomUid();

		
	}

	bool operator==(const MyUser &u) const
	{
		return u._uid == this->_uid && u._username == this->_username && u._password == this->_password;
	}

	bool operator!=(const MyUser &u) const {
		return !this->operator==(u);
	}

	UserId get_uid()			const	{ return this->_uid; }
	std::string get_username() const			{ return this->_username; }
	std::string get_password()	const		{ return this->_password; }
	SecurityPolicy get_secpol()	const	{ return this->_securityPolicy; }
};