#include <QCoreApplication>
#include <QDebug>

#include <vector>
#include <algorithm>

using namespace std;

class Node
{
public:
    Node (int left = 1, Node *parent = 0) : parent(parent)
    {
        this->left = left;
        right = this->left + 1;

        level = 0;

    };

    int level;
    int left; // левое число
    int right; // правое число
    Node *parent; // родиель
    vector<Node *> children; // потомки
};

// поиск ветки по номеру (в упорядоченном дереве!)
Node* find(int pos, Node *node)
{
    if (pos >= node->left && pos <= node->right) {
        // если искомый номер попадает в промежуток [left, right]

        if (pos == node->left || pos == node->right) {
            // если искомый номер совпадает с левым номером текущего узла
            // если искомый номер совпадает с правым номером текущего узла

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

int update(int branch, int num, Node *node)
{

    //node->left = num;
    //node->level = node->parent->level + 1;


    if(!node->children.empty() && branch < node->children.size()){
        for (int i = branch; i < node->children.size(); i++) {
            int forOther = (i == 0) ? (node->left + 1) : node->children[i - 1]->right;
            node->right = update(0, forOther, node->children[i]);
        }
    }
    else
        node->right = num + 1;

    return node->right + 1;
}

//int levelUp(int branch, int num, Node *node)
//{
//    Node *parent =  node->parent;
//    auto iter = std::find(parent->children.begin(), parent->children.end(), node);
//    iter++; // переходим к дургой ветке

//    for(; iter != parent->children.end(); iter++) {
//        parent->right = update(0, num, *iter);

//    }
//}

void insert(int pos, Node *node, Node *parent)
{
    Node *p = find(pos, parent);

    node->parent = p;

    if(pos == p->left){
        p->children.push_back(node);
        p->right = update(p->children.size() - 1, p->left, p);
    }

    //levelUp(node)


}

void print(Node *node)
{
    if(node == 0)
        return;

    qDebug() << "level " << node->level;
    qDebug() << node->left << ", " << node->right;

    for(auto x: node->children)
        print(x);

}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Node *root = new Node;
    insert(1, new Node, root);
    //insert(2, new Node, root);
    //insert(2, new Node, root);

    print(root);

    /*

    qDebug() << "---------------------";

    Node *root2 = new Node;
    insert(1, new Node, root2);
    insert(1, new Node, root2);

    print(root2);

    qDebug() << "---------------------";

    insert(2, root2, root);
    print(root);
*/


    return a.exec();
}
