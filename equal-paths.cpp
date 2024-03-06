#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <algorithm>

#endif

#include "equal-paths.h"
using namespace std;



// You may add any prototypes of helper functions here
bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL)
	{
		return true;
	}
	if(height(root) == minDepth(root))
    {
        return true;
    }
    else 
    {
        return false;
    }




}


int height(Node* node)
{
    if (node == NULL)
        return 0;
    else {

        int lheight = height(node->left);
        int rheight = height(node->right);

        if (lheight > rheight)
            return (lheight + 1);
        else
            return (rheight + 1);
    }
}

int minDepth(Node* node)
{
    if (node == NULL)
        return 0;
    else {

        int ldepth = minDepth(node->left);
        int rdepth = minDepth(node->right);
 

    if (node->left == NULL)
    {
        return rdepth + 1;
    }
    else if (node->right == NULL)
    {
        return ldepth + 1;
    }
    else
        return min(ldepth, rdepth) + 1;
    }
}
