#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct TreeItem
{
    string value;
    long int size;
    TreeItem* parent;
    TreeItem* firstchild;
    TreeItem* next_sibling;

    TreeItem( ) :
        firstchild( nullptr ),
        next_sibling( nullptr ),
        parent( nullptr )
    {}

    TreeItem( const string &value, long int size, TreeItem *parent ) :
        firstchild( nullptr ),
        next_sibling( nullptr ),
        parent( parent ),
        value( value ),
        size( size )
    {}

    TreeItem* lastChild()
    {
        TreeItem *nextChild = firstchild, *child = nullptr;

        while( nextChild != nullptr )
        {
            child = nextChild;
            nextChild = nextChild->next_sibling;
        }

        return child;
    }

    TreeItem* find( const string &entry )
    {
        TreeItem *tmp = firstchild;

        while( tmp != nullptr && tmp->value != entry )
            tmp = tmp->next_sibling;

        return tmp;
    }

    void add( const std::string &value, long int size )
    {
        if( firstchild == nullptr )
            firstchild = new TreeItem( value, size, this );
        else
            lastChild( )->next_sibling = new TreeItem( value, size, this );
    }
};

bool add( const std::string &path, long int size, TreeItem *currentFolder )
{
    std::string nextFolder;
    size_t pos = path.find( '/' );

    nextFolder = ( pos != std::string::npos ) ? path.substr( 0, pos ) : "";

    if( nextFolder.length( ) > 0 )
    {
        TreeItem *foundEntry;
        foundEntry = currentFolder->find( nextFolder );

        if( foundEntry == nullptr )
            return false;

        add( path.substr( pos + 1 ), size, foundEntry );
    }
    else
    {
        if( currentFolder->find( path ) != nullptr )
            return false;

        currentFolder->add( path, size );

    }

    return true;
}

string extractIntegerWords(const string& str)
{
    stringstream ss;

    /* Storing the whole string into string stream */
    ss << str;

    /* Running loop till the end of the stream */
    string temp;
    int found;
    string number;
    while (!ss.eof()) {

        /* extracting word by word from stream */
        ss >> temp;

        /* Checking the given word is integer or not */
        if (stringstream(temp) >> found)
            number += to_string(found);

        /* To save from space at the end of string */
        temp = "";
    }

    return number;
}


// Parse the input file
void parseFile(string inFileName, TreeItem* root)
{
    ifstream inFile;

    inFile.open(inFileName);

    string line;
    string path;

    if (inFile.is_open())
    {
        while (!inFile.eof())
        {
            getline(inFile, line);
            if (line.find("$") != std::string::npos)
            {
                if (line.find("cd") != std::string::npos)
                {
                    if (line.find("..") != std::string::npos)
                    {
                        if (path.length() == 1)
                        {
                            path = "";
                        }
                        else
                        {
                            reverse(path.begin(), path.end());
                            path = path.substr(path.find("/") + 1);
                            reverse(path.begin(), path.end());
                            cout << "path " << path << endl;
                        }
                        /*
                        int pos = path.find("/");
                        string current_folder = path.substr(0 , pos);
                        reverse(current_folder.begin(), current_folder.end());
                        TreeItem *entry = root->find( current_folder );
                        entry->size = folder_size;
                        folder_size = 0;
                        cout << "folder: " << current_folder << " " << folder_size << endl;
                        */

                    }
                    else
                    {
                        if (line.substr(5) != "/")
                        {
                            if ( (path != "") && (path.back() != '/') )
                            {
                                path += '/';
                            }
                            path += line.substr(5);
                        }
                        else
                        {
                            path = "";
                        }
                        add(path, 0, root);
                        cout << path << endl;
                    }
                }
            }
            else
            {
                if (line.find("dir") == std::string::npos)
                {
                    string number = extractIntegerWords(line);
                    if ( (path != "") && (path.back() != '/') )
                    {
                        path += '/';
                    }
                    string file_path = path + line.substr(number.length()+1);

                    std::string::size_type sz;   // alias of size_t
                    add(file_path, stoi(number, &sz), root);
                    cout << "file: " << file_path << " " << stoi(number, &sz) << endl;
                }
            }
        }
        inFile.close();
    }
    else
    {
        cout << "Cannot open file: " << inFileName << endl;
    }
}


int compute_folder_size(TreeItem* root , int max_size)
{
    if ( root->firstchild == nullptr )
    {
        if (root->size <= max_size)
        {
            return root->size;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return compute_folder_size(root->firstchild, max_size) + compute_folder_size(root->firstchild->next_sibling, max_size);
    }
}


int part1(string buffer)
{
    TreeItem root;
    parseFile(buffer, &root);
    int sum_total_sizes = 0;
    cout << compute_folder_size(&root, 100000);
    return sum_total_sizes;
}


int main(int argc, char **argv)
{
    cout << "Part 1: " << endl << part1(argv[1]) << endl;

    return 0;
}