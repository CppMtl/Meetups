#ifndef CUSTOMER_INFO_H
#define CUSTOMER_INFO_H

#include "Address.h"
#include <ostream>
#include <string>

struct CustomerInfo
{
	std::string firstName;
	std::string lastName;
	Address address;
};

std::ostream& operator<<(std::ostream& o, const CustomerInfo& c);

#endif
