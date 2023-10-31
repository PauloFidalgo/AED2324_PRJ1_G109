#include <iostream>// Include the header file
#include "FilesRepository/Manager.h"
#include "FilesRepository/Menu.h"

int main() {
    Manager m;
    m.readFiles();
    auto a = m.getUcPorAnos(1);
    auto b = m.getUcPorAno(1);

    for (auto c : a) {
        cout << c << endl;
    }

    for (auto c : b) {
        cout << c << endl;
    }

    return 0;

}


