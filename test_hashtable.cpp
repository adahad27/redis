#include "table.hpp"
#include <iostream>

int main() {
    
    HTable hashmap(1);
    hashmap.insert("hello", "from the other side");
    hashmap.insert("We will", "We will Rock you");
    hashmap.insert("They don't really", "Care about us");

    std::cout << hashmap.get("hello") << std::endl;
    std::cout << hashmap.get("We will") << std::endl;
    std::cout << hashmap.get("They don't really") << std::endl;
    // hashmap.remove("We will");
    // hashmap.remove("They don't really");
    std::cout << hashmap.get("hello") << std::endl;
    std::cout << hashmap.get("They don't really") << std::endl;
    std::cout << hashmap.get("We will") << std::endl;

    HTable map(2);
    map.insert("Ophelia", "The Lumineers");
    map.insert("Ho Hey", "The Lumineers");

    map = hashmap;

    std::cout << map.get("They don't really") << std::endl;
}