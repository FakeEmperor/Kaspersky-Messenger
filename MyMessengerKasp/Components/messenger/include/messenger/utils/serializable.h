#include <json/json.h>
#include <exception>
#pragma once

namespace utils {
	
	class ISerializable {
	protected:
		virtual void Deserialize(const Json::Value value) const = 0;
		//TODO: Make this a pure virtual call and realize it in all subclasses
		virtual void Deserialize(const Json::Reader reader) const {
			throw std::logic_error("Not implemented");
		}
	
	public:
		virtual Json::Value Serialize() const = 0;
	};

}
