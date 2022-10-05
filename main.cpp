#include<iostream>

class Dictionary {
private:
    struct Node {
        std::string key;
        int value;
        Node* left;
        Node* right;

        Node(std::string key, int value) : key(key), value(value),
            left(nullptr), right(nullptr) {}
        Node() : key(""), value(0), right(nullptr), left(nullptr) {}
        
    } *root;

    void copy_helper(Node* original, Node*& result); 
    void delete_helper(Node* cur);
    void insert_helper(Node*& cur, std::string key, int value);
    void print_helper(Node* cur);
    bool is_identical(Node* cur1, Node* cur2);

public:
    Dictionary();
    Dictionary(const Dictionary& dict);
    ~Dictionary();
    Dictionary& operator=(const Dictionary& right);

    friend bool operator==(const Dictionary& left, const Dictionary& right);
    friend bool operator!=(const Dictionary& left, const Dictionary& right);

    void print();
    /*
    Печатаем в виде
     ("apple" : 13),
     ("conscientiousness" : 27),
     ("crankshaft" : 364)
     */

    void insert(std::string key, int value);
    void find(Node*& cur, std::string key, Node*& parent);
    Node* find(std::string key);    //Находит элемент по его ключу
    //Поддерживаем уникальность! Если такой ключ есть - просто перезаписать значение
    void erase(std::string key);
};

Dictionary::Dictionary() {
    root = nullptr;
}
Dictionary::Dictionary(const Dictionary& dict) : root(nullptr) {
    copy_helper(dict.root, root);
}

void Dictionary::copy_helper(Node* original, Node*& result) {
    if (!original) {
        return;
    }
    result = new Node(original->key, original->value);
    copy_helper(original->left, result->left);
    copy_helper(original->right, result->right);
}
Dictionary::~Dictionary() {
    delete_helper(root);
}

void Dictionary::delete_helper(Node* cur) {
    if (!cur) {
        return;
    }
    delete_helper(cur->left);
    delete_helper(cur->right);
    delete cur;
}
Dictionary& Dictionary::operator=(const Dictionary& right) {
    copy_helper(right.root, root);
    return *this;
}

void Dictionary::insert(std::string key, int value) {
    insert_helper(root, key, value);
}

void Dictionary::insert_helper(Node*& cur, std::string key, int value) {
    if (!cur) {
        cur = new Node(key, value);
        return;
    }
    if (key == cur->key) {
        cur->value = value;
        return;
    }
    if (key < cur->key) {
        insert_helper(cur->left, key, value);
    }
    else {
        insert_helper(cur->right, key, value);
    }
}

void Dictionary::print() {
    print_helper(root);
    std::cout << '\n';
}

void Dictionary::print_helper(Node* cur) {
    if (!cur) {
        return;
    }
    print_helper(cur->left);
    std::cout <<"(\"" << cur->key << '"'<< " : " << cur->value << ')';
    print_helper(cur->right);
}

bool operator==(const Dictionary& left, const Dictionary& right) {
    return is_identical(left.root, right.root);
}

bool Dictionary::is_identical(Node* cur1, Node* cur2) {
    return cur1 && cur2 ? cur1->key == cur2->key && cur1->value == cur2->value 
        && is_identical(cur1->left, cur2->left)
        && is_identical(cur1->right, cur2->right) : !cur1 && !cur2;
}
 
bool operator!=(const Dictionary& left, const Dictionary& right) {
    return !(left == right);
}

Dictionary::Node* Dictionary::find(std::string key) {
    Node* cur = root;
    while (cur) {
        if (key == cur->key) {
            return cur;
        }
        else if (key < cur->key) {
             cur = cur->left;
        }
        else if (key > cur->key) {
            cur = cur->right;
        }

    }
    return nullptr;
}
void Dictionary::find(Node*& cur, std::string key, Node*& parent) {
    while (cur && cur->key != key) {
        parent = cur;
        if (key < cur->key) {
            cur = cur->left;
        }
        else {
            cur = cur->right;
        }
    }

}
void Dictionary::erase(std::string key) {
        Node* parent = nullptr;
        Node* cur = root;

        find(cur, key, parent);

        //Если ключа нет
        if (!cur) {
            return;
        }

        //Если удаляемый узел является листом
        if (!cur->left && !cur->right) {
            if (cur != root) {
                if (parent->left == cur) {
                    parent->left = nullptr;
                }
                else {
                    parent->right = nullptr;
                }
            }
            else {
                root = nullptr;
            }
            delete cur;
        }
        //Если удаляемый узел имеет два потомка
        else if (cur->left && cur->right) {
            Node* min = cur->right;
            while (min->left) {
                min = min->left;
            }
            int minval = min->value;
            erase(minval);
            cur->value = minval;
        }
        //Если удаляемый узел имеет одного потомка
        else {
            Node* child = (cur->left ? cur->left : cur->right);

            if (cur != root) {
                if (cur == parent->left) {
                    parent->left = child;
                }
                else {
                    parent->right = child;
                }
            }
            else {
                root = child;
            }
            delete cur;
        }
    }

}

int main() {
    
    Dictionary tree;
    tree.insert("ghkahg", 10);
    tree.insert("ksjhksh", 67);
    tree.insert("ghkahg", 35);
    tree.insert("zebra", 45);
    tree.insert("abra", 345);

    
    tree.print();
  
    return 0;
}
