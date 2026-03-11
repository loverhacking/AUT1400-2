#ifndef BST_H
#define BST_H
#include <functional>
#include <ostream>

class BST
{
    public:
        class Node
        {
            public:
                Node(int value, Node* left, Node* right);
                Node();
                // shallow copy constructors
                Node(const Node& node);

                // Implement the << operator so that you would be able to use std::cout to print a node in one line
                // and in a beautiful way, the output should contain the node's address,
                // the node's value, node's left child address, and node's right child address.
                friend std::ostream& operator<<(std::ostream& os, const BST::Node& node);

                // implement proper functions so that every possible inequality
                // between a node and an int can be made i.e.
                bool operator==(int value) const;
                friend bool operator==(int value, const Node& node);

                bool operator!=(int value) const;
                friend bool operator!=(int value, const Node& node);

                bool operator<(int value) const;
                friend bool operator<(int value, const Node& node);

                bool operator>(int value) const;
                friend bool operator>(int value, const Node& node);

                bool operator<=(int value) const;
                friend bool operator<=(int value, const Node& node);

                bool operator>=(int value) const;
                friend bool operator>=(int value, const Node& node);

                int value;
                Node* left;
                Node* right;
        };

        BST();

        // copy constructor and copy assignment operator
        BST(const BST& bst);
        BST& operator=(const BST& bst);

        // move constructor and move assignment operator
        BST(BST&& bst) noexcept;
        BST& operator=(BST&& bst) noexcept;

        // constructor that takes an initializer list of integers
        // and adds them to the BST in the order they are given.
        BST(std::initializer_list<int> values);

        // returnts the private member variable root.
        Node*& get_root() const;

        // This function has a input of std::function,
        // this type behaves like a pointer to function
        // and can wrapp any callable objects in c++ like ordinary or lambda functions
        void bfs(std::function<void(Node*& node)> func);

        // returnts the number of nodes exist in bst.
        size_t length() const;

        // adds a new node to BST in the proper place.
        // note. the user should not be able to add a value that already exists in the tree.
        bool add_node(int value);

        // finds the node in tree with the specified value
        // and returns a pointer to the pointer of the node.
        Node** find_node(int value) const;

        // finds the parrent node of a node with specified value
        // and returns a pointer to the pointer of that parrent.
        Node** find_parrent(int value) const;

        // finds a successor of a node with specified value.
        // accordign to Test15, it actually requries the function to find the predecessor.
        // which is the rightmost node in the left subtree
        Node** find_successor(int value) const;

        // deletes a node with the specified value.
        // if there is no such node with that value return false.
        bool delete_node(int value);

        // Implement the << operator so that you would be able to use std::cout to print a BST in a beautiful way.
        // to do so print each node of the BST
        friend std::ostream& operator<<(std::ostream& os, const BST& bst);

        // implement both ++ operators so that they would add 1 to each and every nodes in the tree.
        // do not forget the difference between bst++ and ++bst.
        BST& operator++(); //++BST
        BST operator++(int); //BST++

        ~BST();

    private:
        Node* root;
};
#endif //BST_H