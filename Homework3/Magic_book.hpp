#pragma once
#include <string>
#include <vector>
#include "pugixml/pugixml.hpp"
#include <unordered_map>

//fixed we dont use namespace std

struct Routes {
    std::string route_id;
    std::string route_long_name;
    std::string transport_type;
    int stops_counter;
    float lenth;
};

class Reader_routes {
public:
    Reader_routes();
    void max_route() const;
private:
    pugi::xml_document doc;
    std::vector<Routes> routes;
};

class Reader_stops {
public:
    Reader_stops();
    void max_street() const;
private:
    pugi::xml_document doc;
    std::unordered_map<std::string, int> streets;
};

class Magic_stick {
public:
    void do_all() const;
};