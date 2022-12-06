#include <iostream>
#include <fstream>
#include <map>

using namespace std;

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


int main()
{
    cout << "What's the input file's name?" << endl;
    string inFileName;
    cin >> inFileName;

    ifstream inFile;
    map<char, int> shape_score_first_half;
    shape_score_first_half['X'] = 1;
    shape_score_first_half['Y'] = 2;
    shape_score_first_half['Z'] = 3;

    map<char, int> shape_score_second_half;
    shape_score_second_half['A'] = 1;
    shape_score_second_half['B'] = 2;
    shape_score_second_half['C'] = 3;

    map<char, int> outcome_score;
    outcome_score['X'] = 0;
    outcome_score['Y'] = 3;
    outcome_score['Z'] = 6;

    int total_score_first_half = 0;
    int total_score_second_half = 0;

    map<char, string> game;
    map<char, string> shape_name;


    game['A'] = "Rock";
    game['B'] = "Paper";
    game['C'] = "Scissors";

    shape_name['X'] = "Rock";
    shape_name['Y'] = "Paper";
    shape_name['Z'] = "Scissors";

    game['X'] = "Lose";
    game['Y'] = "Draw";
    game['Z'] = "Win";

    inFile.open(inFileName);
    string round;

    if (inFile.is_open())
    {

        while (!inFile.eof())
        {
            getline(inFile, round);

            total_score_first_half += calculateOutcome(round[0], round[2]) + shape_score_first_half[round[2]];

            total_score_second_half += shape_score_second_half[whatToPlay(round[0], round[2])] + outcome_score[round[2]];
         }

        inFile.close();
    }
    else
    {
        cout << "Cannot open file: " << inFileName << endl;
    }

    cout << "total_score first half " << total_score_first_half << endl;

    cout << "total_score second half " << total_score_second_half << endl;

    return 0;
}