#pragma once
#include <string>
#include <vector>
#include "pugixml/pugixml.hpp"
#include <unordered_map>

//todo we dont use namespace std
using namespace std;

struct Routes {
    string route_id;
    string route_long_name;
    string transport_type;
    int stops_counter;
    float lenth;
};

class Reader_routes {
public:
    Reader_routes();
    void max_lenth_route() const;
    void max_stops_route() const;
private:
    pugi::xml_document doc;
    vector<Routes> routes;
};

class Reader_stops {
public:
    Reader_stops();
    void max_street() const;
private:
    pugi::xml_document doc;
    unordered_map<string, int> streets;
};

class Magic_stick {
public:
    void do_all() const;
};