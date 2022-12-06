#include <iostream>
#include <fstream>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

vector<int> parse(string inFileName)
{
    ifstream inFile;

    inFile.open(inFileName);

    vector<int> sum_calories_vector;

    if (inFile.is_open())
    {
        string calories;
        int sum_calories = 0;
        while (inFile)
        {
            getline(inFile, calories);

            if (calories != "")
            {
                sum_calories += stoi(calories);
            }
            else
            {
                sum_calories_vector.push_back(sum_calories);
                sum_calories = 0;
            }
        }

        inFile.close();
    }
    else
    {
        cout << "Cannot open file: " << inFileName << endl;
    }

    return sum_calories_vector;
}

int part1(vector<int> sum_calories_vector)
{
    return *max_element(sum_calories_vector.begin(), sum_calories_vector.end());
}

int part2(vector<int> sum_calories_vector)
{
    sort(sum_calories_vector.begin(), sum_calories_vector.end(), greater<int>());
    return accumulate(sum_calories_vector.begin(), sum_calories_vector.begin()+3, 0);
}

int main(int argc, char **argv)
{
    vector<int> sum_calories_vector = parse(argv[1]);

    cout << "Part 1: " << part1(sum_calories_vector) << endl;

    cout << "Part 2: " << part2(sum_calories_vector) << endl;

    return 0;
}