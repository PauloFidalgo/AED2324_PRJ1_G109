//
// Created by nelson on 05-10-2023.
//

#ifndef AED2324_PRJ1_G_109__SCHEDULE_H
#define AED2324_PRJ1_G_109__SCHEDULE_H
#include <vector>
#include "CourseUnit.h"

class Schedule {
private:
    std::vector<CourseUnit> classes;

public:
    Schedule(const std::vector<CourseUnit> classes);
    const std::vector<CourseUnit> getClasses();

};


#endif //AED2324_PRJ1_G_109__SCHEDULE_H
