#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include "polinoms.h"
#include "list.h"

#pragma once

template <class T>

class SortVectorTable {

private:

    std::vector<std::pair<int, Polinom<T>>> data;

    auto findloc(int key) const {
        return std::lower_bound(data.begin(), data.end(), key, [](const auto& pair, int key) {return pair.first < key;});
    }

    size_t size = 0;

public:

    SortVectorTable() {};

    void insert(int key, const Polinom<T>& value) {
        auto it = findloc(key);
        data.insert(it, std::make_pair(key, value));
        ++size;
    }

    void remove(int key) {
        auto it = findloc(key);
        if (it != data.end()) {
            data.erase(it);
        }
        else {
            throw "Error";
        }
    }

    Polinom<T> find(int key) const {
        auto it = findloc(key);
        if (it != data.end()) {
            return it->second;
        }
        else {
            Polinom<int> tmp;
            return tmp;
        }
    }

    size_t Vsize() {
        return size;
    }
};

template <class T, class V>

class RedBlackTree {

private:

    enum class Color { RED, BLACK };

    class Node {
    public:
        T key;
        V value;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(T k, Color c, V v) : key(k), value(v), color(c), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;

    void rotateLeft(Node* x) {
       Node* y = x->right;
        x->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->left) {
            x->parent->left = y;
        }
        else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node* x) {
       Node* y = x->left;
        x->left = y->right;
        if (y->right != nullptr) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        }
        else if (x == x->parent->right) {
            x->parent->right = y;
        }
        else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node* z) {
        while (z != root && z->parent->color == Color::RED) {
            if (z->parent == z->parent->parent->left) {
               Node* y = z->parent->parent->right;
                if (y != nullptr && y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateRight(z->parent->parent);
                }
            }
            else {
               Node* y = z->parent->parent->left;
                if (y != nullptr && y->color == Color::RED) {
                    z->parent->color = Color::BLACK;
                    y->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    z = z->parent->parent;
                }
                else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = Color::BLACK;
                    z->parent->parent->color = Color::RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = Color::BLACK;
    }

   Node* findNode(T key) const {
       Node* current = root;
        while (current != nullptr) {
            if (current->key == key) {
                return current;
            }
            else if (current->key < key) {
                current = current->right;
            }
            else {
                current = current->left;
            }
        }
        return nullptr;
    }

    void transplant(Node* u,Node* v) {
        if (u->parent == nullptr) {
            root = v;
        }
        else if (u == u->parent->left) {
            u->parent->left = v;
        }
        else {
            u->parent->right = v;
        }
        if (v != nullptr) {
            v->parent = u->parent;
        }
    }

    void fixDelete(Node* x) {
        while (x != root && x->color == Color::BLACK) {
            if (x == x->parent->left) {
               Node* w = x->parent->right;
                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;
                }
                else {
                    if (w->right->color == Color::BLACK) {
                        w->left->color = Color::BLACK;
                        w->color = Color::RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->right->color = Color::BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            }
            else {
               Node* w = x->parent->left;

                if (w->color == Color::RED) {
                    w->color = Color::BLACK;
                    x->parent->color = Color::RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                    w->color = Color::RED;
                    x = x->parent;
                }
                else {
                    if (w->left->color == Color::BLACK) {
                        w->right->color = Color::BLACK;
                        w->color = Color::RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = Color::BLACK;
                    w->left->color = Color::BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = Color::BLACK;
    }

   Node* minimum(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    void printHelper(Node* node) {
        if (node != nullptr) {
            printHelper(node->left);
            std::cout << node->key << " ";
            printHelper(node->right);
        }
    }

public:

    RedBlackTree() : root(nullptr) {};

    void insert(T key, V value) {
       Node* z = new Node(key, Color::RED, value);
       Node* y = nullptr;
       Node* x = root;

        while (x != nullptr) {
            y = x;
            if (z->key < x->key) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }
        z->parent = y;
        if (y == nullptr) {
            root = z;
        }
        else if (z->key < y->key) {
            y->left = z;
        }
        else {
            y->right = z;
        }
        fixInsert(z);
    }

    void remove(T key) {
       Node* z = findNode(key);
        if (z == nullptr) {
            return;
        }
       Node* x;
       Node* y = z;
        Color yOriginalColor = y->color;
        if (z->left == nullptr) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nullptr) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            }
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        if (yOriginalColor == Color::BLACK) {
            fixDelete(x);
        }
        delete z;
    }

    void print() {
        printHelper(root);
    }
};