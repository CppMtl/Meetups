#include "CustomerInfo.h"

std::ostream& operator<<(std::ostream& o, const CustomerInfo& c)
{
	o << "Employee {\n"
	<< "  firstName: " << c.firstName << ",\n"
	<< "  lastName: " << c.lastName << ",\n"
	<< "  address: " << c.address << '\n'
	<< '}';
	return o;
}
