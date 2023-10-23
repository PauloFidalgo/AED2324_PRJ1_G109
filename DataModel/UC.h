//
// Created by Paulo Fidalgo on 18/10/2023.
//

#ifndef PROJECT_UC_H
#define PROJECT_UC_H
#include <string>
#include <unordered_map>
#include <set>
#include "Aula.h"

struct TurmaInfo {
    std::set<int> estudantes;
    Aula teorica;
    Aula pratica;
};

class UC {
private:
    std::string codigoUC;
    std::unordered_map<std::string, std::unordered_map<std::string, TurmaInfo>> ucTurma;

public:
    UC(const std::string &codigoUc, const std::unordered_map<std::string, std::unordered_map<std::string, TurmaInfo>> &ucTurma);
};


#endif //PROJECT_UC_H
