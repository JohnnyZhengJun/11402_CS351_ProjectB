#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <cassert>

using namespace std;

// --- Data Structures ---
struct Table {
    string name;
    vector<string> headers;
    vector<vector<string>> rows;
};

// --- Functions & Core Logic ---
void printHelp() {
    cout << "Usage: mini-database --csv <filename.csv>\n"
        << "Options:\n"
        << "  --help       Show this message\n"
        << "  --csv [file] Load a CSV file into the database\n";
}

void printSQLStructureAlert() {
    cout << "ERROR: Invalid SQL syntax.\n"
        << "Supported MySQL/PostgreSQL structures:\n"
        << "  - SELECT * FROM <table_name>;\n"
        << "  - SELECT col1, col2 FROM <table_name>;\n";
}

// Utility to remove whitespace and invisible carriage returns (\r)
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// --- State-Machine CSV Parser ---
vector<string> parseCSVLine(const string& line) {
    vector<string> row;
    string cell;
    bool inQuotes = false;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        
        // Toggle state when we hit a quote
        if (c == '"') {
            inQuotes = !inQuotes;
        } 
        // If we hit a comma AND we are not inside quotes, finalize the cell
        else if (c == ',' && !inQuotes) {
            row.push_back(cell.empty() ? "NULL" : trim(cell));
            cell.clear();
        } 
        // Otherwise, keep building the cell data
        else {
            cell += c;
        }
    }
    // Push the final cell after the loop ends
    row.push_back(cell.empty() ? "NULL" : trim(cell));
    
    return row;
}

Table loadCSV(const string& filepath, const string& tableName) {
    Table table;
    table.name = tableName;
    ifstream file(filepath);
    
    if (!file.is_open()) {
        cerr << "Failed to open memory/file: " << filepath << "\n";
        table.name = ""; 
        return table;
    }

    string line;
    // Parse headers using the new state machine
    if (getline(file, line)) {
        table.headers = parseCSVLine(line);
    }

    // Parse rows using the new state machine
    while (getline(file, line)) {
        if (trim(line).empty()) continue; // Skip accidental blank lines
        table.rows.push_back(parseCSVLine(line));
    }
    return table;
}

bool executeQuery(const string& query, const Table& table) {
    if (table.name.empty()) {
        cout << "ERROR: No database loaded. Cannot execute queries.\n"
            << "Please restart the program with: ./QE --csv <filename.csv>\n";
        return false;
    }

    // Updated Regex: Added \s* at the start and end to allow leading/trailing spaces
    regex sqlPattern(R"(^\s*SELECT\s+(.+)\s+FROM\s+([a-zA-Z0-9_]+)\s*;?\s*$)", regex_constants::icase);
    smatch matches;

    if (!regex_match(query, matches, sqlPattern)) {
        printSQLStructureAlert();
        return false; 
    }

    string columns = matches[1].str();
    string targetTable = matches[2].str();

    if (targetTable != table.name) {
        cout << "ERROR: Table '" << targetTable << "' does not exist.\n";
        return false; 
    }

    // --- Projection (Column Mapping) Logic ---
    vector<int> targetIndices;
    if (trim(columns) == "*") {
        for (size_t i = 0; i < table.headers.size(); ++i) {
            targetIndices.push_back(i);
        }
    } else {
        stringstream ss(columns);
        string colName;
        while (getline(ss, colName, ',')) {
            colName = trim(colName);
            bool found = false;
            for (size_t i = 0; i < table.headers.size(); ++i) {
                if (table.headers[i] == colName) {
                    targetIndices.push_back(i);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "ERROR: Unknown column '" << colName << "' in table '" << targetTable << "'.\n";
                return false;
            }
        }
    }

    // Execution: Output headers based on target indices
    for (int idx : targetIndices) cout << table.headers[idx] << "\t| ";
    cout << "\n----------------------------------------\n";

    // Execution: Output rows based on target indices
    for (const auto& row : table.rows) {
        for (int idx : targetIndices) {
            if (idx < row.size()) cout << row[idx] << "\t| ";
            else cout << "NULL\t| "; 
        }
        cout << "\n";
    }
    cout << "(" << table.rows.size() << " rows returned)\n";
    return true;
}

// --- Main Entry & REPL ---
int main(int argc, char* argv[]) {
    Table activeTable;

    if (argc >= 3 && string(argv[1]) == "--csv") {
        string fullPath = argv[2];
        
        // Find the last slash to get just the filename
        size_t lastSlash = fullPath.find_last_of("/\\");
        string filename = (lastSlash == string::npos) ? fullPath : fullPath.substr(lastSlash + 1);
        
        // Strip the extension (.csv)
        size_t dotPos = filename.find_last_of('.');
        string tableName = (dotPos == string::npos) ? filename : filename.substr(0, dotPos);
        
        activeTable = loadCSV(fullPath, tableName);
        if (!activeTable.name.empty()) {
            cout << "Loaded database memory block: " << filename << " as table '" << tableName << "'\n";
        }
    }else {
        printHelp();
        cout << "\n[Warning] No database loaded.\n";
    }

    string userInput;
    while (true) {
        cout << "\nmini-db> ";
        if (!getline(cin, userInput)) break; 

        if (userInput == "exit" || userInput == "quit") break;
        if (userInput.empty()) continue;

        executeQuery(userInput, activeTable);
    }

    return 0;
}