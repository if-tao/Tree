#include "RBTree.h"

#include <vld.h>

#if true
int main()
{
	R_BTree<int> mytree;
	int x;
	while (cin >> x, x != -1)
	{
		mytree.Insert(x);
	}
	mytree.InOrder();


	while (cin >> x, x != -1)
	{
		mytree.Delete(x);
		mytree.InOrder();
	}
	return 0;
}
#endif