#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <iostream>
using namespace std;

map<string, vector<string>> openClassPerUc(string fileName) {
    cout << "estou dentro" << endl;
    ifstream iff(fileName);

    if (!iff.is_open()) {
        cout << "não consegui abrir" << endl;
    }

    map<string, vector<string>> classPerUC;

    string line;
    getline(iff, line);

    if (line != "UcCode,ClassCode") {
    }


    while (getline(iff, line)) {
        size_t firstComma = line.find_first_of(',');
        
        if (firstComma != string::npos) {

            string ucCode = line.substr(0, firstComma);
            string classCode = line.substr(firstComma + 1);
            cout << ucCode << " " << classCode << " ";

            classPerUC[ucCode].push_back(classCode);
        }
    }

    iff.close();
    return classPerUC;
}
