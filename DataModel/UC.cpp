//
// Created by Paulo Fidalgo on 18/10/2023.
//

#include "UC.h"

UC::UC(const std::string &codigoUc, const std::unordered_map<std::string, std::unordered_map<std::string, TurmaInfo>> &ucTurma) : codigoUC(codigoUc), ucTurma(ucTurma) {}
