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
    // Constructors
    Estudante();
    Estudante(const int& numero);
    Estudante(const Estudante &estudante);
    Estudante(const int& numero, const std::string& nome, const std::set<std::pair<std::string, std::string>> &turmas, const int& ano);

    // Getters
    const int getStudentNumber() const;
    const std::string getStudentName() const;
    const int getAno() const;
    const std::string getTurma(const std::string &uc) const;
    const std::set<std::pair<std::string, std::string>> getTurmas() const;

    // Setters
    void setTurmas(std::set<std::pair<std::string, std::string>> &turmas);
    void changeTurma(const std::string &uc, const std::string &turmaNova);
    void removerUc(const std::string &uc);
    void adicionarUc(const std::string &uc, const std::string &turma);

    // Validators
    bool inscrito(std::string &uc) const;
    bool operator==(const Estudante &estudante) const;
};


#endif //AED2324_PRJ1_G_109__STUDENT_H
