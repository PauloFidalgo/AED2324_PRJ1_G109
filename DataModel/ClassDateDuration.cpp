//
// Created by paulo on 05-10-2023.
//

#include "ClassDateDuration.h"

ClassDateDuration::ClassDateDuration(const std::string weekday, const float startHour, const float duration) : weekday(weekday), startHour(startHour), duration(duration) {}

const float ClassDateDuration::getEndHour() const {return startHour + duration;}

const bool ClassDateDuration::classDateOverlap(const ClassDateDuration classDateDuration) {
    return ((this->getEndHour() <= classDateDuration.startHour) || (startHour >= classDateDuration.getEndHour())) ? false : true;
}
