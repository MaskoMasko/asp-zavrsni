#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <numeric>

using namespace std;

// Struktura za pohranu zapisa
struct Record
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
    int NumSkills; // Broj vještina
    int Employed;
};

// Funkcija za podjelu stringa na dijelove prema delimiteru
vector<string> split(const string &str, char delim)
{
    stringstream ss(str);
    string item;
    vector<string> tokens;
    while (getline(ss, item, delim))
    {
        tokens.push_back(item);
    }
    return tokens;
}

// Funkcija za čitanje podataka iz CSV datoteke
vector<Record> readCSV(const string &filename)
{
    ifstream file(filename);
    string line;
    vector<Record> records;

    // Preskoči prvi redak (zaglavlja)
    getline(file, line);

    while (getline(file, line))
    {
        vector<string> tokens = split(line, ',');
        Record record;
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
        record.ComputerSkills = split(tokens[12], ';');
        record.NumSkills = record.ComputerSkills.size();
        record.Employed = stoi(tokens[13]);
        records.push_back(record);
    }

    return records;
}

// Max-heap za usporedbu zapisa prema broju vještina
class MaxHeap
{
public:
    MaxHeap() {}

    void push(const Record &record)
    {
        heap.push_back(record);
        pushHeapify(heap.size() - 1);
    }

    Record pop()
    {
        if (heap.empty())
            throw runtime_error("Heap is empty");
        Record top = heap.front();
        heap[0] = heap.back();
        heap.pop_back();
        popHeapify(0);
        return top;
    }

    bool isEmpty() const
    {
        return heap.empty();
    }

private:
    vector<Record> heap;

    void pushHeapify(int index)
    {
        while (index != 0 && heap[parent(index)].NumSkills < heap[index].NumSkills)
        {
            swap(heap[index], heap[parent(index)]);
            index = parent(index);
        }
    }

    void popHeapify(int index)
    {
        int largest = index;
        int left = leftChild(index);
        int right = rightChild(index);

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
            popHeapify(largest);
        }
    }

    int parent(int index) const
    {
        return (index - 1) / 2;
    }

    int leftChild(int index) const
    {
        return 2 * index + 1;
    }

    int rightChild(int index) const
    {
        return 2 * index + 2;
    }
};

// Funkcija za formatirani ispis zapisa
void printRecord(const Record &record)
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
    vector<Record> records = readCSV("skup_podataka.csv");
    MaxHeap heap;

    for (vector<Record>::iterator it = records.begin(); it != records.end(); ++it)
    {
        heap.push(*it);
    }

    cout << left << setw(10) << "Age"
         << setw(15) << "EdLevel"
         << setw(10) << "Country"
         << setw(10) << "Salary"
         << setw(10) << "Skills"
         << setw(30) << "ComputerSkills"
         << endl;
    cout << string(100, '-') << endl;

    while (!heap.isEmpty())
    {
        Record top = heap.pop();
        printRecord(top);
    }

    return 0;
}
