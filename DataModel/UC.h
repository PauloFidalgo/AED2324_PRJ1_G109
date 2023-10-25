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
    std::list<std::pair<int,std::string>> estudantes;
    std::list<Aula> aulas;
};

class UC {
private:
    std::string codigoUC;
    std::unordered_map<std::string, TurmaInfo> ucTurma;

public:
    UC(std::string &codigoUc, const std::unordered_map<std::string, TurmaInfo> &ucTurma);
    UC(std::string &codigoUc);
    std::unordered_map<std::string, TurmaInfo> getUcTurma();
    void addTurma(const std::string &turma, const TurmaInfo &turmaInfo);
    void addEstudantes(const std::string &turma, const std::list<std::pair<int,std::string>> &estudantes);
    void addEstudante(const std::string &turma, int &estudante, std::string &nome);
    std::string &getCodigoUc();
    Aula getPratica(const std::string &turma) const;

};


#endif //PROJECT_UC_H
