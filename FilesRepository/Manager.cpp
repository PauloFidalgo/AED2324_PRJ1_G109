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
               ucs.insert(UC(lastUc,turmaUc));
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
                estudantesNumero.insert(Estudante(lastCode,nomeEstudante,turmas,ano));
                estudantesNome.insert(Estudante(lastCode,nomeEstudante,turmas,ano));
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

            auto it = ucs.find(UC(uc));
            if (it != ucs.end()) {
                UC atualizada = UC(*it);
                atualizada.addEstudante(turma,studentCode,nomeEstudante);
                ucs.erase(it);
                ucs.insert(atualizada);
            }
        }
    }
    catch (const ifstream::failure& e){
        cout << "Failed to open file." << endl;
    }

    iff.close();
}

void Manager::addPedido(Pedido &pedido) {
    if (pedidoValido(pedido)) {
        pedidos.push(pedido);
    }
}

bool Manager::pedidoValido(Pedido &pedido) {
    Estudante estudante = pedido.getEstudante();
    Estudante outro = pedido.getOutroEstudante();
    std::string uc = pedido.getUc();


    if (!(estudante.inscrito(uc) && outro.inscrito(uc))) {
        // Pelo menos um dos alunos não esta inscrito na UC
        cout << "Troca inválida, a Unidade curricular não é comum" << endl;
        return false;
    }

    std::string turma = estudante.getTurma(uc);
    std::string outraTurma = outro.getTurma(uc);

    if (!(turma != outraTurma)) {
        // Já são de turmas iguais
        cout << "Troca inválida, já são da mesma turma" << endl;
        return false;
    }

    // Verificar o horário de cada estudante
    list<Aula> horarioUm = obterHorarioEstudantePraticas(estudante);
    Aula aula = obterPraticaUc(uc,outraTurma);
    list<Aula> horarioOutro = obterHorarioEstudantePraticas(outro);
    Aula aulaNova = obterPraticaUc(uc,turma);

    return verificarAulaSobreposta(horarioUm,aula) && verificarAulaSobreposta(horarioOutro, aulaNova);

    /*
    for (auto &aula : horarioUm) {
        aula.printData();
        if (aula.sobreposta(aulaNova)) {
            cout << "Troca inválida, aula Prática sobreposta" << endl;
            return false;
        }
    }
    list<Aula> horarioOutro = obterHorarioEstudantePraticas(outro);
    aulaNova = obterPraticaUc(uc,turma);

    for (auto &aula : horarioOutro) {
        if (aula.sobreposta(aulaNova)) {
            cout << "Troca inválida, aula Prática sobreposta" << endl;
            return false;
        }
    }*/

    return true;

}

bool Manager::verificarAulaSobreposta(const list<Aula> &horario, const Aula &aulaNova) const {
    for (auto &aula : horario) {
        aula.printData();
        if (aula.sobreposta(aulaNova)) {
            cout << "Alteração inválida, aula Prática sobreposta" << endl;
            return false;
        }
    }
    return true;
}

void Manager::executarPedido(Pedido &pedido) {
    string uc = pedido.getUc();
    Estudante estudante = pedido.getEstudante();
    string turmaEstudante = estudante.getTurma(uc);
    int numeroEstudante = estudante.getStudentNumber();

    Estudante outro = pedido.getOutroEstudante();
    string turmaOutro = outro.getTurma(uc);
    int numeroOutro = outro.getStudentNumber();

    //Alterar UCs
    addEstudanteToUc(uc, turmaOutro, numeroEstudante, estudante.getStudentName());
    addEstudanteToUc(uc, turmaEstudante, numeroOutro, outro.getStudentName());
    removeEstudanteFromUc(uc, turmaEstudante, numeroEstudante);
    removeEstudanteFromUc(uc, turmaOutro, numeroOutro);


    //Alterar Estudante
    estudante.changeTurma(uc,outro.getTurma(uc));
    outro.changeTurma(uc,turmaEstudante);

}

void Manager::addEstudanteToUc(const string &uc, const string& turma, const int &numero, const string &nome) {
    auto it = ucs.find(UC(uc));

    if (it != ucs.end()) {
        UC atualizada = UC(*it);
        atualizada.addEstudante(turma, numero, nome);
        ucs.erase(it);
        ucs.insert(atualizada);
    }
}

void Manager::removeEstudanteFromUc(const string &uc, const string& turma, const int &numero) {
    auto it = ucs.find(UC(uc));

    if (it != ucs.end()) {
        UC atualizada = UC(*it);
        atualizada.removeEstudante(turma, numero);
        ucs.erase(it);
        ucs.insert(atualizada);
    }
}

void Manager::proximoPedido() {
    if (!(pedidos.empty())) {
        executarPedido(pedidos.front());
        historico.push(pedidos.front());
        pedidos.pop();
    }
}



void Manager::reverterPedido() {
    if (!(historico.empty())) {
        Pedido last = historico.top();
        executarPedido(last);
        historico.pop();
    }
}

unordered_map<string,list<Aula>> Manager::obterHorarioEstudante(const Estudante &estudante) const {
    set<pair<string,string>> turmas = estudante.getTurmas();
    unordered_map<string,list<Aula>> res;

    for (const auto& par : turmas) {
        TurmaInfo turmaInfo = obterInfoUc(par.first, par.second);
        res.insert({par.first, turmaInfo.aulas});
    }

    return res;
}

list<Aula> Manager::obterHorarioEstudantePraticas(const Estudante &estudante) const {
    set<pair<string,string>> turmas = estudante.getTurmas();
    list<Aula> res;
    for (const auto& par : turmas) {
        res.push_back(obterPraticaUc(par.first, par.second));

    }

    return res;
}

TurmaInfo Manager::obterInfoUc(const string &uc, const string &turma) const {
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

Aula Manager::obterPraticaUc(const string &uc, const string &turma) const {
    Aula res;

    for (auto u : ucs) {
        if (uc == u.getCodigoUc()) {
            res = u.getPratica(turma);
        }
    }

    return res;
}

Estudante Manager::getEstudante(const int &numero) const {
    Estudante res;
    auto it = estudantesNumero.lower_bound(numero);
    if (it->getStudentNumber() == numero) {
        res = *it;
    }
    return res;
}

auto compareFirstElement = [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
    return a.first < b.first;
};
auto compareSecondElement = [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b)  {
    return a.second < b.second;
};

void Manager::printEstudantesPorTurma(const string &uc, const string &turma, bool order_by_number, bool ascending) const {
    auto it = ucs.find(uc);

    if (it != ucs.end()) {
        unordered_map<string,TurmaInfo> turmaInfo = it->getUcTurma();
        auto iterator = turmaInfo.find(turma);

        if (iterator != turmaInfo.end()) {
            cout << "UC: " << it->getCodigoUc() << " Turma: " << iterator->first << endl;
            cout << "-----------------------------------"  << endl;
            list<pair<int,string>> studentList = iterator->second.estudantes;

            if (order_by_number) studentList.sort(compareFirstElement);

            else studentList.sort(compareSecondElement);

            if (ascending){
                for (const auto& element : studentList){
                    cout << element.first << ' ' << element.second << endl;
                }
            }
            else{
                for (auto i = studentList.rbegin(); i != studentList.rend(); ++i){
                    cout << i->first << ' ' << i->second << endl;
                }
            }
        }
        cout << "-----------------------------------"  << endl;
    }
}

struct CompareByFirst{
    bool operator()(const pair<int, string>& a, const pair<int, string>& b) const {
        return a.first < b.first;
    }
};


struct CompareBySecond{
    bool operator()(const pair<int, string>& a, const pair<int, string>& b) const {
        return a.second < b.second;
    }
};

void Manager::printEstudantesPorUC(const string &uc, const bool& orderByNumber, const bool& ascending) const {

    if (orderByNumber) {
        set<pair<int, string>, CompareByFirst> studentList;
        auto it = ucs.find(uc);

        if (it != ucs.end()) {
            cout << "Estudantes inscritos na UC: " << it->getCodigoUc() << endl;
            cout << "-----------------------------------"  << endl;
            for (const auto& turma : it->getUcTurma()){
                for (const auto& element : turma.second.estudantes){
                    studentList.insert(element);
                }
            }
        }

        if (studentList.empty()){
            cout << "There are no students in UC: " << uc << endl;
            return;
        }

        if (ascending){
            for (const auto& element : studentList){
                cout << element.first << ' ' << element.second << endl;
            }
        }
        else{
            for (auto i = studentList.rbegin(); i != studentList.rend(); ++i){
                cout << i->first << ' ' << i->second << endl;
            }
        }
        cout << "-----------------------------------"  << endl;
    } else {
        set<pair<int, string>, CompareBySecond> studentList;
        auto it = ucs.find(uc);

        if (it != ucs.end()) {
            cout << "Estudantes inscritos na UC: " << it->getCodigoUc() << endl;
            cout << "-----------------------------------"  << endl;
            for (const auto& turma : it->getUcTurma()){
                for (const auto& element : turma.second.estudantes){
                    studentList.insert(element);
                }
            }
        }

        if (studentList.empty()){
            cout << "There are no students in UC: " << uc << endl;
            return;
        }

        if (ascending){
            for (const auto& element : studentList){
                cout << element.first << ' ' << element.second << endl;
            }
        }
        else{
            for (auto i = studentList.rbegin(); i != studentList.rend(); ++i){
                cout << i->first << ' ' << i->second << endl;
            }
        }
        cout << "-----------------------------------"  << endl;
    }

}

void Manager::printEstudantesPorAno(const int &ano, const bool &orderByNumber, const bool &ascending) const {

    if (ano < 1 or ano > 3) {
        cout << "Ano Inválido.";
        return;
    }
    cout << "Estudantes do " << ano << "º ano: " << endl;
    cout << "-----------------------------------"  << endl;
    if (orderByNumber){
        if (ascending){
            for (const auto& estudante : estudantesNumero){
                if (estudante.getAno() == ano){
                    cout << estudante.getStudentNumber() << ' ' << estudante.getStudentName() << endl;
                }
            }
        }
        else {
            for (auto i = estudantesNumero.rbegin(); i != estudantesNumero.rend(); ++i){
                cout << i->getStudentNumber() << ' ' << i->getStudentName() << endl;
            }
        }
    }
    else {
        if (ascending){
            for (const auto& estudante : estudantesNome){
                if (estudante.getAno() == ano){
                    cout << estudante.getStudentNumber() << ' ' << estudante.getStudentName() << endl;
                }
            }
        }
        else {
            for (auto i = estudantesNome.rbegin(); i != estudantesNome.rend(); ++i){
                cout << i->getStudentNumber() << ' ' << i->getStudentName() << endl;
            }
        }
    }
    cout << "-----------------------------------"  << endl;
}

void Manager::printTurmasPorUC(const std::string &uc, const bool &ascending) const {
    set<string> turmas;
    cout << "Turmas da UC: " << uc << endl;
    cout << "-----------------------------------"  << endl;
    auto it = ucs.find(uc);

    if (it != ucs.end()) {
        for (const auto& element : it->getUcTurma()){
            turmas.insert(element.first);
        }
    }

    if (ascending){
        for (auto turma : turmas){
            cout << turma << endl;
        }
    }
    else {
        for (auto i = turmas.rbegin(); i != turmas.rend(); ++i){
            cout << *i << endl;
        }
    }
    cout << "-----------------------------------"  << endl;
}

void Manager::numeroEstudantesEmPeloMenosNUCS(const int &nUcs, const bool& orderByNumber, const bool& ascending) const{
    if (nUcs < 0 || nUcs > ucs.size()) {
        cout << "Número inválido de Unidades Curriculares" << endl;
        return;
    }
    stringstream ss;
    list<Estudante> students;
    if (orderByNumber) {
        if (ascending) {
            for (auto& estudante : estudantesNumero){
                if (estudante.getTurmas().size() >= nUcs){
                    students.push_back(estudante);
                    ss << estudante.getStudentNumber() << ' ' << estudante.getStudentName() << endl;
                }
            }
        }
        else {
            for (auto i = estudantesNumero.rbegin(); i != estudantesNumero.rend(); ++i){
                if (i->getTurmas().size() >= nUcs){
                    students.push_back(*i);
                    ss << i->getStudentNumber() << ' ' << i->getStudentName() << endl;
                }
            }
        }
    }
    else {
        if (ascending) {
            for (auto& estudante : estudantesNome){
                if (estudante.getTurmas().size() >= nUcs){
                    students.push_back(estudante);
                    ss << estudante.getStudentNumber() << ' ' << estudante.getStudentName() << endl;
                }
            }
        }
        else {
            for (auto i = estudantesNome.rbegin(); i != estudantesNome.rend(); ++i){
                if (i->getTurmas().size() >= nUcs){
                    students.push_back(*i);
                    ss << i->getStudentNumber() << ' ' << i->getStudentName() << endl;
                }
            }
        }
    }
    cout << "Estão inscritos " << students.size() << " alunos em pelo menos " << nUcs << " Unidades Curriculares." << endl;
    cout << "-----------------------------------"  << endl;
    cout << ss.str();
    cout << "-----------------------------------"  << endl;
}

// ------------------------------------------------------------------------------------------------

void Manager::printStudents() {

    for (const auto& estudante : estudantesNumero) {
        cout << "Nº: " << estudante.getStudentNumber() << " Nome: " << estudante.getStudentName() << " Ano: " << estudante.getAno() << " ";
        for (const auto& turma : estudante.getTurmas()) {
            cout << " UC: " << turma.first << " Turma: " << turma.second << " ";
        }
        cout << endl;
    }
}

void Manager::printUc() {

    for (auto uc : ucs) {
        cout << "UC: " << uc.getCodigoUc() << endl;
        cout << "-----------------------" << endl;
        for (const auto& a: uc.getUcTurma()) {
            cout << "-----------------------" << endl;
            cout << a.first << endl;
            for (const auto& b : a.second.estudantes) {
                cout << b.first << endl;
            }
            cout << "-----------------------" << endl;
        }
        cout << "-----------------------" << endl;
    }
}

void Manager::printHorario() {
    int i = 0;
    for (const auto& est : estudantesNumero) {
        cout << est.getStudentNumber() << endl;
        cout << est.getStudentName() << endl;
        unordered_map<string,list<Aula>> horario = obterHorarioEstudante(est);
        for (const auto& h : horario) {
            cout << "UC: " << h.first << endl;
            for (const auto& j : h.second) {
                j.printData();
            }
        }
        i++;
        if (i == 1) break;
    }
}


void Manager::testGet() const {
    int num = 202044272;
    Estudante estudante = getEstudante(num);
    cout << estudante.getStudentNumber() << " " << estudante.getStudentName() << endl;
}

void Manager::printHorarioEstudante(Estudante estudante) {
        cout << estudante.getStudentNumber() << endl;
        cout << estudante.getStudentName() << endl;
        unordered_map<string,list<Aula>> horario = obterHorarioEstudante(estudante);
        for (const auto& h : horario) {
            cout << "UC: " << h.first << endl;
            for (const auto &j: h.second) {
                j.printData();
            }
        }
}

void Manager::fakeTroca() {
    int numero1 = 202025232;
    int numero2 = 202024127;
    Estudante estudante1 = getEstudante(numero1);
    Estudante estudante2 = getEstudante(numero2);
    string uc = "L.EIC002";
    bool validUc = false;

    for (auto a : ucs) {
        if (a.getCodigoUc() == uc) validUc = (numero1 != numero2);
    }

    if (estudante1.getStudentNumber() != 0 && estudante2.getStudentNumber() != 0 && validUc) {
        cout << "Pedidos: " << pedidos.size() << endl;
        cout << "Antes pedido: " << endl;
        printHorarioEstudante(estudante1);
        cout << "-------------------------" << endl;
        printHorarioEstudante(estudante2);
        cout << endl << endl;
        Pedido pedido = Pedido(uc, estudante1, estudante2);
        this->addPedido(pedido);
        cout << "Pedidos: " << pedidos.size() << endl;
        this->proximoPedido();
        cout << "Pedidos: " << pedidos.size() << endl;
        cout << "Depois pedido: " << endl;
        printHorarioEstudante(estudante1);
        cout << "-------------------------" << endl;
        printHorarioEstudante(estudante2);
    }
}

