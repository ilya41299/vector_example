#include <catch.hpp>
#include <sstream>

#include "tree.hpp"

TEST_CASE("creating tree")
{
    rb_tree<int> tree;
    REQUIRE(tree.root() == nullptr);
}

TEST_CASE("Test delete 1")
{
    rb_tree<int> tree{ 1, 2, 3, 4, 5 };

    tree.remove(4);

    std::string out{

        "----5\n"
        "------3\n"
        "--2\n"
        "----1\n"
    };

    std::ostringstream ostream;
    tree.print(ostream, 0, tree.root());

    REQUIRE(ostream.str() == out);
}

TEST_CASE("Test delete 2")
{
    rb_tree<int> tree{ 7, 3, 8, 9, 1, 0 };

    tree.remove(1);

    std::string out{ "------9\n"
                     "----8\n"
                     "--7\n"
                     "----3\n"
                     "------0\n" };
    std::ostringstream ostream;
    tree.print(ostream, tree.root(), 1);

    REQUIRE(ostream.str() == out);
}

TEST_CASE("Test case 5")
{
    rb_tree<int> tree{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0 };

    tree.remove(3);
    tree.remove(2);

    std::string out{ "--------11\n"
                     "------10\n"
                     "--------9\n"
                     "----8\n"
                     "------7\n"
                     "--6\n"
                     "------5\n"
                     "----4\n"
                     "------1\n"
                     "--------0\n"

    };

    std::ostringstream ostream;
    tree.print(ostream, 0, tree.root());

    REQUIRE(ostream.str() == out);
}

TEST_CASE("Test case 3")
{
    rb_tree<int> tree{ 5, 3, 7, 8 };

    tree.remove(8);
    tree.remove(3);

    std::string out{ "----7\n"
                     "--5\n" };

    std::ostringstream ostream;
    tree.print(ostream, 0, tree.root());

    REQUIRE(ostream.str() == out);
}

TEST_CASE("Test case 2")
{
    rb_tree<int> tree{ 5, 3, 7, 8, 6, 2, 1, 9 };

    tree.remove(2);

    std::string out{ "--------9\n"
                     "------8\n"
                     "----7\n"
                     "------6\n"
                     "--5\n"
                     "----3\n"
                     "------1\n" };

    std::ostringstream ostream;
    tree.print(ostream, 0, tree.root());

    REQUIRE(ostream.str() == out);
}
