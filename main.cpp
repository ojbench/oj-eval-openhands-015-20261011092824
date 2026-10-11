#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

const string DATA_FILE = "database.dat";

void initializeFile() {
    ifstream check(DATA_FILE);
    if (!check.good()) {
        ofstream create(DATA_FILE);
        create.close();
    }
    check.close();
}

bool exists(const string& index, int value) {
    ifstream file(DATA_FILE);
    if (!file.is_open()) return false;
    
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string fileIndex;
        int fileValue;
        
        if (iss >> fileIndex >> fileValue) {
            if (fileIndex == index && fileValue == value) {
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}

void insert(const string& index, int value) {
    if (exists(index, value)) {
        return;
    }
    
    ofstream file(DATA_FILE, ios::app);
    if (file.is_open()) {
        file << index << " " << value << "\n";
        file.close();
    }
}

void remove(const string& index, int value) {
    const string tempFile = "temp_" + DATA_FILE;
    ifstream inFile(DATA_FILE);
    ofstream outFile(tempFile);
    
    if (!inFile.is_open() || !outFile.is_open()) {
        if (inFile.is_open()) inFile.close();
        if (outFile.is_open()) outFile.close();
        return;
    }
    
    string line;
    while (getline(inFile, line)) {
        string fileIndex;
        int fileValue;
        istringstream iss(line);
        if (iss >> fileIndex >> fileValue) {
            if (!(fileIndex == index && fileValue == value)) {
                outFile << line << "\n";
            }
        }
    }
    
    inFile.close();
    outFile.close();
    
    std::remove(DATA_FILE.c_str());
    std::rename(tempFile.c_str(), DATA_FILE.c_str());
}

void findAndPrint(const string& index) {
    vector<int> values;
    ifstream file(DATA_FILE);
    
    if (!file.is_open()) {
        cout << "null\n";
        return;
    }
    
    string line;
    while (getline(file, line)) {
        string fileIndex;
        int fileValue;
        istringstream iss(line);
        if (iss >> fileIndex >> fileValue) {
            if (fileIndex == index) {
                values.push_back(fileValue);
            }
        }
    }
    file.close();
    
    if (values.empty()) {
        cout << "null\n";
        return;
    }
    
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    
    for (size_t i = 0; i < values.size(); i++) {
        if (i > 0) cout << " ";
        cout << values[i];
    }
    cout << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    initializeFile();
    
    int n;
    cin >> n;
    cin.ignore();
    
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        istringstream iss(line);
        string command;
        iss >> command;
        
        if (command == "insert") {
            string index;
            int value;
            iss >> index >> value;
            insert(index, value);
        } else if (command == "delete") {
            string index;
            int value;
            iss >> index >> value;
            remove(index, value);
        } else if (command == "find") {
            string index;
            iss >> index;
            findAndPrint(index);
        }
    }
    
    return 0;
}