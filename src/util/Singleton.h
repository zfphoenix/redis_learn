#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <cassert>
//make specified class to create one instance only
//Singleton is used to manage it

template < class Type >
class Singleton {
public:
	virtual ~Singleton() {
		one_ = NULL;
	}

	//return the only one instance
	static Type* Get() {
		if (!one_)
			one_ = new Type();
		return one_;
	}

	//clear instance 
	static void Clear() {
		if (one_) {
			delete one_;
			one_ = NULL;
		}
	}

//why protected ? 
//make sure derived class can invoke base structe func
//and users cannot new it
protected:
	Singleton() {
		assert(!one_);	
	}

	static Type* one_;
};

template < class Type>
Type* Singleton<Type>::one_ = NULL;

#endif
