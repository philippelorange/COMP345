#include <iostream>
#include "Map/MapLoader/MapLoader.h"

void validate(Map* pMap);

int main() {
    auto* loader = new MapLoader();

    //Loading a valid map of Canada
    cout << "*** Reading from canada.map ***" << endl;
    auto map = loader->read_map("../canada.map");

    validate(map);

    //Loading an invalid map file, without the three sections (borders, continents, countries)
    cout << "*** Reading from invalid.map ***" << endl;
    map = loader->read_map("../invalid.map");

    validate(map);

    return 0;
}

void validate(Map* pMap) {
    if (pMap == nullptr) {
        cout << "Map has not been generated from MapLoader. The .map file is invalid." << endl;
    } else if (!pMap->validate_connected_graph() || !pMap->validate_continent_singularity()) {
        cout << "Invalid map file; verify map is connected and countries belong to a single continent." << endl;
    } else {
        cout << "Valid map file!" << endl;
    }
}
