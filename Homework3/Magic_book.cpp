#include "Magic_book.hpp"
#include <iostream>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include <windows.h>

class splitstring : public string {
    vector<string> flds;
public:
    splitstring(const string& s) : string(s) { };
    vector<string>& split(char delim, int rep = 0);
};

vector<string>& splitstring::split(char delim, int rep) {
    if (!flds.empty()) flds.clear();
    string work = data();
    string buf = "";
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
        cout << "Smth strange" << endl;
    Routes temp;
    string id, prev = "";

    for (pugi::xml_node node = doc.child("dataset").child("routes_transport"); node; node = node.next_sibling()) {
        id = node.child("route_id").first_child().value();
        if (id == prev) {
            temp.lenth += stof(node.child("stop_distance").first_child().value());
            temp.stops_counter++;
        }
        else {
            if (prev != "")
                routes.push_back(temp);
            temp.route_id = id;
            temp.lenth = stof(node.child("stop_distance").first_child().value());
            temp.stops_counter = 1;
            temp.transport_type = node.child("transport_type").first_child().value();
        }

        prev = id;
    }
    routes.push_back(temp);
}

void Reader_routes::max_lenth_route() const {
    float max_train = 0, max_trolleybus = 0, max_bus = 0;
    string train_route, trolleybus_route, bus_route;
    Routes temp;
    for (size_t i = 0; i < routes.size(); ++i) {
        temp = routes[i];
        if (temp.transport_type == "Трамвай") {
            if (temp.lenth > max_train) {
                max_train = temp.lenth;
                train_route = temp.route_id;
            }
        }
        else if (temp.transport_type == "Автобус") {
            if (temp.lenth > max_bus) {
                max_bus = temp.lenth;
                bus_route = temp.route_id;
            }
        }
        else {
            if (temp.lenth > max_trolleybus) {
                max_trolleybus = temp.lenth;
                trolleybus_route = temp.route_id;
            }
        }
    }

    cout << "Длиннейший маршрут:" << endl;
    cout << "  Для Автобусов: " << bus_route << ". Длина: " << max_bus << "км" << endl;
    cout << "  Для Трамваев: " << train_route << ". Длина: " << max_train << "км" << endl;
    cout << "  Для Троллейбусов: " << trolleybus_route << ". Длина: " << max_trolleybus << "км" << endl;
}

void Reader_routes::max_stops_route() const {
    int max_train = 0, max_trolleybus = 0, max_bus = 0;
    string train_route, trolleybus_route, bus_route;
    Routes temp;
    for (size_t i = 0; i < routes.size(); ++i) {
        temp = routes[i];
        if (temp.transport_type == "Трамвай") {
            if (temp.lenth > max_train) {
                max_train = temp.stops_counter;
                train_route = temp.route_id;
            }
        }
        else if (temp.transport_type == "Автобус") {
            if (temp.lenth > max_bus) {
                max_bus = temp.stops_counter;
                bus_route = temp.route_id;
            }
        }
        else {
            if (temp.lenth > max_trolleybus) {
                max_trolleybus = temp.stops_counter;
                trolleybus_route = temp.route_id;
            }
        }
    }

    cout << "Маршрут с наибольшим количеством остановок:" << endl;
    cout << "  Для Автобусов: " << bus_route << ". Кол-во остановок: " << max_bus << endl;
    cout << "  Для Трамваев: " << train_route << ". Кол-во остановок: " << max_train << endl;
    cout << "  Для Троллейбусов: " << trolleybus_route << ". Кол-во остановок: " << max_trolleybus << endl;
}

Reader_stops::Reader_stops(){
    pugi::xml_parse_result result = doc.load_file("data/data_stops.xml");
    if (!result)
        cout << "Smth strange" << endl;

    string temp;
    vector <string> location;
    vector <string> unnec = { "улица ", " улица", "проспект ", " проспект", "шоссе ", " шоссе", "Большая ", " Большая", "Малая ", " Малая", " пр", " ул", "ул ", "пр ", " ш", "ш ", " Б", "Б ", "М ", " М" };

    size_t pos;
    int counter = 1;
    for (pugi::xml_node node = doc.child("dataset").child("transport_station"); node; node = node.next_sibling()) {
        temp = node.child("location").first_child().value();
        if (temp.empty())
            continue;

        location.clear();

        if (temp.find(",") != string::npos) {
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
                if ((pos = location[i].find(unnec[j])) != string::npos) {
                    location[i].erase(pos, unnec[j].size());
                }
            }
        }

        for (size_t i = 0; i < location.size(); ++i) {
            string name = location[i];
            auto iter = streets.find(name);
            if (iter != streets.end()) {
                int counter = streets[name] + 1;
                streets.erase(iter);
                streets.insert({ name, counter });
            }
            else
                streets.insert({name, 1});
        }
        counter++;
    }
}

void Reader_stops::max_street() const{
    auto x = std::max_element(streets.begin(), streets.end(),
        [](const pair<string, int>& p1, const pair<string, int>& p2) {
            return p1.second < p2.second; });
    cout << "Улица с наибольшим количеством остановок: " << x->first << ". Кол-во остановок: " << x->second << endl;
}

void Magic_stick::do_all() const{
    SetConsoleOutputCP(CP_UTF8);

    Reader_routes reader_routes;
    reader_routes.max_stops_route();
    reader_routes.max_lenth_route();

    Reader_stops reader_stops;
    reader_stops.max_street();
}