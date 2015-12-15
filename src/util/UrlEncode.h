//
#include <string>

namespace base {

inline bool IsDigit(char c) {
	return ((c >= '0') && (c <= '9'));
}

inline bool IsLetter(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
inline char DecodeChar(char a, char b) { char c = 0;
	if (IsDigit(a)) {
		c += (a - '0')*16;
	} else {
		c += (10 + (a - 'A')) * 16;
	}
	
	if (IsDigit(b)) {
		c += (b - '0');
	} else {
		c += (10 + (b - 'A'));
	}
	return c;
}

const static char HexCode[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8',
					'9', 'A', 'B', 'C', 'D', 'E', 'F'};
const static char SpaceSign = ' ';
const static char PlusSign  = '+';
const static char PercSign	= '%';


void UrlEncode(const char *str, size_t len, std::string &dst) {
	dst.clear();
	for (size_t i = 0; i < len; i++, str++) {
		if (SpaceSign == str[0]) {
			dst.append("+");
			continue;
		}
		if (!(IsDigit(str[0]) || IsLetter(str[0]))) {
			dst.append("%").append(1, HexCode[(unsigned char)(str[0])/16]).
				append(1, HexCode[(unsigned char)(str[0])%16]);
		} else {
			dst.append(str, 1);
		}
	}
}

void UrlDecode(const char *str, size_t len, std::string &dst) {
	dst.clear();
	char c;
	for (size_t i = 0; i < len; i++, str++) {
		switch (str[0]) {
		case PlusSign:
			dst.append(" ");
			continue;
		case PercSign:
			dst.append(1, DecodeChar(str[1], str[2]));
			str += 2;
			break;
		default:
			dst.append(1, str[0]);
			break;
		}
	}
}

}

#if 0
#include <iostream>
#include <string>
#include <stdio.h>


int main(int argc, char* argv[])
{
	std::string a = "+++kjdkfd123 1";
	std::cout << "origi string ::" <<  a << std::endl;
	std::string d; 
	base::UrlEncode(a.data(), a.size(), d);
	std::cout << "after encode ::"<< d << std::endl;
	std::string dd;
	base::UrlDecode(d.data(), d.size(), dd);
	std::cout << "after decode ::" << dd << std::endl;
	return 0;
}
#endif
