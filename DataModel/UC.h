//
// Created by Paulo Fidalgo on 18/10/2023.
//

#ifndef PROJECT_UC_H
#define PROJECT_UC_H
#include <string>
#include <unordered_map>
#include <list>
#include "Aula.h"

struct TurmaInfo {
    std::list<int> estudantes;
    Aula teorica;
    Aula pratica;
};

class UC {
private:
    std::string codigoUC;
    std::unordered_map<std::string, TurmaInfo> ucTurma;

public:
    UC(const std::string &codigoUc, const std::unordered_map<std::string, TurmaInfo> &ucTurma);
    UC(const std::string &codigoUc);
    const std::unordered_map<std::string, TurmaInfo> getUcTurma();
    void addTurma(const std::string &turma, const TurmaInfo &turmaInfo);
    void addEstudantes(const std::string &turma, const std::list<int> &estudantes);
    void addEstudante(const std::string &turma, int &estudante);
    std::string &getCodigoUc();

};


#endif //PROJECT_UC_H
