#ifndef ADDRESS_H
#define ADDRESS_H

#include <ostream>
#include <string>

struct Address
{
	int number;
	std::string street;
};

std::ostream& operator<<(std::ostream& o, const Address& a);

#endif
