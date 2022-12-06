#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

int main()
{
    cout << "What's the input file's name?" << endl;
    string inFileName;
    cin >> inFileName;

    ifstream inFile;

    inFile.open(inFileName);

    vector<int> sum_calories_vector;
    int max_sum_calories = 0;

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
                cout << calories << " ";
            }
            else
            {
                sum_calories_vector.push_back(sum_calories);

                if (sum_calories > max_sum_calories)
                {
                    max_sum_calories = sum_calories;
                }

                sum_calories = 0;
            }


        }

        inFile.close();
    }
    else
    {
        cout << "Cannot open file: " << inFileName << endl;
    }

    cout << "max_sum: " << max_sum_calories << endl;

    sort(sum_calories_vector.begin(), sum_calories_vector.end(), greater<int>());

    cout << "Sum of top three elves " << sum_calories_vector[0]+sum_calories_vector[1]+sum_calories_vector[2] << endl;

    return 0;
}