#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <cstring>
#include <set>

using namespace std;

class FileStorage {
private:
    string dataFile = "database.dat";
    static const int MAX_INDEX_LEN = 64;
    
    struct Record {
        char index[MAX_INDEX_LEN + 1];
        int value;
        bool deleted;
        
        Record() : value(0), deleted(false) {
            index[0] = '\0';
        }
    };
    
public:
    FileStorage() {
        // Check if file exists, create if not
        ifstream check(dataFile, ios::binary);
        if (!check.good()) {
            ofstream create(dataFile, ios::binary);
            create.close();
        }
        check.close();
    }
    
    void insert(const string& index, int value) {
        // Check if (index, value) already exists
        if (exists(index, value)) {
            return;
        }
        
        ofstream file(dataFile, ios::binary | ios::app);
        if (file.is_open()) {
            Record record;
            strncpy(record.index, index.c_str(), MAX_INDEX_LEN);
            record.index[MAX_INDEX_LEN] = '\0';
            record.value = value;
            record.deleted = false;
            file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
            file.close();
        }
    }
    
    void remove(const string& index, int value) {
        // Mark record as deleted instead of rewriting entire file
        fstream file(dataFile, ios::binary | ios::in | ios::out);
        if (!file.is_open()) return;
        
        Record record;
        streampos pos;
        
        while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
            if (!record.deleted && string(record.index) == index && record.value == value) {
                pos = file.tellg() - static_cast<streamoff>(sizeof(Record));
                file.seekp(pos);
                record.deleted = true;
                file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
                break;
            }
        }
        file.close();
    }
    
    vector<int> find(const string& index) {
        set<int> valueSet; // Use set to automatically handle duplicates and sort
        ifstream file(dataFile, ios::binary);
        
        if (!file.is_open()) {
            return vector<int>();
        }
        
        Record record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
            if (!record.deleted && string(record.index) == index) {
                valueSet.insert(record.value);
            }
        }
        file.close();
        
        // Convert set to vector
        vector<int> values(valueSet.begin(), valueSet.end());
        return values;
    }
    
private:
    bool exists(const string& index, int value) {
        ifstream file(dataFile, ios::binary);
        if (!file.is_open()) return false;
        
        Record record;
        while (file.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
            if (!record.deleted && string(record.index) == index && record.value == value) {
                file.close();
                return true;
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