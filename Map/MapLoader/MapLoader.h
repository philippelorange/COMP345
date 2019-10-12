#pragma once
#include <string>
#include "Map/Map.h"

using namespace std;

class MapLoader {
public:
    MapLoader();
    Map* read_map(string file_name);
private:
    void parse_continents();
    void parse_countries();
    void parse_borders();
    Map* create_map();

    ifstream* file_stream;
    vector<Continent*>* continents;
    vector<Country*>* countries;
};