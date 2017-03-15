//
#include <iostream>
#include <stack>
#include <queue>

struct Node
{
    struct Node *right;
    struct Node *left;
    int val;
    Node(int v):right(NULL), left(NULL), val(v) {}
};

int DFS(Node *root)
{
    std::stack<Node*> s;
    s.push(root);
    while(!s.empty())
    {
        root = s.top();
        std::cout << root->val << " " ;
        s.pop();
        if (root->right)
        {
            s.push(root->right);  
        }
        if (root->left)
        {
            s.push(root->left);
        }
    }
}

int BFS(Node *root)
{
    std::queue<Node *> q;
    q.push(root);
    while (!q.empty())
    {
        root = q.front();
        std::cout << root->val << " ";
        q.pop();
        if (root->right)
        {
            q.push(root->right);
        }
        if (root->left)
        {
            q.push(root->left);
        }
    }
}

int main(int argc, char* argv[])
{
    Node *a = new Node(1);
    Node *b = new Node(2); 
    Node *c = new Node(3);
    Node *d = new Node(4);
    Node *e = new Node(5);
    Node *f = new Node(6);
    a->right = b;
    a->left = c;
    b->right = d;
    b->left = e;
    c->left = f;
    std::cout << "\nbfs:\n";
    BFS(a);
    std::cout << "\ndfs:\n";
    DFS(a);
	return 0;
}

