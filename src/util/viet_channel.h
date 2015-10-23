#ifndef VIET_CHANNEL_H_
#define VIET_CHANNEL_H_

#include <map>
#include <set>
#include <string>
#include <sstream>

extern "C" {
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
}

#ifdef MYTEST
#include "../base/Singleton.hpp"
#include "../base/EventableObject.hpp"
#include "../base/Event.hpp"
#include "../base/Time.hpp"

#include "fwd_decl.h"
#include "log.h"
#else 
#include "./Singleton.h"
#endif

#define format(a)			 \
	if (a > 0 && a < 10)	 \
        ss << 0 << a;		 \
    else					 \
        ss << a;

namespace base {
	class Time {
	public:
		Time() {
			time(&cur_);
			date = localtime(&cur_);
		}

		Time& start_of_day() {
			date->tm_hour = 0;
			date->tm_min  = 0;
			date->tm_sec  = 0;
			return *this;
		}

		uint32_t seconds() {
			return  (uint32_t)(mktime(date));
		}

	private:
		time_t cur_;
		struct tm *date;
	};

}//enf of namespace base

const static std::string viet_log_dir_name = "viet_log";

static std::string DateToString() {
	std::string str;
	time_t now;
	struct tm *date;
	time(&now);
	date = localtime(&now);
	std::stringstream ss;
	ss << date->tm_year+1900;
	format(date->tm_mon+1);
	format(date->tm_mday);
	ss << " ";
	format(date->tm_hour);
	ss << ":"; format(date->tm_min);
	ss << ":"; format(date->tm_sec);
	str = ss.str();
	ss.str("");
	ss.clear();
	return str;
}

static std::string VietGenFileName() {
	std::string fname;
	time_t now;
	struct tm *date;
	time(&now);
	date = localtime(&now);
	std::stringstream ss;
	ss << viet_log_dir_name << "/";
	ss << date->tm_year+1900 << "-";
	format(date->tm_mon+1);
	ss << "-";
	format(date->tm_mday);
	fname = ss.str();
	ss.str("");
	ss.clear();
	return fname;
}

enum {
	BUFF_LENGTH     = 1024,
	REFRESH_GAP		= 10,
};

enum VIET_CHANNEL{
	VIET_OFFICI		= 1013,
	VIET_APPOTA		= 1014,
};

class VietLogFile {
public:
	VietLogFile() {
		init();
	}

	~VietLogFile() {
		if (temp_) {
			temp_ = 0;
			close(fd_);
		}
	}

	bool diff_day() {
		uint32_t cur = base::Time().seconds();
		uint32_t diff = cur > temp_ ? cur - temp_ : 0;
		if (temp_ && diff > 24*60*60) {
			return true;
		}
		return false;
	}

	void init() {
		temp_ = 0;
		if (-1 == access(viet_log_dir_name.c_str(), W_OK|F_OK)) {
			if (-1 == mkdir(viet_log_dir_name.c_str(), 0755)) {
#ifdef MYTEST
				ERROR_LOG("cannot create dir viet_log");
#endif
				return;
			}
		}
		fd_ = open(VietGenFileName().c_str(), O_WRONLY | O_CREAT | O_APPEND, 0777);
		if (-1 == fd_) {
#ifdef MYTEST
			ERROR_LOG("cannot create fd");
#endif
			return;
		}
		fcntl(fd_, F_SETFD, FD_CLOEXEC);
		temp_ = base::Time().start_of_day().seconds();
	}

	void log(std::string str, size_t len) {
		if (!diff_day()) {
			write(fd_, str.c_str(), len);
		} else {
			if (temp_) {
				close(fd_);
				temp_ = 0;
				fd_ = open(VietGenFileName().c_str(), O_WRONLY | O_CREAT | O_APPEND, 0777);
				if (-1 == fd_) {
#ifdef MYTEST
					ERROR_LOG("cannot create file %s", VietGenFileName().c_str());
#endif
					return;
				}
				fcntl(fd_, F_SETFD, FD_CLOEXEC);
				temp_ = base::Time().start_of_day().seconds();
			}
		}
	}

private:
	int fd_;
	uint32_t temp_;
};

#if 0  //e.g.
class VietLog : public Singleton<VietLog>, 
				public base::EventableObject {
public:
	VietLog() {
		init();
	}
	~VietLog() {
		online_map_.clear();
		print_handler_.deactivate();
	}

public:
	void init() {
		online_map_[VIET_OFFICI] = 0;
		online_map_[VIET_APPOTA] = 0;
		print_handler_ = sGloble.Timer.add_event(*this,
				&VietLog::print,
				base::Time() + 10,
				REFRESH_GAP * 1000,
				-1);
	}

	void add(uint32_t channel) {
		++online_map_[(VIET_CHANNEL)(channel)];
	}

	void del(uint32_t channel) {
		if (online_map_[(VIET_CHANNEL)(channel)])
			--online_map_[(VIET_CHANNEL)(channel)];
	}

	int print() {
		int len = snprintf(buff, BUFF_LENGTH, "%s [online info] OFFICI num is %u, APPOTA num is %u, All channel is %lu\r\n",
				DateToString().c_str(), online_map_[VIET_OFFICI], online_map_[VIET_APPOTA], online_map_.size());
	
		if (len > 0)
			file_.log(buff, len);
		return 0;
	}

private:
	// std::map < channel, all_nums > 
	typedef std::map< VIET_CHANNEL, uint32_t >	OnlinePlayerMap;
	OnlinePlayerMap	online_map_;
	char buff[BUFF_LENGTH];
	base::EventHandler print_handler_;
	VietLogFile file_;
};

#define GVietOnlineLog	VietLog::get_singleton()
#endif
#endif  //VIET_CHANNEL_H_
