#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstring>

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
        // Check if (index, value) already exists
        if (exists(index, value)) {
            return;
        }
        
        ofstream file(dataFile, ios::app);
        if (file.is_open()) {
            file << index << " " << value << "\n";
            file.close();
        }
    }
    
    void remove(const string& index, int value) {
        ifstream inFile(dataFile);
        vector<string> lines;
        string line;
        
        // Read all lines except the one to delete
        while (getline(inFile, line)) {
            istringstream iss(line);
            string fileIndex;
            int fileValue;
            if (iss >> fileIndex >> fileValue) {
                if (!(fileIndex == index && fileValue == value)) {
                    lines.push_back(line);
                }
            }
        }
        inFile.close();
        
        // Write back all remaining lines
        ofstream outFile(dataFile);
        for (const string& l : lines) {
            outFile << l << "\n";
        }
        outFile.close();
    }
    
    vector<int> find(const string& index) {
        vector<int> values;
        ifstream file(dataFile);
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
        
        // Sort values in ascending order
        sort(values.begin(), values.end());
        return values;
    }
    
private:
    bool exists(const string& index, int value) {
        ifstream file(dataFile);
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
            vector<int> values = storage.find(index);
            
            if (values.empty()) {
                cout << "null";
            } else {
                for (size_t j = 0; j < values.size(); j++) {
                    if (j > 0) cout << " ";
                    cout << values[j];
                }
            }
            cout << "\n";
        }
    }
    
    return 0;
}