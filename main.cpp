#include <iostream>
#include "FilesRepository/test.cpp" // Include the header file

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::map<std::string, std::vector<std::string>> test = openClassPerUc("/Users/wagner/Desktop/project/CSV/classes_per_uc.csv");
    int count=0;
    for (auto p : test) {
        std::cout << "UC Code: " << p.first << " (" << p.second.size() << " classes)" << std::endl;
        count++;
        for (auto x : p.second) {
            std::cout << "Class Code: " << x << std::endl;
            count++;
        }
    }
    cout<< count;
    return 0;
}