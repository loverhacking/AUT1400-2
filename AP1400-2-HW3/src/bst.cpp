#include "bst.h"
#include <queue>
#include <iomanip>


BST::Node::Node(int value, Node* left, Node* right) {
    this->value = value;
    this->left = left;
    this->right = right;
}

BST::Node::Node() {
    this->value = 0;
    this->left = nullptr;
    this->right = nullptr;
}

BST::Node::Node(const Node& node) {
    this->value = node.value;
	// shallow copy of left and right pointers
    this->left = node.left;
    this->right = node.right;
}

std::ostream& operator<<(std::ostream& os, const BST::Node& node) {
    os << "Node: addr=" << &node
       << ", value=" << node.value
       << ", left=" << node.left
       << ", right=" << node.right
       << std::endl;
    return os;
}

bool BST::Node::operator==(int value) const {
    return this->value == value;
}

bool operator==(int value, const BST::Node& node) {
    return node == value;
}

bool BST::Node::operator!=(int value) const {
    return !(*this == value);
}

bool operator!=(int value, const BST::Node& node) {
    return node != value;
}

bool BST::Node::operator<(int value) const {
    return this->value < value;
}

bool operator<(int value, const BST::Node& node) {
    return value < node.value;
}

bool BST::Node::operator>(int value) const {
    return this->value > value;
}

bool operator>(int value, const BST::Node& node) {
    return value > node.value;
}

bool BST::Node::operator<=(int value) const {
    return this->value <= value;
}

bool operator<=(int value, const BST::Node& node) {
    return value < node.value || value == node.value;
}

bool BST::Node::operator>=(int value) const {
    return this->value >= value;
}

bool operator>=(int value, const BST::Node& node) {
    return value > node.value || value == node.value;
}

// helper function to copy a tree recursively
void copy_tree(BST& bst, const BST::Node* root) {
    if (root == nullptr) {
        return;
    }
    bst.add_node(root->value);
    copy_tree(bst, root->left);
    copy_tree(bst, root->right);
}

// helper function to delete a tree recursively
static void delete_tree(BST::Node* root) {
    if (root == nullptr) return;
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}

BST::BST() {
    this->root = nullptr;
}

BST::BST(const BST& bst) {
    if (this != &bst) {
        this->root = nullptr;
        copy_tree(*this, bst.root);
    }
}

BST& BST::operator=(const BST& bst) {
    if (this != &bst) {
        delete_tree(this->root);
        copy_tree(*this, bst.root);
    }
    return *this;
}

BST::BST(BST&& bst) noexcept {
    this->root = std::move(bst.root);
    bst.root = nullptr;
}

BST& BST::operator=(BST&& bst) noexcept {
    if (this != &bst) {
        delete_tree(this->root);
        this->root = std::move(bst.root);
        bst.root = nullptr;
    }
    return *this;
}

BST::BST(std::initializer_list<int> values) : BST() {
    for (int value : values) {
        this->add_node(value);
    }
}

BST::Node*& BST::get_root() const {
    return const_cast<BST::Node*&>(root);
}

void BST::bfs(std::function<void(Node*& node)> func) {
    if (this->root == nullptr) {
        return;
    }
    std::queue<Node**> q;
    q.push(&this->root);
    while (!q.empty()) {
        Node** node = q.front();
        q.pop();
        func(*node);
        if ((*node)->left != nullptr) {
            q.push(&(*node)->left);
        }
        if ((*node)->right != nullptr) {
            q.push(&(*node)->right);
        }
    }
}

size_t BST::length() const{
    size_t count = 0;
    Node* root = this->root;
    if (root == nullptr) {
		return 0;
	}
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
        Node* node = q.front();
        q.pop();
        count++;
        if (node->left != nullptr) {
            q.push(node->left);
        }
        if (node->right != nullptr) {
            q.push(node->right);
        }
    }
    return count;
}

bool BST::add_node(int value) {
    if (this->root == nullptr) {
        this->root = new Node(value, nullptr, nullptr);
        return true;
    }
    Node** current = &this->root;
    while (*current != nullptr) {
        if ((*current)->value == value) {
            return false; // value already exists
        }
        if ((*current)->value > value) {
            current = &(*current)->left;
        } else {
            current = &(*current)->right;
        }
    }
    *current = new Node(value, nullptr, nullptr);
    return true;
}

BST::Node** BST::find_node(int value) const {
    Node** current = const_cast<Node**>(&this->root);
    while (*current != nullptr) {
        if ((*current)->value == value) {
            return current;
        }
        if ((*current)->value > value) {
            current = &(*current)->left;
        } else {
            current = &(*current)->right;
        }
    }
    return nullptr; // not found
}

BST::Node** BST::find_parrent(int value) const {
    Node** current = const_cast<Node**>(&this->root);
    Node** parent = nullptr;
    while (*current != nullptr) {
        if (**current == value) {
            return parent;
        }
        parent = current;
        if (**current > value) {
            current = &(*current)->left;
        } else {
            current = &(*current)->right;
        }
    }
    return nullptr; // not found
}


BST::Node** BST::find_successor(int value) const {
    Node** node = this->find_node(value);
    if (node == nullptr || (*node)->left == nullptr) {
        return nullptr; // no successor
    }
    // find the rightmost node in the left subtree
    Node** successor = &(*node)->left;
    while ((*successor)->right != nullptr) {
        successor = &(*successor)->right;
    }
    return successor;
}

bool BST::delete_node(int value) {
    Node** node = this->find_node(value);
    if (node == nullptr) {
        return false; // not found
    }
    if ((*node)->left == nullptr && (*node)->right == nullptr) {
        delete *node;
        *node = nullptr;
    } else if ((*node)->left == nullptr) {
        Node* temp = *node;
        *node = (*node)->right;
        delete temp;
    } else if ((*node)->right == nullptr) {
        Node* temp = *node;
        *node = (*node)->left;
        delete temp;
    } else {
		// find the rightmost node in the left subtree
        Node** successor = this->find_successor(value);
        (*node)->value = (*successor)->value; // copy successor's value to current node
        // delete successor node
        Node* temp = *successor;
        if ((*successor)->left != nullptr) {
            *successor = (*successor)->left; // successor has left child
        } else {
            *successor = nullptr; // successor is a leaf
        }
        delete temp;
    }
    return true;
}

std::ostream& operator<<(std::ostream& os, const BST& bst) {
    os << std::string(80, '*') << std::endl;
    BST::Node* root = bst.get_root();
    size_t size = 0;
    if (root != nullptr) {
        std::queue<BST::Node*> q;
        q.push(root);
        while (!q.empty()) {
            BST::Node* node = q.front();
            q.pop();
            size++;
            os << " " << static_cast<void*>(node) << "       => value:" << node->value << "        left:";
            if (node->left != nullptr) {
                os << static_cast<void*>(node->left);
                q.push(node->left);
            } else {
                os << "0";
            }
            os << "      right:";
            if (node->right != nullptr) {
                os << static_cast<void*>(node->right);
                q.push(node->right);
            } else {
                os << "0";
            }
            os << "      " << std::endl;
        }
    }
    os << " binary search tree size: " << size << std::endl;
    os << std::string(80, '*') << std::endl;
    return os;
}

//++BST
BST& BST::operator++() {
    bfs([](Node*& node) {
        node->value++;
    });
    return *this;
}

//BST++
BST BST::operator++(int) {
    BST temp(*this); // create a copy of the current state
    bfs([](Node*& node) {
        node->value++;
    });
    return temp; // return the copy (the state before increment)
}

BST::~BST() {
    std::vector<Node*> nodes;
    bfs([&nodes](BST::Node*& node){nodes.push_back(node);});
    for(auto& node: nodes)
        delete node;
}

