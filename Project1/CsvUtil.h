#pragma once


#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;


template<typename T>

void loadEntities(const string& filename, vector<T*>& container, int& nextid)
{
    ifstream fin(filename);
    if (!fin)
    {
        return;
    }

    string line;


    while (getline(fin, line))
    {
        if (line.empty())
        {
            continue;
        }

        T* obj = T::fromCsv(line);
        container.push_back(obj);
        nextid = max(nextid, obj->getId() + 1);



    }
}

template<typename T>

void SaveEntities(const string& filename, const vector<T*>container)
{
    ofstream ofs(filename, std::ios::out | std::ios::trunc);

    if (!ofs)
    {
        throw std::runtime_error("Cannot open file for writing");
    }

    for (auto* obj : container)
    {
        ofs << obj->toCsv() << "\n";

    }
}