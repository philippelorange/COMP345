#include <iostream>
#include "MapLoader.h"

int main() {
    MapLoader* loader = new MapLoader();

    auto map = loader->read_map("../canada.map");

    if(!map->validate_connected_graph() || ! map->validate_continent_singularity()) {
        cout << "Invalid map file.";
        return 1;
    }

    cout << "Valid map file.";
    return 0;
}