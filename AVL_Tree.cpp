#include <iostream>
#include <algorithm>
#include <string>
#include<clocale>
struct Node {
    int key;
    int height;
    Node* left;
    Node* right;

    Node(int val) : key(val), height(1), left(nullptr), right(nullptr) {}
};

// --- Вспомогательные функции высоты и баланса ---

int height(Node* n) {
    return n ? n->height : 0;
}

int getBalance(Node* n) {
    return n ? height(n->left) - height(n->right) : 0;
}

void updateHeight(Node* n) {
    if (n) {
        n->height = 1 + std::max(height(n->left), height(n->right));
    }
}

// --- Проверка сбалансированности всего дерева ---
bool isBalanced(Node* root) {
    if (!root) return true;
    int balance = getBalance(root);
    if (balance < -1 || balance > 1) return false;
    return isBalanced(root->left) && isBalanced(root->right);
}

// --- Поиск экстремумов ---

Node* findMin(Node* node) {
    if (!node) return nullptr;
    while (node->left) node = node->left;
    return node;
}

Node* findMax(Node* node) {
    if (!node) return nullptr;
    while (node->right) node = node->right;
    return node;
}

// --- Поиск элемента по ключу ---

Node* search(Node* root, int key) {
    if (!root || root->key == key) return root;
    if (key < root->key) return search(root->left, key);
    return search(root->right, key);
}

// --- Вращения ---

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

// --- Балансировка узла ---

Node* balanceNode(Node* node) {
    updateHeight(node);
    int balance = getBalance(node);

    // Left Left
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }
    // Left Right
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    // Right Right
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }
    // Right Left
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// --- Вставка ---

Node* insert(Node* node, int key) {
    if (!node) return new Node(key);

    if (key < node->key) {
        node->left = insert(node->left, key);
    }
    else if (key > node->key) {
        node->right = insert(node->right, key);
    }
    else {
        return node; // Дубликаты игнорируются
    }

    return balanceNode(node);
}

// --- Удаление ---

Node* remove(Node* root, int key) {
    if (!root) return root;

    if (key < root->key) {
        root->left = remove(root->left, key);
    }
    else if (key > root->key) {
        root->right = remove(root->right, key);
    }
    else {
        if (!root->left || !root->right) {
            Node* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            }
            else {
                *root = *temp;
            }
            delete temp;
        }
        else {
            Node* temp = findMin(root->right);
            root->key = temp->key;
            root->right = remove(root->right, temp->key);
        }
    }

    if (!root) return root;

    return balanceNode(root);
}

// --- Способы обхода дерева ---

void preOrder(Node* root) {
    if (root) {
        std::cout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(Node* root) {
    if (root) {
        inOrder(root->left);
        std::cout << root->key << " ";
        inOrder(root->right);
    }
}

void postOrder(Node* root) {
    if (root) {
        postOrder(root->left);
        postOrder(root->right);
        std::cout << root->key << " ";
    }
}

// --- Визуализация структуры дерева в консоли ---

void printTree(Node* root, const std::string& prefix = "", bool isLeft = false) {
    if (root) {
        std::cout << prefix;
        std::cout << (isLeft ? "├── L-- " : "└── R-- ");
        std::cout << root->key << " (h:" << root->height << ", b:" << getBalance(root) << ")\n";

        printTree(root->left, prefix + (isLeft ? "│   " : "    "), true);
        printTree(root->right, prefix + (isLeft ? "│   " : "    "), false);
    }
}

// --- Очистка памяти ---

void clear(Node* root) {
    if (root) {
        clear(root->left);
        clear(root->right);
        delete root;
    }
}

// --- Основная программа ---

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    Node* root = nullptr;
    int n, value, choice;

    std::cout << "Введите количество элементов для инициализации дерева: ";
    if (!(std::cin >> n) || n < 0) {
        std::cout << "Некорректный ввод.\n";
        return 1;
    }

    if (n > 0) {
        std::cout << "Введите " << n << " элементов(а) через пробел: ";
        for (int i = 0; i < n; ++i) {
            std::cin >> value;
            root = insert(root, value);
        }
        std::cout << "Дерево успешно инициализировано.\n";
    }

    while (true) {
        std::cout << "\n=== МЕНЮ AVL-ДЕРЕВА ===\n"
            << "1. Вставить новый элемент\n"
            << "2. Удалить элемент\n"
            << "3. Найти элемент (Поиск)\n"
            << "4. Визуализировать дерево\n"
            << "5. Прямой обход (Pre-order)\n"
            << "6. Симметричный обход (In-order)\n"
            << "7. Обратный обход (Post-order)\n"
            << "8. Показать высоту дерева и баланс корня\n"
            << "9. Показать минимум и максимум\n"
            << "10. Проверить сбалансированность\n"
            << "0. Выход\n"
            << "Выберите действие: ";

        if (!(std::cin >> choice)) {
            std::cout << "Ошибка ввода. Завершение программы.\n";
            break;
        }

        if (choice == 0) break;

        switch (choice) {
        case 1:
            std::cout << "Введите значение для вставки: ";
            std::cin >> value;
            root = insert(root, value);
            std::cout << "Элемент добавлен.\n";
            break;
        case 2:
            std::cout << "Введите значение для удаления: ";
            std::cin >> value;
            root = remove(root, value);
            std::cout << "Команда удаления выполнена.\n";
            break;
        case 3:
            std::cout << "Введите число для поиска: ";
            std::cin >> value;
            if (search(root, value)) {
                std::cout << "Элемент " << value << " НАЙДЕН в дереве.\n";
            }
            else {
                std::cout << "Элемент " << value << " НЕ НАЙДЕН.\n";
            }
            break;
        case 4:
            if (!root) {
                std::cout << "Дерево пустое.\n";
            }
            else {
                std::cout << "\nСтруктура дерева (L - левая ветвь, R - правая ветвь):\n";
                std::cout << "Ключ (h: высота узла, b: фактор баланса)\n";
                std::cout << "---------------------------------------------\n";
                printTree(root, "", false);
            }
            break;
        case 5:
            std::cout << "Прямой обход: ";
            preOrder(root);
            std::cout << "\n";
            break;
        case 6:
            std::cout << "Симметричный обход: ";
            inOrder(root);
            std::cout << "\n";
            break;
        case 7:
            std::cout << "Обратный обход: ";
            postOrder(root);
            std::cout << "\n";
            break;
        case 8:
            std::cout << "Общая высота дерева: " << height(root) << "\n";
            std::cout << "Фактор баланса корня: " << getBalance(root) << "\n";
            break;
        case 9: {
            Node* minNode = findMin(root);
            Node* maxNode = findMax(root);
            if (minNode) std::cout << "Минимум: " << minNode->key << "\n";
            if (maxNode) std::cout << "Максимум: " << maxNode->key << "\n";
            if (!minNode) std::cout << "Дерево пустое.\n";
            break;
        }
        case 10:
            if (isBalanced(root)) {
                std::cout << "Дерево полностью сбалансировано.\n";
            }
            else {
                std::cout << "Дерево НЕ сбалансировано!\n";
            }
            break;
        default:
            std::cout << "Неверный пункт меню.\n";
        }
    }

    clear(root);
    return 0;
}