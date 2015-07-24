//
#ifndef _RED_REDIS_H_
#define _RED_REDIS_H_
#include <iostream>
#include <cassert>
#include <string>
extern "C" {
#include <stdint.h>
#include <hiredis/hiredis.h>
}

#include "log.h"

//Max db number 255 default
typedef uint8_t		DBIndex;

//Redis class hold a connection with redis-server
//one Redis object is just manipulated on one specified db_index
//if wanna change to other db, better create one more object
class Redis {
public:
	Redis(std::string ip, uint32_t port, DBIndex db):
		ip_(ip), port_(port), db_id_(db)  {
			assert(connect() && select());
		}
	~Redis() {
		redisFree(connect_);
		connect_ = NULL;
	}
public:
	typedef		std::string		Key;
	typedef		uint32_t		ExpireTime;
public:
	bool connect() {
		connect_ = redisConnect(ip_.c_str(), port_);
		if (!connect_ || connect_->err) {
			MLOG->Error("CONNECT ERR err %d", connect_->err);
			return false;
		}
		return true;
	}
	
	bool reconnect() {
		redisFree(connect_);
		connect_ = NULL;
		connect();
		return select();
	}

	bool auth(std::string pwd) ;

	bool select() ;

	bool ping() ;

	int check(redisReply* reply);

	void release(redisReply*);

	void* execute(const char* format,...);
	//set expire time with key
	bool expire(Key k, ExpireTime ept);

	bool expireAt(Key k, uint32_t dead_tm);
	bool exists(Key k);
private:
	std::string   ip_;
	uint32_t	  port_;
	redisContext* connect_;
	DBIndex		  db_id_;
};

#endif // _RED_REDIS_H_

