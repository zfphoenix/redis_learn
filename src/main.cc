//
#include <iostream>
#include "redis.h"
#include "container.h"

int main() {
	Redis red("127.0.0.1", 6677, 0);
	red.ping();
	//RedString kv(&red);
//	RedString::KVSet kvs;
//	std::cout << kv.Get("key001%") << std::endl;
//	kvs.push_back(std::make_pair<RedString::Key, RedString::Value>("key003%", "nnvalue001d"));

	//std::cout << kv.StrLen("key") << std::endl;
	//kvs.push_back(std::make_pair<RedString::Key, RedString::Value>("key002%", "value002"));
	/*
	ks.push_back("key002134529417");
	RedString::ValueSet vs = kv.MGet(ks) ;
	RedString::ValueSetItr itr = vs.begin();
	for (; itr != vs.end(); ++itr) {
		std::cout << *itr << std::endl;
	}
	std::cout << vs.size() << std::endl;*/

	//RedHash hash(&red);
/*	RedHash::FieldSet fs;
	fs.push_back("%%");
	fs.push_back("11%%");
	fs.push_back("name");*/
/*	RedHash::FVSet fvs;
	fvs.push_back(std::make_pair<RedHash::Field, RedHash::Value>("name", "new_name"));
	fvs.push_back(std::make_pair<RedHash::Field, RedHash::Value>("%%", "new_%%"));
	*/
	RedZSet set(&red);
	RedZSet::MemberSet ms;
	//sms.push_back(std::make_pair<RedZSet::Score, RedZSet::Member>(98,"tom"));
	ms.push_back("tom");
	std::cout << (set.ZRem("tset", ms))<< std::endl;
	return 0;
}
