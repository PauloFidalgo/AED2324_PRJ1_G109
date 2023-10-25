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
            turmaInfo.aulas.emplace_back(dia, stof(inicio), stof(duracao), tipo);

            if (tipo == "T") {
                auto it = turmaUc.find(turma);

                if (it != turmaUc.end()) {
                    it -> second.aulas.emplace_back(dia, stof(inicio), stof(duracao), tipo);
                }
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
                    ele.addEstudante(turma,studentCode,nomeEstudante);
                }
            }
        }
    }
    catch (const ifstream::failure& e){
        cout << "Failed to open file." << endl;
    }

    iff.close();
}

void Manager::addPedido(Pedido &pedido) {
    pedidos.push(pedido);
}

void Manager::executarPedido() {
    Pedido pedido = pedidos.front();
    Estudante estudante = pedido.getEstudante();
    Estudante outro = pedido.getOutroEstudante();
    std::string uc = pedido.getUc();

    if (!(estudante.inscrito(uc) && outro.inscrito(uc))) {
        // Pelo menos um dos alunos não esta inscrito na UC
        pedidos.pop();
        return;
    }

    std::string turma = estudante.getTurma(uc);
    std::string outraTurma = outro.getTurma(uc);

    if (!(turma != outraTurma)) {
        // Já são de turmas iguais
        pedidos.pop();
        return;
    }

    // Verificar o horário de cada estudante;
    set<pair<string,string>> ucsAluno = estudante.getTurmas();
    set<pair<string,string>> ucsOutro = outro.getTurmas();


}

unordered_map<string,list<Aula>> Manager::obterHorarioEstudante(Estudante &estudante) {
    set<pair<string,string>> turmas = estudante.getTurmas();
    unordered_map<string,list<Aula>> res;
    for (const auto& par : turmas) {
        TurmaInfo turmaInfo = obterInfoUc(par.first, par.second);
        res.insert({par.first, turmaInfo.aulas});
    }

    return res;
}

TurmaInfo Manager::obterInfoUc(const string &uc, const string &turma) {
    TurmaInfo res;

    for (auto u : ucs) {
        if (uc == u.getCodigoUc()) {
            unordered_map<string, TurmaInfo> turmaInfo = u.getUcTurma();
            auto it = turmaInfo.find(turma);
            if (it->first == turma) {
                res = it->second;
            }
        }
    }

    return res;
}

void Manager::printStudents() {

    for (auto estudante : estudantes) {
        cout << "Nº: " << estudante.getStudentNumber() << " Nome: " << estudante.getStudentName() << " Ano: " << estudante.getAno() << " ";
        for (const auto& turma : estudante.getTurmas()) {
            cout << " UC: " << turma.first << " Turma: " << turma.second << " ";
        }
        cout << endl;
    }
}

void Manager::printUc() {

    for (auto uc : ucs) {
        cout << "UC: " << uc.getCodigoUc() << endl; /*
        cout << "-----------------------" << endl;
        for (const auto& a: uc.getUcTurma()) {
            cout << "-----------------------" << endl;
            cout << a.first << endl;
            for (auto b : a.second.estudantes) {
                cout << b << endl;
            }
            cout << "-----------------------" << endl;
        }
        cout << "-----------------------" << endl;*/
    }
}

void Manager::printHorario() {
    int i = 0;
    for (auto est : estudantes) {
        cout << est.getStudentNumber() << endl;
        cout << est.getStudentName() << endl;
        unordered_map<string,list<Aula>> horario = obterHorarioEstudante(est);
        for (auto h : horario) {
            cout << "UC: " << h.first << endl;
            for (auto i : h.second) {
                i.printData();
            }
        }
        i++;
        if (i == 1) break;
    }
}





