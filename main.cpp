#include <algorithm>
#include <stack>
#include <vector>

template <class Type>
class AVLTree {
    struct Node {
        Type key = Type{};
        Node* left = nullptr;
        Node* right = nullptr;
        int height = 0;
    };

    Node* root_ = nullptr;

    bool Contains(Node* node, const Type& key) const {
        if (node == nullptr) {
            return false;
        }
        if (key == node->key) {
            return true;
        }
        if (key < node->key) {
            return Contains(node->left, key);
        }
        return Contains(node->right, key);
    }

    int Height(Node* const node) {
        if (node == nullptr) {
            return -1;
        }
        return node->height;
    }

    int diff(Node* const node) {
        return Height(node->left) - Height(node->right);
    }

    void LeftRotate(Node*& node) {
        Node* root_subtree = node;
        node = node->right;
        root_subtree->right = node->left;
        node->left = root_subtree;
        node->left->height = std::max(Height(node->left->left), Height(node->left->right)) + 1;
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
    }

    void RightRotate(Node*& node) {
        Node* root_subtree = node;
        node = node->left;
        root_subtree->left = node->right;
        node->right = root_subtree;
        node->right->height = std::max(Height(node->right->left), Height(node->right->right)) + 1;
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
    }

    void insert(Node*& node, const Type& key) {
        if (node == nullptr) {
            node = new Node();
            node->key = key;
            return;
        }
        if (key == node->key) {
            return;
        }
        if (key < node->key) {
            insert(node->left, key);
            node->height = std::max(Height(node->left), Height(node->right)) + 1;
            int diff_b = diff(node);
            if (diff_b == 2) {
                int diff_a = diff(node->left);
                if (diff_a == 0 or diff_a == 1) {
                    RightRotate(node);
                } else {
                    LeftRotate(node->left);
                    RightRotate(node);
                }
            }
            return;
        }
        insert(node->right, key);
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
        int diff_a = diff(node);
        if (diff_a == -2) {
            int diff_b = diff(node->right);
            if (diff_b == 0 or diff_b == -1) {
                LeftRotate(node);
            } else {
                RightRotate(node->right);
                LeftRotate(node);
            }
        }
    }

    Node* const WorkWithInorderSucessor(Node*& node) {
        if (node->left != nullptr) {
            Node* node_to_return = WorkWithInorderSucessor(node->left);
            node->height = std::max(Height(node->left), Height(node->right)) + 1;
            int diff_a = diff(node);
            if (diff_a == -2) {
                int diff_b = diff(node->right);
                if (diff_b == 0 or diff_b == -1) {
                    LeftRotate(node);
                } else {
                    RightRotate(node->right);
                    LeftRotate(node);
                }
            }
            return node_to_return;
        }
        Node* node_to_return = node;
        node = node->right;
        return node_to_return;
    }

    void remove(Node*& node, const Type& key) {
        if (node == nullptr) {
            return;
        }
        if (key == node->key) {
            if (node->left == nullptr and node->right == nullptr) {
                delete node;
                node = nullptr;
                return;
            }
            if (node->left == nullptr) {
                Node* const node_right = node->right;
                delete node;
                node = node_right;
                return;
            }
            if (node->right == nullptr) {
                Node* const node_left = node->left;
                delete node;
                node = node_left;
                return;
            }

            Node* node_returned = WorkWithInorderSucessor(node->right);
            node_returned->left = node->left;
            node_returned->right = node->right;
            delete node;
            node = node_returned;
            node->height = std::max(Height(node->left), height(node->right)) + 1;
            int diff_b = diff(node);
            if (diff_b == 2) {
                int diff_a = diff(node->left);
                if (diff_a == 0 or diff_a == 1) {
                    RightRotate(node);
                } else {
                    LeftRotate(node->left);
                    RightRotate(node);
                }
            }
            return;
        }
        if (key < node->key) {
            remove(node->left, key);
            node->height = std::max(Height(node->left), Height(node->right)) + 1;
            int diff_a = diff(node);
            if (diff_a == -2) {
                int diff_b = diff(node->right);
                if (diff_b == 0 or diff_b == -1) {
                    LeftRotate(node);
                } else {
                    RightRotate(node->right);
                    LeftRotate(node);
                }
            }
            return;
        }
        remove(node->right, key);
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
        int diff_b = diff(node);
        if (diff_b == 2) {
            int diff_a = diff(node->left);
            if (diff_a == 0 or diff_a == 1) {
                RightRotate(node);
            } else {
                LeftRotate(node->left);
                RightRotate(node);
            }
        }
    }

    void destroy(Node* const node) {
        if (node == nullptr) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

    void print(Node* const node) {
        if (node == nullptr) {
            return;
        }

        print(node->left);
        std::cout << node->key << " ";
        print(node->right);
    }

public:
    bool Contains(const Type& key) const {
        return Contains(root_, key);
    }

    void Insert(const Type& key) {
        insert(root_, key);
    }

    void Print() {
        print(root_);
    }
    void Remove(const Type& key) {
        remove(root_, key);
    }

    void Clear() {
        destroy(root_);
        root_ = nullptr;
    }

    ~AVLTree() {
        destroy(root_);
    }

    class Iterator {
        std::stack<Node*> stack_of_nodes_;

    public:
        explicit Iterator(Node* node) {
            while (node != nullptr) {
                stack_of_nodes_.push(node);
                node = node->left;
            }
        }

        const Type& operator*() const {
            return stack_of_nodes_.top()->key;
        }

        Iterator& operator++() {
            Node* node = stack_of_nodes_.top();
            stack_of_nodes_.pop();
            node = node->right;
            while (node != nullptr) {
                stack_of_nodes_.push(node);
                node = node->left;
            }
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return stack_of_nodes_ == other.stack_of_nodes_;
        }

        bool operator!=(const Iterator& other) const {
            return !operator==(other);
        }
    };

    Iterator Begin() const {
        return Iterator(root_);
    }

    Iterator End() const {
        return Iterator(nullptr);
    }
};

int main() {
    AVLTree<int> tree;
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.insert(8);
    tree.insert(9);
    tree.insert(10);
    tree.insert(4);
    tree.insert(2);
    tree.insert(3);
    tree.insert(1);

    for (auto it = tree.Begin(); it != tree.End(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    tree.remove(1);
    tree.print();
    std::cout << '\n';
    tree.remove(7);
    tree.print();
    std::cout << '\n';
    tree.remove(3);
    tree.print();
    std::cout << '\n';
    tree.remove(8);
    tree.print();
    std::cout << '\n';
    tree.remove(5);
    tree.print();
    std::cout << '\n';
    // tree.remove(1);
    // tree.print(); std::cout << '\n';
    tree.remove(9);
    tree.print();
    std::cout << '\n';
    tree.remove(2);
    tree.print();
    std::cout << '\n';
    tree.remove(2);
    tree.print();
    std::cout << '\n';
    tree.remove(6);
    tree.print();
    std::cout << '\n';
    // tree.remove(7);
    // tree.print(); std::cout << '\n';
    tree.remove(10);
    tree.print();
    std::cout << '\n';
    tree.remove(4);
    tree.print();
    std::cout << '\n';
}
