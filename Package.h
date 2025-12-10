#ifndef PACKAGE_H
#define PACKAGE_H

#include <string>
#include "Address.h"

struct Package {
    std::string id;            // The key for the Hash Table, e.g., "PKG001"
    Address destination;       // The destination address (A, B, C, etc.)
    // Add other details like weight, delivery window, etc., if needed
};

#endif // PACKAGE_H