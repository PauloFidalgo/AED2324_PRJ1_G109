//
// Created by nelson on 05-10-2023.
//

#ifndef AED2324_PRJ1_G_109__STUDENT_H
#define AED2324_PRJ1_G_109__STUDENT_H
#include <string>
#include "TurmaUC.h"
#include <set>


class Estudante {
private:
    int numero;
    std::string nome;
    std::set<TurmaUC> turmas;
    int ano;

public:
    Estudante(const int& numero, const std::string& nome, const std::set<TurmaUC>& turmas, const int& ano);
    const int getStudentNumber();
    const std::string getStudentName();
    const std::list<TurmaUC> getStudentClassUc();
    bool operator<(const Estudante &estudante) const;
};


#endif //AED2324_PRJ1_G_109__STUDENT_H
