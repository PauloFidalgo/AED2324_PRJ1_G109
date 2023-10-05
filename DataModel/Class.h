//
// Created by Paulo Fidalgo on 05/10/2023.
//

#ifndef AED2324_PRJ1_G_109__CLASS_H
#define AED2324_PRJ1_G_109__CLASS_H
#include <string>
#include <iostream>

enum Type {TP, P, PL};

class Class {
private:
    std::string ucCode;
    std::string weekday;
    float startTime;
    float duration;
    std::string type;

public:
    Class(std::string ucCode, std::string weekday, float startTime, float duration, std::string type);
};




#endif //AED2324_PRJ1_G_109__CLASS_H
