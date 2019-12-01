#pragma once

#include <string>
#include "Map/Map.h"

using namespace std;

class MapLoader {
public:
    MapLoader();

    virtual Map* read_map(const string& file_name);

private:
    ifstream* file_stream;

    void parse_continents(vector<Continent*>* continents);

    void parse_countries(vector<Continent*>* continents, vector<Country*>* countries);

    void parse_borders(vector<Country*>* countries);
};

class ConquestMapLoader {
public:
    Map* read_conquest_map(const string& file_name);

private:
    ifstream* file_stream;

    void parse_continents(vector<Continent*>* continents);

    void parse_countries(vector<Continent*>* continents, vector<Country*>* countries);

};

class ConquestAdapter : public MapLoader {
public:
    ConquestAdapter();

    Map* read_map(const string& file_name) override;

private:
    ConquestMapLoader* conquest_map_loader;
};