#include <QCoreApplication>
#include <QDebug>

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

struct Node
{
public:
    Node (int left = 1, Node *parent = 0) : parent(parent), level(0)
    {
        this->left = left;
        right = this->left + 1;



    }

    int level; // уровень (нужно только для вывода на экран)
    int left; // левое число
    int right; // правое число
    Node *parent; // родиель
    vector<Node *> children; // потомки
};

namespace Tree {
// поиск ветки по номеру (в упорядоченном дереве!)
Node* find(int pos, Node *node)
{
    if (pos >= node->left && pos <= node->right) {
        // если искомый номер попадает в промежуток [left, right]

        if (pos == node->left || pos == node->right) {
            // если искомый номер совпадает с левым номером текущего узла
            // или если искомый номер совпадает с правым номером текущего узла

            return node;

        }

        for (auto x: node->children) {
            if (x != 0 && pos >= x->left && pos <= x->right) {
                // если искомый номер попадает в промежуток дочернего узла

                return find(pos, x);
            }

        }

    }

    return 0;

}

// обновление дерева вниз от node исключая ветки меньшие чем branchNumber
int update(int branchNumber, int num, Node *node)
{

    node->left = num;
    node->level = (node->parent != 0) ? (node->parent->level + 1) : 0; // уровень (только для отображения!)


    if (!node->children.empty()) {
        // если есть потомки, то рекурсивно вызываем upadte() и для них

        for (int i = 0; i < node->children.size(); i++) {

            int forOther = (i == 0) ? (node->left + 1) : node->children[i - 1]->right + 1; // номер последовательнсти для потомка

            if(i >= branchNumber) {
                // обновляем только нужные ветки (которые больше заданного ограничения branchNumber)
                // и берем значение right у крайней ветки

                node->right = update(0, forOther, node->children[i]);
            }
            else {
                // или просто берем значение right у крайней ветки

                node->right = node->children[i]->right + 1;
            }
        }
    }
    else {
        // если потомков нет, то right = left + 1

        node->right = node->left + 1;

    }

    return node->right + 1; // возвращаем для родителя его правый номер
}

// поиск номера ветки в векторе потомков (можно заменить добавлением поля в структуру Node  сожержащую номер ветки)
int findBranchNumber(Node *node)
{
    if(node->parent == 0)
        return -1;

    Node *parent =  node->parent;
    auto iter = std::find(parent->children.begin(), parent->children.end(), node);
    return parent->children.end() - iter - 1; // переходим к дургой ветке
}

// поднимаемся на уровень выше с обновлением потомков (не всех!)
void levelUp(int num, Node *node)
{
    if(node->parent == 0)
        return;

    // Пиоиск номера ветки (можно заменить добавлением поля в структуру Node  сожержащую номер ветки)
    int branchNumber = findBranchNumber(node);

    Node *parent = node->parent;

    update(branchNumber, parent->left, parent); // обновляем ветки родительского узла (исключая уже измененные (т.е < branchNumber))

    levelUp(parent->right, parent); // рекурсивно переходим на уровень вверх
}

// вставка ветки node в parent на место pos
void insert(int pos, Node *node, Node *root)
{
    Node *p = find(pos, root); // поиск места вставки

    if(p == 0)
        return;

    if(pos == p->left){
        // если место вставки найдено - дабавляем ветку node в потомки и обновляем ее полностью (рекурсивно)

        node->parent = p; // назначаем родителя новому узлу
        p->children.push_back(node);
        update(p->children.size() - 1, p->left, p);

        levelUp(p->right, p); // поднимаемся на уровень выше, и обновляем все ветки справа (рекурсивно)
    }

}

// удаление ветки (перемещение в node) из parent на месте pos (включая узел с pos)
void remove(int pos, Node *node, Node *root)
{
    Node *p = find(pos, root); // поиск места удаления

    if(pos == p->left){
        node = p; // перемещаем узел и все его ветки в новый корень

        if(p->parent != 0) {
            int branchNumber = findBranchNumber(p);
            int forOther = p->parent->left;
            Node *parent = p->parent;

            parent->children.erase(find(parent->children.begin(), parent->children.end(), p)); // удаляем узел и все го ветки
            update(branchNumber, forOther, parent); // обновляем все ветки спава от удаленного узла
            levelUp(parent->right, parent); // поднимаемся на уровень выше, и обновляем все ветки справа (рекурсивно)

            update(0, 1, node); // обновляем новое дерево (часть старого)

        }

    }




}

// вывод информации об узлах на экран
void print(Node *node)
{
    if(node == 0)
        return;

    qDebug() << "level " << node->level;
    qDebug() << node->left << ", " << node->right;

    for(auto x: node->children)
        print(x);

}

} // namecpace Tree

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Node *root = new Node;
    Tree::insert(1, new Node, root);
    Tree::insert(2, new Node, root);
    Tree::insert(1, new Node, root);
    //insert(2, new Node, root);

    Tree::print(root);

    cout << "Root after insert ---------" << endl;

    Node *root2 = new Node;
    Tree::insert(1, new Node, root2);
    Tree::insert(1, new Node, root2);

    Tree::print(root2);

    cout << "Root2 after insert --------" << endl;

    Tree::insert(3, root2, root);
    Tree::print(root);

    Node *root3 = nullptr;
    Tree::remove(4, root2, root);

    cout << "Root after remove ----------" << endl;
    Tree::print(root);
    cout << "Root2 after remove ---------" << endl;
    Tree::print(root2);


    return a.exec();
}
