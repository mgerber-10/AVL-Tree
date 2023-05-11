#include "AVL.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

/*
	To check output (At the Project1 directory):
		g++ -std=c++14 -Werror -Wuninitialized -o build/test test-unit/test.cpp && build/test
*/


// Test 1: inserting a duplicate ID into the tree; should not be able to insert, height should not change
TEST_CASE("InsertingDupicateIDsTest")
{
	AVLTree T;
	T.insert("Jacob", "12345566");
	int before = T.height(T.root);
	T.insert("Marcus", "12345566");
	int after = T.height(T.root);
	REQUIRE(before == after);
}


//  Test 2: inserting names with a space (firstname lastname) works successfully; height should change as the names passed as valid and thus were inserted
TEST_CASE("InsertingNamesWithSpaceTest")
{
	AVLTree T;
	T.insert("Billy Joe", "12341234");
	REQUIRE(T.height(T.root) == 1);
	T.insert("Johnny Smith", "45674567");
	REQUIRE(T.height(T.root) == 2);
}


// Test 3: tree handles over 100 insertions
TEST_CASE("TreeHandlesOver100InsertionsTest")
{
	AVLTree T;

	// Insert 101 nodes
	for (int i = 1; i <= 101; i++)
	{
		string name = "mike" + to_string(i);
		string ufid = "00000000" + to_string(i);
		T.insert(name,ufid);
	}

	// check that all nodes were inserted
	T.inorder(T.root);
	REQUIRE(T.inorderVec.size() == 101);
}


// Test 4: Test that a right rotation is performed to balance the tree in a left-left alignment
TEST_CASE("PerformsRightRotationTest")
{
	AVLTree T;
	T.insert("Adam", "00000003");
	int firstH = T.height(T.root); 	// first height should be 1
	T.insert("Michael", "00000002");
	int secondH = T.height(T.root);	// then height should be 2
	T.insert("Jordan", "00000001");
	// In a BST, the height would be 3, but if the correct right rotation is performed, tree will self-balance and height will be 2
	REQUIRE(T.height(T.root) == secondH);
	// Test that the root node has the ufid of 00000002
	REQUIRE(T.root->ufid == "00000002");
}


// Test 5: Test that a left rotation is performed to balance the tree in a right-right alignment
TEST_CASE("PerformsLeftRotationTest")
{
	AVLTree T;
	T.insert("Jordan", "00000001");
	int firstH = T.height(T.root); 	// first height should be 1
	T.insert("Michael", "00000002");
	int secondH = T.height(T.root);	// then height should be 2
	T.insert("Adam", "00000003");
	// In a BST, the height would be 3, but if the correct left rotation is performed, tree will self-balance and height will be 2
	REQUIRE(T.height(T.root) == secondH);
	// Test that the root node has the ufid of 00000002
	REQUIRE(T.root->ufid == "00000002");
}



