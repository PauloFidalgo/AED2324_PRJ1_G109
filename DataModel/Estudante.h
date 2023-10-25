//
// Created by nelson on 05-10-2023.
//

#ifndef AED2324_PRJ1_G_109__STUDENT_H
#define AED2324_PRJ1_G_109__STUDENT_H
#include <string>
#include <set>


class Estudante {
private:
    int numero;
    std::string nome;
    std::set<std::pair<std::string, std::string>> turmas;
    int ano;

public:
    Estudante(const int& numero, const std::string& nome, const std::set<std::pair<std::string, std::string>> &turmas, const int& ano);
    const int getStudentNumber();
    const std::string getStudentName();
    const int getAno();
    const std::set<std::pair<std::string, std::string>> getTurmas();
    const bool inscrito(std::string &uc);
    bool changeTurma(const std::string uc, const std::string turmaNova);
    bool operator<(const Estudante &estudante) const;
    const std::string getTurma(const std::string &uc);
};


#endif //AED2324_PRJ1_G_109__STUDENT_H
