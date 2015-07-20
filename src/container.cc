#include "container.h"
#include "redis.h"
#include "log.h"

/*
void Container::release() {
	if (redis_->Reply()) {
		freeReplyObject(redis_->Reply());
	}
}

//after operate on db and invoke this func 
bool Container::check_status() {
	if(redis_->Connect()->err)  {
		MLOG->Error("Connect ERR err is %d", redis_->Connect()->err);
		return false;
	}
	else if (!redis_->Reply()){
		MLOG->Error("ERR empty reply");
		return false;
	} else if (REPLY_TYPE_ERROR == redis_->Reply()->type) {
		MLOG->Error("Reply ERR err is %s", redis_->Reply()->str);
		return false;
	} else {
		return true;
	}
}*/

//throw the errors
int RedString::Set(Key k, Value v) {
	redisReply* reply = (redisReply*)redis_->execute("SET %s %s", k.c_str(), v.c_str()); int ret = -1; if (reply) {
		MLOG->Debug("reply type is %d", reply->type);
		if (reply->type == REDIS_REPLY_STATUS &&
				std::string(reply->str) == "OK")
			ret = 0;
	}
	return ret;
}

RedString::Value RedString::Get(Key k) {
	redisReply* reply = (redisReply*)redis_->execute("GET %s", k.c_str());
	Value val("");
	if (reply) {
		if (reply->type == REDIS_REPLY_STRING)
			val = std::string(reply->str);
		MLOG->Debug("val is %s", val.c_str());
	}
	freeReplyObject(reply);
	return val;
}

int RedString::SetNx(Key k, Value v) {
	redisReply* reply = (redisReply*)redis_->execute("SETNX %s %s", k.c_str(), v.c_str());
	int ret = -1;
	if (reply) {
		if (reply->type == REDIS_REPLY_INTEGER &&
				reply->integer == 1)
			ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}

int RedString::MSet(KVSet& kvs) {
	std::string cmd;
	cmd.append("MSET ");
	KVSetItr itr = kvs.begin();
	for (; itr != kvs.end(); ++itr) {
		cmd.append(itr->first);
		cmd.append(" ");
		cmd.append(itr->second);
		cmd.append(" ");
	}
	if (5 == cmd.size()) return -2;
	MLOG->Debug("CMD is:||%s||",cmd.c_str());
	redisReply* reply = (redisReply*)redis_->execute(cmd.c_str());
	int ret = -1;
	if (reply) {
		ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}

RedString::ValueSet RedString::MGet(KeySet& ks) {
	std::string cmd;
	ValueSet vs;
	cmd.append("MGET ");
	KeySetItr itr = ks.begin();
	for (; itr != ks.end(); ++itr) {
		cmd.append(*itr);
		cmd.append(" ");
	}
	if (5 == cmd.size()) return vs;
	MLOG->Debug("CMD is:||%s||",cmd.c_str());
	redisReply* reply = (redisReply*)redis_->execute(cmd.c_str());
	if (reply && reply->type == REDIS_REPLY_ARRAY) {
		for (int i = 0; i < reply->elements; ++i) {
			vs.push_back(Value(reply->element[i]->str));
		}
	}
	freeReplyObject(reply);
	return vs;
}

