#include "container.h"
#include "redis.h"
#include "log.h"

#include <sstream>

//throw the errors
int RedString::Set(Key k, Value v) {
	redisReply* reply = (redisReply*)redis_->execute("SET %s %s", k.c_str(), v.c_str()); int ret = -1; if (reply) {
		MLOG->Debug("reply type is %d", reply->type);
		if (reply->type == REDIS_REPLY_STATUS &&
				std::string(reply->str) == "OK")
			ret = 0;
	}
	freeReplyObject(reply);
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
		std::string str;
		str = transferEsc(itr->first);
		cmd.append(str);
		cmd.append(" ");
		str.clear();
		str = transferEsc(itr->second);
		cmd.append(str);
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

int RedString::MSetNx(KVSet& kvs) {
	std::string cmd;
	cmd.append("MSETNX ");
	KVSetItr itr = kvs.begin();
	for (; itr != kvs.end(); ++itr) {
		std::string str;
		str = transferEsc(itr->first);
		cmd.append(str);
		cmd.append(" ");
		str.clear();
		str = transferEsc(itr->second);
		cmd.append(str);
		cmd.append(" ");
	}
	if (7 == cmd.size()) return -2;
	MLOG->Debug("CMD is:||%s||",cmd.c_str());
	int ret = -1;
	redisReply* reply = (redisReply*)redis_->execute(cmd.c_str());
	if (reply) {
		if (reply->type == REDIS_REPLY_INTEGER
				&& reply->integer == 1)
			ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}

//throw error
int RedString::GetSet(Key k, Value new_v) {
	redisReply* reply = (redisReply*)redis_->execute("GETSET %s %s", k.c_str(), new_v.c_str());
	int ret = -1;
	if (reply) {
		ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}

//throw error
int RedString::SetRange(Key k, uint32_t index, Value replace) {
	redisReply* reply = (redisReply*)redis_->execute("SETRANGE %s %d %s", k.c_str(), index, replace.c_str());
	int ret = -1;
	if (reply) {
		ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}

RedString::Value RedString::GetRange(Key k, uint32_t start, uint32_t end) {
	redisReply* reply = (redisReply*)redis_->execute("GETRANGE %s %d %d", k.c_str(), start, end);
	Value val("");
	if (reply) {
		if (reply->type == REDIS_REPLY_STRING)
			val = std::string(reply->str);
	}
	freeReplyObject(reply);
	return val;
}

//throw error
int RedString::Append(Key k, Value append) {
	redisReply* reply = (redisReply*)redis_->execute("APPEND %s %s", k.c_str(), append.c_str());
	int ret = -1;
	if (reply) {
		ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}

size_t RedString::StrLen(Key k) {
	redisReply* reply = (redisReply*)redis_->execute("STRLEN %s", k.c_str());
	size_t ret = 0;
	if (reply) {
		if (reply->type == REDIS_REPLY_INTEGER)
			ret = reply->integer;
	}
	freeReplyObject(reply);
	return ret;
}

//class RedHash implementation

//throw error
int RedHash::HExists(Key k, Field f) {
	redisReply* reply = (redisReply*)redis_->execute("HEXISTS %s %s", k.c_str(), f.c_str());
	int ret = -1;
	if (reply) {
		if (reply->type == REDIS_REPLY_INTEGER &&
				1 == reply->integer)
			ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}

RedHash::Value RedHash::HGet(Key k, Field f) {
	redisReply* reply = (redisReply*)redis_->execute("HGET %s %s", k.c_str(), f.c_str());
	Value val("");
	if (reply) {
		if (reply->type == REDIS_REPLY_STRING)
			val = std::string(reply->str);
	}
	freeReplyObject(reply);
	return val;
}

//throw error
int RedHash::HDel(Key k, FieldSet& fs) {
	std::string cmd("HDEL ");
	cmd.append(k);
	cmd.append(" ");
	FieldSetItr itr = fs.begin();
	for (; itr != fs.end(); ++itr) {
		std::string str;
		str = transferEsc(*itr);
		cmd.append(str);
		cmd.append(" ");
	}
	MLOG->Debug("CMD is:||%s||",cmd.c_str());
	redisReply* reply = (redisReply*)redis_->execute(cmd.c_str());
	int ret = -1;
	if (reply) {
		MLOG->Debug("HDEL reply type is: %d error: %s", reply->type, reply->str);
		ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}

RedHash::Dict RedHash::HGetAll(Key k) {
	redisReply* reply = (redisReply*)redis_->execute("HGETALL %s", k.c_str());
	Dict dict;
	if (reply) {
		if (reply->type == REDIS_REPLY_ARRAY &&
				reply->elements && 0 == reply->elements%2) {
			for (size_t i = 0; i < reply->elements; ) {
				dict.push_back(std::make_pair< Key, Value >(reply->element[i]->str,
							reply->element[i+1]->str));
				i+=2;
			}
		}
	}
	freeReplyObject(reply);
	return dict;
}

RedHash::FieldSet RedHash::HKeys(Key k) {
	redisReply* reply = (redisReply*)redis_->execute("HKEYS %s", k.c_str());
	FieldSet fs;
	if (reply) {
		if (reply->type == REDIS_REPLY_ARRAY &&
				reply->elements) {
			MLOG->Debug("HKEYS size is %d", reply->elements);
			for (size_t i = 0; i < reply->elements; ++i) {
				fs.push_back(reply->element[i]->str);
			}
		}
	}
	freeReplyObject(reply);
	return fs;
}

size_t RedHash::HLen(Key k) {
	redisReply* reply = (redisReply*)redis_->execute("HLEN %s", k.c_str());
	size_t len = 0;
	if (reply && reply->type == REDIS_REPLY_INTEGER)
		len = reply->integer;
	freeReplyObject(reply);
	return len;
}

//throw inexistent field
RedHash::Dict RedHash::HMGet(Key k, FieldSet& fs) {
	Dict dict;
	if (!fs.size()) return dict;
	std::string cmd("HMGET ");
	cmd.append(k);
	cmd.append(" ");
	FieldSetItr itr = fs.begin();
	for (; itr != fs.end(); ++itr) {
		std::string str;
		str = transferEsc(*itr);
		cmd.append(str);
		cmd.append(" ");
	}
	MLOG->Debug("HMGET CMD is |%s|", cmd.c_str());
	redisReply* reply = (redisReply*)redis_->execute(cmd.c_str());
	if (reply) {
		if (reply->type == REDIS_REPLY_ARRAY &&
				reply->elements) {
			for (size_t i = 0; i < reply->elements; ++i) {
				if (reply->element[i]->type == REDIS_REPLY_STRING) {
					dict.push_back(std::make_pair< Key, Value > (fs[i], reply->element[i]->str));
					MLOG->Debug("HMGET reply is %s : %s", fs[i].c_str(), reply->element[i]->str);
				}
			}
		}
	}
	freeReplyObject(reply);
	return dict;
}

//throw error
int RedHash::HMSet(Key k, FVSet& fvs) {
	if (!fvs.size()) return -2;
	std::string cmd("HMSET ");
	cmd.append(k);
	cmd.append(" ");
	FVSetItr itr = fvs.begin();
	for(; itr != fvs.end(); ++itr) {
		std::string str;
		str = transferEsc(itr->first);
		cmd.append(str);
		cmd.append(" ");
		str.clear();
		str = transferEsc(itr->second);
		cmd.append(str);
		cmd.append(" ");
	}
	MLOG->Debug("HMSET CMD is |%s|", cmd.c_str());
	int ret = -1;
	redisReply* reply = (redisReply*)redis_->execute(cmd.c_str());
	if (reply) {
		ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}
	
int RedHash::HSet(Key k, Field f, Value v) {
	redisReply* reply = (redisReply*)redis_->execute("HSET %s %s %s", 
			k.c_str(), f.c_str(), v.c_str());
	int ret = -1;
	if (reply) {
		if (reply->type == REDIS_REPLY_INTEGER)
			ret = reply->integer;
	}
	freeReplyObject(reply);
	return ret;
}

int RedHash::HSetNx(Key k, Field f, Value v) {
	redisReply* reply = (redisReply*)redis_->execute("HSETNX %s %s %s", 
			k.c_str(), f.c_str(), v.c_str());
	int ret = -1;
	if (reply) {
		if (REDIS_REPLY_INTEGER == reply->type)
			ret = reply->integer ? 0 : 1;
	}
	freeReplyObject(reply);
	return ret;
}

//RedZSet class implementation

//throw error
int RedZSet::ZAdd(Key k, SMSet& sms) {
	if (!sms.size()) return -2;
	std::stringstream ss;
	ss << "ZADD " << k << " ";
	SMSetItr itr = sms.begin();
	for (; itr != sms.end(); ++itr) {
		ss << itr->first << " " << itr->second << " ";
	}
	std::string cmd = ss.str();
	ss.str("");
	ss.clear();
	MLOG->Debug("ZADD CMD is |%s|", cmd.c_str());
	redisReply* reply = (redisReply*)redis_->execute(cmd.c_str());
	int ret = -1;
	if (reply) {
		ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}

//throw error
int RedZSet::ZAdd(Key k, Score s, Member m) {
	//XXX 
	redisReply* reply = (redisReply*)redis_->execute("ZADD %s %f %s", 
			k.c_str(), s, m.c_str());
	int ret = -1;
	if (reply) {
		ret = 0;
	}
	freeReplyObject(reply);
	return ret;
}


//global funcs
std::string transferEsc(std::string str) {
	std::string ret;
	int i = 0;
	int left = 0;
	while (i < str.size()) {
		if (str[i] == '%') {
			ret.append(str, left, i - left);
			ret.append("%");
			left = i;
		}
		i++;
	}
	ret.append(str, left, i - left);
	return ret;
}



