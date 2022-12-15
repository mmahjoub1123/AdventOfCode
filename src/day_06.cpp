#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>

using namespace std;

// Parse the input file
string parseFile(const string& inFileName)
{
    ifstream inFile;

    inFile.open(inFileName);

    string buffer;

    if (inFile.is_open())
    {
        while (!inFile.eof())
        {
            getline(inFile, buffer);
        }

        inFile.close();
    }
    else
    {
        cout << "Cannot open file: " << inFileName << endl;
    }
    return buffer;
}


int get_marker(const string& buffer, int nb_distinct)
{
    int index = 0;
    bool is_marker = true;
    while (index <= buffer.length())
    {
        string temp_str = buffer.substr(index, nb_distinct);
        sort(temp_str.begin(), temp_str.end());

        for (int i=0; i < temp_str.length()-1; i++)
        {
            if (temp_str[i] == temp_str[i+1])
            {
                is_marker = false;
                break;
            }
        }
        if (is_marker)
        {
            break;
        }
        is_marker = true;
        index++;
    }
    return index+nb_distinct;
}


int part1(const string& buffer)
{
    return get_marker(parseFile(buffer), 4);
}


int part2(const string& buffer)
{
    return get_marker(parseFile(buffer), 14);
}


int main(int argc, char **argv)
{
    cout << "Part 1: " << part1(argv[1]) << endl;
    cout << "Part 2: " << part2(argv[1]) << endl;

    return 0;
}