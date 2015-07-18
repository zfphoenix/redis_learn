#ifndef __OPERATE_H_
#define __OPERATE_H_

#include <vector>
#include <string>
#include "redis.h"
extern "C" {
#include <stdint.h>
}
typedef std::string KEY;

//全局管理--待定
class Operate {
public:
	Operate(REDIS_OPTYPE type, redisContext* ctx, 
			KEY key, DBIndex idx):
		connect_(ctx), type_(type), key_(key),db_id_(idx)  {
	}
	virtual ~Operate();
	virtual redisReply* execute() = 0;
	REDIS_OPTYPE op_type() { return this->type_;}
protected:
	redisContext* connect_;
	REDIS_OPTYPE  type_;
	KEY			  key_;
	DBIndex		  db_id_;
/*private:
	void* operator new(size_t);
	void operator delete(void*);*/
};

class NormSetOperate : public Operate {
public:
	NormSetOperate(REDIS_OPTYPE type, redisContext* ctx, 
		KEY key, DBIndex idx): Operate(type, ctx, key, idx) {
	}
	redisReply* execute() {
		return (redisReply*)redisCommand(connect_, )
	}
};

#endif
