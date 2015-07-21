#ifndef LOG_H_
#define LOG_H_

#include <cstdarg> //c89
#include "./util/Singleton.h"
#include <string>
#include <iostream>

class Log : public Singleton<Log> {
public:
	inline void Debug(const char*,...);

	inline void Info(const char*,...);

	inline void Trace(const char*,...);

	inline void Error(const char*,...);
};

//TODO need implementation
void Log::Debug(const char* str,...) {
	char msg[1024];
	va_list args;
	va_start(args, str);
	vsnprintf(msg, 1024, str, args);
	std::cout << msg << std::endl;
	va_end(args);
}

void Log::Info(const char*,...) {

}

void Log::Trace(const char*,...) {

}

void Log::Error(const char*,...) {

}

#define MLOG Log::Get()

#endif
