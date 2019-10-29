#pragma once

#include <string>
#include "Map/Map.h"

using namespace std;

class MapLoader {
public:
    Map* read_map(const string& file_name);

private:
    void parse_continents(vector<Continent*>* continents);

    void parse_countries(vector<Continent*>* continents, vector<Country*>* countries);

    void parse_borders(vector<Country*>* countries);

    static Map* create_map(vector<Continent*>* continents, vector<Country*>* countries);

    ifstream* file_stream;
};