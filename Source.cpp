#include "HirschBerg.h"

int main(int argc, char *argv[]) {
	if (3 == argc){
		std::string s1 = argv[1];
		std::string s2 = argv[2];
		H::ZW zw = H::HirschBerg(s1.c_str(), s1.c_str() + s1.length(), s2.c_str(), s2.c_str() + s2.length());
		std::cout << "S: " << zw.Z << std::endl;
		std::cout << "T: " << zw.W << std::endl;
		return 0;
	}
	return 1;
}

