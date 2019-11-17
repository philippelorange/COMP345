#include <fstream>
#include <iostream>
#include <sstream>
#include "MapLoader.h"
#include "Map/Map.h"

MapLoader::MapLoader() {}

Map* MapLoader::read_map(const string& file_name) {
    file_stream = new ifstream(file_name);

    auto* countries = new vector<Country*>();
    auto* continents = new vector<Continent*>();

    if (!file_stream->is_open()) {
        cout << "The specified file cannot be opened." << endl;
        exit(1);
    }

    //Start reading the file, until we get to sections we can parse. If the three sections aren't found
    //and we are at the end of the file, return null;
    string cur_line;
    bool has_parsed_continents = false;
    bool has_parsed_countries = false;
    bool has_parsed_borders = false;

    while (getline(*file_stream, cur_line)) {
        if (cur_line.find("[continents]") != std::string::npos) {
            parse_continents(continents);
            has_parsed_continents = true;
        }

        if (cur_line.find("[countries]") != std::string::npos) {
            parse_countries(continents, countries);
            has_parsed_countries = true;
        }

        if (cur_line.find("[borders]") != std::string::npos) {
            parse_borders(countries);
            has_parsed_borders = true;
        }
    }

    if (has_parsed_borders && has_parsed_continents && has_parsed_countries) {
        return new Map(continents, countries);
    } else {
        return nullptr;
    }
}

void MapLoader::parse_continents(vector<Continent*>* continents) {
    string cur_line;
    string parsed[3];
    while (getline(*file_stream, cur_line)) {
        if (cur_line == "\r") {
            return;
        }

        int i = 0;
        stringstream string_stream(cur_line);
        while (string_stream >> parsed[i] && i < 3) {
            i++;
        }

        auto* c = new Continent(parsed[0], stoi(parsed[1]));
        continents->push_back(c);
    }
}

void MapLoader::parse_countries(vector<Continent*>* continents, vector<Country*>* countries) {
    string cur_line;
    string parsed[5]; //A country will have 5 elements on a line: an ID, a name, a continent ID, and x,y coordinates.
    while (getline(*file_stream, cur_line)) {
        if (cur_line == "\r") {
            return;
        }

        int i = 0;
        stringstream string_stream(cur_line);
        while (string_stream >> parsed[i] && i < 5) {
            i++;
        }

        Continent* continent = continents->at(stoi(parsed[2]) - 1); //Find continent given an ID
        auto* c = new Country(parsed[1], continent);
        countries->push_back(c);
        continent->add_country(c);
    }
}

void MapLoader::parse_borders(vector<Country*>* countries) {
    string cur_line;
    while (getline(*file_stream, cur_line)) {
        if (cur_line == "\r") {
            return;
        }

        string border_index;
        stringstream string_stream(cur_line);

        Country* c;

        //Add adjacent countries to the first country in the line, as long as the line still has values.
        if (string_stream >> border_index) {
            c = countries->at(stoi(border_index) - 1);
            while (string_stream >> border_index) {
                c->add_adjacent_country(countries->at(stoi(border_index) - 1));
            }
        }
    }
}

Map* ConquestMapLoader::read_conquest_map(const string &file_name) {
    file_stream = new ifstream(file_name);

    auto* countries = new vector<Country*>();
    auto* continents = new vector<Continent*>();

    if (!file_stream->is_open()) {
        cout << "The specified file cannot be opened." << endl;
        exit(1);
    }

    //Start reading the file, until we get to sections we can parse. If the three sections aren't found
    //and we are at the end of the file, return null;
    string cur_line;
    bool has_parsed_continents = false;
    bool has_parsed_countries = false;

    while (getline(*file_stream, cur_line)) {
        if (cur_line.find("[Continents]") != std::string::npos) {
            parse_continents(continents);
            has_parsed_continents = true;
        }

        if (cur_line.find("[Territories]") != std::string::npos) {
            parse_countries(continents, countries);
            has_parsed_countries = true;
        }
    }

    if (has_parsed_continents && has_parsed_countries) {
        return new Map(continents, countries);
    } else {
        return nullptr;
    }
}

void ConquestMapLoader::parse_continents(vector<Continent *> *continents) {
    string cur_line;
    string parsed[2];

    while (getline(*file_stream, cur_line)) {
        if (cur_line == "\r") {
            return;
        }

        int i = 0;
        string cur_value;
        stringstream string_stream(cur_line);
        while (getline(string_stream, cur_value, '=') && i < 2) {
            parsed[i] = cur_value;
            i++;
        }

        auto* c = new Continent(parsed[0], stoi(parsed[1]));
        continents->push_back(c);
    }
}

void ConquestMapLoader::parse_countries(vector<Continent *> *continents, vector<Country *> *countries) {
    //We're going to first add all the countries in a similar way to the regular files.
    string cur_line;
    string parsed[4];
    auto* border_countries = new vector<vector<string>*>();

    while (getline(*file_stream, cur_line)) {
        if(cur_line == "\r") {
            continue;
        }
        int i = 0;
        string cur_value;
        stringstream string_stream(cur_line);
        auto* borders = new vector<string>();

        while (getline(string_stream, cur_value, ',')) {
            if(i < 4) {
                parsed[i] = cur_value;
            } else {
                cur_value.erase( std::remove(cur_value.begin(), cur_value.end(), '\r'), cur_value.end() );
                borders->push_back(cur_value);
            }
            i++;
        }

        border_countries->push_back(borders);

        int index = -1;
        for(int i=0; i<continents->size(); i++) {
            if(continents->at(i)->get_name() == parsed[3])
                index = i;
        }
        Continent* continent = continents->at(index); //Find continent given an ID

        auto* c = new Country(parsed[0], continent);
        countries->push_back(c);
        continent->add_country(c);

    }

    for(int i=0; i<countries->size(); i++) {
        for(int j=0; j<border_countries->at(i)->size(); j++) {
            for(int k=0; k<countries->size(); k++) {
                if(countries->at(k)->get_name() == border_countries->at(i)->at(j)) {
                    countries->at(i)->add_adjacent_country(countries->at(k));
                    break;
                }
            }
        }
    }
}

ConquestAdapter::ConquestAdapter() {
    conquest_map_loader = new ConquestMapLoader();
}

Map *ConquestAdapter::read_map(const string &file_name) {
    return conquest_map_loader->read_conquest_map(file_name);
}
