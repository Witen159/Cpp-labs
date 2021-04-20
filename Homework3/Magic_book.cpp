#include "Magic_book.hpp"
#include <iostream>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>

class splitstring : public std::string {
    std::vector<std::string> flds;
public:
    splitstring(const std::string& s) : std::string(s) { };
    std::vector<std::string>& split(char delim, int rep = 0);
};

std::vector<std::string>& splitstring::split(char delim, int rep) {
    if (!flds.empty()) flds.clear();
    std::string work = data();
    std::string buf = "";
    unsigned int i = 0;
    while (i < work.length()) {
        if (work[i] != delim)
            buf += work[i];
        else if (rep == 1) {
            flds.push_back(buf);
            buf = "";
        }
        else if (buf.length() > 0) {
            flds.push_back(buf);
            buf = "";
        }
        i++;
    }
    if (!buf.empty())
        flds.push_back(buf);
    return flds;
}

Reader_routes::Reader_routes(){
    pugi::xml_parse_result result = doc.load_file("data/data_routes.xml");
    if (!result)
        std::cout << "Smth strange" << std::endl;
    Routes temp;
    std::string id, prev = "";

    for (pugi::xml_node node = doc.child("dataset").child("routes_transport"); node; node = node.next_sibling()) {
        id = node.child("route_id").first_child().value();
        if (id == prev) {
            temp.lenth += std::stof(node.child("stop_distance").first_child().value());
            temp.stops_counter++;
        }
        else {
            if (prev != "")
                routes.push_back(temp);
            temp.route_id = id;
            temp.lenth = std::stof(node.child("stop_distance").first_child().value());
            temp.stops_counter = 1;
            temp.transport_type = node.child("transport_type").first_child().value();
        }

        prev = id;
    }
    routes.push_back(temp);
}

//fixed copy-paste

void Reader_routes::max_route() const {
    Routes temp;
    std::string transport;
    std::unordered_map<std::string, std::pair<std::string, unsigned int>> stops;
    std::unordered_map<std::string, std::pair<std::string, float>> lenth;
    for (size_t i = 0; i < routes.size(); ++i) {
        temp = routes[i];
        transport = temp.transport_type;
        auto iter = stops.find(transport);
        if (iter == stops.end()) {
            stops.insert({ transport, std::make_pair(temp.route_long_name, temp.stops_counter) });
            lenth.insert({ transport, std::make_pair(temp.route_long_name, temp.lenth) });
        }
        else {
            if (temp.stops_counter > stops[transport].second) {
                stops[transport].second = temp.stops_counter;
                stops[transport].first = temp.route_id;
            }
            if (temp.lenth > lenth[transport].second) {
                lenth[transport].second = temp.lenth;
                lenth[transport].first = temp.route_id;
            }
        }
    }

    std::cout << "Маршрут с наибольшим количеством остановок:" << std::endl;
    std::cout << "  Для Автобусов: " << stops["Автобус"].first << ". Кол-во остановок: " << stops["Автобус"].second << std::endl;
    std::cout << "  Для Трамваев: " << stops["Трамвай"].first << ". Кол-во остановок: " << stops["Трамвай"].second << std::endl;
    std::cout << "  Для Троллейбусов: " << stops["Троллейбус"].first << ". Кол-во остановок: " << stops["Троллейбус"].second << std::endl;

    std::cout << "Длиннейший маршрут:" << std::endl;
    std::cout << "  Для Автобусов: " << lenth["Автобус"].first << ". Длина: " << lenth["Автобус"].second << "км" << std::endl;
    std::cout << "  Для Трамваев: " << lenth["Трамвай"].first << ". Длина: " << lenth["Трамвай"].second << "км" << std::endl;
    std::cout << "  Для Троллейбусов: " << lenth["Троллейбус"].first << ". Длина: " << lenth["Троллейбус"].second << "км" << std::endl;
}

Reader_stops::Reader_stops(){
    pugi::xml_parse_result result = doc.load_file("data/data_stops.xml");
    if (!result)
        std::cout << "Smth strange" << std::endl;

    std::string temp;
    std::vector <std::string> location;
    std::vector <std::string> unnec = { "улица ", " улица", "проспект ", " проспект", "шоссе ", " шоссе", "Большая ", " Большая", "Малая ", " Малая", " пр", " ул", "ул ", "пр ", " ш", "ш ", " Б", "Б ", "М ", " М" };

    size_t pos;
    int counter = 1;
    for (pugi::xml_node node = doc.child("dataset").child("transport_station"); node; node = node.next_sibling()) {
        temp = node.child("location").first_child().value();
        if (temp.empty())
            continue;

        location.clear();

        if (temp.find(",") != std::string::npos) {
            splitstring spl = splitstring(temp);
            location = spl.split(',');
        }
        else {
            location.push_back(temp);
        }

        for (size_t i = 0; i < location.size(); ++i) {
            if (i > 0)
                location[i].erase(0, 1);
            
            location[i].erase(remove(location[i].begin(), location[i].end(), '.'), location[i].end());

            for (size_t j = 0; j < unnec.size(); ++j) {
                if ((pos = location[i].find(unnec[j])) != std::string::npos) {
                    location[i].erase(pos, unnec[j].size());
                }
            }
        }

        for (size_t i = 0; i < location.size(); ++i) {
            std::string name = location[i];
            auto iter = streets.find(name);
            if (iter != streets.end()) {
                streets[name]++;
            }
            else
                streets.insert({name, 1});
        }
        counter++;
    }
}

void Reader_stops::max_street() const{
    auto x = std::max_element(streets.begin(), streets.end(),
        [](const std::pair<std::string, int>& p1, const std::pair<std::string, int>& p2) {
            return p1.second < p2.second; });
    std::cout << "Улица с наибольшим количеством остановок: " << x->first << ". Кол-во остановок: " << x->second << std::endl;
}

void Magic_stick::do_all() const{
    SetConsoleOutputCP(CP_UTF8);

    Reader_routes reader_routes;
    reader_routes.max_route();

    Reader_stops reader_stops;
    reader_stops.max_street();
}