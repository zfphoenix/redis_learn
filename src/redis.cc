#include "redis.h"

int main() {

	Redis *r = new Redis();
	if (!r->connect("127.0.0.1", 6379)) {
		std::cout << "connect error\n";
		delete r;
		return 0;
	}
	r->set("name", "felix");
	std::cout << "GET name == " << r->get("name") << std::endl;
	delete r;
	return 0;
}
