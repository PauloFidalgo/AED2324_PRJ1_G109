//
// Created by Paulo Fidalgo on 05/10/2023.
//

#include "Class.h"

Class::Class(std::string ucCode, std::string weekday, float startTime, float duration, std::string type) {
        this->ucCode = ucCode;
        this->weekday = weekday;
        this->startTime = startTime;
        this->duration = duration;
        this->type = type;
}

