#ifndef KEYVALUE_H_
#define KEYVALUE_H_

#include <string>
#include <map>
#include <vector>

extern "C" {
#include <stdint.h>
}

class Redis;

enum RStatus {


};

class Container {
public:
	Container(Redis* redis) {

	}
	virtual ~Container() {
		release();
	}
	
	void release();

	//after operate on db and invoke this func 
	bool check_status();
 	
private:
	Redis *redis_;

};

//KeyValue class used to manipulate normal
//[key-value] data structure
class RedString : public Container {
public:
	typedef std::string								Key;
	typedef std::string								Value;
	typedef std::vector< std::string >				KeySet;
	typedef std::vector< std::string >				ValueSet;
	typedef KeySet::iterator						KeySetItr;
	typedef Valueset::iterator						ValueSetItr;
	typedef std::vector< std::pair< Key, Value > >	KVSet;
	typedef KVSet:iterator					        KVSetItr;
public;
	RedString(Redis* redis):Container(redis) {

	}
	
	//set key value if not exist
	//else update key's value with 'v'
	int Set(Key k, Value v);
	//get key's value if exist
	Value Get(Key k);
	//set if not exists
	int SetNx(Key k, Value v);
	//multi set
	int MSet(KVSet kvs);
	//multi get
	MValue MGet(KeySet ks);
	//multi set if not exists
	int MSetNx(KVSet kvs);
	//if k's value exists, update value with new_v
	//else set [k - new_v]
	int GetSet(Key k, Value new_v);
	//change key's value if exist
	//and replace part value with 'replace' string start from 
	//index-th char
	int SetRange(Key k, uint32_t index, Value replace);
	//return part key's value which in range[start, end]
	Value GetRange(Key k, uint32_t start, uint32_t end);
	//update k'value and append value with 'append'
	int Append(Key k, Value append);
	//return key's value's length,unit is byte
	size_t StrLen(Key k);
	//!!!
	//!!!
	//!!!below funcs were only invoked while key's value is type-Integer
	//increase key's value with 1
	// -1 represent decrease 1
	int Incr(Key k);
	//increase key's value with num
	int IncrBy(Key k, uint32_t num);
	//decrease key's value with 1
	//-1 represent increase 1
	int Decr(Key k);
	//decrease key's value with num
	int DecrBy(Key k, uint32_t num);
};

//Hash Key : field1-value1 field2-value2 field3-value3....
class RedHash : public Container {
public:
	typedef	std::string								 Key;
	typedef std::string								 Field;
	typedef std::string								 Value;
	typedef std::vecotr< Key >						 FieldSet;
	typedef FieldSet::iterator						 FieldSetItr;
	typedef FieldSet								 ValueSet;
	typedef FieldSetItr								 ValueSetItr;
	typedef std::vector< std::pair< Field, Value > > Dict;
	typedef Dict::iterator							 DictItr;
	typedef Dict									 FVSet
	typedef DictItr									 FVSetItr;
};

#endif
