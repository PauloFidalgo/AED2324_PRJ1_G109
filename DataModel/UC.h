//
// Created by Paulo Fidalgo on 18/10/2023.
//

#ifndef PROJECT_UC_H
#define PROJECT_UC_H
#include <string>
#include "TurmaUC.h"

class UC {
private:
    std::string codigoUC;
    std::set<TurmaUC> turmas;

public:
    UC(const std::string &codigoUc, const std::set<TurmaUC> &turmas);
};


#endif //PROJECT_UC_H
