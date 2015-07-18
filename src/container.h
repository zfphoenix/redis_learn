#ifndef KEYVALUE_H_
#define KEYVALUE_H_

#include <string>
#include "log.h"
class Container {
public:
	Container(Redis* redis) {

	}
	virtual ~Container() {
		release();
	}
	
	void release() {
		if (redis_->Reply()) {
				freeReplyObject(redis_->Reply());
		}
	}

	//after operate on db and invoke this func 
	bool check_status() {
 		if(redis_->Connect()->err)  {
			MLOG->Error("Connect ERR err is %d", redis_->Connect()->err);
			return false;
		}
		else if (!redis_->Reply()){
			MLOG->Error("ERR empty reply");
			return false;
		} else if (REPLY_TYPE_ERROR == redis_->Reply()->type) {
			MLOG->Error("Reply ERR err is %s", redis_->Reply()->str);
		} else {
			return true;
		}
	}

private:
	Redis *redis_;

};

class KeyValue : public Container {
public:
	KeyValue(Redis* redis):Container(redis) {

	}
	
	bool Set(std::string key, std::string value);

	bool Get(std::string key);

};


#endif
