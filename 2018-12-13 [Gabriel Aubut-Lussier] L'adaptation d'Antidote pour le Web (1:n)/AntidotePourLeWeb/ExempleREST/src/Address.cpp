#include "Address.h"

std::ostream& operator<<(std::ostream& o, const Address& a)
{
	o << "Address{number: " << a.number << ", street: " << a.street << "}";
	return o;
}
