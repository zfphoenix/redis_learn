//
#ifndef _RED_REDIS_H_
#define _RED_REDIS_H_
#include <iostream>
#include <string>

extern "C" {
#include <hiredis/hiredis.h>
}

class Redis {
public:
	Redis() {}
	~Redis() {
		connect_ = NULL;
		reply_	 = NULL;
	}
public:
	bool connect(std::string host, int port) {
		connect_ = redisConnect(host.c_str(), port);
		if (connect_ && connect_->err) {
			std::cout << "connect error : " << connect_->errstr << std::endl;
			return 0;
		}
		return 1;
	}

	std::string get(std::string key) {
		reply_ = (redisReply*)redisCommand(connect_, "GET %s", key.c_str());
		std::string str = reply_->str;
		freeReplyObject(reply_);
		return str;
	}

	void set(std::string key, std::string value) {
		redisCommand(connect_, "SET %s %s", key.c_str(), value.c_str());
	}
private:
	redisContext* connect_;
	redisReply*   reply_;
};

#endif // _RED_REDIS_H_

