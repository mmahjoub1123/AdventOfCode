#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

vector<string> parse(string inFileName)
{
    ifstream inFile;

    inFile.open(inFileName);

    vector<string> rucksacks;

    if (inFile.is_open())
    {
        string items;
        while (!inFile.eof())
        {
            getline(inFile, items);
            rucksacks.push_back(items);
        }

        inFile.close();
    }
    else
    {
        cout << "Cannot open file: " << inFileName << endl;
    }
    return rucksacks;
}

int getPriority(char c)
{
    int priority = 0;
    if (c >= 'a' && c <= 'z' )
        priority = c-'a'+1;
    else if (c >= 'A' && c <= 'Z')
        priority = c-'A'+ 27;
    return priority;
}

int part1(vector<string> rucksacks)
{
    int sum_priorities = 0;
    for (auto rucksack : rucksacks)
    {
        string first_compartment = rucksack.substr(0, rucksack.length()/2);
        string second_compartment = rucksack.substr(rucksack.length()/2, rucksack.length());
        for (auto element : second_compartment)
        {
            if (first_compartment.find(element ) != string::npos )
            {
                sum_priorities += getPriority(element);
                break;
            }
        }
    }
    return sum_priorities;
}
int part2(vector<string> rucksacks)
{
    int index = 0;
    int sum_priorities = 0;
    while (index < rucksacks.size())
    {
        string first_intersection, second_intersection;

        std::sort(rucksacks[index].begin(), rucksacks[index].end());
        std::sort(rucksacks[index+1].begin(), rucksacks[index+1].end());
        std::set_intersection(rucksacks[index].begin(), rucksacks[index].end(), rucksacks[index+1].begin(), rucksacks[index+1].end(), std::back_inserter(first_intersection));

        std::sort(rucksacks[index+2].begin(), rucksacks[index+2].end());
        std::set_intersection(rucksacks[index+2].begin(), rucksacks[index+2].end(), first_intersection.begin(), first_intersection.end(), std::back_inserter(second_intersection));
        sum_priorities += getPriority(second_intersection[0]);
        index += 3;
    }

    return sum_priorities;
}

int main(int argc, char **argv)
{
    vector<string> rucksacks = parse(argv[1]);

    cout << "Part 1: " << part1(rucksacks) << endl;
    cout << "Part 2: " << part2(rucksacks) << endl;

    return 0;
}