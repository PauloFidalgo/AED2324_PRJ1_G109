//
// Created by Paulo Fidalgo on 05/10/2023.
//

#include "Aula.h"

/*! Construtor com todos os parâmetros */
Aula::Aula(std::string dia, float inicio, float duracao, std::string tipo) : dia(dia), inicio(inicio), duracao(duracao), tipo(tipo) {}

/*! Construtor vazio */
Aula::Aula() : dia(""), inicio(0), duracao(0), tipo("") {}

/*! Retorna o tipo de uma Aula */
const std::string Aula::getTipo() const {
    return this->tipo;
}

/*! Retorna o dia de uma Aula */
const std::string Aula::getDia() const {
    return this->dia;
}

/*! Retorna a hora de início de uma Aula */
const float Aula::getInicio() const {
    return this->inicio;
}

/*! Retorna a duração da Aula */
const float Aula::getDuracao() const {
    return this->duracao;
}

/*! Verifica se as aulas têm exatamente o mesmo horário e tipo */
bool Aula::mesmoHorario(const Aula &aula) const {
    return (this->dia == aula.dia && this->inicio == aula.inicio && this->duracao == aula.duracao && this->tipo == aula.tipo);
}

/*! Verifica se uma aula é sobreposta a outra */
bool Aula::sobreposta(const Aula &aula) const {
    return !(this->dia != aula.dia ||
             (((this->inicio + this->duracao) <= aula.inicio) || ((this->inicio >= aula.inicio + aula.duracao))));
}

