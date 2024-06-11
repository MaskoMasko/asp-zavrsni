#include <iostream>
#include <fstream>
#include <sstream>
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

vector<string> split(const string &str, char delim)
{
    // splits string by delimiter (, in CSV file)
    stringstream ss(str);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim))
    {
        tokens.push_back(item);
    }
    return tokens;
}

vector<InputDataStructure> readCSV(const string &filename)
{
    ifstream file(filename);
    string line;
    vector<InputDataStructure> records;

    // skip header line (where column names are stored)
    getline(file, line);

    while (getline(file, line))
    {
        vector<string> tokens = split(line, ',');
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
        record.ComputerSkills = split(tokens[12], ';');
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

void printRecord(const InputDataStructure &record)
{
    string concatenatedSkills;
    for (size_t i = 0; i < record.ComputerSkills.size(); ++i)
    {
        concatenatedSkills += record.ComputerSkills[i];
        if (i < record.ComputerSkills.size() - 1)
        {
            concatenatedSkills += ", ";
        }
    }

    std::cout << std::left << std::setw(10) << record.Age
              << std::setw(15) << record.EdLevel
              << std::setw(10) << record.Country
              << std::setw(10) << record.PreviousSalary
              << std::setw(10) << record.NumSkills
              << std::setw(30) << concatenatedSkills
              << std::endl;
}

int main()
{
    vector<InputDataStructure> records = readCSV("skup_podataka.csv");
    vector<InputDataStructure> heap;

    for (vector<InputDataStructure>::iterator it = records.begin(); it != records.end(); ++it)
    {
        heap.push_back(*it);
        pushHeapify(heap, heap.size() - 1);
    }

    cout << left << setw(15) << "Age"
         << setw(15) << "EdLevel"
         << setw(20) << "Country"
         << setw(15) << "Salary"
         << setw(10) << "Skills"
         << setw(50) << "ComputerSkills"
         << endl;
    cout << string(100, '-') << endl;

    while (!heap.empty())
    {
        InputDataStructure top = pop(heap);
        printRecord(top);
    }

    return 0;
}
