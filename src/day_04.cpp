#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

// Split the string separated by delimiter del
vector<string> tokenizer(const string& s, char del)
{
    vector<string> separator;
    stringstream ss(s);
    string word;
    while (!ss.eof()) {
        getline(ss, word, del);
        separator.push_back(word);
    }
    return separator;
}


// Parse the input file
auto parse(const string& inFileName)
{
    ifstream inFile;

    inFile.open(inFileName);

    vector<pair<pair<int,int>,pair<int,int>>> rounds;
    string line;

    if (inFile.is_open())
    {
        while (!inFile.eof())
        {
            getline(inFile, line);
            auto assignments = tokenizer(line, ',');
            auto first_section = tokenizer(assignments[0], '-');
            auto second_section = tokenizer(assignments[1], '-');

            rounds.push_back(make_pair(make_pair(stoi(first_section[0]), stoi(first_section[1])),
                                       make_pair(stoi(second_section[0]), stoi(second_section[1]))));

        }
        inFile.close();
    }
    else
    {
        cout << "Cannot open file: " << inFileName << endl;
    }
    return rounds;
}

bool isRangeFullyContained(pair<pair<int,int>,pair<int,int>> round)
{
    bool isContained = false;
    auto range1 = round.first;
    auto range2 = round.second;

    if (   (range1.first == range2.first)
        || (range1.second == range2.second)
        || ((range1.first < range2.first) && (range2.second < range1.second))
        || ((range2.first < range1.first) && (range1.second < range2.second)))
        {
            isContained = true;
        }

    return isContained;
}

bool doesRangeOverlap(pair<pair<int,int>,pair<int,int>> round)
{
    bool overlap = false;
    auto range1 = round.first;
    auto range2 = round.second;
    if (((range1.first >= range2.first) && (range1.first <= range2.second))
        || ((range2.first >= range1.first) && (range2.first <= range1.second)))
    {
        overlap = true;
    }
    return overlap;
}


int part1(vector<pair<pair<int,int>,pair<int,int>>> &rounds)
{
    int nb_contained_ranges = 0;
    for (auto round: rounds)
    {
        nb_contained_ranges += isRangeFullyContained(round);
    }
    return nb_contained_ranges;
}


int part2(vector<pair<pair<int,int>,pair<int,int>>> &rounds)
{
    int nb_overlap = 0;
    for (auto round: rounds)
    {
        nb_overlap += doesRangeOverlap(round);
    }
    return nb_overlap;
}


int main(int argc, char **argv)
{
    auto rounds = parse(argv[1]);

    cout << "Part 1: " << part1(rounds) << endl;

    cout << "Part 2: " << part2(rounds) << endl;

    return 0;
}