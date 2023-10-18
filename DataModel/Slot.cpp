//
// Created by paulo on 05-10-2023.
//

#include "Slot.h"

Slot::Slot(const std::string weekday, const float startHour, const float duration) : weekday(weekday), startHour(startHour), duration(duration) {}

const float Slot::getEndHour() const {return startHour + duration;}

const bool Slot::classDateOverlap(const Slot classDateDuration) {
    return ((this->getEndHour() <= classDateDuration.startHour) || (startHour >= classDateDuration.getEndHour())) ? false : true;
}
