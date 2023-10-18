//
// Created by paulo on 05-10-2023.
//

#ifndef AED2324_PRJ1_G_109__CLASSDATEDURATION_H
#define AED2324_PRJ1_G_109__CLASSDATEDURATION_H
#include <string>

class Slot {
private:
    std::string weekday;
    float startHour;
    float duration;

public:
    Slot(const std::string weekday, const float startHour, const float duration);
    const bool classDateOverlap(const Slot classDateDuration);
    const float getEndHour() const;
    const std::string getWeekday();
};


#endif //AED2324_PRJ1_G_109__CLASSDATEDURATION_H
