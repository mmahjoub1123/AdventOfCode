#include <vector>
#include <algorithm>
#include <regex>

using namespace std;

vector<std::string> parse_string(const string &input, regex delim)
{
    vector<string> out(
                sregex_token_iterator(input.begin(), input.end(), delim, -1),
                sregex_token_iterator()
                );
    return out;
}

struct ElfFile
{
    string m_name;
    int m_size;

    ElfFile(string name, int size): m_name(name), m_size(size) {};

    static ElfFile parse(const string &input)
    {
        vector<std::string> out = parse_string(input, regex(" "));

        std::string::size_type sz;   // alias of size_t
        return ElfFile(out[1], stoi(out[0], &sz));
    }

    static bool isParseable(const string &input)
    {
        vector<std::string> out = parse_string(input, regex(" "));
        if ( (out.size() == 2)
        && ( !out[0].empty() )
        && ( std::find_if(out[0].begin(),
            out[0].end(), [](unsigned char c) { return !std::isdigit(c); }) == out[0].end() ))
            return true;
        else
            return false;
    }

    friend bool operator== (const ElfFile &file1, const ElfFile &file2)
    {
        return (file1.m_name == file2.m_name) && (file1.m_size == file2.m_size);
    }
};



struct ElfDirectory
{
public:
    string m_name;
    ElfDirectory* m_parent;

private:
    vector<ElfFile*> m_files;
    vector<ElfDirectory*> m_children;

public:

    ElfDirectory() = default;
    vector<ElfFile*> Files() { return m_files; };
    vector<ElfDirectory*> Children() { return m_children; };


    ElfDirectory(const string &name, ElfDirectory* parent)
    {
        m_name = name;
        m_parent = parent;
        if (parent != nullptr)
        {
            parent->m_children.push_back(this);
        }
    }

    static ElfDirectory parse(const string &input, ElfDirectory *parent)
    {
        vector<std::string> out = parse_string(input, regex(" "));
        return ElfDirectory(out[1], parent);
    }

    static bool isParseable(const string &input)
    {
        return input.rfind("dir", 0) == 0;
    }

    void addFile(ElfFile* file)
    {
        m_files.push_back(file);
    }

    int size()
    {
        int sum_size = 0;
        for (auto file: m_files)
            sum_size += file->m_size;
        for (auto folder: m_children)
            sum_size += folder->size();
        return sum_size;
    }

    friend bool operator== (const ElfDirectory &dir1, const ElfDirectory &dir2)
    {
        return (dir1.m_name == dir2.m_name) && (dir1.m_parent == dir2.m_parent);
    }
};


void processDirectoryLine(ElfDirectory* currentDir, const string &input)
{

}


void processFileLine(ElfDirectory* currentDir, const string &input)
{

}


ElfDirectory processChangeDirectory(ElfDirectory* currentDir, const string &input)
{
    vector<std::string> out = parse_string(input, regex(" "));

    if (out[2] == "..")
    {
        currentDir = currentDir->m_parent;
    }
    else if (out[2] == "/")
    {
        while(currentDir->m_name != "/")
        {
            currentDir = currentDir->m_parent;
        }
    }
    else
    {
        for (auto child: currentDir->Children())
        {
            if (child->m_name == out[2])
            {
                currentDir = child;
                break;
            }
        }
    }

    return *currentDir;
}