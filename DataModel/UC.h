//
// Created by Paulo Fidalgo on 18/10/2023.
//

#ifndef PROJECT_UC_H
#define PROJECT_UC_H
#include <string>
#include <map>
#include <list>
#include <vector>
#include <set>


#include "Aula.h"

struct TurmaInfo {
    std::set<std::pair<int,std::string>> estudantes;
    std::list<Aula> aulasTeoricas;
    Aula pratica;
};

class UC {
private:
    std::string codigoUC;
    std::map<std::string, TurmaInfo> ucTurma;
    float media;
    int nAlunos;
    int ano;

public:
    // Constructors
    UC(const std::string &codigoUc, const std::map<std::string, TurmaInfo> &ucTurma, int media = 0);
    UC(std::string codigoUc);
    UC(const UC &other);

    // Getters
    std::map<std::string, TurmaInfo> getUcTurma() const;
    std::list<Aula> getAulasTurma(const std::string &turma) const;
    std::vector<std::pair<std::string,std::pair<std::string,Aula>>> getAulasUc() const;
    std::string getCodigoUc() const;
    Aula getPratica(const std::string &turma) const;
    int getNumeroAlunos(const std::string &turma) const;
    int getMinimum() const;
    int getAno() const;
    int getNumeroAlunosTotal() const;
    int getNumeroTurmas() const;
    float getMedia() const;

    // Setters
    void addTurma(const std::string &turma, const TurmaInfo &turmaInfo);
    void addEstudantes(const std::string &turma, const std::list<std::pair<int,std::string>> &estudantes);
    void addEstudante(const std::string &turma, const int &estudante,const std::string &nome);
    void removeEstudante(const std::string &turma, const int &numero, const std::string &nome);

    // Validators
    bool verificarTurma(const std::string &turma) const;
    bool checkBalance(const std::string &turma) const;
    bool operator<(const UC uc) const;

};


#endif //PROJECT_UC_H
