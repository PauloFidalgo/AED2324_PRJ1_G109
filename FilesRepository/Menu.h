#ifndef PROJECT_MENU_H
#define PROJECT_MENU_H

#include <vector>
#include <string>

class Menu {
private:
    std::vector<std::vector<std::string>> table;

public:
    Menu();
    void createTable();
    void printTableTurma();
    void printTableUC();
    void iniciar();
};

#endif // PROJECT_MENU_H
