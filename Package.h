#ifndef PACKAGE_H
#define PACKAGE_H
#include <string>
#include "Address.h"

struct Package 
{
    std::string id;            //key for the hash table, like "PKG001"
    Address destination;       //the destination address (A, B, C, etc.)
};

#endif
