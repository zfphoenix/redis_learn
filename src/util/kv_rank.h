#ifndef KV_RANK_H_
#define KV_RANK_H_

#include <stdint.h>

#include <map>
#include <string>
#include "object_leveldb.h"

template < class Key, class Value, class Cmp, int Max >
class KvRank {
public:
	typedef std::map< Key, Value >							 InfoMap;
	typedef std::multimap< typename Value::Score, Key, Cmp > ScoreMap;
public:
	KvRank(std::string rank_name):odb_(NULL),rank_name_(rank_name) {
		odb_ = new ObjectDB<>(rank_name);
		init();
	}

	virtual ~KvRank() {
		if (odb_) {
			delete odb_;
			odb_ = NULL;
			info_map_.clear();
			score_map_.clear();
		}
	}

	uint32_t init() {
		leveldb::DB *db = NULL;
		uint32_t  count = 0;
		if (odb_ && (db = odb_->GetDb())) {
			leveldb::Iterator *itr = db->NewIterator(leveldb::ReadOptions());
			for (itr->SeekToFirst(); itr->Valid(); itr->Next()) {
				Key		k;
				Value	v;
				Key::Parse(itr->key().ToString(), k);
				Value::Parse(itr->value().ToString(), v);
				info_map_[k] = v;
				score_map_.insert(std::make_pair< typename Value::Score, Key >(v.score, k));
				++count;
			}
			delete itr;
		} else {
			assert(0);
		}
		return count;
	}

	std::string name() const { return rank_name_; }
	size_t		size() const { return score_map_.size(); }
	int			max()  const { return Max; }
	int     ApproxiRank(const Key &key);

	//XXX
	virtual void   GetTopN() {};

	void Add(Key key, Value value) {
		if (this->size() >= this->max()) {
			typename Value::Score min = score_map_.rbegin()->first;
			if (value.score <= min) {
				return;
			}
			typename InfoMap::iterator uitr = info_map_.find(key);
			if (uitr != info_map_.end()) {
				del_score_map(uitr->second.score, key);
				info_map_.erase(key);
			} else {
				del_last();
			}
		} else {
			typename InfoMap::iterator uitr = info_map_.find(key);
			if (uitr != info_map_.end()) {
				if (value.score <= info_map_[key].score) {
					return;
				}
				del_score_map(uitr->second.score, key);
				info_map_.erase(key);
			}
		}
		add(key, value);
	}

	std::string Debug() const {
		char buff[1024];
		size_t len = snprintf(buff, 1023, "[Kv_Rank]max=%d info_map.size=%u score_map.size=%u",
				this->max(), info_map_.size(), score_map_.size());
		return std::string(buff, len);

	}

	//XXX update extra info,
	int update(Key key, Value value) {
		if (info_map_.find(key) != info_map_.end()) {
			value.score = info_map_[key].score;
			info_map_[key] = value;
			odb_->write(key.Format(), value.Format());
		}
		return 0;
	}

private:
	void add(Key key, Value value) {
		info_map_[key] = value;
		score_map_.insert(std::make_pair< typename Value::Score, Key >(value.score, key));
		odb_->write(key.Format(), value.Format());
	}

	void del_last() {
		Key temp_key = score_map_.rbegin()->second;
		Value temp_value = info_map_[temp_key];
		del_score_map(temp_value.score, temp_key);
		info_map_.erase(temp_key);
		odb_->Del(temp_key.Format());
	}

	void del_score_map(typename Value::Score score, const Key &key) {
		std::pair < typename ScoreMap::iterator, typename ScoreMap::iterator > equal_pair;
		equal_pair = score_map_.equal_range(score);
		typename ScoreMap::iterator itr = equal_pair.first;
		for (; itr != equal_pair.second;) {
			if (key == itr->second) {
				score_map_.erase(itr++);
				break;
			} else {
				++itr;
			}
		}
	}


protected:
	ObjectDB<>  *odb_;
	InfoMap		info_map_;
	ScoreMap	score_map_;
	std::string rank_name_;
};

//implemention of KvRank
template < class Key, class Value, class Cmp, int Max >
int KvRank< Key, Value, Cmp, Max >::ApproxiRank(const Key &key) {
	int rank = 0;
	typename InfoMap::iterator itr = info_map_.find(key);
	if (itr != info_map_.end()) {
		typename ScoreMap::iterator sitr = score_map_.find(itr->second.score);
		if (sitr != score_map_.end()) {
			rank = std::distance(score_map_.begin(), sitr);
			rank+=1;
		}
	}
	return rank;
}
#if 0
template < class Key, class Value, class Cmp, int Max >
template < class Msg >
void KvRank< Key, Value, Cmp, Max >::GetTopN(Msg *msg) {
//TODO 
}
#endif

#endif  //KV_RANK_H
