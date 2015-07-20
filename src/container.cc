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

