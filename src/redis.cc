#include "redis.h"
#include <string>

bool Redis::auth(std::string pwd) {
	redisReply* reply_ = (redisReply*)execute("AUTH %s", pwd.c_str());
	int ret = check(reply_);
	if (!ret)
		return true;
	else 
		return false;
}

bool Redis::select() {
	redisReply* reply_ = (redisReply*)execute("SELECT %d", db_id_);
	int ret = check(reply_);
	release(reply_);
	if (!ret) {
		return true;
	} else 
		return false;
}

bool Redis::ping() {
	redisReply* reply_ = (redisReply*)execute("PING");
	int ret = check(reply_);
	if (!ret && reply_->type == REDIS_REPLY_STATUS &&
			std::string(reply_->str) == "PONG") {
		MLOG->Debug("PING OK %d", ret);
		return true;
	}
	release(reply_);
	MLOG->Debug("PING ERROR %d", ret);
	return false;
}

inline int Redis::check(redisReply* reply_) {
	if (connect_ && connect_->err)
		return connect_->err;
	if (!connect_)
		return -1;
	if (reply_ && reply_->type == REDIS_REPLY_ERROR)
		return -2;
	if (!reply_)
		return -3;
	return 0;
}

inline void Redis::release(redisReply* reply_) {
	freeReplyObject(reply_);
}

//while redisContext returns error, the connection would be disconnected
//and always reconnect to redis-server when error detected;
void* Redis::execute(const char* format,...) {
	va_list ap;
	void* reply = NULL;
	va_start(ap, format);
	reply = redisvCommand(connect_, format, ap);
	if (connect_ && connect_->err)
		reconnect();
	va_end(ap);
	return reply;
}

//[expire] operation will be replaced
bool Redis::expire(Key k, ExpireTime ept) {
	redisReply* reply = (redisReply*)execute("EXPIRE %s %d", k.c_str(), ept);
	bool ret = false;
	if (reply) {
		if (REDIS_REPLY_INTEGER == reply->type && 1 == reply->integer)
			ret = true;
	}
	freeReplyObject(reply);
	return ret;
}

bool Redis::exists(Key k) {
	redisReply* reply = (redisReply*)execute("EXISTS %s", k.c_str());
	bool ret = false;
	if (reply) {
		if (REDIS_REPLY_INTEGER == reply->type && 1 == reply->integer)
			ret = true;
	}
	freeReplyObject(reply);
	return ret;
}


