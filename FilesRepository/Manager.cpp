//
// Created by Wagner Pedrosa on 19/10/2023.
//

#include "Manager.h"
#include "fstream"
#include "sstream"
#include "string"
#include <iostream>


void Manager::readFiles()
{
    // reading students_classes.csv
    fstream iff;
    try {
        iff.open("../CSV/classes.csv", ios::in);
        string line, uc, turma, dia, tipo, lastUc, inicio, duracao;
        bool first = true;

        getline(iff, line);

        unordered_map<string,TurmaInfo> turmaUc;

        while (getline(iff,line)) {
            stringstream s(line);
            getline(s,turma,',');
            getline(s,uc,',');
            getline(s,dia,',');
            getline(s,inicio,',');
            getline(s,duracao,',');
            getline(s,tipo,'\r');
            TurmaInfo turmaInfo;
            turmaInfo.pratica = (tipo == "TP" || tipo == "PL") ? Aula(dia, stof(inicio), stof(duracao), tipo) : Aula();

            if (tipo == "T") {
                auto it = turmaUc.find(turma);

                if (it != turmaUc.end()) {
                    it -> second.teorica = Aula(dia, stof(inicio), stof(duracao), tipo);
                } /*
                for (auto &ele: turmaUc) {
                    if (ele.first == turma) {
                        ele.second.teorica = Aula(dia, stof(inicio), stof(duracao), tipo);
                    }
                } */
            }

            if (first) {
                lastUc = uc;
                first = false;
            }

           if (lastUc == uc) {
               turmaUc.insert({turma,turmaInfo});
           }

           else {
               ucs.emplace_back(lastUc,turmaUc);
               turmaUc.clear();
               turmaUc.insert({turma,turmaInfo});
               lastUc = uc;
           }
        }
    }
    catch (const ifstream::failure& e){
        cout << "Failed to open file." << endl;
    }

    iff.close();

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

            for (auto &ele : ucs) {
                if (ele.getCodigoUc() == uc) {
                    ele.addEstudante(turma,studentCode);
                }
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

void Manager::printUc() {

    for (auto uc : ucs) {
        cout << "UC: " << uc.getCodigoUc() << endl;
        cout << "-----------------------" << endl;
        for (auto a: uc.getUcTurma()) {
            cout << a.first << endl;
            a.second.teorica.printData();
        }
        cout << "-----------------------" << endl;
    }
}

