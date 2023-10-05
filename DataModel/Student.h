//
// Created by nelson on 05-10-2023.
//

#ifndef AED2324_PRJ1_G_109__STUDENT_H
#define AED2324_PRJ1_G_109__STUDENT_H
#include <string>
#include "Schedule.h"

class Student {
private:
    int studentNumber;
    std::string studentName;
    Schedule schedule;

public:
    Student(const int studentNumber, const std::string studentName, const Schedule schedule);
    const int getStudentNumber();
    const std::string getStudentName();
    const Schedule getSchedule();
};


#endif //AED2324_PRJ1_G_109__STUDENT_H
