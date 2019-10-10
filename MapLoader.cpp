#include <fstream>
#include <iostream>
#include <sstream>
#include "MapLoader.h"
#include "Map.h"

MapLoader::MapLoader() :
    continents(new vector<Continent*>()),
    countries(new vector<Country*>()){ }

Map* MapLoader::read_map(string file_name) {
    file_stream = new ifstream(file_name);

    if(!file_stream->is_open()) {
        cout << "The specified file cannot be opened." << endl;
        exit(1);
    }

    //Start reading the file, until we get to sections we can parse
    string cur_line;
    while(getline(*file_stream, cur_line)) {
        if(cur_line == "[continents]\r") {
            parse_continents();
        }

        if(cur_line == "[countries]\r") {
            parse_countries();
        }

        if(cur_line == "[borders]\r") {
            parse_borders();
        }
    }

    return create_map();
}

void MapLoader::parse_continents() {
    string cur_line;
    string parsed[3];
    while(getline(*file_stream, cur_line)) {
        if(cur_line == "\r") {
            return;
        }

        int i = 0;
        stringstream string_stream(cur_line);
        while(string_stream >> parsed[i] && i < 3) {
            i++;
        }

        auto* c = new Continent(parsed[0]);
        continents->push_back(c);
    }
}

void MapLoader::parse_countries() {
    string cur_line;
    string parsed[5];
    while(getline(*file_stream, cur_line)) {
        if (cur_line == "\r") {
            return;
        }

        int i = 0;
        stringstream string_stream(cur_line);
        while(string_stream >> parsed[i] && i < 5) {
            i++;
        }

        Continent* continent = continents->at(stoi(parsed[2]) - 1);
        auto* c = new Country(parsed[1], continent);
        countries->push_back(c);
        continent->add_country(c);
    }
}

void MapLoader::parse_borders() {
    string cur_line;
    while(getline(*file_stream, cur_line)) {
        if(cur_line == "\r") {
            return;
        }

        string border_index;
        stringstream string_stream(cur_line);

        Country* c;

        if(string_stream >> border_index) {
            c = countries->at(stoi(border_index) - 1);
            while (string_stream >> border_index) {
                c->add_adjacent_country(countries->at(stoi(border_index) - 1));
            }
        }
    }
}

Map* MapLoader::create_map() {
    return new Map(continents, countries);
}