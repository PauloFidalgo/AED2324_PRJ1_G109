//
// Created by nelson on 05-10-2023.
//

#ifndef AED2324_PRJ1_G_109__CLASS_H
#define AED2324_PRJ1_G_109__CLASS_H
#include "Schedule.h"

class Class {
private:
    std::string classCode;
    Schedule schedule;
public:
    Class(const std::string classCode, const Schedule schedule);
    const std::string getClassCode();
    const Schedule getSchedule();
};


#endif //AED2324_PRJ1_G_109__CLASS_H
