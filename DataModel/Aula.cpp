//
// Created by Paulo Fidalgo on 05/10/2023.
//

#include "Aula.h"


Aula::Aula(std::string dia, float inicio, float duracao, std::string tipo) : dia(dia), inicio(inicio), duracao(duracao), tipo(tipo) {}
Aula::Aula() : dia(""), inicio(0), duracao(0), tipo(""){}

const void Aula::printData() {
    std::cout << "Dia: " << dia << " Hora: " << inicio << " Até: " << inicio + duracao << " Tipo: " << tipo << std::endl;
}