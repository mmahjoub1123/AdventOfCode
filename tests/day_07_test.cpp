#include "../src/day_07.cpp"
#include <limits.h>
#include "gtest/gtest.h"

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
    EXPECT_EQ(ElfFile::parse("8504156 c.dat"), file_c);
    const ElfFile file_f {"f", 29116};
    EXPECT_EQ(ElfFile::parse("29116 f"), file_f);
}

TEST(parseDirectoryTest, Parse)
{
    ElfDirectory root{"/", nullptr};
    string testInput = "dir a";
    ElfDirectory a = ElfDirectory::parse(testInput, &root);
    ElfDirectory expected = ElfDirectory("a", &root);
    EXPECT_EQ(a.m_name, expected.m_name);
    EXPECT_EQ(a.m_parent, expected.m_parent);

    testInput = "dir e";
    ElfDirectory e = ElfDirectory::parse(testInput, &a);
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

    ElfDirectory result = processChangeDirectory (&root, "$ cd /");
    EXPECT_EQ(result.m_name, root.m_name);
    EXPECT_EQ(result.m_parent, root.m_parent);

    result = processChangeDirectory (&root, "$ cd a");
    EXPECT_EQ(result.m_name, a.m_name);
    EXPECT_EQ(result.m_parent, a.m_parent);

    result = processChangeDirectory (&a, "$ cd e");
    EXPECT_EQ(result.m_name, e.m_name);
    EXPECT_EQ(result.m_parent, e.m_parent);

    result = processChangeDirectory (&a, "$ cd ..");
    EXPECT_EQ(result.m_name, root.m_name);
    EXPECT_EQ(result.m_parent, root.m_parent);
}


TEST(processDirectoryLineTest, Parse)
{
    ElfDirectory root{"/", nullptr};
    ElfDirectory a;
    a.m_name = "a";
    ElfDirectory e;
    e.m_name = "e";

    processDirectoryLine (&root, "dir a");
    bool found = false;
    for (auto child: root.Children())
    {
        if (a.m_name == child->m_name)
        {
            found = true;
            cout << "child " << child->m_name;
            break;
        }
    }
    EXPECT_TRUE(found);

    processDirectoryLine (&a, "dir e");
    found = false;
    for (auto child: a.Children())
    {
        if (e.m_name == child->m_name)
        {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}


TEST(processFileLineTest, Parse)
{
    ElfDirectory root{"/", nullptr};
    ElfDirectory a{"a", &root};
    ElfDirectory e{"e", &a};

    ElfFile foo1{"foo1", 500};
    processFileLine (&root, "500 foo1.txt");
    EXPECT_EQ(root.size(), 500);

    ElfFile foo2{"foo2", 300};
    processFileLine (&root, "300 foo2.txt");
    EXPECT_EQ(root.size(), 800);

    ElfFile foo3{"foo3", 100};
    processFileLine (&a, "100 foo3.txt");
    EXPECT_EQ(a.size(), 100);
    EXPECT_EQ(root.size(), 900);

    ElfFile foo4{"foo4", 200};
    processFileLine (&e, "200 foo4.txt");
    EXPECT_EQ(a.size(), 200);
    EXPECT_EQ(a.size(), 300);
    EXPECT_EQ(root.size(), 1100);

}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}