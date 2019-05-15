#include <iostream>
#include <netdb.h>

int main() {
	std::cout << gethostbyname("localhost")->h_name << "guili guili" << std::endl;
}

