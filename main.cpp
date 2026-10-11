#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

class FileStorage {
private:
    string dataFile = "database.dat";
    
public:
    FileStorage() {
        // Check if file exists, create if not
        ifstream check(dataFile);
        if (!check.good()) {
            ofstream create(dataFile);
            create.close();
        }
        check.close();
    }
    
    void insert(const string& index, int value) {
        // Check if (index, value) already exists by scanning file
        if (exists(index, value)) {
            return;
        }
        
        // Append new record
        ofstream file(dataFile, ios::app);
        if (file.is_open()) {
            file << index << " " << value << "\n";
            file.close();
        }
    }
    
    void remove(const string& index, int value) {
        string tempFile = "temp_" + dataFile;
        ifstream inFile(dataFile);
        ofstream outFile(tempFile);
        
        if (!inFile.is_open() || !outFile.is_open()) {
            if (inFile.is_open()) inFile.close();
            if (outFile.is_open()) outFile.close();
            return;
        }
        
        string line;
        
        // Copy all lines except the one to delete
        while (getline(inFile, line)) {
            istringstream iss(line);
            string fileIndex;
            int fileValue;
            
            if (iss >> fileIndex >> fileValue) {
                if (!(fileIndex == index && fileValue == value)) {
                    outFile << line << "\n";
                }
            } else {
                // Line format is wrong, keep it as is
                outFile << line << "\n";
            }
        }
        
        inFile.close();
        outFile.close();
        
        // Replace original file with temp file
        std::remove(dataFile.c_str());
        std::rename(tempFile.c_str(), dataFile.c_str());
    }
    
    void findAndPrint(const string& index) {
        vector<int> values;
        ifstream file(dataFile);
        
        if (!file.is_open()) {
            cout << "null\n";
            return;
        }
        
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            string fileIndex;
            int fileValue;
            
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
        
        // Sort and remove duplicates
        sort(values.begin(), values.end());
        values.erase(unique(values.begin(), values.end()), values.end());
        
        // Print results
        for (size_t i = 0; i < values.size(); i++) {
            if (i > 0) cout << " ";
            cout << values[i];
        }
        cout << "\n";
    }
    
private:
    bool exists(const string& index, int value) {
        ifstream file(dataFile);
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
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    FileStorage storage;
    
    int n;
    cin >> n;
    cin.ignore(); // Ignore newline after n
    
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
            storage.insert(index, value);
        } else if (command == "delete") {
            string index;
            int value;
            iss >> index >> value;
            storage.remove(index, value);
        } else if (command == "find") {
            string index;
            iss >> index;
            storage.findAndPrint(index);
        }
    }
    
    return 0;
}