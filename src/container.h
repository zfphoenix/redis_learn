//
//Redis server v=2.8.4 sha=00000000:0 malloc=jemalloc-3.4.1 bits=32 build=8f6097d7914679ca
//
//
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
	Container(Redis* redis):redis_(redis) {

	}
	virtual ~Container() {
	}
	
protected:
	Redis *redis_;
private:
	Container(const Container*);
	void operator=(const Container*);
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
	typedef ValueSet::iterator						ValueSetItr;
	typedef std::vector< std::pair< Key, Value > >	KVSet;
	typedef KVSet::iterator					        KVSetItr;
public:
	RedString(Redis* redis):Container(redis) {

	}
	
	/*
	 *@brief	set key value if not exist else update key's value with 'v'
	 *@command	SET [key] [value]
	 *@return	0 succ, else err-code
	 */
	int Set(Key k, Value v);
	/*
	**@brief	get key's value if exist
	**@command	GET [key]
	**@return	value
	*/
	Value Get(Key k);
	/*
	*@brief		set if not exists
	*@command	SETNX [key] [value]
	*@return	0 succ, else  -1
	*/
	int SetNx(Key k, Value v);
	/*
	 *@brief	multi set
	 *@command	MSET [key1] [value1] [key2] [value2] ....
	 @return	0 succ, else -1
	 **/	
	int MSet(KVSet& kvs);
	/*
	 *@brief	multi get
	 *@command	MGET [key1] [key2] [key3] ...
	 *@return	ValueSet with reply succ ,else empty set
	 */
	ValueSet MGet(KeySet& ks);
	/*
	*@brief		multi set if not exists
	*@command	MSETNX [key1] [value1] [key2] [value2] ...
	*@return	0 succ, else -1
	*/
	int MSetNx(KVSet& kvs);
	/*
	**@brief	if k's value exists, update value with new_v
	**			else set [k - new_v]
	*@command	GETSET [key] [value]
	*@retrun	0 succ, else -1
	*/
	int GetSet(Key k, Value new_v);
	/*
	*@breif		change key's value if exist
	**			and replace part value with 'replace' string start from 
	**			index-th char
	*@cmmand	SETRANGE [key] [offset] [value]
	*@return	0 succ, else -1
	*/
	int SetRange(Key k, uint32_t index, Value replace);
	/*
	*@brief		return part key's value which in range[start, end]
	*@command	SETRANGE [key] [start] [end]
	*@return	Value with reply succ, else empty value
	*/
	Value GetRange(Key k, uint32_t start, uint32_t end);
	/*
	*@brief		update k'value and append value with 'append'
	*@command	APPEND [key] [value]
	*@return	0 succ, else -1
	*/
	int Append(Key k, Value append);
	/*
	*@brief		return key's value's length,unit is byte
	*@command	STRLEN [key]
	*@return	len (ne 0) succ, else -1
	*/
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
	typedef std::vector< Key >						 FieldSet;
	typedef FieldSet::iterator						 FieldSetItr;
	typedef FieldSet								 ValueSet;
	typedef FieldSetItr								 ValueSetItr;
	typedef std::vector< std::pair< Field, Value > > Dict;
	typedef Dict::iterator							 DictItr;
	typedef Dict									 FVSet;
	typedef DictItr									 FVSetItr;
public:
	RedHash(Redis* redis): Container(redis) {
	
	}
public:
	/*
	 *@brief	check if exists a hash structuce named 'k' and if exists a field
	 **			named 'f'
	 *@command	HEXISTS [key] [field]
	 *@return	0 succ, else -1
	 */
	int HExists(Key k, Field f);
	/*
	*@brief		Get the 'f' filed value which in 'k' hash
	*@command	HGET [key] [field]
	*@return	value with reply, else empty value
	*/
	Value HGet(Key k, Field f);
	/*
	*@brief		delete field set which in the 'k' hash
	*@command	HDEL [key] [field1] [field2] ...
	*@return	0 succ, else -1
	*/
	int HDel(Key k, FieldSet& fs);
	/*
	*@brief		get all field and value of 'k' hash
	*@command	HGETALL [key]
	*@return	Dict with reply, else empty Dict
	*/
	Dict HGetAll(Key k);
	/*
	*@brief		get all fileld name in 'k' hash
	*@command	HKEYS [key]
	*@return	FieldSet with reply, else empty fieldSet
	*/
	FieldSet HKeys(Key k);
	/*
	*@brief		get field number of 'k' hash
	*@command	HLEN [key]
	*@return	integer with reply
	*/
	size_t HLen(Key k);
	/*
	*@brief		get correspond value of 'fs' which in  'k' hash
	*@command	HMGET [key] [field1] [field2]...
	*@return	ValueSet with reply ,else empley ValueSet
	*/
	Dict HMGet(Key k, FieldSet& fs);
	/*
	*@brief		atomic set multic filed - value of 'k' hash, 
	**			if 'k' not exist, create it
	*@command	HMSET [key] [filed1] [value1] [field2] [value2] ...
	*@return	0 succ, else -1
	*/
	int HMSet(Key k, FVSet& fvs);
	/*
	*@breif		set 'f' field's value with 'v' which in 'k' hash
	*@command	HSET [key] [field] [value]
	*@return	0 override old value, 1 create new filed, -1 failed
	*/
	int HSet(Key k, Field f, Value v);
	/*
	*@brief		set 'k' hash's 'f' filed value with 'v' if not exists
	*@command	HSETNX [key] [field] [value]
	*@return	0 succ, -1 failed, 1 failed and field exists
	*/
	int HSetNx(Key k, Field f, Value v);

};

//ZSet : [double]score1-value1  score2-value2...
//sorted set
class RedZSet : public Container {
public:
	typedef		std::string									Key;
	typedef		double										Score;
	typedef		std::string									Member;
	typedef		std::vector< Member >						MemberSet;
	typedef		MemberSet::iterator							MemberSetItr;
	typedef		std::vector< std::pair< Score, Member> >	SMSet;
	typedef		SMSet::iterator								SMSetItr;
public:
	RedZSet(Redis* redis) : Container(redis) {
	
	}
public:
	/*
	*@brief		add multi socre-member to 'k' ZSet
	*@command	ZADD [key] [score1] [memeber1] [score2] [member2] ...
	*@return	0 succ, else -1
	*@version	not supported where redis version below 2.4.0
	*/
	int ZAdd(Key k, SMSet& sms);
	/*
	*@brief		add one socre-member to 'k' ZSet
	*@command	ZADD [key] [score] [member] 	
	*@return	0 succ, else -1
	*/
	int ZAdd(Key k, Score s, Member m);

};

//used to transfer ESC char
std::string transferEsc(std::string str);


#endif
