//
#ifndef _RED_REDIS_H_
#define _RED_REDIS_H_
#include <iostream>

extern "C" {
#include <stdint.h>
#include <hiredis/hiredis.h>
}

#include "log.h"

//Max db number 255 default
typedef uint8_t		DBIndex;

class Redis {
public:
	Redis(std::string ip, uint32_t port, DBIndex db):
		ip_(ip), port_(port), db_id_(db)  {}
	~Redis() {
		connect_ = NULL;
		reply_	 = NULL;
	}
public:
	bool connect(){
		connect_ = redisConnect(host.c_str(), port);
		if (connect_ && connect_->err) {
			MLOG->Error("CONNECT ERR err %d", connect_->err);
			return false;
		}
		return true;
	}

	bool auth() {
	//TODO
	}

	bool select() {
	//TODO
	}

	redisReply* Reply() const {
		return reply_;
	}

	redisContext* Connect() const {
		return connect_;
	}

private:
	std::string   ip_;
	uint32_t	  port_;
	redisContext* connect_;
	redisReply*   reply_;
	DBIndex		  db_id_;
};

#endif // _RED_REDIS_H_

