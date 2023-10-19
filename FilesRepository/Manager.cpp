//
// Created by Wagner Pedrosa on 19/10/2023.
//

#include "Manager.h"
#include "fstream"
#include "sstream"
#include "string"
using namespace std;

void Manager::readFiles()
{
    // reading students_classes.csv
    fstream iff;
    try {
        iff.open("../CSV/students_classes.csv", ios::in);
        string line, word, temp, nomeEstudante, uc, turma;
        int studentCode, lastCode, ano = 1;
        set<TurmaUC> turmas;
        bool first = true;

        while (iff >> temp) {
            stringstream s(temp);
            s>>studentCode;

            if (first) {
                lastCode = studentCode;
                first = false;
            }

            if (studentCode == lastCode) {
                getline(s,nomeEstudante,',');
                getline(s, uc, ',');
                getline(s,turma,',');
                turmas.insert(TurmaUC(uc,turma));
                if (turma[0] > ano) ano = turma[0];

            }
            else {
                estudantes.insert(Estudante(lastCode,nomeEstudante,turmas,ano));
                lastCode = studentCode;
                turmas.clear();
                getline(s,nomeEstudante,',');
                getline(s, uc, ',');
                getline(s,turma,',');
                turmas.insert(TurmaUC(uc,turma));
                if (turma[0] > ano) ano = turma[0];
            }

        }
    }
    catch (const ifstream::failure& e){
        cout << "Failed to open file." << endl;
    }

    iff.close();

}
