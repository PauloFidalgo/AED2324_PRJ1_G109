#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <iostream>
using namespace std;

map<string, vector<string>> openClassPerUc(string fileName) {
    map<string, vector<string>> classPerUC;
    fstream iff;
    try {
        iff.open(fileName, ios::in);
        string line, word, temp, ucCode, classCode;

        while (iff >> temp) {
            stringstream s(temp);
            getline(s, ucCode, ',');
            getline(s, classCode);
            classPerUC[ucCode].push_back(classCode);
        }
    }
    catch (const ifstream::failure& e){
        cout << "Failed to open file." << endl;
    }

    iff.close();
    return classPerUC;
}

