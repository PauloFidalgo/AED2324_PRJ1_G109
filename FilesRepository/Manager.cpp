//
// Created by Wagner Pedrosa on 19/10/2023.
//

#include "Manager.h"
#include "fstream"
#include "sstream"
#include "string"
#include <iostream>
using namespace std;

void Manager::readFiles()
{
    // reading students_classes.csv
    fstream iff;
    try {
        iff.open("../CSV/students_classes.csv", ios::in);
        string line, word, temp, nomeEstudante, uc, turma;
        int studentCode, lastCode, ano = 1;
        set<pair<string,string>> turmas;
        bool first = true;
        getline(iff,temp);

        while (getline(iff, temp)) {
            stringstream s(temp);
            s >> studentCode;

            if (first) {
                lastCode = studentCode;
                first = false;
            }

            if (studentCode == lastCode) {
                getline(s, word, ',');
                getline(s,nomeEstudante, ',');
                getline(s, uc, ',');
                getline(s,turma, '\r');
                turmas.insert(pair(uc,turma));
                if (turma[0] > ano) ano = turma[0] - '0';

            }
            else {
                estudantes.insert(Estudante(lastCode,nomeEstudante,turmas,ano));
                lastCode = studentCode;
                turmas.clear();
                getline(s, word, ',');
                getline(s,nomeEstudante,',');
                getline(s, uc, ',');
                getline(s,turma,'\r');
                turmas.insert(pair(uc,turma));
                ano = 1;
                if (turma[0] > ano) ano = turma[0] - '0';
            }

        }
    }
    catch (const ifstream::failure& e){
        cout << "Failed to open file." << endl;
    }

    iff.close();

}

void Manager::printStudents() {

    for (auto estudante : estudantes) {
        cout << "Nº: " << estudante.getStudentNumber() << " Nome: " << estudante.getStudentName() << " Ano: " << estudante.getAno() << " ";
        for (auto turma : estudante.getTurmas()) {
            cout << " UC: " << turma.first << " Turma: " << turma.second << " ";
        }
        cout << endl;
    }
}
