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
    // splits string by delimiter (, za zasebne podatke u csv zapisu podataka)
    vector<string> tokens;
    int start = 0;
    int end = str.find(delim); // kada delimiter nije pronadjen u stringu returna se -1

    while (end != -1)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delim, start);
    }
    tokens.push_back(str.substr(start)); // posljedni podatak

    return tokens;
}

vector<InputDataStructure> readCSV(string &filename)
{
    ifstream file(filename);
    string line;
    vector<InputDataStructure> dataList;

    // preskacemo prvi redak (nazivi stupaca)
    getline(file, line);

    while (getline(file, line))
    {
        vector<string> tokens = splitString(line, ',');
        InputDataStructure data;
        data.Age = tokens[1];
        data.Accessibility = tokens[2];
        data.EdLevel = tokens[3];
        data.Employment = tokens[4];
        data.Gender = tokens[5];
        data.MentalHealth = tokens[6];
        data.MainBranch = tokens[7];
        data.YearsCode = stoi(tokens[8]); // stoi je funkcija koja pretvara string double u int
        data.YearsCodePro = stoi(tokens[9]);
        data.Country = tokens[10];
        data.PreviousSalary = stod(tokens[11]); // stod je funkcija koja pretvara string u double
        // split computer skills by ; because they are stored as a single string
        data.ComputerSkills = splitString(tokens[12], ';');
        // computed prop
        data.NumSkills = data.ComputerSkills.size();
        data.Employed = stoi(tokens[13]);
        dataList.push_back(data);
    }

    return dataList;
}

void reheapUp(vector<InputDataStructure> &heap, int index)
{
    // index 0 je root
    while (index != 0 && heap[(index - 1) / 2].NumSkills < heap[index].NumSkills)
    {
        swap(heap[index], heap[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void reheapDown(vector<InputDataStructure> &heap, int index)
{
    int largestChildIndex = index;
    int leftChildIndex = 2 * index + 1;
    int rightChildIndex = 2 * index + 2;

    if (leftChildIndex < heap.size() && heap[leftChildIndex].NumSkills > heap[largestChildIndex].NumSkills)
    {
        largestChildIndex = leftChildIndex;
    }

    if (rightChildIndex < heap.size() && heap[rightChildIndex].NumSkills > heap[largestChildIndex].NumSkills)
    {
        largestChildIndex = rightChildIndex;
    }

    if (largestChildIndex != index)
    {
        // ako najveći element nije na trenutnoj poziciji, zamijeni i nastavi
        swap(heap[index], heap[largestChildIndex]);
        reheapDown(heap, largestChildIndex);
    }
}

InputDataStructure pop(vector<InputDataStructure> &heap)
{
    if (heap.empty())
        throw runtime_error("Heap is empty");
    // klasik switcheroo
    InputDataStructure top = heap.front();
    heap[0] = heap.back();
    heap.pop_back();
    reheapDown(heap, 0);
    return top;
}

void printData(InputDataStructure &data)
{
    string concatenatedSkills;
    int maxSkills = data.ComputerSkills.size() > 4 ? 4 : data.ComputerSkills.size(); // prikazujemo maksimalno 4 computer skillsa zbog preglednosti
    bool truncated = maxSkills < data.ComputerSkills.size();
    for (int i = 0; i < maxSkills; ++i)
    {
        concatenatedSkills += data.ComputerSkills[i];
        if (i < data.ComputerSkills.size() - 1)
        {
            concatenatedSkills += ", ";
        }
    }
    if (truncated)
    {
        concatenatedSkills += "...";
    }

    // left - lijevo poravnanje, setw - sirina ispisa
    cout << left << setw(10) << data.Age
         << setw(15) << data.EdLevel
         << setw(20) << data.Country.substr(0, 14)
         << setw(10) << data.PreviousSalary
         << setw(10) << data.NumSkills
         << setw(30) << concatenatedSkills
         << endl;
}

int main()
{
    string filename = "skup_podataka.csv";
    vector<InputDataStructure> dataList = readCSV(filename);
    vector<InputDataStructure> heap;

    // gradnja gomile
    for (auto it = dataList.begin(); it != dataList.end(); ++it)
    {
        heap.push_back(*it);
        reheapUp(heap, heap.size() - 1);
    }

    cout << left << setw(10) << "Age"
         << setw(15) << "EdLevel"
         << setw(20) << "Country"
         << setw(10) << "Salary"
         << setw(10) << "Skills"
         << setw(30) << "ComputerSkills"
         << endl;
    cout << string(100, '-') << endl;

    while (!heap.empty())
    {
        InputDataStructure top = pop(heap);
        printData(top);
    }

    return 0;
}
