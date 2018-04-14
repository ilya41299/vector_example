#include <catch.hpp>
#include <sstream>

#include "vector.hpp"

TEST_CASE("Сreating tree")
{
	tree_t<int> My_tree;
	REQUIRE( My_tree.root() == nullptr );
}

TEST_CASE("Add elements in tree")
{
	tree_t<int> My_tree;
	My_tree.insert(7);
	My_tree.insert(3);
	My_tree.insert(9);
	My_tree.insert(6);
	std::ostringstream ostream;
	My_tree.print(ostream, 0, My_tree.root());
	std::string output{
		"---9\n"
		"7\n"
		"------6\n"
		"---3\n"};
	REQUIRE(output == ostream.str());
}

TEST_CASE("Check root tree")
{
  tree_t<int> My_tree;
  My_tree.insert(7);
  My_tree.insert(3);
  My_tree.insert(9); 

  REQUIRE(My_tree.find(9) == true);
  REQUIRE(My_tree.find(2) == false);
}


TEST_CASE("Check operator tree '+' and '=' ")
{
	tree_t<int> My_tree;
	std::ostringstream ostream;
	My_tree.check_operator(ostream, '+', 7);
	My_tree.check_operator(ostream, '+', 3);
	My_tree.check_operator(ostream, '+', 9);
	My_tree.check_operator(ostream, '+', 6);
	std::string output {
		"---9\n"
		"7\n"
		"------6\n"
		"---3\n"};
	My_tree.check_operator (ostream, '=', 0);
	
	REQUIRE(output == ostream.str());
}
TEST_CASE("Check operator tree '?' ")
{
	tree_t<int> My_tree;
	My_tree.insert(7);
	My_tree.insert(3);
	My_tree.insert(9); 
	std::ostringstream ostream;
	My_tree.check_operator (ostream, '?', 3);
	REQUIRE(ostream.str() == "true");
}
	
//
TEST_CASE(" initializer_list ")
{
	std::initializer_list <int> list{ 7, 3, 9, 6};
	tree_t<int> My_tree(list);
	std::ostringstream ostream;
	My_tree.print(ostream, 0, My_tree.root());
	std::string output {
		"---9\n"
		"7\n"
		"------6\n"
		"---3\n"};
	REQUIRE(output == ostream.str());
}

TEST_CASE(" operator== true")
{	
	std::initializer_list <int> list{ 7, 3, 9, 6};
	tree_t<int> My_tree_1(list);
	tree_t<int> My_tree_2(list);
	REQUIRE(My_tree_1 == My_tree_2);
}

TEST_CASE(" operator== false")
{	
	std::initializer_list <int> list_1{ 7, 3, 9, 6};
	std::initializer_list <int> list_2{ 7, 3, 9};
	tree_t<int> My_tree_1(list_1);
	tree_t<int> My_tree_2(list_2);
	REQUIRE(!(My_tree_1 == My_tree_2));
}

TEST_CASE("remove element 1")
{	
	std::initializer_list <int> list_1{ 9, 3, 10};
	tree_t<int> My_tree_1(list_1); 
	My_tree_1.remove(3);
	std::ostringstream ostream;
	My_tree_1.print(ostream, 0, My_tree_1.root());
	std::string output {
		"---10\n"
		"9\n"};
	REQUIRE(output == ostream.str());
}

TEST_CASE("remove element 2")
{	
	std::initializer_list <int> list_1{ 5, 4, 6, 3, 2, 8, 7 };
	tree_t<int> My_tree_1(list_1); 
	My_tree_1.remove(6);
	std::ostringstream ostream;
	My_tree_1.print(ostream, 0, My_tree_1.root());
	std::string output {
		"---8\n"
		"------7\n"
		"5\n"
		"---4\n"
		"------3\n"	
		"---------2\n"};
	REQUIRE(output == ostream.str());
}


TEST_CASE("remove element 3")
{	
	std::initializer_list <int> list_1{ 10, 13, 17, 16, 15, 14, 22, 20, 23, 12, 11 };
	tree_t<int> My_tree_1(list_1); 
	My_tree_1.remove(13);
	std::ostringstream ostream;
	My_tree_1.print(ostream, 0, My_tree_1.root());
	std::string output {
		"------------23\n"
		"---------22\n"
		"------------20\n"
		"------17\n"
		"---------16\n"	
		"------------15\n"
		"---14\n"
		"------12\n"
		"---------11\n"
		"10\n"};
	REQUIRE(output == ostream.str());
}
