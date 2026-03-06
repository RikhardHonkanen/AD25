#include "greet.h"
#include <gtest/gtest.h>

TEST(greetTest, noName)
{
    EXPECT_EQ("Hello, my friend.", greet(""));
    EXPECT_EQ("Hello, my friend.", greet("   $ @,   0,   . "));
}

TEST(greetTest, oneName)
{
    EXPECT_EQ("Hello, Bob.", greet("Bob"));
    EXPECT_EQ("Hello, Bob.", greet("    0Bob, @ "));

    EXPECT_EQ("HELLO BOB!", greet("BOB"));
    EXPECT_EQ("HELLO BOB!", greet("    0BOB, @ "));
}

TEST(greetTest, twoNames)
{
    EXPECT_EQ("Hello, Jill and Jane.", greet("Jill, Jane"));
    EXPECT_EQ("HELLO BOB AND JANE!", greet("BOB, JANE"));
}

TEST(greetTest, multipleNames)
{
    EXPECT_EQ("Hello, Amy, Brian, and Jane.", greet("Amy, Brian, Jane"));
    EXPECT_EQ("HELLO AMY, BRIAN, AND JANE!", greet("AMY, BRIAN, JANE"));
}

TEST(greetTest, mixedNames)
{
    EXPECT_EQ("Hello, Amy, Jane, and Jill. AND HELLO BRIAN, BOB, AND CHARLOTTE!", greet("Amy, BRIAN, Jane, BOB, Jill, CHARLOTTE"));
}