#ifndef KEYVALUE_H_
#define KEYVALUE_H_

#include <string>
#include <map>
#include <vector>

extern "C" {
#include <stdint.h>
}

class Redis;

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
	typedef std::vector< std::string >				MKey;
	typedef std::vector< std::string >				MValue;
	typedef MKey::iterator							MKeyItr;
	typedef MValue::iterator						MValueItr;
	typedef std::vector< std::pair< Key, Value > >	MKValue;
	typedef MKValue::iterator					    MKValueItr;
public;
	RedString(Redis* redis):Container(redis) {

	}
	
	//set key value if not exist
	//else update key's value with 'v'
	bool Set(Key k, Value v);
	//get key's value if exist
	Value Get(Key k);
	//set if not exists
	bool SetNx(Key k, Value v);
	//multi set
	bool MSet(MKValue mkv);
	//multi get
	MValue MGet(MKey mk);
	//multi set if not exists
	bool MSetNx(MKValue mkv);
	//if k's value exists, update value with new_v
	//else set [k - new_v]
	bool GetSet(Key k, Value new_v);
	//change key's value if exist
	//and replace part value with 'replace' string start from 
	//index-th char
	bool SetRange(Key k, uint32_t index, Value replace);
	//return part key's value which in range[start, end]
	Value GetRange(Key k, uint32_t start, uint32_t end);
	//update k'value and append value with 'append'
	bool Append(Key k, Value append);
	//return key's value's length,unit is byte
	size_t StrLen(Key k);
	//!!!
	//!!!
	//!!!below funcs were only invoked while key's value is type-Integer
	//increase key's value with 1
	// -1 represent decrease 1
	bool Incr(Key k);
	//increase key's value with num
	bool IncrBy(Key k, uint32_t num);
	//decrease key's value with 1
	//-1 represent increase 1
	bool Decr(Key k);
	//decrease key's value with num
	bool DecrBy(Key k, uint32_t num);
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
	typedef Dict									 MFValue;
	typedef DictItr									 MFValueItr;
};

#endif
