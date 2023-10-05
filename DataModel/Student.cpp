//
// Created by nelson on 05-10-2023.
//

#include "Student.h"

Student::Student(const int studentNumber, const std::string studentName, const Schedule schedule) : studentNumber(studentNumber), studentName(studentName), schedule(schedule) {}

int const Student::getStudentNumber(){
    return studentNumber;
}

std::string const Student::getStudentName(){
    return studentName;
}

Schedule const Student::getSchedule(){
    return schedule;
}