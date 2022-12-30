#include "../src/day_07.cpp"
#include <limits.h>
#include "gtest/gtest.h"

int my_argc;
char** my_argv;

TEST(isParseableFileTest, isParseable)
{
    EXPECT_TRUE(ElfFile::isParseable("8504156 c.dat"));
    EXPECT_TRUE(ElfFile::isParseable("29116 f"));

    EXPECT_FALSE(ElfFile::isParseable("$ cd /"));
    EXPECT_FALSE(ElfFile::isParseable("$ cd e"));
    EXPECT_FALSE(ElfFile::isParseable("$ ls"));
    EXPECT_FALSE(ElfFile::isParseable("dir d"));
}

TEST(parseFileTest, Parse)
{
    const ElfFile file_c {"c.dat", 8504156};
    EXPECT_EQ(ElfFile::parseFile("8504156 c.dat"), file_c);
    const ElfFile file_f {"f", 29116};
    EXPECT_EQ(ElfFile::parseFile("29116 f"), file_f);
}

TEST(parseDirectoryTest, Parse)
{
    ElfDirectory root{"/", nullptr};
    string testInput = "dir a";
    ElfDirectory a = ElfDirectory::parseDir(testInput, &root);
    ElfDirectory expected = ElfDirectory("a", &root);
    EXPECT_EQ(a.m_name, expected.m_name);
    EXPECT_EQ(a.m_parent, expected.m_parent);

    testInput = "dir e";
    ElfDirectory e = ElfDirectory::parseDir(testInput, &a);
    expected = ElfDirectory("e", &a);
    EXPECT_EQ(e.m_name, expected.m_name);
    EXPECT_EQ(e.m_parent, expected.m_parent);
}


TEST(isParseableDirectoryTest, isParseable)
{
    EXPECT_FALSE(ElfDirectory::isParseable("8504156 c.dat"));
    EXPECT_FALSE(ElfDirectory::isParseable("29116 f"));
    EXPECT_FALSE(ElfDirectory::isParseable("$ cd /"));
    EXPECT_FALSE(ElfDirectory::isParseable("$ cd e"));
    EXPECT_FALSE(ElfDirectory::isParseable("$ ls"));

    EXPECT_TRUE(ElfDirectory::isParseable("dir d"));
}


TEST(addFile, file)
{
    ElfDirectory root {"/", nullptr};
    EXPECT_EQ(root.size(), 0);

    ElfFile foo1{"foo.txt", 500};
    root.addFile(&foo1);
    EXPECT_NE(std::find(root.Files().begin(), root.Files().end(), &foo1), root.Files().end());
    EXPECT_EQ(root.size(), 500);

    ElfFile foo2{"foo2.jpeg", 200};
    root.addFile(&foo2);
    EXPECT_NE(std::find(root.Files().begin(), root.Files().end(), &foo2), root.Files().end());
    EXPECT_EQ(root.size(), 700);
}


TEST(changeDirectoryTest, Parse)
{
    ElfDirectory root{"/", nullptr};
    ElfDirectory a{"a", &root};
    ElfDirectory e{"e", &a};

    ElfDirectory* result = processChangeDirectory (&root, "$ cd /");
    EXPECT_EQ(result->m_name, root.m_name);
    EXPECT_EQ(result->m_parent, root.m_parent);

    result = processChangeDirectory (&root, "$ cd a");
    EXPECT_EQ(result->m_name, a.m_name);
    EXPECT_EQ(result->m_parent, a.m_parent);

    result = processChangeDirectory (&a, "$ cd e");
    EXPECT_EQ(result->m_name, e.m_name);
    EXPECT_EQ(result->m_parent, e.m_parent);

    result = processChangeDirectory (&e, "$ cd ..");
    EXPECT_EQ(result->m_name, a.m_name);
    EXPECT_EQ(result->m_parent, a.m_parent);

    result = processChangeDirectory (&a, "$ cd ..");
    EXPECT_EQ(result->m_name, root.m_name);
    EXPECT_EQ(result->m_parent, root.m_parent);
}


TEST(buildFileSystemTest, testBuild)
{
    vector<string> rows = parseFile(my_argv[1]);
    ElfDirectory root = buildFileSystem(rows);

    //ElfDirectory a{"a"};
    //EXPECT_NE(find_if(root.Children().cbegin(), root.Children().cend(), [a](ElfDirectory *dir){return dir->m_name == a.m_name;}), root.Children().cend());
}


int main(int argc, char **argv) {
    my_argc = argc;
    my_argv = argv;
  ::testing::InitGoogleTest(&my_argc, my_argv);
  return RUN_ALL_TESTS();
}