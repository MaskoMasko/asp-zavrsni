#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <numeric>

using namespace std;

struct InputDataStructure
{
    string Age;
    string Accessibility;
    string EdLevel;
    string Employment;
    string Gender;
    string MentalHealth;
    string MainBranch;
    int YearsCode;
    int YearsCodePro;
    string Country;
    double PreviousSalary;
    vector<string> ComputerSkills;
    int NumSkills; // computed num of skills
    int Employed;
};

vector<string> splitString(string &str, char delim)
{
    // splits string by delimiter (, in CSV file)
    vector<string> tokens;
    int start = 0;
    int end = str.find(delim);

    while (end != -1)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delim, start);
    }
    tokens.push_back(str.substr(start));

    return tokens;
}

vector<InputDataStructure> readCSV(string &filename)
{
    ifstream file(filename);
    string line;
    vector<InputDataStructure> records;

    // skip header line (where column names are stored)
    getline(file, line);

    while (getline(file, line))
    {
        vector<string> tokens = splitString(line, ',');
        InputDataStructure record;
        record.Age = tokens[1];
        record.Accessibility = tokens[2];
        record.EdLevel = tokens[3];
        record.Employment = tokens[4];
        record.Gender = tokens[5];
        record.MentalHealth = tokens[6];
        record.MainBranch = tokens[7];
        record.YearsCode = stoi(tokens[8]);
        record.YearsCodePro = stoi(tokens[9]);
        record.Country = tokens[10];
        record.PreviousSalary = stod(tokens[11]);
        // split computer skills by ; because they are stored as a single string
        record.ComputerSkills = splitString(tokens[12], ';');
        // computed prop
        record.NumSkills = record.ComputerSkills.size();
        record.Employed = stoi(tokens[13]);
        records.push_back(record);
    }

    return records;
}

void pushHeapify(vector<InputDataStructure> &heap, int index)
{
    while (index != 0 && heap[(index - 1) / 2].NumSkills < heap[index].NumSkills)
    {
        swap(heap[index], heap[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void popHeapify(vector<InputDataStructure> &heap, int index)
{
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap.size() && heap[left].NumSkills > heap[largest].NumSkills)
    {
        largest = left;
    }

    if (right < heap.size() && heap[right].NumSkills > heap[largest].NumSkills)
    {
        largest = right;
    }

    if (largest != index)
    {
        swap(heap[index], heap[largest]);
        popHeapify(heap, largest);
    }
}

InputDataStructure pop(vector<InputDataStructure> &heap)
{
    if (heap.empty())
        throw runtime_error("Heap is empty");
    InputDataStructure top = heap.front();
    heap[0] = heap.back();
    heap.pop_back();
    popHeapify(heap, 0);
    return top;
}

void printRecord(InputDataStructure &record)
{
    string concatenatedSkills;
    for (int i = 0; i < record.ComputerSkills.size(); ++i)
    {
        concatenatedSkills += record.ComputerSkills[i];
        if (i < record.ComputerSkills.size() - 1)
        {
            concatenatedSkills += ", ";
        }
    }

    cout << left << setw(10) << record.Age
         << setw(15) << record.EdLevel
         << setw(10) << record.Country
         << setw(10) << record.PreviousSalary
         << setw(10) << record.NumSkills
         << setw(30) << concatenatedSkills
         << endl;
}

int main()
{
    string filename = "skup_podataka.csv";
    vector<InputDataStructure> records = readCSV(filename);
    vector<InputDataStructure> heap;

    for (vector<InputDataStructure>::iterator it = records.begin(); it != records.end(); ++it)
    {
        heap.push_back(*it);
        pushHeapify(heap, heap.size() - 1);
    }

    cout << left << setw(10) << "Age"
         << setw(15) << "EdLevel"
         << setw(10) << "Country"
         << setw(10) << "Salary"
         << setw(10) << "Skills"
         << setw(30) << "ComputerSkills"
         << endl;
    cout << string(100, '-') << endl;

    while (!heap.empty())
    {
        InputDataStructure top = pop(heap);
        printRecord(top);
    }

    return 0;
}
