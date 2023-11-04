
#include "Menu.h"
#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

/*! Construtor */
Menu::Menu() {
    Manager manager;
    this->manager = manager;
    this->manager.readFiles();
    this->manager.readChanges();
    this->pedidosAtivo = manager.getPedidos();
    this->menu = false;
    this->pedidosAtivo = 0;
}


/*! Destrutor */
Menu::~Menu() {
    exitTimeThread = true;
    exitPedidoThread =  true;
    if (timeThread.joinable() || pedidoThread.joinable()) {
        timeThread.join();
        pedidoThread.join();
    }
}

// Getters
/*! Pede ao utlizador o nome de um estudante e verifica se este é válido */
void Menu::getStudentName(){
    while (true) {
        cout << "Nome do Aluno (Escreva 'sair' para voltar atrás): ";
        string line;
        cin >> line;

        // Se o input, em vez de um nome de um estudante, for 'sair' então retorna ao menu anterior.
        if (line == "sair") {
            this->sair = true;
            return;
        }

        try {
            if(!manager.nomeValido(line)) {
                cout << "Não existem estudantes com o nome " << line << endl;
                continue;
            }
            this->nome = line;
            break;
        }
        catch (exception e) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            cout << "Número inválido" << endl;
        }
    }
}

/*! Pede ao utilizador para dar input de um 2º estudante e verifica se o estudante é valido */
void Menu::getSecondStudent(){
    while (true) {
        cout << "Número do outro Aluno (Escreva 'sair' para voltar atrás): ";
        string line;
        cin >> line;

        // Se o input em vez de um número de um estudante for 'sair' então retorna ao menu anterior
        if (line == "sair") {
            this->sair = true;
            return;
        }

        try {
            this->estudante2 = stoi(line);
            // Verifica se o input tem o número de caracteres correspondentes a um número de um estudante
            if (line.length() == 9) {
                // Verfifica se o input corresponde a um estudante
                if (!manager.estudanteValido(this->estudante2)) {
                    cout << "Aluno não encontrado! " << endl;
                    continue;
                }
            }
            break;
        }
        catch (exception e) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            cout << "Número inválido" << endl;
        }
    }
}

/*! Pede ao utilizador para dar input de uma opção para navegar pelos menus */
void Menu::getUserInput() {
    while (true) {
        cout << "Escolha uma opção: ";
        string line;
        cin >> line;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        // Verifica se o input é valido
        if (line.length() != 1) {
            cout << "Escolha inválida, tente outra vez." << endl;
            continue;
        }
        this->userInput = line[0];
        break;
    }
}

/*! Pede ao utilizador para dar input de um estudante e verifica se o estudante é valido */
void Menu::getStudentNumber() {
    while (true) {
        cout << "Número do Aluno (Escreva 'sair' para voltar atrás): ";
        string line;
        cin >> line;

        // Se o input em vez de um número de um estudante for 'sair' então volta ao menu anterior
        if (line == "sair") {
            this->sair = true;
            return;
        }

        try {
            // Verifica se o número introduzido pelo utilizador se tem o tamanho certo de um número de estudante e depois verifica se corresponde a algum aluno
            this->numero_estudante = stoi(line);
            if (line.length() == 9) {
                if (!manager.estudanteValido(this->numero_estudante)) {
                    cout << "Aluno não encontrado! " << endl;
                    continue;
                }
            }
            else{
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.clear();
                cout << "Número inválido" << endl;
                continue;
            }
            break;
        }
        catch (exception e) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            cout << "Número inválido" << endl;
        }
    }
}

/*! Pede ao utilizador para inserir uma UC e verifica se essa UC existe */
void Menu::getUC() {
    while (true) {
        cout << "UC (Escreva 'sair' para voltar atrás): ";
        cin >> this->uc;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.clear();

        if (this->uc == "sair") {
            this->sair = true;
            return;
        }
        // Verifica se a UC inserida pelo Utilizador corresponde a alguma UC existente
        if (!manager.ucValida(uc)) {
            cout << "UC não encontrada! " << endl;
            continue;
        }
        break;
    }
}

/*! Pede ao utilizador para inserir a quantidade de UC's de modo a mostrar o número de alunos inscritos no número de UC's que o user inserir */
void Menu::getNuc() {
    while (true) {
        cout << "Número de UC's (Escreva 'sair' para voltar atrás): ";
        string line;
        cin >> line;

        if (line == "sair") {
            this->sair = true;
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.clear();

        try {
            // Verifica se o número de uc's inserido pelo utilizador é válido
            this->nU = stoi(line);
            if (!manager.nUcValido(this->nU)) {
                cout << "Número inválido, tente novamente" << endl;
                continue;
            }
            break;
        }
        catch (exception e) {
            cout << "Número inválido, tente novamente" << endl;
        }
    }
}
/*! Pede ao utilizador para inserir a quantidade de Turmas de modo a mostrar o número de alunos inscritos no número de Turmas que o user inserir */
void Menu::getNTurma(const string& Uc) {
    while (true) {
        cout << "Número de Turmas (Escreva 'sair' para voltar atrás): ";
        string line;
        cin >> line;

        if (line == "sair") {
            this->sair = true;
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.clear();

        try {
            // Verifica se o número de turmas inserido pelo utilizador é válido
            this->turma = stoi(line);
            if (!manager.nTurmasValidas(this->turma, Uc)) {
                cout << "Número inválido, tente novamente" << endl;
                continue;
            }
            break;
        }
        catch (exception e) {
            cout << "Número inválido, tente novamente" << endl;
        }
    }
}

// Menus
/*! Engloba os menus todos começando com o menu principal e consoante a escolha do utilizador mostra o menu escolhido */
void Menu::iniciar() {

    // Permite obter a hora local a partir do tempo decorrido desde 1 de janeiro de 1970 ( Desde epoch)
    hora = std::localtime(&data);

    timeThread = thread(&Menu::updateTime, this);
    pedidoThread = thread(&Menu::updatePedidos, this);

    // consoante a escolha do utilizador vai para o menu escolhido
    while (true) {
        // Inicia o meu principal com a hora atual
        menuInicial(hora);
        switch (this->userInput) {
            case '1':  // Horário
                verificarHorario();
                break;
            case '2': // Listagens
                menuListagens();
                break;
            case '3': // estatisticas
                menuEstatistica();
                break;
            case '4' : // Fazer pedidos
                menuOpcoesPedidos();
                break;
            case '5' : // Proximo pedido
                manager.proximoPedido();
                break;
            case '6': // Ver historico
                manager.printHistorico();
                break;
            case '7': // andar para tras
                manager.reverterPedido();
                break;
            case 'q':
                cout << "Fechando o programa" << endl;
                exit(0);
            default:
                cout << "Escolha uma opção válida" << endl;



        }
    }

}

/*! Mostra o Menu principal com as opções possíveis, a data no canta superior direito, a hora atual do lado superior esquerdo e no canto inferior esquerdo mostra os pedidos pendentes */
void Menu::menuInicial(const tm* hora) {


    cout << "_____________________________________________________________________________________________________" << endl;
    cout << "|  "<< std::setfill('0') << std::setw(2)<<hora->tm_mday<< "/" <<std::setfill('0') << std::setw(2)<< (hora->tm_mon+1) << "/" <<std::setfill('0') << std::setw(2)<<(hora->tm_year+1900)<<"                                                                             "<<std::setfill('0') << std::setw(2)<< hora->tm_hour<< ":" <<std::setfill('0') << std::setw(2)<< hora->tm_min<<"     |" << endl;
    cout << "|                                          Menu Principal                                           |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                         Consulta                          Gestão de Horário                       |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                      1 - Horários                         4 - Fazer pedido                        |" << endl;
    cout << "|                      2 - Listagens                        5 - Executar Pedido                     |" << endl;
    cout << "|                      3 - Estatísticas                     6 - Histórico de Pedidos                |" << endl;
    cout << "|                                                           7 - Reverter Pedido                     |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|                                                                                                   |" << endl;
    cout << "|    q - sair                                                                        Pedidos: " << this->pedidosAtivo << "     |" << endl;
    cout << "-----------------------------------------------------------------------------------------------------" << endl;
    cout << endl;

    this->menu = false;
    getUserInput();

}

/*! Mostra ao utilizador todas as opções de listagens e consoante a escolha do utilizador acede ao menu pretendido */
void Menu::menuListagens() {
    while(true) {
        cout << "________________________________________________________________________________________________________"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|     Listagens                       Escolha uma opção:                                               |" << endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|                                     1 - Turmas por UC                                                |"<< endl;
        cout << "|                                     2 - Estudantes por Ano                                           |"<< endl;
        cout << "|                                     3 - Estudantes por UC                                            |"<< endl;
        cout << "|                                     4 - Estudantes por Turma                                         |"<< endl;
        cout << "|                                     5 - Estudantes em pelo menos N UC's                              |"<< endl;
        cout << "|                                     6 - Número de Estudantes por Turma numa UC                       |"<< endl;
        cout << "|                                     7 - Informação do Estudante                                      |" << endl;
        cout << "|                                     8 - Procurar Estudante por nome                                  |"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------"<< endl;

        getUserInput();

        // Visto que muitos menus utilizam os menus de ordenação total e ordenação parcial utilizando o enum conseguimos reutilizar o mesmo menu
        switch (this->userInput) {
            case '1':// turmas por uc
                menuAno(Tipo::printTurmasPorUC);
                if (this->menu) return;
                break;
            case '2': // estudantes por ano
                menuAno(Tipo::menuEstudantesPorAno);
                if (this->menu) return;
                break;
            case '3': // estudante por uc
                menuAno(Tipo::menuEstudantesPorUc);
                if (this->menu) return;
                break;
            case '4' : // estudante por turma por uc
                menuAno(Tipo::menuAnoTurmaUc);
                if (this->menu) return;
                break;
            case '5' :
                ordernaçãoTotal(OrdenacaoTotal::estudantesNucs);
                if (this->menu) return;
                break;
            case '6':
                menuAno(Tipo::numeroDeEstudantesPorUc);
                if (this->menu) return;
                break;
            case '7':
                getStudentNumber();
                if (sair) {sair = false; return;}
                manager.printInfoEstudante(this->numero_estudante);
                break;

            case '8': {
                ordenaçãoParcial(OrdenacaoParcial::pesquisaEstudantePorNome);
                if (this->menu) return;
                break;
            }
            case 'b' :
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Escolha uma opção valida." << endl;

        }
    }
}

/*! Menu que permite ao utilizador ver os tipos de pedidos que pode realizar e consoante a sua escolha navega para o menu selecionado */
void Menu::menuOpcoesPedidos(){


    while(true){
        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                      Escolha o tipo de Pedido:                                       |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                         1 - Troca hórario UC                                         |" << endl;
        cout << "|                                         2 - Adicionar UC                                             |" << endl;
        cout << "|                                         3 - Remover UC                                               |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();

        switch (this->userInput) {
            case '1':
                trocarHorarioUc();
                if (this->menu) return;
                break;
            case '2':
                adicionarUc();
                if (this->menu) return;
                break;
            case '3':
                removerUc();
                if (this->menu) return;
                break;
            case 'b':
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Tente novamente." << endl;
        }
    }
}

/*! Mostra ao utilizador todas as opções de estatística disponível e consoante o input do utilizador mostra as estatísticas pretendidas */
void Menu::menuEstatistica() {
    while(true){
        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                         Menu Estatística:                                            |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                    1 - N UC's com mais alunos                                        |"<< endl;
        cout << "|                                    2 - N UC's com menos alunos                                       |"<< endl;
        cout << "|                                    3 - N Turmas com mais alunos                                      |"<< endl;
        cout << "|                                    4 - N Turmas com menos alunos                                     |"<< endl;
        cout << "|                                    5 - Número de alunos por UC                                       |"<< endl;
        cout << "|                                    6 - Número de alunos por ano                                      |"<< endl;
        cout << "|                                    7 - Número de estudantes por número de UC's                       |"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------"<< endl;

        getUserInput();

        switch (this->userInput) {
            case '1':
                getNuc();
                if (sair) {
                    sair = false;
                    return;
                }
                manager.printSets(this->nU,"");
                break;
            case '2':
                getNuc();
                if (sair) {
                    sair = false;
                    return;
                }
                manager.printSets(this->nU,"", false);
                break;
            case '3':
                menuAno(Tipo::turmasEstatisticaMais);
                if (menu) return;
                break;
            case '4' :
                menuAno(Tipo::turmasEstatisticaMenos);
                if (menu) return;
                break;
            case '5' :
                manager.printNumeroEstudantesDeTodasUc();
                break;
            case'6':
                escolhaOrdenado(true);
                if (this->menu) return;
                break;
            case '7':
                escolhaOrdenado(false);
                if (this->menu) return;
                break;
            case 'b' :
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Escolha uma opção valida." << endl;

        }
    }
}

/*! Mostra ao utilizador as turmas do ano escolhido do meu anterior e consoante a escolha do utilizador mostra o horário da turma selecionada */
void Menu::menuTurmaPorAno(const int &ano) {
    map<string,TurmaInfo> turmas = manager.getTurmasPorAno(ano);
    while (true) {
        int i = 1;

        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                           Escolha a Turma:                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        for (auto turma: turmas) {
            int leni = to_string(i).length();
            int len = (102 - 3 - turma.first.length() - leni) / 2;
            int lenf = (102 - 3 - turma.first.length() - leni) % 2 == 0 ? len : len + 1;
            cout << "|" << string(lenf, ' ') << i << " - " << turma.first << string(len, ' ') << "|" <<  endl;
            i++;
        }
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |" << endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        string line;
        cout << "Escolha uma opção: ";
        cin >> line;

        cin.clear();
        cin.ignore();

        if (line == "b") return;
        if (line == "m") {this->menu = true; return;}
        if (line == "q") exit(0);
        try {
            int choice = stoi(line);
            if (choice > 0 && choice <= turmas.size()) {

                auto it = turmas.begin();
                for (int i = 1; i < choice; i++) {
                    it++;
                }

                if (it != turmas.end()) {
                    manager.inputToHorario('T', "", it->first,0);
                    break;
                }
            }
            else cout << "Opção Inválida, tente novamente" << endl;
        }
        catch (exception e) {
            cin.ignore();
            cin.clear();
            cout << "Opção inválida, tente novamente" << endl;
        }
    }
}

/*! Mostra ao utilizador as turmas correspondentes a UC que o utilizador selecionou no menu anterior e em seguida mostra o menu onde o utilizador pode escolher como a informação é ordenada */
void Menu::menuListagemTurmasPorUc(const string& uc) {
    set<string> turmas = manager.getTurmasPorUc(uc);

    while (true) {
        int i = 1;
        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                           Escolha a Turma:                                           |" << endl;
        cout << "|                                                                                                      |" << endl;
        for (auto turma: turmas) {
            int leni = to_string(i).length();
            int len = (102 - 3 - turma.length() - leni) / 2;
            int lenf = (102 - 3 - turma.length() - leni) % 2 == 0 ? len : len + 1;
            cout << "|" << string(lenf, ' ') << i << " - " << turma << string(len, ' ') << "|" <<  endl;
            i++;
        }
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |" << endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        string line;
        cout << "Escolha uma opção: ";
        cin >> line;

        cin.clear();
        cin.ignore();

        if (line == "b") return;
        if (line == "m") {this->menu = true; return;}
        if (line == "q") exit(0);
        try {
            int choice = stoi(line);
            if (choice > 0 && choice <= turmas.size()) {

                auto it = turmas.begin();
                for (int i = 1; i < choice; i++) {
                    it++;
                }

                if (it != turmas.end()) {
                    ordernaçãoTotal(OrdenacaoTotal::estudantesPorTurmaNaUc, 1, uc, *it);
                    if (this->menu) return;
                    break;
                }
            }
            else cout << "Opção Inválida, tente novamente" << endl;
        }
        catch (exception e) {
            cin.ignore();
            cin.clear();
            cout << "Opção Inválida, tente novamente" << endl;
        }
    }
}

/*! Menu que mostra ao utilizador as Uc's correspondentes ao ano selecionado no menu anterior e em seguida mostra o horário da UC selecionada */
void Menu::menuListagemUc(const int &ano, ListagemUc listagem) {
    vector<string> ucs = manager.getUcPorAno(ano);
    while (true) {
        int i = 1;
        cout << "________________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                     Escolha a Unidade Curricular:                                    |" << endl;
        cout << "|                                                                                                      |" << endl;
        for (auto uc: ucs) {
            cout << "|                                               " << i << " - " << manager.ucToString(uc) << string( 51 - manager.ucToString(uc).length(), ' ') << "|" <<  endl;
            i++;
        }
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |" << endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        string line;
        cout << "Escolha uma opção: ";
        cin >> line;

        cin.clear();
        cin.ignore();

        if (line == "b") return;
        if (line == "m") {this->menu = true;  return;}
        if (line == "q") exit(0);
        try {
            int choice = stoi(line);
            if (choice > 0 && choice <= ucs.size()) {

                auto it = ucs.begin();
                for (int i = 1; i < choice; i++) {
                    it++;
                }

                if (it != ucs.end()) {
                    switch (listagem) {
                        case ListagemUc::turma: {
                            this->menuListagemTurmasPorUc(*it);
                            if (this->menu) return;
                            break;
                        }
                        case ListagemUc::uc: {
                            ordernaçãoTotal(OrdenacaoTotal::estudantesPorUc, 1, *it);
                            if (this->menu) return;
                            break;
                        }
                        case ListagemUc::numeroUc: {
                            ordernaçãoTotal(OrdenacaoTotal::numeroEstuntesPorTurmaPorUc,1, *it);
                            if (this->menu) return;
                            break;
                        }
                        case ListagemUc::printTurmas: {
                            ordenaçãoParcial(OrdenacaoParcial::printTurmasPorUc, *it);
                            if (this->menu) return;
                            break;
                        }
                        case ListagemUc::horário: {
                            manager.inputToHorario('U',*it,"",0);
                            if (this->menu) return;
                            break;
                        }
                        case ListagemUc::nTurmasMais: {
                            getNTurma(*it);
                            if (sair) {
                                sair = false;
                                return;
                            }
                            manager.printSets(this->turma, *it, true);
                            break;
                        }
                        case ListagemUc::nTurmasMenos: {
                            getNTurma(*it);
                            if (sair) {
                                sair = false;
                                return;
                            }
                            manager.printSets(this->turma, *it, false);
                            break;
                            }
                    }
                    break;
                }
            }
            else cout << "Opção inválida, tente novamente" << endl;
        }
        catch (exception e) {
            cin.ignore();
            cin.clear();
            cout << "Opção inválida, tente novamente" << endl;
        }
    }
}

/*!  Menu que permite ao utilizador escolher o ano pretendido */
void Menu::menuAno(Tipo tipo) {
    while(true){
        cout << "________________________________________________________________________________________________________"<< endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                            Escolha o Ano:                                            |"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|                                              1 - 1º Ano                                              |"<< endl;
        cout << "|                                              2 - 2º Ano                                              |"<< endl;
        cout << "|                                              3 - 3º Ano                                              |"<< endl;
        cout << "|                                                                                                      |"<< endl;
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------"<< endl;

        switch (tipo) {
            case Tipo::menuAnoTurma: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuTurmaPorAno(1);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuTurmaPorAno(2);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuTurmaPorAno(3);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;


                }
                break;
            }
            case Tipo::menuAnoUc: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuListagemUc(1,ListagemUc::horário);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuListagemUc(2,ListagemUc::horário);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuListagemUc(3,ListagemUc::horário);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
            case Tipo::menuAnoTurmaUc: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuListagemUc(1, ListagemUc::turma);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuListagemUc(2, ListagemUc::turma);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuListagemUc(3,ListagemUc::turma);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
            case Tipo::menuEstudantesPorAno: {
                getUserInput();
                switch (userInput){
                    case '1':
                        ordernaçãoTotal(OrdenacaoTotal::estudantesPorAno, 1);
                        if (this->menu) return;
                        break;
                    case'2':
                        ordernaçãoTotal(OrdenacaoTotal::estudantesPorAno,2);
                        if (this->menu) return;
                        break;
                    case '3':
                        ordernaçãoTotal(OrdenacaoTotal::estudantesPorAno, 3);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
            case Tipo::menuEstudantesPorUc: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuListagemUc(1, ListagemUc::uc);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuListagemUc(2, ListagemUc::uc);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuListagemUc(3, ListagemUc::uc);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
            case Tipo::numeroDeEstudantesPorUc: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuListagemUc(1,ListagemUc::numeroUc);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuListagemUc(2,ListagemUc::numeroUc);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuListagemUc(3,ListagemUc::numeroUc);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
            }
            case Tipo::printTurmasPorUC: {
                getUserInput();
                switch (userInput){
                    case '1':
                        menuListagemUc(1,ListagemUc::printTurmas);
                        if (this->menu) return;
                        break;
                    case'2':
                        menuListagemUc(2,ListagemUc::printTurmas);
                        if (this->menu) return;
                        break;
                    case '3':
                        menuListagemUc(3,ListagemUc::printTurmas);
                        if (this->menu) return;
                        break;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'b' :
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida." << endl;

                }
                break;
                case Tipo::turmasEstatisticaMais: {
                    getUserInput();
                    switch (userInput){
                        case '1':
                            menuListagemUc(1, ListagemUc::nTurmasMais);
                            if (this->menu) return;
                            break;
                        case'2':
                            menuListagemUc(2, ListagemUc::nTurmasMais);
                            if (this->menu) return;
                            break;
                        case '3':
                            menuListagemUc(3, ListagemUc::nTurmasMais);
                            if (this->menu) return;
                            break;
                        case 'm':
                            this->menu = true;
                            return;
                        case 'b' :
                            return;
                        case 'q':
                            exit(0);
                        default:
                            cout << "Opção inválida. Escolha uma opção valida." << endl;

                    }
                    break;
                }
                case Tipo::turmasEstatisticaMenos: {
                    getUserInput();
                    switch (userInput){
                        case '1':
                            menuListagemUc(1, ListagemUc::nTurmasMenos);
                            if (this->menu) return;
                            break;
                        case'2':
                            menuListagemUc(2, ListagemUc::nTurmasMenos);
                            if (this->menu) return;
                            break;
                        case '3':
                            menuListagemUc(3, ListagemUc::nTurmasMenos);
                            if (this->menu) return;
                            break;
                        case 'm':
                            this->menu = true;
                            return;
                        case 'b' :
                            return;
                        case 'q':
                            exit(0);
                        default:
                            cout << "Opção inválida. Escolha uma opção valida." << endl;

                    }
                    break;
                }
            }
        }

    }
}

/*! Menu que permite ao utilizador escolher se quer o número de alunos por ano e o número de estudantes por número de UC's ordenado ou não ordenado */
void Menu::escolhaOrdenado(const bool& first) {
    while(true){

        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                    Escolha a opção de ordenação:                                     |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                       1 - Não ordenado                                               |" << endl;
        cout << "|                                       2 - Ordenado                                                   |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();
        switch (first) {
            case true: {
                switch (userInput) {
                    case '1': {
                        manager.printVectors('A');
                        break;
                    }
                    case '2': {
                        ordenaçãoParcial(OrdenacaoParcial::estatisticaAno);
                        if (this->menu) return;
                        break;
                    }
                    case 'm':
                        this->menu = true;;
                        return;
                    case 'b':
                        return;
                    case 'q' :
                        exit(0);
                    default:
                        cout << "Opção invaldia. Escolha uma opção valida" << endl;
                }
                break;
            }
            case false: {
                switch (userInput) {
                    case '1': {
                        manager.printVectors();
                        break;
                    }
                    case '2': {
                        ordenaçãoParcial(OrdenacaoParcial::estisticaInscricoes);
                        if (this->menu) return;
                        break;
                    }
                    case 'm':
                        this->menu = true;;
                        return;
                    case 'b':
                        return;
                    case 'q' :
                        exit(0);
                    default:
                        cout << "Opção invaldia. Escolha uma opção valida" << endl;
                }
                break;
            }

        }

    }
}

/*! Mostra ao utilizador um Menu com as opções das turmas que o utilizador pode escolher */
void Menu::menuOpcoesTurmas(map<string,list<Aula>> turmas) {
    int i = 1;
    int inicio = 0, fim = 0;
    cout << "--------------------------------" << endl;
    for (auto turma : turmas) {
        int total = 30 - (to_string(i).length() + 3 + turma.first.length());
        if (total % 2 != 0) {
            inicio = (total / 2) + 1;
            fim = total / 2;

        }
        else {
            inicio = total / 2;
            fim = total / 2;
        }
        cout <<"|" << string(inicio, ' ') << i << " - " << turma.first << string(fim, ' ') << "|" << endl;
        i++;
    }
    cout << "--------------------------------" << endl;
}

/*! Mostra ao utilizador um Menu com as opções de horário que pode ver */
void Menu::verificarHorario(){
    while(true){
        cout << "____________________________________________________________________________________________________" << endl;
        cout << "|                                                                                                  |" << endl;
        cout << "|                                        Escolha a Opção:                                          |" << endl;
        cout << "|                                                                                                  |" << endl;
        cout << "|                                       1 - Horário Estudante                                      |" << endl;
        cout << "|                                       2 - Horário UC                                             |" << endl;
        cout << "|                                       3 - Horário Turma                                          |" << endl;
        cout << "|                                                                                                  |" << endl;
        cout << "|    b - anterior                                                                                  |" << endl;
        cout << "|    q - sair                                                                                      |" << endl;
        cout << "----------------------------------------------------------------------------------------------------" << endl;
        cout << endl;

        getUserInput();

        switch (this->userInput) {
            case '1':
                horarioEstudante();
                if (this->menu) return;
                break;
            case '2':
                menuAno(Tipo::menuAnoUc);
                if (this->menu) return;
                break;
            case '3':
                menuAno(Tipo::menuAnoTurma);
                if (this->menu) return;
                break;
            case 'b' :
                return;
            case 'q':
                exit(0);
            default:
                cout << "Opção inválida. Escolha uma opção valida." << endl;

        }
    }

}

// Ordenações
/*! Menu onde permite ao utilizador escolher como a informação é ordenada (número e ordem crescente, numero e ordem decrescente, nome e ordem crescente, nome e ordem decrescente) */
void Menu::ordernaçãoTotal(OrdenacaoTotal ord, int ano, string uc, string turma) {
    while (true) {
        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                    Escolha a opção de ordenação:                                     |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                 1 - Ordenar por nº e ordem crescente                                 |" << endl;
        cout << "|                                 2 - Ordenar por nº e ordem decrescente                               |" << endl;
        cout << "|                                 3 - Ordenar por nome e ordem crescente                               |" << endl;
        cout << "|                                 4 - Ordenar por nome e ordem decrescente                             |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;


        switch (ord) {
            case OrdenacaoTotal::estudantesPorTurmaNaUc: {
                getUserInput();
                switch (this->userInput) {
                    case '1':
                        manager.printEstudantesPorTurmaNaUc(uc, turma, true, true);
                        break;
                    case '2':
                        manager.printEstudantesPorTurmaNaUc(uc, turma, true, false);
                        break;
                    case '3':
                        manager.printEstudantesPorTurmaNaUc(uc, turma, false, true);
                        break;
                    case '4':
                        manager.printEstudantesPorTurmaNaUc(uc, turma, false, false);
                        break;
                    case 'b':
                        return;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção inválida. Escolha uma opção valida" << endl;
                }
                break;
            }
            case OrdenacaoTotal::estudantesPorAno: {
                getUserInput();
                switch (userInput) {
                    case '1': {
                        manager.printEstudantesPorAno(ano, true, true);
                        break;
                    }
                    case '2': {
                        manager.printEstudantesPorAno(ano, true, false);
                        break;
                    }
                    case '3': {
                        manager.printEstudantesPorAno(ano, false, true);
                        break;
                    }
                    case '4': {
                        manager.printEstudantesPorAno(ano, false, false);
                        break;
                    }
                    case 'b': {
                        return;
                    }
                    case 'm': {
                        this->menu = true;
                        return;
                    }
                    case 'q': {
                        exit(0);
                    }
                }
                break;
            }
            case OrdenacaoTotal::estudantesNucs: {
                getUserInput();
                switch (this->userInput) {
                    case '1':
                        getNuc();
                        if (sair) {sair = false; return;}
                        manager.numeroEstudantesEmPeloMenosNUCS(this->nU, true, true);
                        break;
                    case '2':
                        getNuc();
                        if (sair) {sair = false; return;}
                        manager.numeroEstudantesEmPeloMenosNUCS(this->nU, true, false);
                        break;
                    case '3':
                        getNuc();
                        if (sair) {sair = false; return;}
                        manager.numeroEstudantesEmPeloMenosNUCS(this->nU, false, true);
                        break;
                    case '4':
                        getNuc();
                        if (sair) {sair = false; return;}
                        manager.numeroEstudantesEmPeloMenosNUCS(this->nU, false, false);
                        break;
                    case 'b':
                        return;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'q':
                        exit(0);
                    default:
                        cout << "Opção invalida. Escolha uma opção valida" << endl;
                }
                break;
            }
            case OrdenacaoTotal::numeroEstuntesPorTurmaPorUc: {
                getUserInput();
                switch (this->userInput) {
                    case '1':
                        manager.printNumeroEstudantesPorTurmaPorUc(uc, false, true);
                        break;
                    case '2':
                        manager.printNumeroEstudantesPorTurmaPorUc(uc, false, false);
                        break;
                    case '3':
                        manager.printNumeroEstudantesPorTurmaPorUc(uc, true, true);
                        break;
                    case '4':
                        manager.printNumeroEstudantesPorTurmaPorUc(uc, true, false);
                        break;
                    case 'b' :
                        return;
                    case 'm':
                        this->menu = true;
                        return;
                    case 'q' :
                        exit(0);
                    default:
                        cout << "Opção invalida. Escolha uma opção valida"<<endl;
                }
                break;
            }
            case OrdenacaoTotal::estudantesPorUc: {
                getUserInput();
                switch (this->userInput) {
                    case '1':
                        manager.printEstudantesPorUC(uc, true, true);
                        break;
                    case '2':
                        manager.printEstudantesPorUC(uc, true, false);
                        break;
                    case '3':
                        manager.printEstudantesPorUC(uc, false, true);
                        break;
                    case '4':
                        manager.printEstudantesPorUC(uc, false, false);
                        break;
                    case 'b' :
                        return;
                    case 'm':
                        this->menu = true;;
                        return;
                    case 'q' :
                        exit(0);
                    default:
                        cout << "Opção invaldia. Escolha uma opção valida" << endl;
                }
                break;
            }

        }

    }

}

/*! Menu onde permite ao utilizador escolher como a informação é ordenada (ordem crescente e ordem decrescente) */
void Menu::ordenaçãoParcial(OrdenacaoParcial ord, string uc) {
    while(true){

        cout << "________________________________________________________________________________________________________" <<endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                    Escolha a opção de ordenação:                                     |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|                                       1 - Ordem crescente                                            |" << endl;
        cout << "|                                       2 - Ordem decrescente                                          |" << endl;
        cout << "|                                                                                                      |" << endl;
        cout << "|  m - menu                                                                                            |" << endl;
        cout << "|  b - anterior                                                                                        |"<< endl;
        cout << "|  q - sair                                                                                            |" << endl;
        cout << "--------------------------------------------------------------------------------------------------------" << endl;

        getUserInput();
        switch (ord) {
            case OrdenacaoParcial::pesquisaEstudantePorNome: {
                switch (userInput) {
                    case '1':
                    {
                        getStudentName();
                        if (sair) return;
                        manager.printEstudantesPorNome(nome, true);
                        break;
                    }
                    case '2':
                    {
                        getStudentName();
                        if (sair) return;
                        manager.printEstudantesPorNome(nome, false);
                        break;
                    }
                    case 'b': // back
                        return;
                    case 'q':
                        exit(0);
                    case 'm':
                        this->menu = true;;
                        return;
                    default:
                        cout << "Opção invalida. Escolha uma opção valida."<<endl;
                }
                break;
            }
            case OrdenacaoParcial::printTurmasPorUc: {
                switch (userInput) {
                    case '1':
                        manager.printTurmasPorUC(uc, true);
                        break;
                    case '2':
                        manager.printTurmasPorUC(uc, false);
                        break;
                    case 'b':
                        return;
                    case 'q':
                        exit(0);
                    case 'm':
                        this->menu = true;;
                        return;
                    default:
                        cout << "Opção invalida. Escolha uma opção valida."<<endl;
                }
                break;
            }
            case OrdenacaoParcial::estatisticaAno: {
                switch (userInput) {
                    case '1':
                        manager.printVectors('A', true);
                        break;
                    case '2':
                        manager.printVectors('A', true, false);
                        break;
                    case 'b': // back
                        return;
                    case 'q':
                        exit(0);
                    case 'm':
                        this->menu = true;;
                        return;
                    default:
                        cout << "Opção invalida. Escolha uma opção valida."<<endl;
                }
                break;
            }
            case OrdenacaoParcial::estisticaInscricoes: {
                switch (userInput) {
                    case '1':
                        manager.printVectors(' ', true);
                        break;
                    case '2':
                        manager.printVectors(' ', true, false);
                        break;
                    case 'b': // back
                        return;
                    case 'q':
                        exit(0);
                    case 'm':
                        this->menu = true;;
                        return;
                    default:
                        cout << "Opção invalida. Escolha uma opção valida."<<endl;
                }
                break;
            }

        }

    }
}

// Execuções
/*! Mostra o horário do estudante consoante o número de estudante que o utilizador inseriu */
void Menu::horarioEstudante() {
    getStudentNumber();
    if (sair) {sair = false; return;}
    cout << endl << "O horário do Estudante " << this->numero_estudante << " é o seguinte:" << endl;
    manager.inputToHorario('E', "", "", this->numero_estudante);
}

/*! Quando o utilizador volta ao menu principal permite atualizar a hora para a hora atual */
void Menu::updateTime() {
    while (!exitTimeThread) {
        // Get the current time
        data = time(0);
        hora = localtime(&data);

        this_thread::sleep_for(chrono::seconds(1));
    }
}

/*! Permite atualizar a contagem dos pedidos no menu principal */
void Menu::updatePedidos() {
    while (!exitPedidoThread) {
        pedidosAtivo = manager.getPedidos();
    }
}

/*! Permite ao utilizador trocar de turma numa mesma UC com outro estudante */
void Menu::trocarHorarioUc() {
    // Pede ao utilizador uma UC e 2 estudantes e verifica se tanto a uc e os 2 estudantes sao validos, caso nao sejam volta a pedir. tambem permite ao utilizador inserir 'sair' de modo a voltar ao menu anterior
    getUC();
    if (sair) {sair = false; return;}
    getStudentNumber();
    if (sair) {sair = false; return;}
    getSecondStudent();
    if (sair) {sair = false; return;}
    while (!manager.inputToPedido(this->uc, this->numero_estudante,"H",this->estudante2)){
        getUC();
        if (sair) {sair = false; return;}
        getStudentNumber();
        if (sair) {sair = false; return;}
        getSecondStudent();
        if (sair) {sair = false; return;}
    }
}

/*! Permite ao utilizador adicionar uma UC */
void Menu::adicionarUc() {
    getUC();
    if (sair) {sair = false; return;}
    getStudentNumber();
    if (sair) {sair = false; return;}
    if (manager.validarNovaUc(this->uc, this->numero_estudante)) {
        bool confirmou = false;
        map<string,list<Aula>> turmas = manager.enviaListaDeAulaPossivel(this->uc, this->numero_estudante);
        if (!turmas.empty()) {
            while (!confirmou) {
                // Mostra ao utilizador um menu com as turmas da UC para que este possa escolher a turma pretendida
                menuOpcoesTurmas(turmas);
                while (true) {
                    cout << "Escolha uma turma (escreva 'sair' para voltar atrás): ";
                    string line;
                    cin >> line;
                    int idx;
                    if (line == "sair") return;
                    try {
                        idx = stoi(line);
                        if (idx > 0 && idx <= turmas.size()) {
                            auto it = turmas.begin();

                            for (int i = 1; i < idx; i++) {
                                it++;
                            }
                            // Pergunta ao utilizador se pretende ver como vai ficar o seu horário antes de confirmar as alterações
                            while (true) {
                                cout << "Pretende ver como irá ficar o seu horário antes de confirmar? (s ou n): ";
                                cin >> line;
                                if (line == "n") {
                                    confirmou = true;
                                    string t = it->first;
                                    manager.inputToPedido(this->uc, this->numero_estudante, "A", 0, t);
                                    return;
                                }
                                if (line == "s") {
                                    manager.verHorarioAntesDeConfirmar(this->numero_estudante, this->uc, it->first, it->second);
                                    while (true) {
                                        cout << "Confirma as alterações? (s ou n): ";
                                        cin >> line;
                                        if (line == "n") {
                                            break;
                                        }
                                        if (line == "s") {
                                            confirmou = true;
                                            string t = it->first;
                                            manager.inputToPedido(this->uc, this->numero_estudante, "A", 0, t);
                                            return;
                                        }
                                    }
                                    if (!confirmou) {
                                        break;
                                    }
                                }
                                else {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Valor inválido, escolha entre 's' ou 'n'" << endl;
                                }
                            }
                            if (!confirmou) {
                                break;
                            }
                        }
                        else {
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Indique um número dentro de 1 até " << turmas.size()  << endl;
                        }
                    }
                    catch (exception e) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Indique um número dentro de 1 até " << turmas.size()  << endl;
                    }
                }
            }
        }
        else {
            cout << "Não é possível adicionar um novo aluno à UC" << endl;
        }
    }
}

/*! Permite ao utilizador remover uma UC do seu horário */
void Menu::removerUc() {
    // Pede ao utilizadar um estudante e a UC que pretende remover
    getStudentNumber();
    if (sair) {sair = false; return;}
    set<pair<string,string>> ucs = manager.enviaUCParaRemover(this->numero_estudante);

    if (!ucs.empty()) {
        while (true) {
            int i = 1;
            cout << "________________________________________________________________________________________________________" << endl;
            cout << "|                                                                                                      |" << endl;
            cout << "|                                   Escolha a UC que pretende remover:                                 |" << endl;
            cout << "|                                                                                                      |" << endl;
            for (auto uc: ucs) {
                int leni = to_string(i).length();
                int len = (102 - 3 - uc.first.length() - leni) / 2;
                int lenf = (102 - 3 - uc.first.length() - leni) % 2 == 0 ? len : len + 1;
                cout << "|" << string(lenf, ' ') << i << " - " << uc.first << string(len, ' ') << "|" <<  endl;
                i++;
            }
            cout<< "|  m - menu                                                                                            |"<< endl;
            cout<< "|  b - anterior                                                                                        |"<< endl;
            cout<< "|  q - sair                                                                                            |"<< endl;
            cout<< "--------------------------------------------------------------------------------------------------------"<< endl;

            string line;
            cout << "Escolha uma opção: ";
            cin >> line;

            cin.clear();
            cin.ignore();
            if (line == "b") return;
            if (line == "m") {this->menu = true; return;}
            if (line == "q") exit(0);
            try {
                int choice = stoi(line);
                if (choice > 0 && choice <= ucs.size()) {

                    auto it = ucs.begin();
                    for (int i = 1; i < choice; i++) {
                        it++;
                    }

                    if (it != ucs.end()) {
                        manager.inputToPedido(it->first, this->numero_estudante, "R");
                    }
                }
            }
            catch (exception e) {
                cin.ignore();
                cin.clear();
                cout << "Opção inválida, tente novamente" << endl;
            }
        }
    }
    else {
        cout << "O aluno não está inscrito a nenhuma UC" << endl;
    }
}