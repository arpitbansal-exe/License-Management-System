#pragma once
#include <string>
#include <iostream>

class License {
public:
    std::string name;
    std::string license_id;
    std::string expiry;
    int quantity;
    std::string key;
    License() {};
    License(const std::string& n, const std::string& id,
        const std::string& exp, int qty, const std::string& k)
        : name(n), license_id(id), expiry(exp), quantity(qty), key(k) {
    }

    //getters
    std::string getName() const {
        return name;
    }
    std::string getlicense_id() const {
        return license_id;
    }
    std::string getexpiry() const {
        return expiry;
    }
    std::string getkey() const {
        return key;
    }
};