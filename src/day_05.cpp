#include <iostream>
#include <fstream>
#include <sstream>
#include <stack>
#include <vector>
#include <tuple>

using namespace std;

struct Crate
{
    Crate(int nb_stacks);
    vector<stack<char>> m_crate_stacks;
    vector<string> m_stacks_movement;

    void parse_stacks(vector<string> content_stacks);
    tuple<int, int, int> parse_mouvement(string mvt);
    void move_crates9000(vector<string> move_infos);
    void move_crates9001(vector<string> move_infos);
    string get_stack_tops();

    ~Crate();
};


Crate::Crate(int nb_stacks)
{
    for (int i=0; i< nb_stacks; i++)
    {
        m_crate_stacks.push_back(stack<char>());
    }
}


Crate::~Crate()
{}


int get_nb_stacks(vector<string> content_stacks)
{
    return content_stacks[content_stacks.size()-1].back() - '0';
}

void Crate::parse_stacks(vector<string> content_stacks)
{
    int i = content_stacks.size()-2;
    int index_stack = 0;
    while (i >=0)
    {
        while (index_stack < content_stacks[i].length())
        {
            if (!isspace(content_stacks[i][index_stack+1]))
            {
                m_crate_stacks[index_stack/4].push(content_stacks[i][index_stack+1]);
            }
            index_stack += 4;
        }
        index_stack = 0;
        i--;
    }
}


tuple<int, int, int> Crate::parse_mouvement(string mvt)
{
    vector<int> int_extraction;
    stringstream ss;

    /* Storing the whole string into string stream */
    ss << mvt;

    /* Running loop till the end of the stream */
    string temp;
    int found;
    while (!ss.eof()) {

        /* extracting word by word from stream */
        ss >> temp;

        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found)
        {
            int_extraction.push_back(found);
        }
        /* To save from space at the end of string */
        temp = "";
    }
    return make_tuple(int_extraction[0], int_extraction[1], int_extraction[2]);
}



void Crate::move_crates9000(vector<string> move_infos)
{
    for (auto str_mvt : move_infos)
    {
        auto mvt = parse_mouvement(str_mvt);

        for (int i=0; i< get<0>(mvt); i++)
        {
            m_crate_stacks[get<2>(mvt)-1].push(m_crate_stacks[get<1>(mvt)-1].top());
            m_crate_stacks[get<1>(mvt)-1].pop();
        }
    }
}


void Crate::move_crates9001(vector<string> move_infos)
{
    for (auto str_mvt : move_infos)
    {
        auto mvt = parse_mouvement(str_mvt);
        stack<char> temp_stack;
        for (int i=0; i< get<0>(mvt); i++)
        {
            temp_stack.push(m_crate_stacks[get<1>(mvt)-1].top());
            m_crate_stacks[get<1>(mvt)-1].pop();
        }

        while(!temp_stack.empty())
        {
            m_crate_stacks[get<2>(mvt)-1].push(temp_stack.top());
            temp_stack.pop();
        }
    }
}

string Crate::get_stack_tops()
{
    string tops = "";
    for (auto stack : m_crate_stacks)
    {
        tops += stack.top();
    }
    return tops;
}


// Parse the input file
pair<vector<string>, vector<string>> parseFile(string inFileName)
{
    ifstream inFile;

    inFile.open(inFileName);

    vector<string> content_stacks;
    vector<string> move_infos;

    string line;
    bool parse_initial_stacks_done = false;

    if (inFile.is_open())
    {
        while (!inFile.eof())
        {
            getline(inFile, line);
            if ( !parse_initial_stacks_done )
            {
                if (line != "")
                {
                    content_stacks.push_back(line);
                }
                else
                {
                    parse_initial_stacks_done = true;
                }
            }
            else
            {
                move_infos.push_back(line);
            }
        }

        inFile.close();
    }
    else
    {
        cout << "Cannot open file: " << inFileName << endl;
    }
    return make_pair(content_stacks, move_infos);
}


string part1(string input_file)
{
    auto stack_infos = parseFile(input_file);
    Crate crate = Crate(get_nb_stacks(stack_infos.first));
    crate.parse_stacks(stack_infos.first);
    crate.move_crates9000(stack_infos.second);
    return crate.get_stack_tops();
}


string part2(string input_file)
{
    auto stack_infos = parseFile(input_file);
    Crate crate = Crate(get_nb_stacks(stack_infos.first));
    crate.parse_stacks(stack_infos.first);
    crate.move_crates9001(stack_infos.second);
    return crate.get_stack_tops();
}


int main(int argc, char **argv)
{
    cout << "Part 1: " << part1(argv[1]) << endl;
    cout << "Part 2: " << part2(argv[1]) << endl;

    return 0;
}