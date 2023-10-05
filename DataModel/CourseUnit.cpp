//
// Created by Paulo Fidalgo on 05/10/2023.
//

#include "CourseUnit.h"

CourseUnit::CourseUnit(const std::string ucCode, const std::string weekday, const float startTime, const float duration, const std::string type): ucCode(ucCode),
classDateDuration(ClassDateDuration(weekday,startTime,duration)), type(type) {}

