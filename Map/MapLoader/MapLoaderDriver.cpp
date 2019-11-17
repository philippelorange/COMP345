#include <iostream>
#include "Map/MapLoader/MapLoader.h"

void validate(Map* pMap);

int main() {
    auto* loader = new MapLoader();

    //Loading a valid map of Canada
    cout << endl << "*** Reading from canada.map ***" << endl;
    auto map = loader->read_map("../Map/Maps/Domination/canada.map");

    validate(map);

    //Loading an invalid map file, without the three sections (borders, continents, countries)
    cout << endl << "*** Reading from invalid.map ***" << endl;
    map = loader->read_map("../Map/Maps/Domination/invalid.map");

    validate(map);

    MapLoader* l = new ConquestAdapter();
    cout << endl << "*** Reading from Chicago.map, the Conquest map ***" << endl;
    auto adaptedMap = l->read_map("../Map/Maps/Conquest/Chicago.map");
    validate(adaptedMap);

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
