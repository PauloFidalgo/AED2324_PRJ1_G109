//
// Created by paulo on 05-10-2023.
//

#include "Slot.h"

Slot::Slot(const std::string weekday, const float startHour, const float duration) : weekday(weekday), startHour(startHour), duration(duration) {}

Slot::Slot() : weekday(""), startHour(0), duration(0) {}

const float Slot::getEndHour() const {return startHour + duration;}

const bool Slot::slotOverlap(const Slot slot) {
    return ((this->getEndHour() <= slot.startHour) || (startHour >= slot.getEndHour())) ? false : true;
}

