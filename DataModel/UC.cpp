//
// Created by Paulo Fidalgo on 18/10/2023.
//

#include "UC.h"

#include <utility>
/*! Construtor por cópia */
UC::UC(const UC &other) {
    this->codigoUC = other.codigoUC;
    this->ucTurma = other.ucTurma;
    this->media = other.media;
    this->ano = other.ano;
    this->nAlunos = other.nAlunos;
}

/*! Construtor com todos os parâmetros */
UC::UC(const std::string &codigoUc, const std::map<std::string, TurmaInfo> &ucTurma, int media) {
    this->codigoUC = codigoUc;
    this->ucTurma = ucTurma;
    this->media = media;
    this->nAlunos = 0;

    auto it = ucTurma.begin();
    if (it != ucTurma.end()) {
        this->ano = std::stoi(it->first.substr(0,1));
    }
}

/*! Construtor com código, usado para procurar UC's pelo código */
UC::UC(const std::string codigoUc) : codigoUC(codigoUc) {}

/*! Retorna o map ucTurma */
std::map<std::string, TurmaInfo> UC::getUcTurma() const {
    return this->ucTurma;
}

/*! Retorna o valor que divide os dois turnos das teóricas */
float UC::getMedia() const {
    return this->media;
}

/*!Retorna o ano da UC */
int UC::getAno() const {
    return this->ano;
}

/*!Retorna o número de alunos total da UC
 * O(1)
 */
int UC::getNumeroAlunosTotal() const {
    return this->nAlunos;
}

/*!Retorna a aula prática da turma
 * O(log(n))
 */
Aula UC::getPratica(const std::string &turma) const {
    Aula res;
    auto it = this->ucTurma.find(turma);
    if (it != this->ucTurma.end()) {
        res = it->second.pratica;
    }
    return res;
}

/*!Retorna o número de turmas da UC */
int UC::getNumeroTurmas() const {
    return ucTurma.size();
}

/*!Retorna um vetor formado por pares, em que o primeiro elemento é o nome da UC e o segundo é um par com o nome da turma e uma aula dessa turma
 * O(n * m)
 * */
std::vector<std::pair<std::string,std::pair<std::string,Aula>>> UC::getAulasUc() const {
    std::vector<std::pair<std::string,std::pair<std::string,Aula>>> res;
    for (const auto& turma : ucTurma) {
        for (const auto& aula : turma.second.aulasTeoricas) {
            res.push_back({codigoUC, {turma.first, aula}});
        }
        res.push_back({codigoUC, {turma.first, turma.second.pratica}});
    }
    return res;
}

/*!Retorna o código da UC */
std::string UC::getCodigoUc() const {
    return this->codigoUC;
}

/*!Retorna uma lista de aulas de uma determinada turma
 * O(log(n))
 */
std::list<Aula> UC::getAulasTurma(const std::string &turma) const {
    auto it = ucTurma.find(turma);
    std::list<Aula> res;

    if (it != ucTurma.end()) {
        res = it->second.aulasTeoricas;
        res.push_back(it->second.pratica);
    }

    return res;
}

/*!Retorna o número de alunos de uma determinada turma
 * O(log(n))
 */
int UC::getNumeroAlunos(const std::string &turma) const {
    auto it = ucTurma.find(turma);
    int res = 0;

    if (it != ucTurma.end()) {
        res = it->second.estudantes.size();
    }
    return res;
}

/*!Adiciona um estudante a uma turma
 * O(log(n))
 */
void UC::addEstudante(const std::string &turma, const int &estudante, const std::string &nome) {
    auto it = ucTurma.find(turma);

    if (it != ucTurma.end()) {
        it -> second.estudantes.insert({estudante,nome});
        this->nAlunos++;
    }
}

/*!Remove um estudante de uma turma
 * O(log(n))
 */
void UC::removeEstudante(const std::string &turma, const int &numero, const std::string &nome) {
    auto it = ucTurma.find(turma); // O(log(n))

    auto estudante  = it->second.estudantes.find({numero,nome});
    if (estudante != it->second.estudantes.end()) {
        it->second.estudantes.erase(estudante);
        this->nAlunos--;
    }
}

/*!Retorna o número de alunos da turma com menos alunos
 * O(n)
 */
int UC::getMinimum() const {
    bool first = true;
    int min;

    for (auto t : ucTurma) {
        if (first) {
            min = t.second.estudantes.size();
            first = false;
        }
        else {
            if (t.second.estudantes.size() < min) {
                min = t.second.estudantes.size();
            }
        }
    }
    return min;
}

/*! Verifica se ao adicionar um aluno novo à turma, não é comprometido o equilíbrio entre turmas
 * O(n)
 */ 
bool UC::checkBalance(const std::string &turma) const {
   int min = this->getMinimum();

   auto it = ucTurma.find(turma);
   if ((it->second.estudantes.size() + 1) - min > 4) {
        return false;
   }
    return true;
}

/*! Compara UC's, pelo seu código */
bool UC::operator<(const UC uc) const {
    return this->codigoUC < uc.codigoUC;
}