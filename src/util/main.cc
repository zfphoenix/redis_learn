//
#include <iostream>
#include "kv_rank.h"
#include "Singleton.hpp"
template < class Key, class Value, class Cmp, int Max >
class MyRank : public KvRank< Key, Value, Cmp, Max >, public Singleton< MyRank<Key,Value,Cmp,Max> > {
public:
	MyRank(std::string rank_name):KvRank<Key,Value,Cmp,Max>(rank_name) {}
	~MyRank() {
		std::cout << "``````\n";
	}
	void GetTopN() {
		std::string str = this->Debug();
		std::cout << str << std::endl;
		std::cout << this->info_map_.size() << std::endl;
		typename KvRank< Key, Value, Cmp, Max >::InfoMap::iterator itr = this->info_map_.begin();
		for (; itr != this->info_map_.end(); ++itr) {
			std::cout << itr->first.Format() << " == " << itr->second.Format() << std::endl;
		}
		typename KvRank< Key, Value, Cmp, Max >::ScoreMap::iterator iter = this->score_map_.begin();
		for (; iter != this->score_map_.end(); ++iter) {
			std::cout <<  iter->first <<  " == " << iter->second.Format() << std::endl;
		}
	}
};


class Ukey {
public:
	Ukey() {}
	Ukey(int arg):a(arg) {}
	std::string Format() const {
		char buff[10];
		size_t len = sprintf(buff, "%d", a);
		return std::string(buff, len);
	}

	static void Parse(std::string str, Ukey &k) {
		sscanf(str.c_str(), "%d", &k.a);
	}

	bool operator==(const Ukey &right) const {
		return a == right.a;
	}
	bool operator<(const Ukey &right) const {
		return a < right.a;
	}
	int a;
};

class Uvalue {
public:
	typedef uint32_t Score;
	std::string Format() const {
		char buff[10];
		size_t len = sprintf(buff, "%d", score);
		return std::string(buff, len);
	}
	static void Parse(std::string str, Uvalue &v) {
		sscanf(str.c_str(), "%d", &v.score);
	}
	Score score;
};

template < class Key >
struct Ucmp {
	bool operator()(const Key &a, const Key &b) {
		return a > b;
	}
};

template<>
struct Ucmp<Uvalue::Score> {
	bool operator()(const Uvalue::Score &a, const Uvalue::Score &b) {
		return a > b;
	}
};

//watch out
#define GRank				  MyRank< Ukey,Uvalue,Ucmp<Uvalue::Score>, 10 >::get_singleton("tdb")
#define GRank_Clear			  MyRank< Ukey,Uvalue,Ucmp<Uvalue::Score>, 10 >::delete_singleton()

int main()
{
#if 0
	KvRank<Ukey,Uvalue,Ucmp<uint32_t>,10> *rank = new MyRank<Ukey, Uvalue, Ucmp<uint32_t>, 10>("tdb");
	Ukey   k;
	Uvalue v;
	/*for (int i = 0 ; i < 20; ++i) {
		k.a =i;
		v.score = i*i;
		rank.Add(k,v);
	}*/
	k.a = 100;
	v.score = 1000;
	//rank->Add(k, v);
	//rank->update(k, v);
	rank->GetTopN();
	std::cout << "============" << std::endl;
	std::cout << rank->ApproxiRank(k) << std::endl;
	delete rank;
#endif
//	MyRank<Ukey,Uvalue,Ucmp<uint32_t>, 10> *ptr = MyRank<Ukey,Uvalue,Ucmp<uint32_t>, 10>::get_singleton_ptr("tdb");
//	if (ptr != NULL)
//		delete ptr;
	Ukey   k;
	Uvalue v;
	k.a = 2290;
	v.score = 225;

//	GRank.Add(k,v);
	GRank.GetTopN();
	GRank_Clear;
	return 0;
}
