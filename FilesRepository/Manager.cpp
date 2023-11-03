#include "Manager.h"
#include "fstream"
#include "sstream"
#include "string"
#include <iostream>
#include <cmath>
#include <algorithm>

//! Constructor
Manager::Manager() {
    this->nPedido = 1;
}

//! Parsing
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

        map<string,TurmaInfo> turmaUc;

        while (getline(iff,line)) {
            stringstream s(line);
            getline(s,turma,',');
            getline(s,uc,',');
            getline(s,dia,',');
            getline(s,inicio,',');
            getline(s,duracao,',');
            getline(s,tipo,'\r');
            TurmaInfo turmaInfo;

            if (tipo != "T") {
                turmaInfo.pratica = Aula(dia, stof(inicio), stof(duracao), tipo);
                n += stof(turma.substr(5));
                i++;
            }

            if (tipo == "T") {
                auto it = turmaUc.find(turma);

                if (it != turmaUc.end()) {
                    it -> second.aulasTeoricas.emplace_back(dia, stof(inicio), stof(duracao), tipo);
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

//! Carrega as alterações pendentes, bem como as executadas
//! O(n log(n))
void Manager::readChanges() {
    fstream iff;
    try {
        iff.open("../Changes/changes.txt", ios::in);
        string line, tipo, estudante1, uc, turma, estudante2, reverse;
        while (getline(iff, line)) { // O(n)
            istringstream iss(line);
            iss >> reverse;

            if (reverse == "N") {
                iss >> tipo;

                if (tipo == "R") {
                    iss >> estudante1;
                    iss >> uc;
                    iss >> turma;
                    Estudante student = getEstudante(stoi(estudante1));
                    Pedido pedido = Pedido(uc,student,tipo, turma);
                    historico.push(pedido);
                    printHist.insert({this->nPedido, pedido.pedidoToString()});
                    this->nPedido++;
                    removerEstudanteDaUc(pedido); // O(log(n))
                }
                else if (tipo == "A") {
                    iss >> estudante1;
                    iss >> uc;
                    iss >> turma;
                    Estudante student = getEstudante(stoi(estudante1));
                    Pedido pedido = Pedido(uc,student,tipo, turma);
                    historico.push(pedido);
                    printHist.insert({this->nPedido, pedido.pedidoToString()});
                    this->nPedido++;
                    adicionarUcAoEstudante(pedido); // O(log(n))
                }
                else if (tipo == "H") {
                    iss >> estudante1;
                    iss >> uc;
                    iss >> turma;
                    iss >> estudante2;
                    Estudante student = getEstudante(stoi(estudante1));
                    Estudante student2 = getEstudante(stoi(estudante2));
                    Pedido pedido = Pedido(uc,student, student2);
                    historico.push(pedido);
                    printHist.insert({this->nPedido, pedido.pedidoToString()});
                    this->nPedido++;
                    executarPedidoTrocaHorario(pedido); // O(log(n))
                }
            }
            else if (reverse == "R") {
                iss >> tipo;
                if (tipo == "R") {
                    iss >> estudante1;
                    iss >> uc;
                    iss >> turma;
                    Estudante student = getEstudante(stoi(estudante1));
                    Pedido pedido = Pedido(uc,student,tipo, turma);
                    string s = "Reverteu o pedido " + pedido.pedidoToString();
                    historico.pop();
                    printHist.insert({this->nPedido, s});
                    this->nPedido++;
                    adicionarUcAoEstudante(pedido); // O(log(n))
                }
                else if (tipo == "A") {
                    iss >> estudante1;
                    iss >> uc;
                    iss >> turma;
                    Estudante student = getEstudante(stoi(estudante1));
                    Pedido pedido = Pedido(uc,student,tipo, turma);
                    string s = "Reverteu o pedido " + pedido.pedidoToString();
                    historico.pop();
                    printHist.insert({this->nPedido, s});
                    this->nPedido++;
                    removerEstudanteDaUc(pedido); // O(log(n))
                }
                else if (tipo == "H") {
                    iss >> estudante1;
                    iss >> uc;
                    iss >> turma;
                    iss >> estudante2;
                    Estudante student = getEstudante(stoi(estudante1));
                    Estudante student2 = getEstudante(stoi(estudante2));
                    Pedido pedido = Pedido(uc,student, student2);
                    string s = "Reverteu o pedido " + pedido.pedidoToString();
                    historico.pop();
                    printHist.insert({this->nPedido, s});
                    this->nPedido++;
                    executarPedidoTrocaHorario(pedido); // O(log(n))
                }
            }
        }
        iff.close();
    }
    catch (exception e) {
        cout << "Falha ao abrir o ficheiro";
    }
    try {
        ifstream wagnerSalvadorDaPatria("../Changes/queue.txt");
        string line, tipo, estudante1, uc, turma, estudante2;

        while (getline(wagnerSalvadorDaPatria, line)) {
            istringstream iss(line);
            iss >> tipo;

            if (tipo == "H") {
                iss >> estudante1;
                iss >> uc;
                iss >> turma;
                iss >> estudante2;
                Estudante student = getEstudante(stoi(estudante1));
                Estudante student2 = getEstudante(stoi(estudante2));
                Pedido pedido = Pedido(uc,student, student2);
                pedidos.push(pedido);
                pedidosEspera.push_back(pedido);
            }
            else {
                iss >> estudante1;
                iss >> uc;
                iss >> turma;
                Estudante student = getEstudante(stoi(estudante1));
                Pedido pedido = Pedido(uc,student,tipo, turma);
                pedidos.push(pedido);
                pedidosEspera.push_back(pedido);
            }
        }
        wagnerSalvadorDaPatria.close();
    }
    catch (exception e) {
        cout << "Falha ao abrir o ficheiro" << endl;
    }
}

//! Escreve as alterações permanentes num ficheiro
//! O(1)
void Manager::writeRequestInFile(const Pedido& pedido, const bool& reverse) {
    fstream off("../Changes/changes.txt", off.app);
    try {
        string n = reverse ? "R " : "N ";
        switch (pedido.getTipoAlteracao()) {
            case TipoAlteracao::R: {
                string line = n + "R " + to_string(pedido.getEstudante().getStudentNumber()) + " " + pedido.getUc() + " " +
                        pedido.getTurma() + "\n";
                off << line;
                break;
            }
            case TipoAlteracao::A: {
                string line = n +
                        "A " + to_string(pedido.getEstudante().getStudentNumber()) + " " + pedido.getUc() + " " +
                        pedido.getTurma() + "\n";
                off << line;
                break;
            }
            case TipoAlteracao::H: {
                string line = n +
                        "H " + to_string(pedido.getEstudante().getStudentNumber()) + " " + pedido.getUc() + " " +
                        to_string(pedido.getOutroEstudante().getStudentNumber()) + "\n";
                off << line;
                break;
            }
        }
        off.close();
    }
    catch (exception e) {
        cout << "Falha a abrir o ficheiro" << endl;
    }
}

//! Escreve os pedidos pendentes num ficheiro
//! O(n)
void Manager::guardaPedidosPendentes() {
    ofstream off;
    try {
        off.open("../Changes/queue.txt", ofstream ::trunc);
        vector<Pedido> pedidoPendentes = this->pedidosEspera;
        for (auto it = pedidoPendentes.begin(); it != pedidoPendentes.end(); it++) {
            Pedido pedido = *it;
            switch (pedido.getTipoAlteracao()) {
                case TipoAlteracao::R: {
                    string line = "R " + to_string(pedido.getEstudante().getStudentNumber()) + " " + pedido.getUc() + " " +
                                  pedido.getTurma() + "\n";
                    off << line;
                    break;
                }
                case TipoAlteracao::A: {
                    string line = "A " + to_string(pedido.getEstudante().getStudentNumber()) + " " + pedido.getUc() + " " +
                                  pedido.getTurma() + "\n";
                    off << line;
                    break;
                }
                case TipoAlteracao::H: {
                    string line = "H " + to_string(pedido.getEstudante().getStudentNumber()) + " " + pedido.getUc() + " " +
                                  to_string(pedido.getOutroEstudante().getStudentNumber()) + "\n";
                    off << line;
                    break;
                }
            }
        }
    }
    catch (exception e) {
        cout << "Falha a abrir o ficheiro" << endl;
    }
}

//! Getters
//! Retorna o número de pedidos pendentes
//! O(1)
int Manager::getPedidos() const {
    return this->pedidos.size();
}

//! Retorna um estudante, procurado no set de estudantes pelo número
//! 0(log(n))
Estudante Manager::getEstudante(const int &numero) const {
    Estudante res;
    auto it = estudantesNumero.find(numero);
    if (it != estudantesNumero.end()) {
        res = *it;
    }
    return res;
}

//! Retorna um vetor de pares, formado por uma string que corresponde ao nome da UC, e um par formado pelo nome da turma e a aula correspondente
//! O(n log(n))
vector<pair<string,pair<string,Aula>>> Manager::getAulas(const Estudante &estudante) const {
    vector<pair<string,pair<string,Aula>>> result;
    set<pair<string,string>> turmas = estudante.getTurmas(); // O(1)
    for (const auto& par : turmas) { // O(n)
        TurmaInfo turmaInfo = obterInfoUc(par.first, par.second);

        for (const auto& element : turmaInfo.aulasTeoricas)  { // O(n)
            result.emplace_back(par.first, make_pair(par.second, element));
        }
        result.emplace_back(par.first, make_pair(par.second, turmaInfo.pratica));
    }
    return result;
}

//! Retorna a lista de aulas de um estudante, excluindo uma unidade curricular em específico O(n log(n))
list<Aula> Manager::obterHorarioEstudantePraticasExceto(const Estudante &estudante, const string &uc) const {
    set<pair<string,string>> turmas = estudante.getTurmas(); // O(1)
    list<Aula> res;
    for (const auto& par : turmas) { // O(n)
        if (par.first != uc) {
            res.push_back(obterPraticaUc(par.first, par.second)); // O(log(n))
        }

    }

    return res;
}

//! Retorna a lista de aulas práticas de um estudante
//! O(n log(n))
list<Aula> Manager::obterHorarioEstudantePraticas(const Estudante &estudante) const {
    set<pair<string,string>> turmas = estudante.getTurmas();
    list<Aula> res;

    for (const auto& par : turmas) {
        res.push_back(obterPraticaUc(par.first, par.second)); // O(log(n))
    }

    return res;
}

//! Retorna a informação da turma na UC O(log(n))
TurmaInfo Manager::obterInfoUc(const string &uc, const string &turma) const {
    TurmaInfo res;

    auto it = ucs.find(uc); // O(log(n))

    if (it != ucs.end()) {
        map<string, TurmaInfo> turmaInfo = it->getUcTurma();
        auto it = turmaInfo.find(turma); // O(log(n))
        if (it->first == turma) {
            res = it->second;
        }
    }


    return res;
}

//! Retorna a aula prática da turma na UC O(log(n))
Aula Manager::obterPraticaUc(const string &uc, const string &turma) const {
    Aula res;
    auto it = ucs.find(uc); // O(log(n))

    if (it != ucs.end()) {
        res = it->getPratica(turma); // O(1)
    }

    return res;
}

//! Retorna um map com todas as turmas e aulas correspondentes de uma UC em que é possível inserir o estudante
//! O(n log(n))
map<string,list<Aula>> Manager::getTurmasPossiveis(const string &uc, list<Aula> &horarioPraticas) {
    auto it = ucs.find(uc); // O(log(n))
    map<string,list<Aula>> res;

    if (it != ucs.end()) {
        map<string,TurmaInfo> turmas = it->getUcTurma();

        for (const auto& t : turmas) {
            Aula pratica = obterPraticaUc(uc, t.first); // O(log(n))
            if (verificarAulaSobreposta(horarioPraticas,pratica)) {
                TurmaInfo info = obterInfoUc(uc,t.first);
                //! Verifica se a turma tem menos de 30 alunos e se adicionando um elemento não compromete o equilíbrio entre as turmas
                if (info.estudantes.size() < 30 && it->checkBalance(t.first)) {
                    list<Aula> aulas = info.aulasTeoricas;
                    aulas.push_back(info.pratica);
                    res.insert({t.first, aulas});
                }
            }
        }
    }
    return res;
}

//! Função utilizada pelo Menu para visualizar as turmas em que é possível inserir o estudante e as respetivas aulas
map<string,list<Aula>> Manager::enviaListaDeAulaPossivel(const string &uc, const int &estudante) {
    Estudante student = getEstudante(estudante);

    list<Aula> praticas = obterHorarioEstudantePraticas(student); // O(n log(n))
    map<string,list<Aula>> res = getTurmasPossiveis(uc, praticas);
    return res;
}

//! Retorna um set de UC's de um determinado ano
//! O(n)
vector<string> Manager::getUcPorAno(const int &ano) const {
    vector<string> res;
    for (const auto& uc : ucs) {
        if (uc.getAno() == ano) {
            res.push_back(uc.getCodigoUc());
        }
    }
    return res;
}

//! Retorna um set de turmas num determinado ano
//! O(n)
map<string,TurmaInfo> Manager::getTurmasPorAno(const int &ano) const {
    map<string,TurmaInfo> res;
    for (const auto& uc : ucs) {
        if (uc.getAno() == ano) {
            if (uc.getUcTurma().size() > res.size()) {
                res = uc.getUcTurma();
            }
        }
    }
    return res;
}

//! Retorna um set de turmas de uma turminada uc
//! O(n log(n))
set<string> Manager::getTurmasPorUc(const string& uc) const {
    auto it = ucs.find(uc); //! O(log(n))
    set<string> res;

    if (it != ucs.end()) {
        for (const auto& turma : it->getUcTurma()) { //! O(n log(n))
            res.insert(turma.first);
        }
    }
    return res;
}

//! Retorna um set de pares, com o nome da UC e turma em que o estudante está incrito, de forma a aparecer listado no menu, quando pretender remover uma UC
//! O(log(n))
set<pair<string,string>> Manager::enviaUCParaRemover(const int &numero) const {
    auto estudante = estudantesNumero.find(numero); //! O(log(n))
    set<pair<string, string>> res;

    if (estudante != estudantesNumero.end()) {
        res = estudante->getTurmas();
    }
    return res;
}

//! Retorna um set de pares com o número de estudantes de uma UC e o respetivo nome da UC
//! O(n log(n))
set<pair<int,string>> Manager::getOcupacaoUcs() const {
    set<pair<int,string>> allUCs;
    for (const auto& uc : ucs) {
        allUCs.insert({uc.getNumeroAlunosTotal(), uc.getCodigoUc()});
    }
    return allUCs;
}

//! Retorna um set de pares com o número de alunos de uma turma e o nome da turma
//! O(n log(n))
set<pair<int,string>> Manager::getOcupacaoTurmas(const string &uc) const {
    auto it = ucs.find(uc); // O(log(n))
    set<pair<int, string>> allTurmas;

    if (it != ucs.end()) {
        for (const auto& turma: it->getUcTurma()) {
            allTurmas.insert({it->getNumeroAlunos(turma.first), turma.first});
        }
    }
    return allTurmas;
}

//! Retorna um vetor de pares de inteiros, em que o valor da esquerda corresponde ao nº de estudantes inscritos no nº da direita UC's
//! O(n)
vector<pair<int, int>> Manager::getAlunosPorNIncscricoes() const {
    vector<pair<int,int>> result = {{0,1}, {0,2}, {0,3}, {0,4}, {0,5}, {0,6}, {0,7}};
    for (const auto& estudante : estudantesNumero){
        int i = estudante.getTurmas().size();
        result[i-1].first++;
    }
    return result;
}

//! Retorna um vetor de pares de inteiros, em que o valor da esquerda corresponde ao nº de alunos no ano da direita
//! O(n)
vector<pair<int, int>> Manager::getNumeroDeAlunosPorAno() const {
    vector<pair<int, int>> result = {{0,1}, {0,2}, {0,3}};
    int first = 0, second = 0, third = 0;
    for (const auto& estudante : estudantesNumero) {
        int ano = estudante.getAno();
        result[ano-1].first++;
    }
    return result;
}

//! Retorna o número de turmas de uma UC
//! O(log(n))
int Manager::getNumeroTurmas(const string& uc) {
    auto it = ucs.find(uc); //! O(log(n))
    int res = 0;
    if (it != ucs.end()) {
        res = it->getNumeroTurmas();
    }
    return res;
}

//! Auxiliary
//! Função auxiliar utilizada para construir as horas no horário e na tabela de aulas sobrepostas
string getHoras(const float& begin, const float& duration = 0.5){
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

//! Retorna a abreviatura de uma UC, consoante o seu código, utilizado no horário
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

//! Função auxiliar que compara um par de interiros e strings pelo primeiro elemento
auto compareFirstElement = [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b) {
    return a.first < b.first;
};

//! Função auxiliar que compara um par de interiros e strings pelo segundo elemento
auto compareSecondElement = [](const std::pair<int, std::string>& a, const std::pair<int, std::string>& b)  {
    return a.second < b.second;
};

//! Struct utilizada para criar um set
struct CompareByFirst{
    bool operator()(const pair<int, string>& a, const pair<int, string>& b) const {
        return a.first < b.first;
    }
};

//! Struct utilizada para criar um set
struct CompareBySecond{
    bool operator()(const pair<int, string>& a, const pair<int, string>& b) const {
        return a.second < b.second;
    }
};

//! Validators
//! Verifica se o novo pedido é concorrente com um pedido ainda em espera. Retorna true caso não seja concorrente com nenhum pedido em espera e false no caso contrário.
//! O(n)
bool Manager::verificarPedidosRepetidos(const Pedido &pedido) {
    for (auto p : pedidosEspera) { // O(n)
        if (! p.pedidosConcorrentes(pedido)) {
            return false;
        }
    }
    return true;
}

//! Adiciona um pedido novo à fila de pedidos e ao vetor de pedidos pendentes, se o pedido não for concorrente com os pedidos em espera e se for válido, dependendo do tipo de pedido. Retorna true no caso de ter adicionado e false no caso contrário
//! O(n^2)
bool Manager::addPedido(Pedido pedido) {
    bool res = false;

    if (!verificarPedidosRepetidos(pedido)) return false;

    switch (pedido.getTipoAlteracao()) {
        //! No caso de ser uma alteração de turma entre dois estudantes
        case TipoAlteracao::H: res = trocaValida(pedido); // O(n log(n))
            break;
            //! No caso de ser um pedido de remoção de UC
        case TipoAlteracao::R: res = removerValida(pedido); // O(log(n))
            break;
            //! No caso de ser um pedido de adicionar UC
        case TipoAlteracao::A: res = validarNovaUc(pedido.getUc(), pedido.getEstudante().getStudentNumber()); // O(n^2)
            break;
    }
    if (res) {
        cout << string(38,'-') << endl;
        cout << "| Pedido adicionado à fila de espera |" << endl;
        cout << string(38,'-') << endl;
        pedidos.push(pedido);
        pedidosEspera.push_back(pedido);
        guardaPedidosPendentes(); // O(n)
    }
    return res;
}

//! Verifica se o estudante pode remover uma UC, para isso tem de estar inscrito. Retorna true se é válido e false se é inválido
//! O(log(n))
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

//! Verifica se a troca de turma entre dois estudantes é válida, não sendo quando um deles não está inscrito na UC, quando ambos já estão na mesma turma ou quando não existe compatibilidade de horário (Aulas PL e P sobrepostas). Retorna true no caso de ser válida e false no outro caso
//! O(n log(n))
bool Manager::trocaValida(Pedido &pedido) const {
    Estudante estudante = pedido.getEstudante();
    Estudante outro = pedido.getOutroEstudante();
    std::string uc = pedido.getUc();

    //! Verifica se os dois estudantes estão inscritos na UC
    if (!(estudante.inscrito(uc) && outro.inscrito(uc))) {
        // Pelo menos um dos alunos não esta inscrito na UC
        cout << string(52,'-') << endl;
        cout << "| Troca inválida, a Unidade curricular não é comum |" << endl;
        cout << string(52,'-') << endl;
        return false;
    }

    std::string turma = estudante.getTurma(uc);
    std::string outraTurma = outro.getTurma(uc);

    //! Verifica se os dois estudantes já são da mesma turma
    if (turma == outraTurma) {
        // Já são de turmas iguais
        cout << string(47,'-') << endl;
        cout << "| Troca inválida, já são da mesma turma |" << endl;
        cout << string(47,'-') << endl;
        return false;
    }

    //! Verifica o horário de cada estudante (excluindo a aula que quero trocar)
    list<Aula> horarioUm = obterHorarioEstudantePraticasExceto(estudante,uc); // O(n log(n))
    Aula aula = obterPraticaUc(uc,outraTurma); // O(log(n))
    list<Aula> horarioOutro = obterHorarioEstudantePraticasExceto(outro,uc); // O(n log(n))
    Aula aulaNova = obterPraticaUc(uc,turma); // O(log(n))

    //! Caso algum aluno não tenha compatibilidade de horário para trocar, a troca é inválida O(n)
    if (!(verificarAulaSobreposta(horarioUm,aula) && verificarAulaSobreposta(horarioOutro, aulaNova))) {
        cout << string(47,'-') << endl;
        cout << "| Alteração inválida, aula Prática sobreposta |" << endl;
        cout << string(47,'-') << endl;
        return false;
    }
    return true;
}

//! Verifica se uma nova aula é sobreposta a alguma aula de um horário, retornando true quando a aula é compativel com o horário e false no caso contrário
//! O(n)
bool Manager::verificarAulaSobreposta(const list<Aula> &horario, const Aula &aulaNova) {
    for (const auto &aula : horario) {
        if (aula.sobreposta(aulaNova)) {
            return false;
        }
    }
    return true;
}

//! Verificar se o estudante tem possibilidade de se inscrever na nova turma. Para isso não pode estar já inscrito nessa UC, não pode estar inscrito a 7 UC's e tem de ter compatibilidade de horário com as auals da UC. Retorna true se a nova UC é válida e false no caso contrário
//! O(n^2)
bool Manager::validarNovaUc(const string &uc, const int &student) {
    auto estudante = getEstudante(student); // O(log(n))
    if (estudante.inscrito(const_cast<string &>(uc))) {
        cout << string(33,'-')  << endl;
        cout << "O estudante já está incrito na UC" << endl;
        cout << string(33,'-')  << endl;
        return false;
    }

    if (estudante.getTurmas().size() >= 7) {
        cout << string(58,'-') << endl;
        cout << "O estudante já está incrito a muitas Unidades Curriculares" << endl;
        cout << string(58,'-') << endl;
        return false;
    }

    // O(log(n))
    auto it = ucs.find(uc);
    list<Aula> praticas = obterHorarioEstudantePraticas(estudante);

    if (it != ucs.end()) {
        map<string,TurmaInfo> turmas = it->getUcTurma();
        // O(n^2)
        for (const auto &t : turmas) { // O(n)
            if ((verificarAulaSobreposta(praticas, obterPraticaUc(uc,t.first)))) { // O(log(n))
                return true;
            }
        }
        cout << "O estudante não tem compatibilidade de horário" << endl;
        return false;
    }
    cout << string(19,'-')  << endl;
    cout << "| A UC não existe |" << endl;
    cout << string(19,'-')  << endl;
    return false;
}

//! Verifica se um número n uc's é válido. Retorna true se for válido e false caso contrário
//! O(1)
bool Manager::nUcValido(const int &n) const {
    return (n > 0 && n <= ucs.size());
}

//! Verifica se o número de estudante é válido. Retorna true se for válido e false caso contrário
//! O(log(n)).
bool Manager::estudanteValido(const int &numero) const {
    auto it = estudantesNumero.find(numero);
    return it != estudantesNumero.end();
}

//! Recebe um pedido do Menu para criar um novo pedido. Cria o pedido e envia para as funções que verificam se é válido, retornando true nesse caso e false no caso contrário
//! O(n^2)
bool Manager::inputToPedido(const string& uc, const int &estudante, const string &tipo, const int outro, const string &turma) {
    Estudante student1 = getEstudante(estudante); //! O(log(n))
    if (tipo == "H") {
        Estudante student2 = getEstudante(outro);
        return addPedido(Pedido(uc, student1, student2));
    }
    return addPedido(Pedido(uc, student1, tipo, turma));
}

//! Verifica se uma UC é válida, retornando true nesse caso e false no caso contrário
//! O(log(n))
bool Manager::ucValida(const string &uc) const {
    return (ucs.find(uc) != ucs.end());
}

//! Verifica se uma turma é válida retornando true nesse caso e false no caso contrário
//! O(n log(n))
bool Manager::turmaValida(const string &turma) const {

    for (const auto& element : ucs) {
        auto find = element.getUcTurma().find(turma);
        if (find != element.getUcTurma().end()) {
            return true;
        }
    }
    return false;
}

//! Função utilizada para verificar se uma aula já foi contabilizada no horário. Retorna true caso a aula já esteja no horário e false caso contrário
//! O(n)
bool Manager::checkAlreadyIn(vector<pair<string,pair<string,Aula>>> &horario, pair<string,pair<string,Aula>> &aula) const {
    string line;
    for (const auto& slot : horario) {
        if (slot.first == aula.first && slot.second.second.mesmoHorario(aula.second.second)) {
            return true;
        }
    }
    float turno = stof(aula.second.first.substr(5));

    auto it = ucs.find(aula.first);

    //! O primeiro turno é atribuído às turmas com o número inferior ao valor médio de turmas dessa UC e o segundo às turmas com valor superior
    if (turno <= it->getMedia()) {
        line = "Turno 1";
    }
    else {
        line = "Turno 2";
    }
    aula.second.first = line;
    return false;
}

//! Printers
//! Da print do histórico de pedidos executados
//! 0(n)
void Manager::printHistorico() const {
    if (!this->printHist.empty()) {
        cout << string(69,'-') << endl;
        cout << "Histórico de alterações: " << endl;
        cout << string(69,'-') << endl;
        for (const auto& line : this->printHist) {
            cout << line.first << " - " << line.second;
        }
        cout << string(69,'-') << endl;
    }
    else {
        cout << "Não existem pedidos" << endl;
    }
}

//! Da print dos estudantes por turma, com opção de ordenação por número ou nome por ordem crescente e decrescente
//! O(n log(n))
void Manager::printEstudantesPorTurmaNaUc(const string &uc, const string &turma, bool orderByNumber, bool ascending) const {
    auto it = ucs.find(uc); //! O(log(n))

    if (it != ucs.end()) {
        map<string,TurmaInfo> turmaInfo = it->getUcTurma();
        auto iterator = turmaInfo.find(turma); //! O(log(n))
        //! No caso da turma não ter alunos
        if (iterator->second.estudantes.empty()) {
            cout << string(56,'-')<<endl;
            cout << '|' << string ( 12, ' ') << "Não existem alunos nesta turma"<< string(12, ' ') << '|' << endl;
            cout << string(56,'-')<<endl;

            return;
        }
        if (iterator != turmaInfo.end()) {
            string line = ucToString(it->getCodigoUc());
            int len = (54 - 27 - 4 - line.length()) / 2;
            int lenf = (54 - 27 - 4 - line.length()) % 2 == 0 ? len : len + 1;
            cout << string(56,'-') << endl;
            cout <<'|' << string(len,' ') << "UC: " << line << string(lenf, ' ') << "|      Turma: " << iterator->first << string(6,' ') << '|' << endl;
            cout << string(56,'-') << endl;


            if (!orderByNumber) {
                set<pair<int,string>, CompareBySecond> studentList;
                for (auto element : iterator->second.estudantes) {
                    studentList.insert(element);
                }
                //! No caso de pretender ascendente
                if (ascending){
                    for (const auto& element : studentList){

                        int totalWidth = 26;
                        if (element.second == "Verónica"){
                            for (int i = 0; i < element.second.length(); i++) {
                                if (!isalpha(element.second[i])){
                                    cout << 1;
                                    totalWidth++;
                                    i++;
                                }
                            }
                            cout << endl;
                        }

                        /*
                        if (element.second == "Verónica" || element.second == "Ambrósio"){
                            totalWidth = 27 ;
                        }*/

                        cout << '|'<< string(18,' ') << element.first << ' ' << element.second << string(totalWidth-element.second.length(),' ') << '|' << endl;
                    }
                }
                    //! No caso de pretender descendente
                else{
                    for (auto i = studentList.rbegin(); i != studentList.rend(); ++i){
                        int totalWidth = 26;

                        if (i->second == "Verónica" || i->second == "Ambrósio"){
                            totalWidth = 27 ;
                        }

                        cout << '|'<< string(18,' ') << i->first << ' ' << i->second << string(totalWidth- i->second.length(),' ') << '|' << endl;
                    }
                }
            } else {
                set<pair<int,string>> studentList = iterator->second.estudantes;
                //! No caso de pretender ascendente
                if (ascending){
                    for (const auto& element : studentList){

                        int totalWidth = 26;

                        if (element.second == "Verónica" || element.second == "Ambrósio"){
                            totalWidth = 27 ;
                        }

                        cout << '|'<< string(18,' ') << element.first << ' ' << element.second << string(totalWidth-element.second.length(),' ') << '|' << endl;
                    }
                }
                    //! No caso de pretender descendente
                else{
                    for (auto i = studentList.rbegin(); i != studentList.rend(); ++i){
                        int totalWidth = 26;

                        if (i->second == "Verónica" || i->second == "Ambrósio"){
                            totalWidth = 27 ;
                        }

                        cout << '|'<< string(18,' ') << i->first << ' ' << i->second << string(totalWidth- i->second.length(),' ') << '|' << endl;
                    }
                }
            }
        }
        cout << string(56,'-')<< endl;
    }
}

//! Lista o número de estudantes por turma numa determinada UC, de acordo com o código da turma ou o número de estudantes, por ordem crescente ou decrescente.
//! O(n log(n))
void Manager::printNumeroEstudantesPorTurmaPorUc(const std::string &uc, const bool &orderByFirst , const bool &ascending) const {
    vector<pair<string,int>> turmas;
    string line = ucToString(uc);
    int len = (32 - (18 + line.length())) / 2;
    int lenF = (32 - (18 + line.length())) % 2 == 0 ? len : len + 1;
    cout << string(35,'-') << endl;
    cout << '|' <<string(len, ' ') << "Estudantes na UC: " << line << string(lenF, ' ') <<" |"<< endl;
    cout << string(35,'-') << endl;
    auto it = ucs.find(uc); // O(log(n))

    if (it != ucs.end()) {
        for (const auto& element : it->getUcTurma()) { // O(n)
            turmas.emplace_back(element.first, it->getNumeroAlunos(element.first));
        }
    }
    // Ordem de acordo com o código da turma.
    // O(n log(n))
    if (orderByFirst) {
        sort(turmas.begin(), turmas.end(), [] (const pair<string,int>& first, const pair<string,int>& second) {return first.first < second.first;});
    }
    // Ordem de acordo com o número de estudantes.
    else {
        sort(turmas.begin(), turmas.end(), [] (const pair<string,int>& first, const pair<string,int>& second) {return first.second < second.second;});
    }
    // Ordem crescente
    if (ascending) {
        for (const auto& elem : turmas) {
            int len = (11 - to_string(elem.second).length());
            cout <<"|          "<< elem.first << " Nº: " << elem.second << string(len, ' ') << "|" <<endl;
        }
    }
    // Ordem decrescente
    else {
        for (auto it = turmas.rbegin(); it != turmas.rend(); it++) {
            int len = (11 - to_string(it->second).length());
            cout <<"|          "<< it->first << " Nº: " << it->second << string(len, ' ') << "|" << endl;
        }
    }
    cout << "-----------------------------------"  << endl;
}

//! Lista o nome e o número dos estudantes inscritos em determinada UC, ordenados de acordo com o número ou nome do estudante, por ordem crescente ou decrescente.
//! O(n^2 log(n))
void Manager::printEstudantesPorUC(const string &uc, const bool& orderByNumber, const bool& ascending) const {
    // Ordem de acordo com o número do estudante.
    if (orderByNumber) {
        set<pair<int, string>, CompareByFirst> studentList;
        auto it = ucs.find(uc); // O(log(n))

        if (it != ucs.end()) {
            for (const auto& turma : it->getUcTurma()) {
                for (const auto& element : turma.second.estudantes) {
                    studentList.insert(element);
                }
            }
        }

        if (studentList.empty()){
            cout << string(56,'-')<< endl;
            cout <<"|" << string(10,' ')<< "Não existem estudantes na UC: " << uc << string(9,' ') << "|"<<endl;
            cout << string(56,'-')<< endl;
            return;
        }

        string line = ucToString(it->getCodigoUc());
        int lenI = (54 - (28 + line.length())) / 2;
        int lenF = (54 - (28 + line.length())) % 2 == 0 ? lenI : lenI + 1;
        cout << string (56,'-')  << endl;
        cout <<"|" << string(lenI,' ') <<"Estudantes inscritos na UC: " << line << string(lenF,' ') << "|"<<endl;
        cout << string (56,'-')  << endl;
        // Ordem ascendente
        if (ascending){

            for (const auto& element : studentList){
                int totalWidth = 28;

                if (element.second == "Verónica" || element.second == "Ambŕosio"){
                    totalWidth = 29 ;
                }
                cout << '|' << string (16,' ') << element.first << " " << element.second << string(totalWidth-element.second.length(),' ') << "|" <<endl;
            }
        }
        // Ordem decrescente
        else{
            for (auto i = studentList.rbegin(); i != studentList.rend(); ++i){
                int totalWidth = 28;

                if (i->second == "Verónica" || i->second == "Ambŕosio"){
                    totalWidth = 29 ;
                }
                cout << '|' << string (16,' ') << i->first << " " << i->second << string(totalWidth - i->second.length(),' ') << "|" <<endl;
            }
        }
        cout << string (56,'-')  << endl;
    }
    // Ordem de acordo com o nome do estudante.
    else {
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
            cout << string (56,'-')  << endl;
            cout <<"|" << string(10,' ')<< "Não existem estudantes na UC: " << ucToString(uc) << string(9,' ') << "|"<<endl;
            cout << string (56,'-')  << endl;
            return;
        }

        string line = ucToString(it->getCodigoUc());
        int lenI = (54 - (28 + line.length())) / 2;
        int lenF = (54 - (28 + line.length())) % 2 == 0 ? lenI : lenI + 1;
        cout << string (56,'-')  << endl;
        cout <<"|" << string(lenI,' ') <<"Estudantes inscritos na UC: " << ucToString(it->getCodigoUc()) << string(lenF,' ') << "|"<<endl;
        cout << string (56,'-')  << endl;

        // Ordem ascendente
        if (ascending){
            for (const auto& element : studentList){

                int totalWidth = 28;

                if (element.second == "Verónica" || element.second == "Ambrósio"){
                    totalWidth = 29 ;
                }

                cout<< "|"<< string (16,' ') << element.first << ' ' << element.second << string(totalWidth-element.second.length(),' ') << "|" <<endl;
            }
        }
        // Ordem descendente
        else{
            for (auto i = studentList.rbegin(); i != studentList.rend(); ++i){
                int totalWidth = 28;

                if (i->second == "Verónica" || i->second == "Ambrósio"){
                    totalWidth = 29 ;
                }

                cout<< "|"<< string (16,' ') << i->first << ' ' << i->second << string(totalWidth-i->second.length(),' ') << "|" <<endl;
            }
        }
        cout << string (56,'-')  << endl;
    }
}

//! Lista o nome e o número dos estudantes inscritos em cada ano, ordenados de acordo com o número ou nome do estudante, por ordem crescente ou decrescente.
//! O(n)
void Manager::printEstudantesPorAno(const int &ano, const bool &orderByNumber, const bool &ascending) const {
    cout << string (56,'-')  << endl;
    cout << "|" << string(16,' ')<< "Estudantes do " << ano << "º ano:"<< string(17,' ')<< "|" << endl;
    cout << string (56,'-')  << endl;

    // Ordem de acordo com o nome do estudante
    if (orderByNumber){
        // Ordem ascendente
        if (ascending){
            for (const auto& estudante : estudantesNumero) { // O(n)
                if (estudante.getAno() == ano){
                    std::string studentNumber = to_string(estudante.getStudentNumber());
                    std::string studentName = estudante.getStudentName();

                    int totalWidth = 28;


                    if (studentName == "Verónica" || studentName == "Ambrósio"){
                        totalWidth = 29 ;
                    }

                    cout << "|"<< string(16,' ') << studentNumber << ' ' << studentName << string(totalWidth-studentName.length(),' ') << "|" << endl;
                }
            }
        }
        // Ordem descendente
        else {
            for (auto i = estudantesNumero.rbegin(); i != estudantesNumero.rend(); ++i){
                if (i->getAno() == ano) {
                    std::string studentNumber = to_string(i->getStudentNumber());
                    std::string studentName = i->getStudentName();


                    int totalWidth =28;
                    if (studentName == "Verónica" || studentName == "Ambrósio") {

                        totalWidth = 29 ;
                    }
                    cout << "|"<< string(16,' ') << studentNumber << ' ' << studentName << string(totalWidth-studentName.length(),' ') << "|" << endl;
                }
            }
        }
    }
    // Ordem de acordo com o nome do estudante
    else {
        // Ordem ascendente
        if (ascending){
            for (const auto& estudante : estudantesNome){
                if (estudante.getAno() == ano){

                    std::string studentNumber = to_string(estudante.getStudentNumber());
                    std::string studentName = estudante.getStudentName();

                    int totalWidth = 28;

                    if (studentName == "Verónica" || studentName == "Ambrósio"){
                        totalWidth = 29 ;
                    }

                    cout << "|"<< string(16,' ') << studentNumber << ' ' << studentName << string(totalWidth-studentName.length(),' ') << "|" << endl;
                }
            }
        }
        // Ordem descendente
        else {
            for (auto i = estudantesNome.rbegin(); i != estudantesNome.rend(); ++i){
                if(i->getAno() == ano) {
                    std::string studentNumber = to_string(i->getStudentNumber());
                    std::string studentName = i->getStudentName();

                    int totalWidth =28;
                    if (studentName == "Verónica" || studentName == "Ambrósio"){
                        totalWidth = 29 ;
                    }
                    cout << "|"<< string(16,' ') << studentNumber << ' ' << studentName << string(totalWidth-studentName.length(),' ') << "|" << endl;
                }
            }
        }
    }
    cout << string(56,'-') << endl;
}

//! Lista o código das turmas em deteminada UC, por ordem crescente ou decrescente
//! O(n log(n))
void Manager::printTurmasPorUC(const std::string &uc, const bool &ascending) const {
    set<string> turmas;
    cout <<  string (35,'-')<< endl;
    string line = ucToString(uc);
    int lenI = (33 - (14 + line.length())) / 2;
    int lenF = (33 - (14 + line.length())) % 2 == 0 ? lenI : lenI + 1;
    cout << "|" << string (lenI,' ') << "Turmas da UC: " << line <<string(lenF,' ')<<"|"<<endl ;
    cout <<  string (35,'-')<< endl;
    auto it = ucs.find(uc); // O(log(n))

    if (it != ucs.end()) {
        for (const auto& element : it->getUcTurma()) {
            turmas.insert(element.first); // O(log(n))
        }
    }
    // Ordem ascendente
    if (ascending){
        for (const auto& turma : turmas){
            cout <<"|             "<< turma <<"             |" <<endl;
        }
    }
    // Ordem descendente
    else {
        for (auto i = turmas.rbegin(); i != turmas.rend(); ++i){
            cout <<"|             "<< *i <<"             |"<< endl;
        }
    }
    cout << "-----------------------------------"  << endl;
}

//! Lista o número e nome dos estudante incritos em pelo menos n UC's(nUcs), ordenados de acordo com o número ou nome do estudante, por ordem crescente ou decrescente. O(n)
//! O(n)
void Manager::numeroEstudantesEmPeloMenosNUCS(const int &nUcs, const bool& orderByNumber, const bool& ascending) const{
    stringstream ss;
    list<Estudante> students;
    // Ordem de acordo com o número do estudante
    if (orderByNumber) {
        // Ordem ascendente
        if (ascending) {
            for (const auto& estudante : estudantesNumero){
                if (estudante.getTurmas().size() >= nUcs){
                    students.push_back(estudante);

                    string studentName = estudante.getStudentName();
                    int totalWidth = 48;

                    if (studentName == "Verónica" || studentName == "Ambrósio"){
                        totalWidth = 49 ;
                    }

                    ss <<'|' << string(40,' ')<< estudante.getStudentNumber() << ' ' << estudante.getStudentName() << string(totalWidth-studentName.length(),' ')<< '|' << endl;
                }
            }
        }
        // Ordem descendente
        else {
            for (auto i = estudantesNumero.rbegin(); i != estudantesNumero.rend(); ++i){
                if (i->getTurmas().size() >= nUcs){
                    students.push_back(*i);
                    string studentName = i->getStudentName();

                    int totalWidth = 48;

                    if (studentName == "Verónica" || studentName == "Ambrósio"){
                        totalWidth = 49 ;
                    }

                    ss <<'|' << string(40,' ')<< i->getStudentNumber() << ' ' << i->getStudentName() << string(totalWidth-studentName.length(),' ')<< '|' << endl;
                }
            }
        }
    }
    // Ordem de acordo com o número do estudante
    else {
        // Ordem ascendente
        if (ascending) {
            for (const auto& estudante : estudantesNome){
                if (estudante.getTurmas().size() >= nUcs){
                    students.push_back(estudante);

                    string studentName = estudante.getStudentName();
                    int totalWidth = 48;

                    if (studentName == "Verónica" || studentName == "Ambrósio"){
                        totalWidth = 49 ;
                    }

                    ss <<'|' << string(40,' ')<< estudante.getStudentNumber() << ' ' << estudante.getStudentName() << string(totalWidth-studentName.length(),' ')<< '|' << endl;
                }
            }
        }
        // Ordem descendente
        else {
            for (auto i = estudantesNome.rbegin(); i != estudantesNome.rend(); ++i){
                if (i->getTurmas().size() >= nUcs){
                    students.push_back(*i);

                    string studentName = i->getStudentName();

                    int totalWidth = 48;

                    if (studentName == "Verónica" || studentName == "Ambrósio"){
                        totalWidth = 49 ;
                    }

                    ss <<'|' << string(40,' ')<< i->getStudentNumber() << ' ' << i->getStudentName() << string(totalWidth-studentName.length(),' ')<< '|' << endl;

                }
            }
        }
    }
    if (!students.empty()) {
        cout << string(100,'-') << endl;
        cout << '|' << string (17, ' ') << "Estão inscritos " << students.size() << " alunos em pelo menos " << nUcs << " Unidades Curriculares." << string(17,' ')<< '|' << endl;
        cout << string(100,'-') << endl;
        cout<< ss.str();
        cout << string(100,'-') << endl;

    }
    else {
        cout << string(100,'-') << endl;
        cout << '|' << string (17, ' ') << "Não existem alunos inscritos a " << nUcs << " Unidades Curriculares."<<  string(16,' ')<< '|' << endl;
        cout << string(100,'-') << endl;
    }

}

//! Consoante o tipo (tipo) de horário que se quer obter (horário de estudante, UC ou turma), o método chama um método que busca as aulas desse horário e chama o método printHorario para essas aulas.
//! O(n^3)
void Manager::inputToHorario(const char &tipo, const string &uc, const string &turma, const int &numero) {
    switch (tipo) {
        case 'E': {
            Estudante estudante = getEstudante(numero);
            vector<pair<string,pair<string,Aula>>> horario = getAulas(estudante); // O(n^2)
            printHorario(horario);
            break;
        }
        case 'U': {
            auto it = ucs.find(uc);
            vector<pair<string,pair<string,Aula>>> horario = it->getAulasUc(); // O(n^2)
            printHorario(horario);
            break;
        }
        case 'T': {
            vector<pair<string,pair<string,Aula>>> horario;
            for (const auto& unidade : ucs) {
                list<Aula> aulasUc = unidade.getAulasTurma(turma); // O(log(n))
                for (const auto& aula : aulasUc) {
                    horario.push_back({unidade.getCodigoUc(), {turma, aula}});
                }
            }
            printHorario(horario);
            break;
        }
    }
}

//! Permite visualizar o horário de um aluno, UC ou turma a partir de um vetor de pares composto por uma string que corresponde ao nome da UC, e um par formado pelo nome da turma e a aula correspondente.
//! O(n^3)
void Manager::printHorario(vector<pair<string,pair<string,Aula>>> horario) const {

    // Chama o método createSobrepostas para definir o conteúdo da tabela de aulas sobrepostas
    // e retirar essas aulas do horário.
    vector<pair<string,pair<string,Aula>>> sobrepostas = createSobrepostas(horario);

    //mapa que estabelece correspondência entre os dias da semana em inglês e em portugês que irão aparecer no horário.
    map<string,string> translate = {{"Monday","Segunda"}, {"Tuesday","Terça"}, {"Wednesday","Quarta"}, {"Thursday","Quinta"}, {"Friday","Sexta"}};
    //mapa que estabelece correspondência entre os dias da semana e um número. Vai ser utilizado na construção da sparseMatrix.
    map<string,int> weekdays = {{"Monday",1}, {"Tuesday",2}, {"Wednesday",3}, {"Thursday",4}, {"Friday",5}};
    //É utilizada uma sparseMatrix para determinar onde se encontram blocos do horário com aulas,
    // o que é estabelecido pelas coordenadas atribuídas a cada bloco.
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
        float hours = i /2.0 + 8.0;
        sparseMatrix.insert({{0,i*2}, getHoras(hours)});
        sparseMatrix.insert({{0,i*2+1}, "-----------------"});
    }
    cout << endl;
    cout << "-------------------------------------------------------------------------------------------------" << endl;
    cout << "|     Horas     |    Segunda    |     Terça     |    Quarta     |    Quinta     |    Sexta      |" << endl;
    cout << "-------------------------------------------------------------------------------------------------" << endl;
    // Dá se a impressão de cada bloco. Se no mapa sparseMatrix se encontrar um bloco com as coordenadas
    // (i,j), então é impresso esse bloco. Caso contrário, é impresso um bloco default.
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
        cout << endl;
    }

    if (sobrepostas.empty()) return;
    cout << endl << endl << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "|                     Aulas Sobrepostas                     |" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << "| Unidade Curricular |   Turma   |   Dia    |    Horário    |" << endl;
    cout << "-------------------------------------------------------------" << endl;
    // Impressão das aulas sobrepostas
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

//!Permite visualizar o nome, número, ano, UC's e Turmas de um estudante com um determinado número (numero)
//! O(n)
void Manager::printInfoEstudante(const int &numero) const {
    Estudante estudante = getEstudante(numero); //! O(log(n))
    set<pair<string,string>> lista;
    if (estudante.getStudentNumber() != 0) {
        lista = estudante.getTurmas();
    }


    int len, lenf;
    int nameSpace = 27;
    auto it = lista.begin();
    cout << string(57 , '-') << endl;
    cout << "|" << string(34,' ') << "|    UC    |  Turma  |" << endl;
    for (int i = 0; i < lista.size() || i < 3; i++) {
        if (i == 0){
            len = (7 - ucToString(it->first).length());
            cout << "| Nome: " << estudante.getStudentName() << string(27 - estudante.getStudentName().length(), ' ') << '|';
            cout << string(3, ' ') << ucToString(it->first) << string(len, ' ') << '|';
            cout << ' ' << it->second << " |" << endl;
            it++;
        }
        else if (i == 1){
            if (it != lista.end()){
                len = (7 - ucToString(it->first).length());
                cout << "| Número: " << estudante.getStudentNumber() << string(16 , ' ') << '|';
                cout << string(3, ' ') << ucToString(it->first) << string(len, ' ') << '|';
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
                len = (7 - ucToString(it->first).length());
                cout << "| Ano: " << estudante.getAno() << string(27, ' ') << '|';
                cout << string(3, ' ') << ucToString(it->first) << string(len, ' ') << '|';
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
            len = (7 - ucToString(it->first).length());
            cout << string(3, ' ') << ucToString(it -> first) << string(len, ' ') << '|';
            cout << ' ' << it->second << " |" << endl;
            it++;
        }
    }
    cout << "---------------------------------------------------------" << endl;
}
bool Manager::nomeValido(string& nome) const {
    bool first = true;
    for (auto& ch : nome) { // O(n)
        ch = first ? toupper(ch) : tolower(ch);
        break;
    }

    for (const auto& estudante: estudantesNome) { // O(n log(n))
        if(estudante.getStudentName().find(nome) != string::npos) {
            return true;
        }
    }
    return false;
}

//! Faz uma busca por todos os estudantes com determinada string (nome) incluída no seu nome e mostra o seu nome, número e ano.
//! O(n log(n))
void Manager::printEstudantesPorNome(string& nome, const bool& ascending) const {

    bool first = true;
    for (auto& ch : nome) { // O(n)
        ch = first ? toupper(ch) : tolower(ch);
        break;
    }

    vector<Estudante> students;
    for (const auto& estudante: estudantesNome) { // O(n log(n))
        if(estudante.getStudentName().find(nome) != string::npos) {
            students.push_back(estudante);
        }
    }
    int nameSpace = 28;
    cout << string(47,'-')<< endl;
    cout << "|            Nome            |  Número   | Ano |" << endl;
    cout << string(47,'-')<< endl;
    if (ascending) {
        for (const auto &elem: students) {
            if (elem.getStudentName() == "Verónica" || elem.getStudentName() == "Ambrósio") nameSpace++;
            int len = (nameSpace - elem.getStudentName().length()) / 2;
            int lenf = (nameSpace - elem.getStudentName().length()) % 2 == 0 ? len : len + 1;
            cout << "|" << string(len, ' ') << elem.getStudentName() << string(lenf, ' ') << "| "
            << elem.getStudentNumber() << " |  " << elem.getAno() << "  |" << endl;
            cout << "------------------------------------------------" << endl;
        }
    } else {
        for (auto elem = students.rbegin(); elem < students.rend(); elem++) {
            if (elem->getStudentName() == "Verónica" || elem->getStudentName() == "Ambrósio") nameSpace++;
            int len = (28 - elem->getStudentName().length()) / 2;
            int lenf = (28 - elem->getStudentName().length()) % 2 == 0 ? len : len + 1;
            cout << "|" << string(len, ' ') << elem->getStudentName() << string(lenf, ' ') << "| "
            << elem->getStudentNumber() << " |  " << elem->getAno() << "  |" << endl;
            cout << "------------------------------------------------" << endl;
        }
    }
}

//! É utilizada no menu de Estatísticas para visualizar o número de estudantes inscrito em cada uma das UC's
//! O(log(n))
void Manager::printNumeroEstudantesDeTodasUc() const {
    string ucP = "L.EIC001";
    string ucS = "L.EIC011";
    string ucT = "L.EIC021";
    cout << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "|      1º Ano      |      2º Ano      |      3º Ano      |" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << "|    UC       Nº   |    UC       Nº   |    UC       Nº   |" << endl;
    cout << "|   ALGA:     " <<ucs.find(ucP)->getNumeroAlunosTotal() << "   |   AED:     " << ucs.find(ucS)->getNumeroAlunosTotal() << "   |   FSI:     " << ucs.find(ucT)->getNumeroAlunosTotal() << "   |" << endl;
    ucP = "L.EIC002", ucS = "L.EIC012", ucT = "L.EIC022";
    cout << "|   AM I:     " << ucs.find(ucP)->getNumeroAlunosTotal() << "   |   BD:      " << ucs.find(ucS)->getNumeroAlunosTotal() << "   |   IPC:     " << ucs.find(ucT)->getNumeroAlunosTotal() << "   |" << endl;
    ucP = "L.EIC003", ucS = "L.EIC013", ucT = "L.EIC023";
    cout << "|   FP:       "<< ucs.find(ucP)->getNumeroAlunosTotal() << "   |   F II:    " << ucs.find(ucS)->getNumeroAlunosTotal() << "   |   LBAW:    " << ucs.find(ucT)->getNumeroAlunosTotal() << "   |" << endl;
    ucP = "L.EIC004", ucS = "L.EIC014", ucT = "L.EIC024";
    cout << "|   FSC:      " << ucs.find(ucP)->getNumeroAlunosTotal() << "   |   LDTS:    " << ucs.find(ucS)->getNumeroAlunosTotal() << "   |   PFL:     " << ucs.find(ucT)->getNumeroAlunosTotal() << "   |" << endl;
    ucP = "L.EIC005", ucS = "L.EIC015", ucT = "L.EIC025";
    cout << "|   MD:       " <<  ucs.find(ucP)->getNumeroAlunosTotal() << "   |   SO:      " << ucs.find(ucS)->getNumeroAlunosTotal() << "   |   RC:      " << ucs.find(ucT)->getNumeroAlunosTotal() << "   |" << endl;
    ucP = "UP001";
    cout << "|   PUP:      " << ucs.find(ucP)->getNumeroAlunosTotal() << "    |                  |                  |" << endl;
    cout << "----------------------------------------------------------" << endl;
    cout << endl;
}

//! É utilizada no menu de estatísticas, para visualizar as "n" UC's com mais ou menos estudantes ou para visualizar as "n" turmas de determinada UC com mais ou menos estudantes.
//! O(n log(n))
void Manager::printSets(int n, const string& uc, const bool& mais) const {
    set<pair<int,string>> allTurmas;

    if (uc.empty()) allTurmas = getOcupacaoUcs(); // O(n log(n))
    else allTurmas = getOcupacaoTurmas(uc); // O(n log(n))


    if (!allTurmas.empty()) {
        float tamanho = allTurmas.rbegin()->first;
        if (mais) {
            auto it = allTurmas.rbegin();
            cout << "          |" << endl;
            while(n != 0 && it != allTurmas.rend()){

                string line = (uc == "") ? ucToString(it->second) : it->second;
                int len = (9 - line.length());

                if (it->first == 0){
                    cout << "          |" << endl;
                    cout << string(len, ' ') << line << " |" << it->first << endl;
                    cout << "          |" << endl;
                    n--;
                    it++;
                    continue;
                }
                int lenBarra = (it->first / tamanho) * 80;
                cout << string(10, ' ') << "|" << string(lenBarra, '-') << endl;
                cout << string(len, ' ') << line << " |" << string(lenBarra - 1, ' ') << "| " << it->first << endl;
                cout << string(10, ' ') << "|" << string(lenBarra, '-') << endl;
                cout << "          |" << endl;
                n--;
                it++;
            }
        }
        else {
            auto it = allTurmas.begin();
            cout << "          |" << endl;
            while(n != 0 && it != allTurmas.end()){
                string line = (uc == "") ? ucToString(it->second) : it->second;
                int len = (10 - line.length()) / 2;
                int lenf = (10 - line.length()) % 2 == 0 ? len : len + 1;
                if (it->first == 0){
                    cout << "          |" << endl;
                    cout << string(len, ' ') << line << string(lenf, ' ') << "|" << it->first << endl;
                    cout << "          |" << endl;
                    n--;
                    it++;
                    continue;
                }
                if (tamanho == 0) tamanho = 1.0;
                int lenBarra = (it->first / tamanho) * 80;
                cout << string(len + lenf + line.length(), ' ') << "|" << string(lenBarra, '-') << endl;
                cout << string(len, ' ') << line << string(lenf, ' ') << "|" << string(lenBarra - 1, ' ') << "| " << it->first << endl;
                cout << string(len + lenf + line.length(), ' ') << "|" << string(lenBarra, '-') << endl;
                cout << "          |" << endl;
                n--;
                it++;
            }
        }
    }

}

//! É utilizada no menu de estatísticas, para visualizar o número de alunos por ano ou para visualizar o número de alunos por número de inscrições em UC's.
//! O display do gráfico de barras pode ser desordenado, ou ordenado por ordem crescente ou descrescente do número de alunos.
//! O(n log(n))
void Manager::printVectors(const char &tipo, const bool &ordered, const bool& ascending) const {
    vector<pair<int,int>> res;

    //! Se se quiser visualizar o número de alunos por ano. (O(n))
    if (tipo == 'A') res = getNumeroDeAlunosPorAno();

    //! Se se quiser visualizar o número de alunos por ano. (O(n))
    else res = getAlunosPorNIncscricoes();

    if (!res.empty()) {
        cout << "       |" << endl;
        float tamanho = -1;
        for (const auto& it : res) { //! O(n)
            if (it.first > tamanho) tamanho = it.first;
        }
        //! Se o display do gráfico de barras for ordenado, o vetor res é sorted. (O(nlog(n))
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

//! Execution
//! É utilizado no Menu, para permitir a visualização do horário de um estudante, caso passe a integrar uma determinada turma de uma nova UC.
//! O(n^3)
void Manager::verHorarioAntesDeConfirmar(const int &numero, const string &uc, const string &turma, const list<Aula> &aulas) const {
    auto estudante = getEstudante(numero);

    vector<pair<string,pair<string,Aula>>> horario = getAulas(estudante); // O(n^2)

    for (const auto& aula : aulas) {
        horario.push_back({uc,{turma,aula}}); // O(n)
    }
    printHorario(horario);
}

//! Retorna um vetor de aulas (sobrepostas) que se sobrepõem a outras, apagando simultaneamente de um outro vetor de aulas (horario), correspondente ao horario.
//! O vetor de pares é composto por uma string que corresponde ao nome da UC, e um par formado pelo nome da turma e a aula correspondente.
//! O(n^3)
vector<pair<string,pair<string,Aula>>> Manager::createSobrepostas (vector<pair<string,pair<string,Aula>>> &horario) const {
    vector<pair<string,pair<string,Aula>>> sobrepostas;
    for (auto i = horario.begin(); i != horario.end(); ++i){ // O(n)
        auto j = i;
        j++;
        while(j < horario.end() && j > horario.begin()){ // O(n)

            Aula aula1 = i->second.second;
            Aula aula2 = j->second.second;
            if (aula1.sobreposta(aula2)) {
                // As aulas práticas prevalecem sempre as teóricas
                if (aula1.getTipo() != "T"  && aula2.getTipo() == "T") {
                    if (!checkAlreadyIn(sobrepostas,*j)) // O(n)
                        sobrepostas.push_back(*j);
                    horario.erase(j);
                }
                else if (aula2.getTipo() != "T" && aula1.getTipo() == "T"){
                    if (!checkAlreadyIn(sobrepostas,*i))
                        sobrepostas.push_back(*i);
                    horario.erase(i);

                }
                // As duas aulas são teóricas ou são práticas.
                else {
                    // A aula que se sobrepõe a menos aulas prevalece.
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
                        if (!(aula1.getTipo() == "T" && checkAlreadyIn(sobrepostas,*i)))
                            sobrepostas.push_back(*i);
                        horario.erase(i);
                    }
                    else if (sobre2 > sobre1){
                        sobrepostas.push_back(*j);
                        horario.erase(j);
                    }
                    // As aulas sobrepõem-se ao mesmo número de aulas.
                    else {
                        // A aula que começa mais cedo prevalece.
                        if (aula1.getInicio() < aula2.getInicio()) {
                            if (!(aula2.getTipo() == "T" && checkAlreadyIn(sobrepostas,*j)))
                                sobrepostas.push_back(*j);
                            horario.erase(j);
                        }
                        else if (aula1.getInicio() > aula2.getInicio()) {
                            if (!(aula1.getTipo() == "T" && checkAlreadyIn(sobrepostas,*i)))
                                sobrepostas.push_back(*i);
                            horario.erase(i);
                        }
                        // As aulas aula1 e aula2 começam ao mesmo tempo.
                        else {
                            // A aula cuja duração é superior prevalece.
                            if (aula1.getDuracao() > aula2.getDuracao()) {
                                //se a aula1 for maior que a aula2
                                if (!(aula2.getTipo() == "T" && checkAlreadyIn(sobrepostas,*j)))
                                    sobrepostas.push_back(*j);
                                horario.erase(j);
                            }
                            else if (aula1.getDuracao() < aula2.getDuracao()) {
                                //se a aula2 for maior que a aula1.
                                if (!(aula1.getTipo() == "T" && checkAlreadyIn(sobrepostas,*i)))
                                    sobrepostas.push_back(*i);
                                horario.erase(i);
                            }
                            // As aulas têm a mesma duração.
                            else {
                                // É escolhida a aula1.
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

//! Executa o pedido de troca de horário, atualiza os sets e as uc's
//! O(log(n))
void Manager::executarPedidoTrocaHorario(Pedido &pedido) {
    string uc = pedido.getUc();
    Estudante estudante = pedido.getEstudante();
    string turmaEstudante = estudante.getTurma(uc);
    int numeroEstudante = estudante.getStudentNumber();

    Estudante outro = pedido.getOutroEstudante();
    string turmaOutro = outro.getTurma(uc);
    int numeroOutro = outro.getStudentNumber();


    trocaTurma(uc, turmaEstudante, numeroEstudante, estudante.getStudentName(), turmaOutro, numeroOutro, outro.getStudentName()); // O(log(n))

    auto it = estudantesNumero.find(estudante); // O(log(n))
    auto itO = estudantesNumero.find(outro); // O(log(n))

    if (it != estudantesNumero.end() && itO != estudantesNumero.end()) {
        Estudante estudanteAtualizado = Estudante(*it);
        Estudante estudanteMandelao = Estudante(*itO);
        estudanteAtualizado.changeTurma(uc,turmaOutro);  // O(log(n))
        estudanteMandelao.changeTurma(uc,turmaEstudante); // O(log(n))
        estudantesNumero.erase(it); // O(log(n))
        estudantesNumero.erase(itO);// O(log(n))
        estudantesNumero.insert(estudanteAtualizado); // O(log(n))
        estudantesNumero.insert(estudanteMandelao); // O(log(n))
    }

    it = estudantesNome.find(estudante); // O(log(n))
    itO = estudantesNome.find(outro); // O(log(n))

    if (it != estudantesNome.end() && itO != estudantesNome.end()) {
        Estudante estudanteAtualizado = Estudante(*it);
        Estudante estudanteMandelao = Estudante(*itO);
        estudanteAtualizado.changeTurma(uc,turmaOutro); // O(log(n))
        estudanteMandelao.changeTurma(uc,turmaEstudante); // O(log(n))
        estudantesNome.erase(it); // O(log(n))
        estudantesNome.erase(itO); // O(log(n))
        estudantesNome.insert(estudanteAtualizado); // O(log(n))
        estudantesNome.insert(estudanteMandelao); // O(log(n))
    }
}

//! Anula a inscricão de um Estudante numa UC.
//! O(log(n))
void Manager::removerEstudanteDaUc(Pedido &pedido) {
    string uc = pedido.getUc();
    Estudante estudante = pedido.getEstudante();
    auto it = ucs.find(uc); // O(log(n))

    if (it != ucs.end()) {
        UC atualizada = UC(*it);
        atualizada.removeEstudante(pedido.getTurma(), estudante.getStudentNumber(), estudante.getStudentName()); // O(log(n))
        ucs.erase(it); // O(log(n))
        ucs.insert(atualizada); // O(log(n))
    }

    auto iterator = estudantesNumero.find(estudante); // O(log(n))

    if (iterator != estudantesNumero.end()) {
        Estudante atualizado = Estudante(*iterator);
        atualizado.removerUc(uc); // O(log(n))
        estudantesNumero.erase(iterator); // O(log(n))
        estudantesNumero.insert(atualizado); // O(log(n))
    }

    auto iteratorNome = estudantesNome.find(estudante); // O(log(n))
    if (iteratorNome != estudantesNome.end()) {
        Estudante atualizadoNome = Estudante(*iteratorNome);
        atualizadoNome.removerUc(uc); // O(log(n))
        estudantesNome.erase(iteratorNome); // O(log(n))
        estudantesNome.insert(atualizadoNome); // O(log(n))
    }
}

//! Troca a turma em que dois estudantes estão inscritos, dentro da mesma UC.
//! O(log(n))
void Manager::trocaTurma(const string &uc, const string& turma1, const int &numero1, const string &nome1, const string& turma2, const int &numero2, const string &nome2) {
    auto it = ucs.lower_bound(uc); // O(log(n))

    if (it->getCodigoUc() == uc) {
        UC atualizada = UC(*it);
        atualizada.removeEstudante(turma1, numero1,nome1); // O(log(n))
        atualizada.removeEstudante(turma2, numero2,nome2); // O(log(n))
        atualizada.addEstudante(turma2, numero1, nome1);
        atualizada.addEstudante(turma1,numero2,nome2);
        ucs.erase(it); // O(log(n))
        ucs.insert(atualizada); // O(log(n))
    }

}

//! Adiciona uma UC a um Estudante.
//! O(log(n))
void Manager::adicionarUcAoEstudante(Pedido &pedido) {
    string uc = pedido.getUc();
    string turma = pedido.getTurma();
    Estudante estudante = pedido.getEstudante();
    auto it = ucs.find(uc); // O(log(n))

    if (it != ucs.end()) {
        UC atualizada = UC(*it);
        atualizada.addEstudante(turma, estudante.getStudentNumber(), estudante.getStudentName()); // O(log(n))
        ucs.erase(it); // O(log(n))
        ucs.insert(atualizada); // O(log(n))
    }

    auto iterator = estudantesNumero.find(estudante); // O(log(n))
    if (iterator != estudantesNumero.end()) {
        Estudante atualizado = Estudante(estudante);
        atualizado.adicionarUc(uc,turma); // O(log(n))
        estudantesNumero.erase(iterator); // O(log(n))
        estudantesNumero.insert(atualizado); // O(log(n))
    }

    iterator = estudantesNome.find(estudante); // O(log(n))
    if (iterator != estudantesNome.end()) {
        Estudante atualizado = Estudante(estudante);
        atualizado.adicionarUc(uc,turma); // O(log(n))
        estudantesNome.erase(iterator); // O(log(n))
        estudantesNome.insert(atualizado); // O(log(n))
    }
}

//! Executa o próximo Pedido que se encontra nos Pedidos, coloca esse Pedido no histórico e remove-o dos Pedidos em espera.
//! O(log(n))
void Manager::proximoPedido() {
    if (!(pedidos.empty())) {
        writeRequestInFile(pedidos.front(), false);
        switch (pedidos.front().getTipoAlteracao()) {
            case TipoAlteracao::H: {
                executarPedidoTrocaHorario(pedidos.front()); // O(log(n))
                printHist.insert({this->nPedido, pedidos.front().pedidoToString()});
                this->nPedido++;
                break;
            }
            case TipoAlteracao::R: {
                removerEstudanteDaUc(pedidos.front()); // O(log(n))
                printHist.insert({this->nPedido, pedidos.front().pedidoToString()});
                this->nPedido++;
                break;
            }
            case TipoAlteracao::A: {
                adicionarUcAoEstudante(pedidos.front()); // O(log(n))
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
        guardaPedidosPendentes();
    }
    else {
        cout << "-----------------------------"  << endl;
        cout << "Não existem pedidos pendentes" << endl;
        cout << "-----------------------------"  << endl;
    }
}

//! Reverte um pedido efetuado
//! O(log(n))
void Manager::reverterPedido() {
    if (!(historico.empty())) {
        Pedido last = historico.top();
        string temp = "Reverteu o pedido: " + last.pedidoToString();
        writeRequestInFile(last,  true);
        switch (last.getTipoAlteracao()) {
            case TipoAlteracao::H: {
                executarPedidoTrocaHorario(last); // O(log(n))
                printHist.insert({this->nPedido, temp});
                this->nPedido++;
                break;
            }
            case TipoAlteracao::R: {
                adicionarUcAoEstudante(last); // O(log(n))
                printHist.insert({this->nPedido, temp});
                this->nPedido++;
                break;
            }
            case TipoAlteracao::A: {
                removerEstudanteDaUc(last); // O(log(n))
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