#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

vector<pair<char, char>> parse(string inFileName)
{
    ifstream inFile;

    inFile.open(inFileName);

    vector<pair<char, char>> rounds;
    string line;

    if (inFile.is_open())
    {
        while (!inFile.eof())
        {
            getline(inFile, line);

            rounds.push_back(make_pair(line[0], line[2]));

        }
        inFile.close();
    }
    else
    {
        cout << "Cannot open file: " << inFileName << endl;
    }
    return rounds;
}


int calculateOutcome(char opponent, char leMe)
{
    int score = 0;
    if ( (opponent == 'A' && leMe == 'X')
    ||   (opponent == 'B' && leMe == 'Y')
    ||   (opponent == 'C' && leMe == 'Z') )
    {
        score = 3;
    }
    if ( (opponent == 'A' && leMe == 'Y' )
    ||   (opponent == 'B' && leMe == 'Z' )
    ||   (opponent == 'C' && leMe == 'X' ) )
    {
        score = 6;
    }

    return score;
}

char whatToPlay(char opponent, char outcome)
{
    char shape = opponent;
    if (outcome == 'X')
    {
        if (opponent == 'A')
        {
            shape = 'C';
        }
        else if (opponent == 'B')
        {
            shape = 'A';
        }
        else if (opponent == 'C')
        {
            shape = 'B';
        }
    }
    else if (outcome == 'Z')
    {
        if (opponent == 'A')
        {
            shape = 'B';
        }
        else if (opponent == 'B')
        {
            shape = 'C';
        }
        else if (opponent == 'C')
        {
            shape = 'A';
        }
    }

    return shape;
}

int part1(vector<pair<char, char>> rounds)
{
    map<char, int> shape_score;
    shape_score['X'] = 1;
    shape_score['Y'] = 2;
    shape_score['Z'] = 3;

    int total_score = 0;

    for (auto round: rounds)
    {
        total_score += calculateOutcome(round.first, round.second) + shape_score[round.second];
    }

    return total_score;
}

int part2(vector<pair<char, char>> rounds)
{
    map<char, int> shape_score;
    shape_score['A'] = 1;
    shape_score['B'] = 2;
    shape_score['C'] = 3;

    map<char, int> outcome_score;
    outcome_score['X'] = 0;
    outcome_score['Y'] = 3;
    outcome_score['Z'] = 6;

    int total_score = 0;

    for (auto round: rounds)
    {
        total_score += shape_score[whatToPlay(round.first, round.second)] + outcome_score[round.second];
    }

    return total_score;

}


int main(int argc, char **argv)
{
    vector<pair<char, char>> rounds = parse(argv[1]);

    cout << "Part 1: " << part1(rounds) << endl;

    cout << "Part 2: " << part2(rounds) << endl;

    return 0;
}