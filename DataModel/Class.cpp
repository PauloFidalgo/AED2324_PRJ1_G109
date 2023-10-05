//
// Created by nelson on 05-10-2023.
//

#include "Class.h"

Class::Class(const std::string classCode, const Schedule schedule) : classCode(classCode), schedule(schedule) {}

const std::string Class::getClassCode() {
    return classCode;
}
const Schedule Class::getSchedule(){
    return schedule;
}
