#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

//=====================================================//
//              AVLTree Class Header                   //
//=====================================================//
class AVLTree 
{
    private:

        // TreeNode struct for storing data
        struct TreeNode
        {
            string name;
            string ufid;
            int balanceFactor;
            TreeNode* left;
            TreeNode* right;
            TreeNode() : name(""), ufid(""), balanceFactor(0), left(nullptr), right(nullptr) {};
        };
        
        // Helper function to insert "node" into the AVLTree
        TreeNode* insertHelper(TreeNode* node, string name, string ufid); 
        
        // Helper functions for print Traversals
        void inorderHelper(TreeNode* node, vector<string>& vec);
        void preorderHelper(TreeNode* node, vector<string>& vec);
        void postorderHelper(TreeNode* node, vector<string>& vec);
        
        // Helper functions to search by name and id for a node in the AVLTree
        void searchNameHelper(TreeNode* node, string name, vector<string>& ids);
        TreeNode* searchIdHelper(TreeNode* node, string ufid);

        // Helper function to remove "node" from the AVLTree
        TreeNode* removeHelper(TreeNode* node, string ufid);

        // Helper function to remove "n"th node in inorder traversal from AVLTree
        TreeNode* removeInorderHelper(TreeNode* node, int n);

    public:
        
        // Pointer for storing root node of tree  
        TreeNode* root;                        

        // Default constructor
        AVLTree(){root = nullptr;};                                  

        // Success strings
        string success = "successful";         
        string unsuccess = "unsuccessful";      

        // Helper functions to determine height, balance factor, and smallest (leftmost) node of a tree
        int height(TreeNode* node);             
        int getBalanceFactor(TreeNode* node);   
        TreeNode* minNode(TreeNode* node);

        // Rotation functions
        TreeNode* rotateLeft(TreeNode* node);   
        TreeNode* rotateRight(TreeNode* node);
        TreeNode* rotateLeftRight(TreeNode* node);
        TreeNode* rotateRightLeft(TreeNode* node);

        // Insert function
        void insert(string name, string ufid);

        // Print traversal functions 
        void printInorder();
        void printPreorder();
        void printPostOrder();
        void printLevelCount();

        // Search functions
        void searchName(string name);
        void searchId(string ufid);

        // Remove functions
        void remove(string ufid);
        void removeInorder(int n);

        // vector for storing inorder traversal of all nodes in the tree (used for removeInorder)
        vector<TreeNode*> inorderVec;

        // helper function for populating "inorderVec" with the inorder traversal nodes (used for removeInorder)
        void inorder(TreeNode* node);
};


//=====================================================//
//   height, balanceFactor, minNode Helper Functions   //
//=====================================================//

// returns height of a subtree with node as its root node; O(log n)
int AVLTree::height(TreeNode* node)
{
    if (node == nullptr)
        return 0;
    else
    {
        // recursively check height of left subtree then right subtree
        int leftHeight = height(node->left);
        int rightHeight = height(node->right);

        return (max(leftHeight,rightHeight) + 1);
    }
}


// checks balance factor of a given node; O(log n)
int AVLTree::getBalanceFactor(TreeNode* node)
{
    // balance factor = height of nodes left subtree - height of nodes right subtree
    int heightLeftSubTree = height(node->left);
    int heightRightSubTree = height(node->right); 

    return (heightLeftSubTree - heightRightSubTree);
}


// returns the minimum value node (smallest node) of a tree, used for removing a node with 2 children; O(log n)
AVLTree::TreeNode* AVLTree::minNode(TreeNode* node)
{
    TreeNode* currNode = node;
    
    // traverse down leftmost path of the left subtree until nullptr is reached
    while(currNode != nullptr && currNode->left != nullptr)
    {
        currNode = currNode->left;
    }
    return currNode;
}


//=====================================================//
//              Rotation Function Definitions           //
//=====================================================//

// given tree with a right-right alignment, returns updated tree after a left rotation; O(1)
AVLTree::TreeNode* AVLTree::rotateLeft(TreeNode* node)
{
    TreeNode* grandChild = node->right->left;
    TreeNode* newParent = node->right;
    newParent->left = node;
    node->right = grandChild;
    return newParent;
}


// given tree with a left-left alignment, returns updated tree after a right rotation; O(1)
AVLTree::TreeNode* AVLTree::rotateRight(TreeNode* node)
{
    TreeNode* grandChild = node->left->right;
    TreeNode* newParent = node->left;
    newParent->right = node;
    node->left = grandChild;
    return newParent;
}


// given tree with a left-right alignment, returns updated tree after a left-right rotation; O(1)
AVLTree::TreeNode* AVLTree::rotateLeftRight(TreeNode* node)
{
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}


// given tree with a right-left alignment, returns updated tree after a right-left rotation; O(1)
AVLTree::TreeNode* AVLTree::rotateRightLeft(TreeNode* node)
{
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}


//=====================================================//
//              Insert Function Definitions            //
//=====================================================//

// Helper function to insert "node" into the AVLTree (using recursion) ; O(log n)
AVLTree::TreeNode* AVLTree::insertHelper(TreeNode* node, string name, string ufid)
{

    // if tree is empty, add new root
    if (root == nullptr)
    {
        TreeNode* newRoot = new TreeNode();
        newRoot->ufid = ufid;
        newRoot->name = name;
        root = newRoot;
        cout << success << endl;
        return newRoot;
    }


    // if node is empty, add new node
    if (node == nullptr)
    {
        TreeNode* newNode = new TreeNode();
        newNode->ufid = ufid;
        newNode->name = name;
        cout << success << endl;
        return newNode;
    }


    // convert ufid we want to insert to an integer "key" for comparing with the ufid currently in the node
    int key = stoi(ufid);

    // Also convert current ufid to an integer for comparing with this previous "key"
    int currId = stoi(node->ufid);

    // check through node's children until a leaf is reached, and insert if unique 
    if (key < currId)
    {
        // recursively insert into left subtree 
        node->left = insertHelper(node->left, name, ufid);
    }
    else if (key > currId)
    {
        // recursively insert into right subtree 
        node->right = insertHelper(node->right, name, ufid);
    }
    else
    {
        // else, duplicate "ufid" CANNOT INSERT, returns original node
        cout << unsuccess << endl;
        return node; 
    }


    // check the balance factor & perform rotations if necessary
    node->balanceFactor = getBalanceFactor(node);
    int balance = node->balanceFactor;


    // rotate if necessary:

    // Tree is RIGHT heavy
    if (balance < -1)
    {
        if (key < stoi(node->right->ufid))
        {
            // Right-Left Alignment
            return rotateRightLeft(node);           
        }
        else if (key > stoi(node->right->ufid))
        {
            // Right-Right Alignment
            return rotateLeft(node);
        }
    }

    // Tree is LEFT heavy
    if (balance > 1)
    {
        if (key < stoi(node->left->ufid))
        {
            // Left-Left Alignment
            return rotateRight(node);
        }
        else if (key > stoi(node->left->ufid))
        {
            // Left-Right Alignment
            return rotateLeftRight(node);
        }
    }
    return node;
}


// inserts the given name and id into the tree; O(log n)
void AVLTree::insert(string name, string ufid) 
{
    this->root = insertHelper(this->root, name, ufid);
}   


//=====================================================//
//           Traversal Function Definitions            //
//=====================================================//

// recursive helper function to preorder traverse (NLR) the tree and add the traversal to a vector; O(n)
void AVLTree::preorderHelper(TreeNode* node, vector<string>& vec)
{
    if (node == nullptr)
    {
        return;
    }
    else
    {
        vec.push_back(node->name);          // N
        preorderHelper(node->left, vec);    // L
        preorderHelper(node->right, vec);   // R
    }
}


// recursive helper function to inorder traverse (LNR) the tree and add the traversal to a vector; O(n)
void AVLTree::inorderHelper(TreeNode* node, vector<string>& vec)
{
    if (node == nullptr)
    {
        return;
    }
    else
    {
        inorderHelper(node->left, vec);     // L
        vec.push_back(node->name);          // N
        inorderHelper(node->right, vec);    // R
    }
}


// recursive helper function to postorder traverse (LRN) the tree and add the traversal to a vector; O(n)
void AVLTree::postorderHelper(TreeNode* node, vector<string>& vec)
{
    if (node == nullptr)
    {
        return;
    }
    else
    {
        postorderHelper(node->left, vec);   // L
        postorderHelper(node->right, vec);  // R
        vec.push_back(node->name);          // N
    }
}


// prints preorder traversal of the AVLTree; O(n)
void AVLTree::printPreorder()
{
    if (root == nullptr)
    {
        return;
    }

    // create vector for storing preorder traversal & populate it by calling preorder helper function
    vector<string> preVec;
    preorderHelper(root, preVec);

    // for each item in preVec (contains Preorder traversal), print it
    for(int i = 0; i < preVec.size() - 1; i++)
    {
        cout << preVec[i] << ", ";
    }
    cout << preVec[preVec.size() - 1] << endl;
}


// prints inorder traversal of the AVLTree; O(n)
void AVLTree::printInorder()
{
    if (root == nullptr)
    {
        return;
    }

    // create vector for storing inorder traversal & populate it by calling inorder helper function
    vector<string> inVec;
    inorderHelper(root, inVec);

    // for each item in inVec (contains Inorder traversal), print it
    for(int i = 0; i < inVec.size() - 1; i++)
    {
        cout << inVec[i] << ", ";
    }
    cout << inVec[inVec.size() - 1] << endl;
}


// prints postorder traversal of the AVLTree; O(n)
void AVLTree::printPostOrder()
{
    if (root == nullptr)
    {
        return;
    }
    
    // create vector for storing postorder traversal & populate it by calling postorder helper function
    vector<string> postVec;
    postorderHelper(root, postVec);

    // for each item in postVec (contains Postorder traversal), print it
    for(int i = 0; i < postVec.size() - 1; i++)
    {
        cout << postVec[i] << ", ";
    }
    cout << postVec[postVec.size() - 1] << endl;
}


// prints number of levels that exist in the tree; O(log n)
void AVLTree::printLevelCount()
{
    int levelCount;

    // if tree is empty, levelCount = 0
    if (root == nullptr)
    {
        levelCount = 0;
        cout << levelCount << endl;
    }
    // Else, levelCount = height of the root node
    else
    {
        levelCount = height(root);
        cout << levelCount << endl;
    }
}


//=====================================================//
//           Search Function Definitions               //
//=====================================================//

// Helper function using preorder traversal to search for "node" in the AVLTree w/ specified "name"; O(n)
void AVLTree::searchNameHelper(TreeNode* node, string name, vector<string>& ids)
{
    // if node to search for is empty, return nullptr
    if (node == nullptr)
    {
        return;
    }

    // else, recursively preorder traverse (NLR) through the tree until it is found
        
    // if node is found, return it (N)
    if (node->name == name)
    {
        ids.push_back(node->ufid);
    }

    // recursively check left subtree (L)
    searchNameHelper(node->left, name, ids);

    // recursively check right subtree (R)
    searchNameHelper(node->right, name, ids);
}


// Searches for "name" in the tree; O(n)
void AVLTree::searchName(string name)
{
    // create vector for storing preorder traversal of found names, and call to searchName helper to populate it
    vector<string> preorderAns;
    searchNameHelper(root, name, preorderAns);

    //if found prints associated "ufid", otherwise prints "unsuccessful"
    if(preorderAns.size() == 0)
    {
        // name was not found
        cout << unsuccess << endl;
    }
    else
    {
        // name was found, print the associated ufid
        for (int i = 0; i < preorderAns.size(); i++)
        {
            cout << preorderAns[i] << endl;
        }
    }
}


// Helper function uses recursion to search for "node" in the AVLTree w/ specified "ufid" ; O(log n)
AVLTree::TreeNode* AVLTree::searchIdHelper(TreeNode* node, string ufid)
{
    // if node to search for is empty, return nullptr
    if (node == nullptr)
    {
        return nullptr;
    }

    // if node is found, return it
    if (node->ufid == ufid)
    {
        return node;
    }

    // else, recursively check through the nodes children until it is found, else return nullptr
    int key = stoi(ufid);

    // recursively search through left subtree
    if (key < stoi(node->ufid))
    {
        return searchIdHelper(node->left, ufid);
    }
    // recursively search through right subtree
    else if (key > stoi(node->ufid))
    {
        return searchIdHelper(node->right, ufid);
    }

    // return nullptr if ufid cant be found
    return nullptr;
}


// Searches for "ufid" in the tree; O(log n)
void AVLTree::searchId(string ufid)
{
    TreeNode* foundNode = searchIdHelper(root, ufid);
    
    // if found prints associated "name", otherwise prints "unsuccessful"
    if(foundNode == nullptr)
    {
        // name was not found
        cout << unsuccess << endl;
    }
    else
    {
        // name was found, print the associated name
        cout << foundNode->name << endl;
    }
}


//=====================================================//
//           Remove Function Definitions               //
//=====================================================//

// Helper function to remove "node" from the AVLTree; O(log n)
AVLTree::TreeNode* AVLTree::removeHelper(TreeNode* node, string ufid)
{
    if (node == nullptr)
    {
        // node is not in tree
        return nullptr; 
    }

    // variables to compare the passed in ufid to remove and the current ufid in the node
    int key = stoi(ufid);
    int currId = stoi(node->ufid);

    if (key < currId)
    {
        // return result of deleting from left subtree
        node->left = removeHelper(node->left, ufid);
    }
    else if (key > currId)
    {
        // return result of deleting from right subtree
        node->right = removeHelper(node->right, ufid);
    }
    else
    {
        // item is found, store local root in "tempNode"

        // if local root has no children, 
        if (node->left == nullptr && node->right == nullptr)
        {
            // set parent of local root to nullptr
            node = nullptr;
            return node;
        }

        // local root has 1 right child
        else if (node->left == nullptr)
        {
            // set parent of local root to reference that child
            TreeNode* tempNode = node->right;
            delete node;
            return tempNode;
        }

        // local root has 1 left child
        else if (node->right == nullptr)
        {
            // set parent of local root to reference that child
            TreeNode* tempNode = node->left;
            delete node;
            return tempNode;
        }

        // local root has 2 children
        else
        {
            // find inorder successor to replace removed node; inorder successor = minimum node of right subtree
            TreeNode* tempNode = minNode(node->right);

            // copy its data into local roots data 
            node->name = tempNode->name;
            node->ufid = tempNode->ufid;

            // remove the inorder successor
            node->right = removeHelper(node->right, tempNode->ufid);
        }
    }
    return node;
}


// removes node with given "ufid" from the tree, if it exists; O(log n)
void AVLTree::remove(string ufid)
{
    this->root = removeHelper(this->root, ufid);

    if (removeHelper(this->root,ufid) != nullptr)
    {
        cout << success << endl;
    }
    else
    {
        cout << unsuccess << endl;
    }
}


// fills "inorderVec" with the inorder traversal (LNR) of the nodes in the tree; O(n)
void AVLTree::inorder(TreeNode* node)
{
    if (node == nullptr)
    {
        return;
    }
    else
    {
        inorder(node->left);          // L
        inorderVec.push_back(node);   // N
        inorder(node->right);         // R
    }
}


// Helper function to remove "n"th node in inorder traversal from AVLTree; O(n)
AVLTree::TreeNode* AVLTree::removeInorderHelper(TreeNode* node, int n)
{
    // if node is empty, return nullptr
    if (node == nullptr)
    {
        return nullptr;
    }

    // fill "inorderVec" with the inorder traversal of all nodes in the AVLTree
    inorder(root);

    if (n >= inorderVec.size())
    {
        // n is larger than the number of nodes in the tree, and so the node does not exist
        cout << unsuccess << endl;
        return node;
    }

    // create variable of the node to be removed by calling the n'th node in "inorderVec"
    TreeNode* nodeToRemove = inorderVec[n];
    
    // return the result of removing this node
    cout << success << endl;
    return removeHelper(root, nodeToRemove->ufid);
}


// removes the n'th ufid in the inorder traversal of the tree; O(n)
void AVLTree::removeInorder(int n)
{
    removeInorderHelper(root, n);
}
