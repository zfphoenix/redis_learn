#ifndef OBJECT_LEVELDB_H_
#define OBJECT_LEVELDB_H_

#include <string>
#include <cassert>

#include "leveldb/comparator.h"
#include "leveldb/db.h"

class OwnComparator : public leveldb::Comparator {
public:
	int Compare(const leveldb::Slice &a, const leveldb::Slice &b) const {
		if (a.ToString() > b.ToString()) return 1;
		else if (a.ToString() < b.ToString()) return -1;
		else return 0;
	}
	const char* Name() const { return "OwnComparator"; }
	void FindShortestSeparator(std::string*, const leveldb::Slice&) const {}
	void FindShortSuccessor(std::string*) const {}
};

template < class Comp = OwnComparator >
class ObjectDB {
public:
	ObjectDB(std::string db_path):db_(NULL) {
		leveldb::Options options;
		options.create_if_missing = true;
		options.comparator = &cmp_;
		leveldb::DB::Open(options, db_path, &db_);
	}
	~ObjectDB() {
		if (db_) {
			delete db_;
			db_ = NULL;
		}
	}

	leveldb::DB* GetDb() const {
		return db_;
	}

	leveldb::Status write(std::string key, std::string value) {
		return db_->Put(leveldb::WriteOptions(), key, value);
	}

	leveldb::Status Get(std::string key, std::string &value) {
		return db_->Get(leveldb::ReadOptions(), key, &value);
	}

	leveldb::Status Del(std::string key) {
		return db_->Delete(leveldb::WriteOptions(), key);
	}

private:
	ObjectDB(const ObjectDB*);
private:
	leveldb::DB *db_;
	Comp cmp_;
};

#endif  //OBJECT_LEVELDB_H_
