
#include "Manager.h"
#include "fstream"
#include "sstream"
#include "string"
#include <iostream>

int Manager::getPedidos() const {
    return this->pedidos.size();
}

void Manager::printHistorico() const {
    if (this->printHist.size() != 0) {
        cout << "---------------------------------------------------------------------" << endl;
        cout << "Histórico de alterações: " << endl;
        cout << "---------------------------------------------------------------------" << endl;
        for (auto line : this->printHist) {
            cout << line.first << " - " << line.second;
        }
        cout << "---------------------------------------------------------------------" << endl;
    }
    else {
        cout << "Não existem pedidos" << endl;
    }
}

bool Manager::verificarPedidosRepetidos(const Pedido &pedido) {
    for (auto p : pedidosEspera) {
        if (! p.pedidosConcorrentes(pedido)) {
            return false;
        }
    }
    return true;
}

void Manager::readFiles()
{
    // reading students_classes.csv
    fstream iff;
    try {
        iff.open("../CSV/classes.csv", ios::in);
        string line, uc, turma, dia, tipo, lastUc, inicio, duracao;
        bool first = true;
        float i = 0, n = 0;

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

            if (tipo != "T") {
                n += stoi(turma.substr(5));
                i++;
            }

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
               float media = round(n / i);
               ucs.insert(UC(lastUc,turmaUc, media));
               i = 0;
               n = 0;
               turmaUc.clear();
               turmaUc.insert({turma,turmaInfo});
               lastUc = uc;
           }
        }
        ucs.insert(UC(lastUc,turmaUc, n/i));
    }
    catch (const ifstream::failure& e){
        cout << "Falha a abrir o ficheiro" << endl;
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
        estudantesNumero.insert(Estudante(lastCode,nomeEstudante,turmas,ano));
        estudantesNome.insert(Estudante(lastCode,nomeEstudante,turmas,ano));
    }
    catch (const ifstream::failure& e){
        cout << "Falha a abrir o ficheiro" << endl;
    }

    iff.close();
}

bool Manager::addPedido(Pedido pedido) {
    bool res = false;

    if (!verificarPedidosRepetidos(pedido)) return false;

    switch (pedido.getTipoAlteracao()) {
        case TipoAlteracao::H: res = trocaValida(pedido);
            break;
        case TipoAlteracao::R: res = removerValida(pedido);
            break;
        case TipoAlteracao::A: res = validarNovaUc(pedido.getUc(), pedido.getEstudante().getStudentNumber());
            break;
    }
    if (res) {
        cout << "-----------------------------------"  << endl;
        cout << "Pedido adicionado à fila de espera" << endl;
        cout << "-----------------------------------"  << endl;
        pedidos.push(pedido);
        pedidosEspera.push_back(pedido);
    }
    return res;
}

bool Manager::removerValida(Pedido &pedido) {
    Estudante estudante = pedido.getEstudante();
    string uc = pedido.getUc();
    if (!estudante.inscrito(uc)) {
        cout << "O estudante não está inscrito à UC que pretende remover" << endl;
        return false;
    }
    pedido.setTurma(estudante.getTurma(uc));
    return true;
}

bool Manager::trocaValida(Pedido &pedido) {
    Estudante estudante = pedido.getEstudante();
    Estudante outro = pedido.getOutroEstudante();
    std::string uc = pedido.getUc();


    if (!(estudante.inscrito(uc) && outro.inscrito(uc))) {
        // Pelo menos um dos alunos não esta inscrito na UC
        cout << "------------------------------------------------"  << endl;
        cout << "Troca inválida, a Unidade curricular não é comum" << endl;
        cout << "------------------------------------------------"  << endl;
        return false;
    }

    std::string turma = estudante.getTurma(uc);
    std::string outraTurma = outro.getTurma(uc);


    if (turma == outraTurma) {
        // Já são de turmas iguais
        cout << "-------------------------------------------"  << endl;
        cout << "Troca inválida, já são da mesma turma" << endl;
        cout << "-------------------------------------------"  << endl;
        return false;
    }

    // Verificar o horário de cada estudante
    list<Aula> horarioUm = obterHorarioEstudantePraticasExceto(estudante,uc);
    Aula aula = obterPraticaUc(uc,outraTurma);
    list<Aula> horarioOutro = obterHorarioEstudantePraticasExceto(outro,uc);
    Aula aulaNova = obterPraticaUc(uc,turma);

    if (!(verificarAulaSobreposta(horarioUm,aula) && verificarAulaSobreposta(horarioOutro, aulaNova))) {
        cout << "-------------------------------------------"  << endl;
        cout << "Alteração inválida, aula Prática sobreposta" << endl;
        cout << "-------------------------------------------"  << endl;
        return false;
    }
    return true;
}

bool Manager::verificarAulaSobreposta(const list<Aula> &horario, const Aula &aulaNova) const {
    for (auto &aula : horario) {
        if (aula.sobreposta(aulaNova)) {
            return false;
        }
    }
    return true;
}


void Manager::executarPedidoTrocaHorario(Pedido &pedido) {
    string uc = pedido.getUc();
    Estudante estudante = pedido.getEstudante();
    string turmaEstudante = estudante.getTurma(uc);
    int numeroEstudante = estudante.getStudentNumber();

    Estudante outro = pedido.getOutroEstudante();
    string turmaOutro = outro.getTurma(uc);
    int numeroOutro = outro.getStudentNumber();


    trocaTurma(uc, turmaEstudante, numeroEstudante, estudante.getStudentName(), turmaOutro, numeroOutro, outro.getStudentName());

    auto it = estudantesNumero.find(estudante);
    auto itO = estudantesNumero.find(outro);

    if (it != estudantesNumero.end() && itO != estudantesNumero.end()) {
        Estudante estudanteAtualizado = Estudante(*it);
        Estudante estudanteMandelao = Estudante(*itO);
        estudanteAtualizado.changeTurma(uc,turmaOutro);
        estudanteMandelao.changeTurma(uc,turmaEstudante);
        estudantesNumero.erase(it);
        estudantesNumero.erase(itO);
        estudantesNumero.insert(estudanteAtualizado);
        estudantesNumero.insert(estudanteMandelao);
    }

    it = estudantesNome.find(estudante);
    itO = estudantesNome.find(outro);

    if (it != estudantesNome.end() && itO != estudantesNome.end()) {
        Estudante estudanteAtualizado = Estudante(*it);
        Estudante estudanteMandelao = Estudante(*itO);
        estudanteAtualizado.changeTurma(uc,turmaOutro);
        estudanteMandelao.changeTurma(uc,turmaEstudante);
        estudantesNome.erase(it);
        estudantesNome.erase(itO);
        estudantesNome.insert(estudanteAtualizado);
        estudantesNome.insert(estudanteMandelao);
    }
}

void Manager::removerEstudanteDaUc(Pedido &pedido) {
    string uc = pedido.getUc();
    Estudante estudante = pedido.getEstudante();
    auto it = ucs.find(uc);

    if (it != ucs.end()) {
        UC atualizada = UC(*it);
        atualizada.removeEstudante(pedido.getTurma(), estudante.getStudentNumber(), estudante.getStudentName());
        ucs.erase(it);
        ucs.insert(atualizada);
    }

    auto iterator = estudantesNumero.find(estudante);

    if (iterator != estudantesNumero.end()) {
        Estudante atualizado = Estudante(*iterator);
        atualizado.removerUc(uc);
        estudantesNumero.erase(iterator);
        estudantesNumero.insert(atualizado);
    }

    auto iteratorNome = estudantesNome.find(estudante);
    if (iteratorNome != estudantesNome.end()) {
        Estudante atualizadoNome = Estudante(*iteratorNome);
        atualizadoNome.removerUc(uc);
        estudantesNome.erase(iteratorNome);
        estudantesNome.insert(atualizadoNome);
    }
}

map<string,list<Aula>> Manager::getTurmasPossiveis(const string &uc, list<Aula> &horarioPraticas) {
    auto it = ucs.find(uc);
    map<string,list<Aula>> res;

    if (it != ucs.end()) {
        unordered_map<string,TurmaInfo> turmas = it->getUcTurma();

        for (const auto& t : turmas) {
            Aula pratica = obterPraticaUc(uc, t.first);
            if (verificarAulaSobreposta(horarioPraticas,pratica)) {
                TurmaInfo info = obterInfoUc(uc,t.first);
                if (info.estudantes.size() < 30 && it->checkBalance(t.first)) {
                    res.insert({t.first, info.aulas});
                }
            }
        }
    }
    return res;
}


map<string,list<Aula>> Manager::enviaListaDeAulaPossivel(const string &uc, const int &estudante) {
    Estudante student = getEstudante(estudante);

    list<Aula> praticas = obterHorarioEstudantePraticas(student);
    map<string,list<Aula>> res = getTurmasPossiveis(uc, praticas);
    return res;
}

// Verificar se o estudante tem possibilidade de se inscrever na nova turma
bool Manager::validarNovaUc(const string &uc, const int &student) {
    auto estudante = getEstudante(student);
    if (estudante.inscrito(const_cast<string &>(uc))) {
        cout << "---------------------------------"  << endl;
        cout << "O estudante já está incrito na UC" << endl;
        cout << "---------------------------------"  << endl;
        return false;
    }

    if (estudante.getTurmas().size() >= 7) {
        cout << "----------------------------------------------------------" << endl;
        cout << "O estudante já está incrito a muitas Unidades Curriculares" << endl;
        cout << "----------------------------------------------------------" << endl;
        return false;
    }

    auto it = ucs.find(uc);
    list<Aula> praticas = obterHorarioEstudantePraticas(estudante);

    if (it != ucs.end()) {
        unordered_map<string,TurmaInfo> turmas = it->getUcTurma();
        for (const auto &t : turmas) {
            if ((verificarAulaSobreposta(praticas, obterPraticaUc(uc,t.first)))) {
                return true;
            }
        }
        cout << "O estudante não tem compatibilidade de horário" << endl;
        return false;
    }
    cout << "---------------"  << endl;
    cout << "A UC não existe" << endl;
    cout << "---------------"  << endl;
    return false;
}


void Manager::trocaTurma(const string &uc, const string& turma1, const int &numero1, const string &nome1, const string& turma2, const int &numero2, const string &nome2) {
    auto it = ucs.lower_bound(uc);

    if (it->getCodigoUc() == uc) {
        UC atualizada = UC(*it);
        atualizada.removeEstudante(turma1, numero1,nome1);
        atualizada.removeEstudante(turma2, numero2,nome2);
        atualizada.addEstudante(turma2, numero1, nome1);
        atualizada.addEstudante(turma1,numero2,nome2);
        ucs.erase(it);
        ucs.insert(atualizada);
    }

}

void Manager::adicionarUcAoEstudante(Pedido &pedido) {
    string uc = pedido.getUc();
    string turma = pedido.getTurma();
    Estudante estudante = pedido.getEstudante();
    auto it = ucs.find(uc);

    if (it != ucs.end()) {
        UC atualizada = UC(*it);
        atualizada.addEstudante(turma, estudante.getStudentNumber(), estudante.getStudentName());
        ucs.erase(it);
        ucs.insert(atualizada);
    }

    auto iterator = estudantesNumero.find(estudante);
    if (iterator != estudantesNumero.end()) {
        Estudante atualizado = Estudante(estudante);
        atualizado.adicionarUc(uc,turma);
        estudantesNumero.erase(iterator);
        estudantesNumero.insert(atualizado);
    }

    iterator = estudantesNome.find(estudante);
    if (iterator != estudantesNome.end()) {
        Estudante atualizado = Estudante(estudante);
        atualizado.adicionarUc(uc,turma);
        estudantesNome.erase(iterator);
        estudantesNome.insert(atualizado);
    }
}



void Manager::proximoPedido() {
    if (!(pedidos.empty())) {
        switch (pedidos.front().getTipoAlteracao()) {
            case TipoAlteracao::H: {
                executarPedidoTrocaHorario(pedidos.front());
                printHist.insert({this->nPedido, pedidos.front().pedidoToString()});
                this->nPedido++;
                break;
            }
            case TipoAlteracao::R: {
                removerEstudanteDaUc(pedidos.front());
                printHist.insert({this->nPedido, pedidos.front().pedidoToString()});
                this->nPedido++;
                break;
            }
            case TipoAlteracao::A: {
                adicionarUcAoEstudante(pedidos.front());
                printHist.insert({this->nPedido,pedidos.front().pedidoToString()});
                this->nPedido++;
                break;
            }
        }
        cout << pedidos.front().pedidoToString();
        historico.push(pedidos.front());

        for (auto it = pedidosEspera.begin(); it != pedidosEspera.end(); it++) {
            if (*it == pedidos.front()) {
                pedidosEspera.erase(it);
                break;
            }
        }
        pedidos.pop();
    }
    else {
        cout << "-----------------------------"  << endl;
        cout << "Não existem pedidos pendentes" << endl;
        cout << "-----------------------------"  << endl;
    }
}


bool Manager::estudanteValido(const int &numero) const {
    auto it = estudantesNumero.find(numero);
    return it != estudantesNumero.end();
}

void Manager::reverterPedido() {
    if (!(historico.empty())) {
        Pedido last = historico.top();
        string temp = "Reverteu o pedido: " + last.pedidoToString();
        switch (last.getTipoAlteracao()) {
            case TipoAlteracao::H: {
                executarPedidoTrocaHorario(last);
                printHist.insert({this->nPedido, temp});
                this->nPedido++;
                break;
            }
            case TipoAlteracao::R: {
                adicionarUcAoEstudante(last);
                printHist.insert({this->nPedido, temp});
                this->nPedido++;
                break;
            }
            case TipoAlteracao::A: {
                removerEstudanteDaUc(last);
                printHist.insert({this->nPedido, temp});
                this->nPedido++;
                break;
            }
        }
        cout << temp;
        historico.pop();
    }
    else {
        cout << "Não existem pedidos para reverter" << endl;
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

list<Aula> Manager::obterHorarioEstudantePraticasExceto(const Estudante &estudante, const string &uc) const {
    set<pair<string,string>> turmas = estudante.getTurmas();
    list<Aula> res;
    for (const auto& par : turmas) {
         if (par.first != uc) {
             res.push_back(obterPraticaUc(par.first, par.second));
         }

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

    for (const auto& u : ucs) {
        if (uc == u.getCodigoUc()) {
            res = u.getPratica(turma);
        }
    }

    return res;
}

Estudante Manager::getEstudante(const int &numero) const {
    Estudante res;
    auto it = estudantesNumero.find(numero);
    if (it != estudantesNumero.end()) {
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

void Manager::printEstudantesPorTurmaNaUc(const string &uc, const string &turma, bool orderByNumber, bool ascending) const {
    auto it = ucs.find(uc);

    if (it != ucs.end()) {
        unordered_map<string,TurmaInfo> turmaInfo = it->getUcTurma();
        auto iterator = turmaInfo.find(turma);

        if (iterator != turmaInfo.end()) {
            cout << "------------------------------------"  << endl;
            cout << "UC: " << it->getCodigoUc() << " | Turma: " << iterator->first << endl;
            cout << "------------------------------------"  << endl;
            list<pair<int,string>> studentList = iterator->second.estudantes;

            if (orderByNumber) studentList.sort(compareFirstElement);

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

void Manager::printNumeroEstudantesPorTurmaPorUc(const std::string &uc, const bool &orderByFirst , const bool &ascending) const {
    vector<pair<string,int>> turmas;
    cout << "-----------------------------------"  << endl;
    cout << "     Estudantes na UC: " << uc << endl;
    cout << "-----------------------------------"  << endl;
    auto it = ucs.find(uc);

    if (it != ucs.end()) {
        for (const auto& element : it->getUcTurma()){
            turmas.push_back({element.first, it->getNumeroAlunos(element.first)});
        }
    }

    if (orderByFirst) {
        sort(turmas.begin(), turmas.end(), [] (pair<string,int> first, pair<string,int> second) {return first.first < second.first;});
    }
    else {
        sort(turmas.begin(), turmas.end(), [] (pair<string,int> first, pair<string,int> second) {return first.second < second.second;});
    }

    if (ascending) {
        for (const auto& elem : turmas) {
            cout << elem.first << " Nº: " << elem.second << endl;
        }
    }
    else {
        for (auto it = turmas.rbegin(); it != turmas.rend(); it++) {
            cout << it->first << " Nº: " << it->second << endl;
        }
    }
    cout << "-----------------------------------"  << endl;
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
            for (const auto& turma : it->getUcTurma()){
                for (const auto& element : turma.second.estudantes){
                    studentList.insert(element);
                }
            }
        }

        if (studentList.empty()){
            cout << "----------------------------------------"  << endl;
            cout << "Não existem estudantes na UC: " << uc << endl;
            cout << "----------------------------------------"  << endl;
            return;
        }

        cout << "--------------------------------------"  << endl;
        cout << "Estudantes inscritos na UC: " << it->getCodigoUc() << endl;
        cout << "--------------------------------------"  << endl;

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
        cout << "----------------------------------------"  << endl;
    } else {
        set<pair<int, string>, CompareBySecond> studentList;
        auto it = ucs.find(uc);

        if (it != ucs.end()) {
            for (const auto& turma : it->getUcTurma()){
                for (const auto& element : turma.second.estudantes){
                    studentList.insert(element);
                }
            }
        }

        if (studentList.empty()){
            cout << "----------------------------------------"  << endl;
            cout << "Não existem estudantes na UC: " << uc << endl;
            cout << "----------------------------------------"  << endl;
            return;
        }

        cout << "--------------------------------------"  << endl;
        cout << "Estudantes inscritos na UC: " << it->getCodigoUc() << endl;
        cout << "--------------------------------------"  << endl;

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
        cout << "----------------------------------------"  << endl;
    }
}

void Manager::printTurmasDoAluno(const int &numero, const bool& ascending) const {
    Estudante estudante = getEstudante(numero);
    set<pair<string,string>> lista;

    if (estudante.getStudentNumber() != 0) {
        lista = estudante.getTurmas();
    }

    if (ascending && !lista.empty()) {
        cout << "------------------" << endl;
        cout << "   UC      Turma  " << endl;
        cout << "------------------" << endl;
        for (const auto& element : lista) {
            cout << element.first << "   " << element.second << endl;
        }
    }
    else if (!lista.empty()) {
        cout << "------------------" << endl;
        cout << "   UC      Turma  " << endl;
        cout << "------------------" << endl;
        for (auto it = lista.rbegin(); it != lista.rend(); it++) {
            cout << it->first << "   " << it->second << endl;
        }
    }
}

void Manager::printEstudantesPorAno(const int &ano, const bool &orderByNumber, const bool &ascending) const {
    cout << "-----------------------------------"  << endl;
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
                if (i->getAno() == ano) {
                    cout << i->getStudentNumber() << ' ' << i->getStudentName() << endl;
                }
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
                if (i->getAno() == ano) {
                    cout << i->getStudentNumber() << ' ' << i->getStudentName() << endl;
                }
            }
        }
    }
    cout << "-----------------------------------"  << endl;
}

void Manager::printTurmasPorUC(const std::string &uc, const bool &ascending) const {
    set<string> turmas;
    cout << "-----------------------------------"  << endl;
    cout << "     Turmas da UC: " << uc << endl;
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

bool Manager::nUcValido(const int &n) const {
    return (n > 0 && n <= ucs.size());
}

void Manager::numeroEstudantesEmPeloMenosNUCS(const int &nUcs, const bool& orderByNumber, const bool& ascending) const{
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
    if (students.size() > 0) {
        cout << "------------------------------------------------------------------"  << endl;
        cout << "Estão inscritos " << students.size() << " alunos em pelo menos " << nUcs << " Unidades Curriculares." << endl;
        cout << "------------------------------------------------------------------"  << endl;
        cout << ss.str();
        cout << "------------------------------------------------------------------"  << endl;
    }
    else {
        cout << "---------------------------------------------------------"  << endl;
        cout << "Não existem alunos inscritos a " << nUcs << " Unidades Curriculares." << endl;
        cout << "---------------------------------------------------------"  << endl;
    }

}

bool Manager::inputToPedido(const string& uc, const int &estudante, const string &tipo, const int outro, const string &turma) {
    Estudante student1 = getEstudante(estudante);
    if (tipo == "H") {
        Estudante student2 = getEstudante(outro);
        return addPedido(Pedido(uc, student1, student2));
    }
    return addPedido(Pedido(uc, student1, tipo, turma));
}

bool Manager::ucValida(const string &uc) const {
    return (ucs.find(uc) != ucs.end());
}

bool Manager::turmaValida(const string &turma) const {
    for (auto element : ucs) {
        for (auto turmaUc : element.getUcTurma()) {
            if (turmaUc.first == turma) {
                return true;
            }
        }
    }
    return false;
}

bool Manager::turmaValidaNaUc(const string &uc, const string &turma) {
    auto it = ucs.find(uc);

    if (it == ucs.end()) {
        cout << "---------------"  << endl;
        cout << "A UC não existe" << endl;
        cout << "---------------"  << endl;
        return false;
    }

    if (!(it->verificarTurma(turma))) {
        cout << "---------------------------"  << endl;
        cout << "A turma não existe nessa UC" << endl;
        cout << "---------------------------"  << endl;
        return false;
    }

    return true;
}

vector<pair<string,pair<string,Aula>>> Manager::getAulas(const Estudante &estudante) const {
    vector<pair<string,pair<string,Aula>>> result;
    set<pair<string,string>> turmas = estudante.getTurmas();
    for (const auto& par : turmas) {
        TurmaInfo turmaInfo = obterInfoUc(par.first, par.second);
        for (const auto& element : turmaInfo.aulas){
            result.emplace_back(par.first, make_pair(par.second, element));
        }
    }
    return result;
}

const string makeHoras(const float& a){
    string result ="| ";
    if (a < 10){
        result+='0';
    }
    int aux = a;
    if (a-aux == 0.5){
        result += to_string(aux) + ":30 - ";
        if (a + 0.5 < 10) {
            result+='0';
        }
        result += to_string(aux + 1) + ":00 |";
        return result;
    }
    result += to_string(aux) + ":00 - ";
    if (aux + 0.5 < 10) {
        result += '0';
    }
    result += to_string(aux) + ":30 |";
    return result;
}


void Manager::inputToHorario(const char &tipo, const string &uc, const string &turma, const int &numero) {
    switch (tipo) {
        case 'E': {
            Estudante estudante = getEstudante(numero);
            vector<pair<string,pair<string,Aula>>> horario = getAulas(estudante);
            printHorario(horario);
            break;
        }
        case 'U': {
            auto it = ucs.find(uc);
            vector<pair<string,pair<string,Aula>>> horario = it->getAulasUc();
            printHorario(horario);
            break;
        }
        case 'T': {
            vector<pair<string,pair<string,Aula>>> horario;
            for (const auto& unidade : ucs) {
                list<Aula> aulasUc = unidade.getAulasTurma(turma);
                for (const auto& aula : aulasUc) {
                    horario.push_back({unidade.getCodigoUc(), {turma, aula}});
                }
            }
            printHorario(horario);
            break;
        }
    }
}

bool Manager::checkAlreadyIn(vector<pair<string,pair<string,Aula>>> &horario, pair<string,pair<string,Aula>> &aula) const {
    string line;
    for (auto slot : horario) {
        if (slot.first == aula.first && slot.second.second.mesmoHorario(aula.second.second)) {
            return true;
        }
    }
    float turno = stof(aula.second.first.substr(5));

    auto it = ucs.find(aula.first);


    if (turno <= it->getMedia()) {
        line = "Turno 1";
    }
    else {
        line = "Turno 2";
    }
    aula.second.first = line;
    return false;
}


vector<pair<string,pair<string,Aula>>> Manager::createSobrepostas (vector<pair<string,pair<string,Aula>>> &horario) const {
    vector<pair<string,pair<string,Aula>>> sobrepostas;
    for (auto i = horario.begin(); i != horario.end(); ++i){
        auto j = i;
        j++;
        while(j < horario.end() && j > horario.begin()){

            Aula aula1 = i->second.second;
            Aula aula2 = j->second.second;
            if (aula1.sobreposta(aula2)) {
                if (aula1.getTipo() != "T"  && aula2.getTipo() == "T") {
                    // se aula1 é prática e aula2 é teórica.
                    if (!checkAlreadyIn(sobrepostas,*j))
                    sobrepostas.push_back(*j);
                    horario.erase(j);
                }
                else if (aula2.getTipo() != "T" && aula1.getTipo() == "T"){
                    // se aula1 é prática e aula2 é teórica.
                    if (!checkAlreadyIn(sobrepostas,*i))
                    sobrepostas.push_back(*i);
                    horario.erase(i);

                }
                else {
                    // se as duas aulas são teoricas
                    int sobre1 = 0, sobre2 = 0;
                    auto k = j;
                    k++;
                    while(k < horario.end() && k > horario.begin()){
                        Aula aula3 = k->second.second;
                        if (aula1.sobreposta(aula3)) sobre1++;
                        if (aula2.sobreposta(aula3)) sobre2++;
                        k++;
                    }
                    if (sobre1 > sobre2) {
                        //se aula1 se sobrepõe a mais aulas que aula2
                        if (!(aula1.getTipo() == "T" && checkAlreadyIn(sobrepostas,*i)))
                        sobrepostas.push_back(*i);
                        horario.erase(i);
                    }
                    else if (sobre2 > sobre1){
                        sobrepostas.push_back(*j);
                        horario.erase(j);
                    }
                    else {
                        // se aula1 e aula2 se sobrepõem ao mesmo número de aulas.
                        if (aula1.getInicio() < aula2.getInicio()) {
                            // se aula1 começa antes da aula2
                            if (!(aula2.getTipo() == "T" && checkAlreadyIn(sobrepostas,*j)))
                            sobrepostas.push_back(*j);
                            horario.erase(j);
                        }
                        else if (aula1.getInicio() > aula2.getInicio()) {
                            // se a aula2 começa antes da aula1
                            if (!(aula1.getTipo() == "T" && checkAlreadyIn(sobrepostas,*i)))
                            sobrepostas.push_back(*i);
                            horario.erase(i);
                        }
                        else {
                            if (aula1.getDuracao() > aula2.getDuracao()) {
                                //se a aula1 for maior que a aula2
                                if (!(aula2.getTipo() == "T" && checkAlreadyIn(sobrepostas,*j)))
                                sobrepostas.push_back(*j);
                                horario.erase(j);
                            }
                            else if (aula1.getDuracao() < aula2.getDuracao()) {
                                //se a aula2 for maior que a aula1
                                if (!(aula1.getTipo() == "T" && checkAlreadyIn(sobrepostas,*i)))
                                sobrepostas.push_back(*i);
                                horario.erase(i);
                            }
                            else {
                                // a aula 1 prevalece sobre a 2
                                if (j!= horario.end()) {
                                    if (!(aula2.getTipo() == "T" && checkAlreadyIn(sobrepostas,*j)))
                                    sobrepostas.push_back(*j);
                                    horario.erase(j);
                                }

                            }
                        }
                    }
                }
                j = i;
            }
            j++;
        }
    }
    return sobrepostas;
}

const string getHoras(const float& begin, const float& duration = 0.5){
    string result ="| ";
    if (begin < 10){
        result+='0';
    }
    int aux = begin;
    if (begin-aux == 0.5) {
        result += to_string(aux) + ":30 - ";
    }
    else result += to_string(aux) + ":00 - ";
    float end = begin + duration;
    if (end < 10){
        result+='0';
    }
    aux = end;
    if (end-aux == 0.5) {
        result += to_string(aux) + ":30 |";
    }
    else result += to_string(aux) + ":00 |";
    return result;
}



void Manager::printHorario(vector<pair<string,pair<string,Aula>>> horario) const {

    vector<pair<string,pair<string,Aula>>> sobrepostas = createSobrepostas(horario);
    unordered_map<string,string> translate = {{"Monday","Segunda"}, {"Tuesday","Terça"}, {"Wednesday","Quarta"}, {"Thursday","Quinta"}, {"Friday","Sexta"}};
    unordered_map<string,int> weekdays = {{"Monday",1}, {"Tuesday",2}, {"Wednesday",3}, {"Thursday",4}, {"Friday",5}};
    map<pair<int,int>,string> sparseMatrix;
    static int LEN = 15;

    for(const auto& elem : horario){
        pair<int,int> coordinates = make_pair(weekdays[elem.second.second.getDia()], (elem.second.second.getInicio() - 8.0) * 4.0);
        int lenTotal = ucToString(elem.first).length() + elem.second.second.getTipo().length() + 3;
        int lenUC = (LEN - lenTotal) / 2;
        string uc;
        if ((LEN - lenTotal) % 2 == 1) {
            uc += (string(lenUC,' ') + ucToString(elem.first) + " (" + elem.second.second.getTipo() + ")" + string(lenUC + 1,' '));
        }
        else {
            uc += (string(lenUC,' ') + ucToString(elem.first) + " (" + elem.second.second.getTipo() + ")" + string(lenUC,' '));
        }
        string turma = "    " + elem.second.first;
        if (elem.second.second.getDuracao() == 2){
            uc += '|';
            turma += "    |";
            sparseMatrix.insert({coordinates,"               |"});
            sparseMatrix.insert({{coordinates.first,coordinates.second+1},"               -"});
            sparseMatrix.insert({{coordinates.first,coordinates.second+2},uc});
            sparseMatrix.insert({{coordinates.first,coordinates.second+3},"               -"});
            sparseMatrix.insert({{coordinates.first,coordinates.second+4},turma});
            sparseMatrix.insert({{coordinates.first,coordinates.second+5},"               -"});
            sparseMatrix.insert({{coordinates.first,coordinates.second+6},"               |"});
            sparseMatrix.insert({{coordinates.first,coordinates.second+7},"----------------"});
        }
        else if (elem.second.second.getDuracao() == 1){
            uc += '|';
            turma += "    |";
            sparseMatrix.insert({coordinates,uc});
            sparseMatrix.insert({{coordinates.first,coordinates.second+1},"               -"});
            sparseMatrix.insert({{coordinates.first,coordinates.second+2},turma});
            sparseMatrix.insert({{coordinates.first,coordinates.second+3},"----------------"});
        }
        else {
            uc += '-';
            turma += "    -";
            sparseMatrix.insert({coordinates,"               |"});
            sparseMatrix.insert({{coordinates.first,coordinates.second+1},uc});
            sparseMatrix.insert({{coordinates.first,coordinates.second+2},"               |"});
            sparseMatrix.insert({{coordinates.first,coordinates.second+3},turma});
            sparseMatrix.insert({{coordinates.first,coordinates.second+4},"               |"});
            sparseMatrix.insert({{coordinates.first,coordinates.second+5},"----------------"});
        }
    }
    for (int i = 0; i < 24; i++) {
        float hours = i/2.0 + 8.0;
        sparseMatrix.insert({{0,i*2}, getHoras(hours)});
        sparseMatrix.insert({{0,i*2+1}, "-----------------"});
    }
    cout << endl;
    cout << "-------------------------------------------------------------------------------------------------" << endl;
    cout << "|     Horas     |    Segunda    |     Terça     |    Quarta     |    Quinta     |    Sexta      |" << endl;
    cout << "-------------------------------------------------------------------------------------------------" << endl;
    for (int y = 0; y < 48; y++) {
        for (int x = 0; x < 6; x++) {
            auto it = sparseMatrix.find({x,y});
            if (it != sparseMatrix.end()) {
                cout << it->second;
            }
            else {
                string aux = (y % 2 == 0) ?  "               |" : "----------------";
                cout << aux;
            }
        }
        cout << endl; // 16-5
    }

    if (sobrepostas.empty()) return;
    cout << endl << endl << endl; //L.EIC001
    cout << "-------------------------------------------------------------" << endl;
    cout << "|                     Aulas Sobrepostas                     |" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "| Unidade Curricular |   Turma   |   Dia    |    Horário    |" << endl;

    cout << "-------------------------------------------------------------" << endl;
    for(const auto& sobreposta : sobrepostas){
        int lenT = sobreposta.second.second.getTipo().length();
        int totalUC = lenT + 3 + ucToString(sobreposta.first).length();
        int lenUC = ucToString(sobreposta.first).length() > 3 ? 6 : 7;
        int lenUCfim = lenUC == 7 ? 13 : 14;
        int total = (translate[sobreposta.second.second.getDia()] == "Terça") ? 5 : translate[sobreposta.second.second.getDia()].length();
        int lenDia = (10 - total) / 2;
        int lenDiaFim = ((10 - total) % 2 == 0) ? lenDia : lenDia + 1;
        int lenTurma = (11 - sobreposta.second.first.length()) / 2;
        cout << "|" << string(lenUC, ' ') << ucToString(sobreposta.first) << " (" << sobreposta.second.second.getTipo() << ")" << string(lenUCfim - totalUC,' ')
             << '|' << string(lenTurma, ' ') << sobreposta.second.first <<  string(lenTurma, ' ')
             << '|' << string(lenDia,' ') << translate[sobreposta.second.second.getDia()] << string(lenDiaFim,' ')
             << getHoras(sobreposta.second.second.getInicio(),sobreposta.second.second.getDuracao()) << endl
             << "-------------------------------------------------------------" << endl;;

    }
}

void Manager::printInfoEstudante(const int &numero) const {
    Estudante estudante = getEstudante(numero);
    set<pair<string,string>> lista;
    if (estudante.getStudentNumber() != 0) {
        lista = estudante.getTurmas();
    }
    // L.EIC001
    int len, lenf;
    auto it = lista.begin();
    cout << "---------------------------------------------------------" << endl;
    cout << "|                                  |    UC    |  Turma  |" << endl;
    for (int i = 0; i < lista.size() || i < 3; i++){
        if (i == 0){
            len = (10 - it->first.length()) / 2;
            lenf = (10 - it->first.length()) % 2 == 0 ? len : len + 1;
            cout << "| Nome: " << estudante.getStudentName() << string(27 - estudante.getStudentName().length(), ' ') << '|';
            cout << string(len, ' ') << it -> first << string(lenf, ' ') << '|';
            cout << ' ' << it->second << " |" << endl;
            it++;
        }
        else if (i == 1){
            if (it != lista.end()){
                len = (10 - it->first.length()) / 2;
                lenf = (10 - it->first.length()) % 2 == 0 ? len : len + 1;
                cout << "| Número: " << estudante.getStudentNumber() << string(16 , ' ') << '|';
                cout << string(len, ' ') << it -> first << string(lenf, ' ') << '|';
                cout << ' ' << it->second << " |" << endl;
                it++;
            }
            else {
                cout << "| Número: " << estudante.getStudentNumber() << string(16, ' ')
                     << "|          |         |" << endl;
            }
        }
        else if (i == 2){
            if (it != lista.end()){
                len = (10 - it->first.length()) / 2;
                lenf = (10 - it->first.length()) % 2 == 0 ? len : len + 1;
                cout << "| Ano: " << estudante.getAno() << string(27, ' ') << '|';
                cout << string(len, ' ') << it -> first << string(lenf, ' ') << '|';
                cout << ' ' << it->second << " |" << endl;
                it++;
            }
            else {
                cout << "| Ano: " << estudante.getAno() << string(27, ' ')
                     << "|          |         |" << endl;
            }
        }
        else{
            cout << "|                                  |";
            len = (10 - it->first.length()) / 2;
            lenf = (10 - it->first.length()) % 2 == 0 ? len : len + 1;
            cout << string(len, ' ') << it -> first << string(lenf, ' ') << '|';
            cout << ' ' << it->second << " |" << endl;
            it++;
        }
    }
    cout << "---------------------------------------------------------" << endl;
}

// UC to String
string Manager::ucToString(const string &uc) const {
    map<string,string> maps = {{"L.EIC001","ALGA"},
                               {"L.EIC002", "AM I"},
                               {"L.EIC003", "FP"},
                               {"L.EIC004","FSC"},
                               {"L.EIC005", "MD"},
                               {"UP001", "PUP"},
                               {"L.EIC011", "AED"},
                               {"L.EIC012", "BD"},
                               {"L.EIC013", "F II"},
                               {"L.EIC014", "LDTS"},
                               {"L.EIC015", "SO"},
                               {"L.EIC021", "FSI"},
                               {"L.EIC022", "IPC"},
                               {"L.EIC023", "LBAW"},
                               {"L.EIC024", "PFL"},
                               {"L.EIC025", "RC"},
                               };

    return maps[uc];
}

// Estatística
set<string> Manager::getUcPorAno(const int &ano) const {
    set<string> res;
    for (auto uc : ucs) {
        if (uc.getAno() == ano) {
            res.insert(uc.getCodigoUc());
        }
    }
    return res;
}

set<string> Manager::getTurmasPorAno(const int &ano) const {
    set<string> res;
    for (auto uc : ucs) {
        if (uc.getAno() == ano) {
            for (auto turma : uc.getUcTurma()) {
                res.insert(turma.first);
            }
        }
    }
    return res;
}


vector<pair<int,int>> Manager::estudantesEmMaisOuMenosUc(const int &n, const bool &mais) const {
    set<pair<int,int>> res;
    for (auto est : estudantesNumero) {
        res.insert({est.getTurmas().size(), est.getStudentNumber()});
    }

    vector<pair<int,int>> result;
    if (mais) {
        auto it = res.begin();
        for (int i = 0; i < n; i++) {
            if (it != res.end()) {
                result.push_back(*it);
            }
        }
    }

    else {
        auto it = res.rbegin();
        for (int i = 0; i < n; i++) {
            if (it != res.rend()) {
                result.push_back(*it);
            }
        }
    }

    return result;
}

set<string> Manager::getTurmasPorUc(const string& uc) const {
    auto it = ucs.find(uc);
    set<string> res;

    if (it != ucs.end()) {
        for (auto turma : it->getUcTurma()) {
            res.insert(turma.first);
        }
    }
    return res;
}

/*
void Manager::ucComMaisOuMenosAlunos(const int &n, const bool &mais) const {
    set<pair<int,string>> res;

    for (auto uc : ucs) {
        int alunos = 0;
        for (auto turma : uc.getUcTurma()) {
            alunos += uc.getNumeroAlunos(turma.first);
        }
        res.insert({n,uc.getCodigoUc()});
    }
}

void Manager::turmaComMaisOuMenosAlunos(const string &uc, const bool &mais) {
    auto it = ucs.find(uc);
    set<pair<int,string>> res;

    if (it != ucs.end()) {
        for (auto turma : it->getUcTurma()) {
            res.insert({it->getNumeroAlunos(turma.first), turma.first});
        }
    }
}*/

void Manager::printNumeroDeAlunosPorAno() const {
    int first = 0, second = 0, third = 0;
    for (auto estudante : estudantesNumero) {
        int ano = estudante.getAno();
        first += ano == 1 ? 1 : 0;
        second += ano == 2 ? 1 : 0;
        third += ano == 3 ? 1 : 0;
    }

    cout << "---------------------------" << endl;
    cout << "| 1º Ano:       " << first << " Alunos |" << endl;
    cout << "| 2º Ano:      " << second << " Alunos |" << endl;
    cout << "| 3º Ano:      " << third << " Alunos |" << endl;
    cout << "---------------------------" << endl;
}

void Manager::printEstudantesPorNome(string& nome, const bool& ascending) const {
    bool first = true;
    for (auto& ch : nome) {
        ch = first ? toupper(ch) : tolower(ch);
        first = false;
    }

    vector<Estudante> students;
    for (const auto& estudante: estudantesNome) {
        if(estudante.getStudentName().find(nome) != string::npos) {
            students.push_back(estudante);
        }
    }
    if (!students.empty()) {
        cout << "------------------------------------------------" << endl;
        cout << "|            Nome            |  Número   | Ano |" << endl;
        cout << "------------------------------------------------" << endl;

        if (ascending) {
            for (const auto &elem: students) {
                int len = (28 - elem.getStudentName().length()) / 2;
                int lenf = (28 - elem.getStudentName().length()) % 2 == 0 ? len : len + 1;
                cout << "|" << string(len, ' ') << elem.getStudentName() << string(lenf, ' ') << "| "
                     << elem.getStudentNumber() << " |  " << elem.getAno() << "  |" << endl;
                cout << "------------------------------------------------" << endl;
            }
        } else {
            for (auto elem = students.rbegin(); elem < students.rend(); elem++) {
                int len = (28 - elem->getStudentName().length()) / 2;
                int lenf = (28 - elem->getStudentName().length()) % 2 == 0 ? len : len + 1;
                cout << "|" << string(len, ' ') << elem->getStudentName() << string(lenf, ' ') << "| "
                     << elem->getStudentNumber() << " |  " << elem->getAno() << "  |" << endl;
                cout << "------------------------------------------------" << endl;
            }
        }
    }
    else {
        cout << "------------------------------------------------------" << endl;
        cout << "Não existem estudantes com o nome " << nome << endl;
        cout << "------------------------------------------------------" << endl;
    }
}

void Manager::printNumeroEstudantesDeTodasUc() const {
    string ucP = "L.EIC001";
    string ucS = "L.EIC011";
    string ucT = "L.EIC021";
    cout << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "|      1º Ano      |      2º Ano      |      3º Ano      |" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "|    UC       Nº   |    UC       Nº   |    UC       Nº   |" << endl;
    cout << "| L.EIC001:   " << ucs.find(ucP)->getNumeroAlunosTotal() << "   | L.EIC011:   " << ucs.find(ucS)->getNumeroAlunosTotal() << "  | L.EIC021:   " << ucs.find(ucT)->getNumeroAlunosTotal() << "  |" << endl;
    ucP = "L.EIC002", ucS = "L.EIC012", ucT = "L.EIC022";
    cout << "| L.EIC002:   " << ucs.find(ucP)->getNumeroAlunosTotal() << "   | L.EIC012:   " << ucs.find(ucS)->getNumeroAlunosTotal() << "  | L.EIC022:   " << ucs.find(ucT)->getNumeroAlunosTotal() << "  |" << endl;
    ucP = "L.EIC003", ucS = "L.EIC013", ucT = "L.EIC023";
    cout << "| L.EIC003:   " << ucs.find(ucP)->getNumeroAlunosTotal() << "   | L.EIC013:   " << ucs.find(ucS)->getNumeroAlunosTotal() << "  | L.EIC023:   " << ucs.find(ucT)->getNumeroAlunosTotal() << "  |" << endl;
    ucP = "L.EIC004", ucS = "L.EIC014", ucT = "L.EIC024";
    cout << "| L.EIC004:   " << ucs.find(ucP)->getNumeroAlunosTotal() << "   | L.EIC014:   " << ucs.find(ucS)->getNumeroAlunosTotal() << "  | L.EIC024:   " << ucs.find(ucT)->getNumeroAlunosTotal() << "  |" << endl;
    ucP = "L.EIC005", ucS = "L.EIC015", ucT = "L.EIC025";
    cout << "| L.EIC005:   " << ucs.find(ucP)->getNumeroAlunosTotal() << "   | L.EIC015:   " << ucs.find(ucS)->getNumeroAlunosTotal() << "  | L.EIC025:   " << ucs.find(ucT)->getNumeroAlunosTotal() << "  |" << endl;
    ucP = "UP001";
    cout << "| UP001:       " << ucs.find(ucP)->getNumeroAlunosTotal() << "   |                  |                  |" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << endl;
}

set<pair<string,string>> Manager::enviaUCParaRemover(const int &numero) const {
    auto estudante = estudantesNumero.find(numero);
    set<pair<string, string>> res;

    if (estudante != estudantesNumero.end()) {
        res = estudante->getTurmas();
    }
    return res;
}

set<pair<int,string>> Manager::getOcupacaoUcs() const {
    set<pair<int,string>> allUCs;
    for (auto uc : ucs) {
        int alunos = 0;
        for (auto turma: uc.getUcTurma()) {
            alunos += uc.getNumeroAlunos(turma.first);
        }
        allUCs.insert({alunos, uc.getCodigoUc()});
    }
    return allUCs;
}


set<pair<int,string>> Manager::getOcupacaoTurmas(const string &uc) const {
    auto it = ucs.find(uc);
    set<pair<int, string>> allTurmas;

    if (it != ucs.end()) {
        for (auto turma: it->getUcTurma()) {
            allTurmas.insert({it->getNumeroAlunos(turma.first), turma.first});
        }
    }
    return allTurmas;
}


void Manager::printSets(int n, const string& uc, const bool& mais) const {
    set<pair<int,string>> allTurmas;

    if (uc == "") allTurmas = getOcupacaoUcs();
    else allTurmas = getOcupacaoTurmas(uc);

    if (!allTurmas.empty()) {
        float tamanho = allTurmas.rbegin()->first;
        if (mais) {
            auto it = allTurmas.rbegin();
            cout << "          |" << endl;
            while(n != 0 && it != allTurmas.rend()){
                int len = (10 - it->second.length()) / 2;
                int lenf = (10 - it->second.length()) % 2 == 0 ? len : len + 1;
                if (it->first == 0){
                    cout << "          |" << endl;
                    cout << string(len, ' ') << it->second << string(lenf, ' ') << "|" << it->first << endl;
                    cout << "          |" << endl;
                    n--;
                    it++;
                    continue;
                }
                int lenBarra = (it->first / tamanho) * 80;
                cout << string(len + lenf + it-> second.length(), ' ') << "|" << string(lenBarra, '-') << endl;
                cout << string(len, ' ') << it->second << string(lenf, ' ') << "|" << string(lenBarra - 1, ' ') << "| " << it->first << endl;
                cout << string(len + lenf + it-> second.length(), ' ') << "|" << string(lenBarra, '-') << endl;
                cout << "          |" << endl;
                n--;
                it++;
            }
        }
        else {
            auto it = allTurmas.begin();
            cout << "          |" << endl;
            while(n != 0 && it != allTurmas.end()){
                int len = (10 - it->second.length()) / 2;
                int lenf = (10 - it->second.length()) % 2 == 0 ? len : len + 1;
                if (it->first == 0){
                    cout << "          |" << endl;
                    cout << string(len, ' ') << it->second << string(lenf, ' ') << "|" << it->first << endl;
                    cout << "          |" << endl;
                    n--;
                    it++;
                    continue;
                }
                if (tamanho == 0) tamanho = 1.0;
                int lenBarra = (it->first / tamanho) * 80;
                cout << string(len + lenf + it-> second.length(), ' ') << "|" << string(lenBarra, '-') << endl;
                cout << string(len, ' ') << it->second << string(lenf, ' ') << "|" << string(lenBarra - 1, ' ') << "| " << it->first << endl;
                cout << string(len + lenf + it-> second.length(), ' ') << "|" << string(lenBarra, '-') << endl;
                cout << "          |" << endl;
                n--;
                it++;
            }
        }
    }

}

vector<pair<int, int>> Manager::getAlunosPorNIncscricoes() const {
    vector<pair<int,int>> result = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}};
    for (const auto& estudante : estudantesNumero){
        int i = estudante.getTurmas().size();
        result[i-1].first++;
    }
    return result;
}

vector<pair<int, int>> Manager::getNumeroDeAlunosPorAno() const {
    vector<pair<int, int>> result = {{0,1}, {0,2}, {0,3}};
    int first = 0, second = 0, third = 0;
    for (auto estudante : estudantesNumero) {
        int ano = estudante.getAno();
        result[ano-1].first++;
    }
    return result;
}

void Manager::printVectors(const char &tipo, const bool &ordered, const bool& ascending) const {
    vector<pair<int,int>> res;

    if (tipo == 'A') res = getNumeroDeAlunosPorAno();
    else res = getAlunosPorNIncscricoes();

    if (!res.empty()) {
        cout << "       |" << endl;
        float tamanho = -1;
        for (const auto& it : res) {
            if (it.first > tamanho) tamanho = it.first;
        }
        if (ordered) {
            if (ascending) sort(res.begin(), res.end());
            else sort(res.rbegin(), res.rend());
        }
        for (const auto& elem : res) {
            if (elem.first == 0) {
                cout << "         |" << endl;
                cout << "   " << elem.second << "   | " << elem.first << endl;
            }
            int lenBarra = (elem.first / tamanho) * 80;
            if (elem.first == 0){};
            cout << "       |" << string(lenBarra, '-') << endl;
            cout << "   " << elem.second << "   |" << string(lenBarra - 1, ' ') << "| " << elem.first << endl;
            cout << "       |" << string(lenBarra, '-') << endl;
            cout << "       |" << endl;
        }
    }
}

int Manager::getNumeroTurmas(const string& uc) {
    auto it = ucs.find(uc);

    if (it != ucs.end()) {
        return it->getNumeroTurmas();
    }
}

void Manager::verHorarioAntesDeConfirmar(const int &numero, const string &uc, const string &turma, const list<Aula> &aulas) const {
    auto estudante = getEstudante(numero);

    vector<pair<string,pair<string,Aula>>> horario = getAulas(estudante);

    for (auto aula : aulas) {
        horario.push_back({uc,{turma,aula}});
    }
    printHorario(horario);
}
