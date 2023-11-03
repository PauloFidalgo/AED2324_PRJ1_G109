//
// Created by Paulo Fidalgo on 05/10/2023.
//

#ifndef AED2324_PRJ1_G_109__COURSEUNIT_H
#define AED2324_PRJ1_G_109__COURSEUNIT_H
#include <string>
#include <iostream>

class Aula {
private:
    std::string dia;
    float inicio, duracao;
    std::string tipo;

public:
    // Constructors
    Aula();
    Aula(std::string dia, float inicio, float duracao, std::string tipo);

    // Getters
    const std::string getTipo() const;
    const std::string getDia() const;
    const float getDuracao() const;
    const float getInicio() const;

    // Validators
    bool sobreposta(const Aula &aula) const;
    bool mesmoHorario(const Aula &aula) const;
};




#endif //AED2324_PRJ1_G_109__COURSEUNIT_H
