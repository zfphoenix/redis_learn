#ifndef LOG_H_
#define LOG_H_

#include <cstdarg> //c89
#include "./util/Singleton.h"
#include <string>

class Log : public Singleton<Log> {
public:
	inline void Debug(std::string...);

	inline void Info(std::string...);

	inline void Trace(std::string...);

	inline void Error(std::string...);
};

//TODO need implementation
void Log::Debug(std::string...) {

}

void Log::Info(std::string...) {

}

void Log::Trace(std::string...) {

}

void Log::Error(std::string...) {

}

#define MLOG Log::Get()

#endif
