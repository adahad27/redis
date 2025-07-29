#include "table.hpp"
#include <iostream>

int main() {
    
    // HTable hashmap(1);
    // hashmap.insert("hello", "from the other side");
    // hashmap.insert("We will", "We will Rock you");
    // hashmap.insert("They don't really", "Care about us");

    // std::cout << hashmap.get("hello") << std::endl;
    // std::cout << hashmap.get("We will") << std::endl;
    // std::cout << hashmap.get("They don't really") << std::endl;
    // // hashmap.remove("We will");
    // // hashmap.remove("They don't really");
    // std::cout << hashmap.get("hello") << std::endl;
    // std::cout << hashmap.get("They don't really") << std::endl;
    // std::cout << hashmap.get("We will") << std::endl;

    // HTable map(2);
    // map.insert("Ophelia", "The Lumineers");
    // map.insert("Ho Hey", "The Lumineers");

    // map = hashmap;

    // std::cout << map.get("They don't really") << std::endl;

    HMap hmap(1);
    hmap.insert("Hello", "Adele");
    hmap.insert("Summer Days", "Macklemore");
    hmap.insert("All The Stars", "Kendrick Lamar");
    hmap.insert("Stay", "Zedd");
    hmap.remove("Hello");
    hmap.insert("Maps", "Maroon 5");
    hmap.insert("Gone are the Days", "Kygo");
    hmap.insert("Hero", "Faouzia");
    hmap.insert("These are the Lies", "The Cab");
    hmap.insert("Duality", "Set it Off");
    hmap.insert("Whisper", "Burn the Ballroom");

    std::cout << hmap.get("Whisper") << std::endl;
    // std::cout << hmap.get("Hero") << std::endl;
    std::cout << hmap.get("Gone are the Days") << std::endl;
    std::cout << hmap.get("Maps") << std::endl;
    std::cout << hmap.get("Stay") << std::endl;
}