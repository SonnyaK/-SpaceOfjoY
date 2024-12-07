#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <stdexcept>
#include <locale>
#include <windows.h>
#include <cwctype> // Для iswdigit и iswspace
#include <algorithm> // Для std::max

class TreeNode {
public:
    int key;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int item) : key(item), left(nullptr), right(nullptr) {}
};

class AVLTreeNode : public TreeNode {
public:
    int height;

    AVLTreeNode(int item) : TreeNode(item), height(1) {}
};

class BinaryTree {
public:
    TreeNode* root;

    BinaryTree() : root(nullptr) {}

    void setRoot(TreeNode* root) {
        this->root = root;
    }

    void inOrderTraversal(TreeNode* node) {
        if (node != nullptr) {
            inOrderTraversal(node->left);
            std::wcout << node->key << L" ";
            inOrderTraversal(node->right);
        }
    }

    void preOrderTraversal(TreeNode* node) {
        if (node != nullptr) {
            std::wcout << node->key << L" ";
            preOrderTraversal(node->left);
            preOrderTraversal(node->right);
        }
    }

    void postOrderTraversal(TreeNode* node) {
        if (node != nullptr) {
            postOrderTraversal(node->left);
            postOrderTraversal(node->right);
            std::wcout << node->key << L" ";
        }
    }
};

class AVLTree : public BinaryTree {
public:
    AVLTreeNode* root;

    AVLTree() : root(nullptr) {}

    void setRoot(AVLTreeNode* root) {
        this->root = root;
    }

    void insert(int key) {
        root = insertRecursive(root, key);
    }

    void deleteNode(int key) {
        root = deleteRecursive(root, key);
    }

    void levelOrderTraversal() {
        if (root == nullptr) return;
        std::queue<AVLTreeNode*> queue;
        queue.push(root);
        while (!queue.empty()) {
            AVLTreeNode* node = queue.front();
            queue.pop();
            std::wcout << node->key << L" ";
            if (node->left != nullptr) queue.push((AVLTreeNode*)node->left);
            if (node->right != nullptr) queue.push((AVLTreeNode*)node->right);
        }
    }

private:
    AVLTreeNode* insertRecursive(AVLTreeNode* node, int key) {
        if (node == nullptr) {
            std::wcout << L"Вставка нового узла с ключом " << key << std::endl;
            return new AVLTreeNode(key);
        }
        if (key < node->key) {
            std::wcout << L"Переход в левое поддерево узла с ключом " << node->key << std::endl;
            node->left = insertRecursive((AVLTreeNode*)node->left, key);
        }
        else if (key > node->key) {
            std::wcout << L"Переход в правое поддерево узла с ключом " << node->key << std::endl;
            node->right = insertRecursive((AVLTreeNode*)node->right, key);
        }
        else {
            std::wcout << L"Узел с ключом " << key << L" уже существует" << std::endl;
            return node;
        }
        node->height = 1 + std::max<int>(height((AVLTreeNode*)node->left), height((AVLTreeNode*)node->right));
        std::wcout << L"Балансировка узла с ключом " << node->key << std::endl;
        return balance(node);
    }

    AVLTreeNode* deleteRecursive(AVLTreeNode* node, int key) {
        if (node == nullptr) {
            return node;
        }
        if (key < node->key) {
            node->left = deleteRecursive((AVLTreeNode*)node->left, key);
        }
        else if (key > node->key) {
            node->right = deleteRecursive((AVLTreeNode*)node->right, key);
        }
        else {
            if (node->left == nullptr) {
                return (AVLTreeNode*)node->right;
            }
            else if (node->right == nullptr) {
                return (AVLTreeNode*)node->left;
            }
            node->key = minValue((AVLTreeNode*)node->right);
            node->right = deleteRecursive((AVLTreeNode*)node->right, node->key);
        }
        if (node == nullptr) {
            return node;
        }
        node->height = 1 + std::max<int>(height((AVLTreeNode*)node->left), height((AVLTreeNode*)node->right));
        return balance(node);
    }

    int height(AVLTreeNode* node) {
        return node == nullptr ? 0 : node->height;
    }

    AVLTreeNode* balance(AVLTreeNode* node) {
        int balanceFactor = getBalance(node);
        if (balanceFactor > 1) {
            if (getBalance((AVLTreeNode*)node->left) >= 0) {
                std::wcout << L"Правый поворот узла с ключом " << node->key << std::endl;
                return rotateRight(node);
            }
            else {
                std::wcout << L"Левый поворот узла с ключом " << node->left->key << std::endl;
                node->left = rotateLeft((AVLTreeNode*)node->left);
                std::wcout << L"Правый поворот узла с ключом " << node->key << std::endl;
                return rotateRight(node);
            }
        }
        if (balanceFactor < -1) {
            if (getBalance((AVLTreeNode*)node->right) <= 0) {
                std::wcout << L"Левый поворот узла с ключом " << node->key << std::endl;
                return rotateLeft(node);
            }
            else {
                std::wcout << L"Правый поворот узла с ключом " << node->right->key << std::endl;
                node->right = rotateRight((AVLTreeNode*)node->right);
                std::wcout << L"Левый поворот узла с ключом " << node->key << std::endl;
                return rotateLeft(node);
            }
        }
        return node;
    }

    int getBalance(AVLTreeNode* node) {
        return node == nullptr ? 0 : height((AVLTreeNode*)node->left) - height((AVLTreeNode*)node->right);
    }

    AVLTreeNode* rotateLeft(AVLTreeNode* y) {
        AVLTreeNode* x = (AVLTreeNode*)y->right;
        AVLTreeNode* T2 = (AVLTreeNode*)x->left;
        x->left = y;
        y->right = T2;
        y->height = std::max<int>(height((AVLTreeNode*)y->left), height((AVLTreeNode*)y->right)) + 1;
        x->height = std::max<int>(height((AVLTreeNode*)x->left), height((AVLTreeNode*)x->right)) + 1;
        return x;
    }

    AVLTreeNode* rotateRight(AVLTreeNode* x) {
        AVLTreeNode* y = (AVLTreeNode*)x->left;
        AVLTreeNode* T2 = (AVLTreeNode*)y->right;
        y->right = x;
        x->left = T2;
        x->height = std::max<int>(height((AVLTreeNode*)x->left), height((AVLTreeNode*)x->right)) + 1;
        y->height = std::max<int>(height((AVLTreeNode*)y->left), height((AVLTreeNode*)y->right)) + 1;
        return y;
    }

    int minValue(AVLTreeNode* node) {
        int minValue = node->key;
        while (node->left != nullptr) {
            minValue = node->left->key;
            node = (AVLTreeNode*)node->left;
        }
        return minValue;
    }
};

class TreeParser {
public:
    static TreeNode* parse(const std::wstring& input) {
        index = 0;
        return parseRecursive(input);
    }

private:
    static int index;

    static TreeNode* parseRecursive(const std::wstring& input) {
        skipSpaces(input);
        if (index >= input.length() || input[index] == L')') {
            return nullptr;
        }

        index++; // skip '('
        TreeNode* node = new TreeNode(std::stoi(extractNumber(input)));
        std::wcout << L"Создан узел с ключом " << node->key << std::endl;

        node->left = parseRecursive(input);
        node->right = parseRecursive(input);

        skipSpaces(input);
        if (index < input.length() && input[index] == L')') {
            index++; // skip ')'
        }
        else {
            throw std::invalid_argument("Неверное представление дерева");
        }
        return node;
    }

    static std::wstring extractNumber(const std::wstring& input) {
        std::wstringstream num;
        while (index < input.length() && std::iswdigit(input[index])) {
            num << input[index++];
        }
        return num.str();
    }

    static void skipSpaces(const std::wstring& input) {
        while (index < input.length() && std::iswspace(input[index])) {
            index++;
        }
    }
};

int TreeParser::index = 0;

void inOrderInsertToAVL(TreeNode* node, AVLTree* avlTree) {
    if (node != nullptr) {
        inOrderInsertToAVL(node->left, avlTree);
        avlTree->insert(node->key);
        inOrderInsertToAVL(node->right, avlTree);
    }
}

int main() {
    
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

  
    std::locale::global(std::locale("ru_RU.UTF-8"));
    std::wcout.imbue(std::locale());
    std::wcin.imbue(std::locale());

    try {
        std::wstring filePath = L"C:\\Users\\KDFX Modes\\Desktop\\tree.txt";
        std::wifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Невозможно открыть файл");
        }

        std::wstring input;
        std::getline(file, input);
        file.close();

        // Парсинг скобочной записи и создание двоичного дерева
        std::wcout << L"Парсинг скобочной записи: " << input << std::endl;
        TreeNode* binaryTreeRoot = TreeParser::parse(input);
        BinaryTree binaryTree;
        binaryTree.setRoot(binaryTreeRoot);

        std::wcout << L"Центральный обход двоичного дерева:" << std::endl;
        binaryTree.inOrderTraversal(binaryTreeRoot);
        std::wcout << std::endl;

        // Создание АВЛ дерева на основе двоичного дерева
        AVLTree avlTree;
        std::wcout << L"Вставка узлов в АВЛ дерево:" << std::endl;
        inOrderInsertToAVL(binaryTreeRoot, &avlTree);

        std::wcout << L"Обход в ширину АВЛ дерева:" << std::endl;
        avlTree.levelOrderTraversal();
        std::wcout << std::endl;

        std::wcout << L"Центральный обход АВЛ дерева:" << std::endl;
        avlTree.inOrderTraversal(avlTree.root);
        std::wcout << std::endl;

        std::wcout << L"Прямой обход АВЛ дерева:" << std::endl;
        avlTree.preOrderTraversal(avlTree.root);
        std::wcout << std::endl;

        std::wcout << L"Обратный обход АВЛ дерева:" << std::endl;
        avlTree.postOrderTraversal(avlTree.root);
        std::wcout << std::endl;

    }
    catch (const std::exception& e) {
        std::wcerr << L"Ошибка: " << e.what() << std::endl;
    }

    return 0;
}



