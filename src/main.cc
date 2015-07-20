//
#include <iostream>
#include "redis.h"
#include "container.h"

int main() {
	Redis red("127.0.0.1", 6677, 0);
	red.ping();
	RedString kv(&red);
	std::cout <<kv.Get("one")<<std::endl;
	return 0;
}
