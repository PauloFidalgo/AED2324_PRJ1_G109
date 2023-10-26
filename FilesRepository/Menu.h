#ifndef PROJECT_MENU_H
#define PROJECT_MENU_H

#include <vector>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>

class Menu {
private:
    std::vector<std::vector<std::string>> table;
    std::time_t data = std::time(0);
    std:: tm* hora = localtime(&data);
    std:: thread timeThread;
    bool exitTimeThread;


public:
    Menu();
    ~Menu();
    void printmenuincial(const tm* hora);
    void createTable();
    void printTableTurma();
    void printTableUC();
    void iniciar();
    void updateTime();
};

#endif // PROJECT_MENU_H
