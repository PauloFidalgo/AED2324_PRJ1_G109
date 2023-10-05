//
// Created by Paulo Fidalgo on 05/10/2023.
//

#ifndef AED2324_PRJ1_G_109__COURSEUNIT_H
#define AED2324_PRJ1_G_109__COURSEUNIT_H
#include <string>
#include <iostream>
#include "ClassDateDuration.h"

enum Type {TP, P, PL};

class CourseUnit {
private:
    std::string ucCode;
    ClassDateDuration classDateDuration;
    std::string type;

public:
    CourseUnit(const std::string ucCode, const std::string weekday, const float startTime, const float duration, std::string type);
};




#endif //AED2324_PRJ1_G_109__COURSEUNIT_H
