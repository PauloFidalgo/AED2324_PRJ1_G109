//
// Created by nelson on 05-10-2023.
//

#include "Schedule.h"

Schedule::Schedule(const std::vector<CourseUnit> classes) { this->classes = classes;}

const std::vector<CourseUnit> Schedule::getClasses() {return this->classes;}