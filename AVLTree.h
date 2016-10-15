#pragma once
#include <iostream>
using namespace std;
#include <cassert>
#include <vld.h>

template<typename KeyType>
class AVLTree
{
protected:
	struct AVLNode
	{
		AVLNode *leftchild;
		AVLNode *rightchild;
		AVLNode *parent;
		int balance;//-1 0 1
		KeyType key;
	};
private:
	AVLNode *root;
public:
	AVLTree() : root(NULL) {}
	~AVLTree() { DestoryTree(root); }

	bool Insert(const KeyType &x)
	{
		return Insert(root, x, NULL);
	}
	int Depth() { return Depth(root); }
	void InOrder() { InOrder(root); cout << endl; }
protected:
	static AVLNode * _Buynode();
	static void _Freenode(AVLNode *p);
	static void DestoryTree(AVLNode *ptr);
	
	static int Depth(AVLNode *ptr);
	static AVLNode * Search(AVLNode *ptr, const KeyType &val);

	static void RotateLeft(AVLNode *&ptr);
	static void RotateRight(AVLNode *&ptr);
	static void LeftBalance(AVLNode *&ptr);
	static void RightBalance(AVLNode *&ptr);

	static bool Insert(AVLNode *&ptr, const KeyType x, AVLNode *pa);
	static void InOrder(AVLNode *ptr);
};

template<typename KeyType>
typename AVLTree<KeyType>::AVLNode * AVLTree<KeyType>::_Buynode()
{
	AVLNode *s = (AVLNode *)malloc(sizeof(AVLNode));
	assert(s != NULL);
	memset(s, 0, sizeof(AVLNode));
	return s;
}

template<typename KeyType>
void AVLTree<KeyType>::_Freenode(AVLNode * p)
{
	if (p != NULL)
		free(p);
}

template<typename KeyType>
void AVLTree<KeyType>::DestoryTree(AVLNode * ptr)
{
	if (ptr != NULL)
	{
		DestoryTree(ptr->leftchild);
		DestoryTree(ptr->rightchild);
		_Freenode(ptr);
	}
}

template<typename KeyType>
int AVLTree<KeyType>::Depth(AVLNode * ptr)
{
	int ret = 0;
	if (ptr != NULL)
	{
		int a = Depth(ptr->leftchild);
		int b = Depth(ptr->rightchild);
		ret = a > b ? a + 1 : b + 1;
	}
	return ret;
}

template<typename KeyType>
typename AVLTree<KeyType>::AVLNode * AVLTree<KeyType>::Search(AVLNode * ptr, const KeyType & val)
{
	if (ptr == NULL || ptr->key == val)return ptr;
	if (val < ptr->key)
		return Search(ptr->leftchild, val);
	else
		return Search(ptr->rightchild, val);
}

template<typename KeyType>
void AVLTree<KeyType>::RotateLeft(AVLNode *& ptr)
{
	AVLNode *newroot = ptr->rightchild;
	newroot->parent = ptr->parent;
	ptr->rightchild = newroot->leftchild;
	if (newroot->leftchild != NULL)
	{
		newroot->leftchild->parent = ptr;
	}
	newroot->leftchild = ptr;
	ptr->parent = newroot;

	//ptr->balance = Depth(ptr->rightchild) - Depth(ptr->leftchild);
	//newroot->balance = Depth(newroot->rightchild) - Depth(newroot->leftchild);
	--ptr->balance;
	if (newroot->balance >= 0)
		ptr->balance -= newroot->balance;
	--newroot->balance;
	if (ptr->balance < 0)
		newroot->balance += ptr->balance;

	ptr = newroot;
}

template<typename KeyType>
void AVLTree<KeyType>::RotateRight(AVLNode *& ptr)
{
	AVLNode *newroot = ptr->leftchild;
	newroot->parent = ptr->parent;
	ptr->leftchild = newroot->rightchild;
	if (newroot->rightchild != NULL)
	{
		newroot->rightchild->parent = ptr;
	}
	newroot->rightchild = ptr;
	ptr->parent = newroot;

	//ptr->balance = Depth(ptr->rightchild) - Depth(ptr->leftchild);
	//newroot->balance = Depth(newroot->rightchild) - Depth(newroot->leftchild);
	++ptr->balance;
	if (newroot->balance < 0)
		ptr->balance -= newroot->balance;
	++newroot->balance;
	if (ptr->balance >= 0)
		newroot->balance += ptr->balance;

	ptr = newroot;
}

template<typename KeyType>
void AVLTree<KeyType>::LeftBalance(AVLNode *& ptr)
{
	AVLNode * &tmp = ptr->leftchild;
	//Rotate Left
	AVLNode * newroot = tmp->rightchild;
	newroot->parent = tmp->parent;
	tmp->rightchild = newroot->leftchild;
	if (newroot->leftchild != NULL)
	{
		newroot->leftchild->parent = tmp;
	}
	newroot->leftchild = tmp;
	tmp->parent = newroot;

	tmp->balance += -1;
	newroot->balance += -1;
	tmp = newroot;

	//Rotate Right
	newroot = ptr->leftchild;
	newroot->parent = ptr->parent;
	ptr->leftchild = newroot->rightchild;
	if (newroot->rightchild != NULL)
	{
		newroot->rightchild->parent = ptr;
	}
	newroot->rightchild = ptr;
	ptr->parent = newroot;

	ptr->balance += 2;
	newroot->balance += 1;
	ptr = newroot;
}

template<typename KeyType>
void AVLTree<KeyType>::RightBalance(AVLNode *& ptr)
{
	AVLNode *&tmp = ptr->rightchild;
	//Rotate Right
	AVLNode * newroot = tmp->leftchild;
	newroot->parent = tmp->parent;
	tmp->leftchild = newroot->rightchild;
	if (newroot->rightchild != NULL)
	{
		newroot->rightchild->parent = tmp;
	}
	newroot->rightchild = tmp;
	tmp->parent = newroot;

	tmp->balance += 1;
	newroot->balance += 1;
	tmp = newroot;

	//Rotate Left
	newroot = ptr->rightchild;
	newroot->parent = ptr->parent;
	ptr->rightchild = newroot->leftchild;
	if (newroot->leftchild != NULL)
	{
		newroot->leftchild->parent = ptr;
	}
	newroot->leftchild = ptr;
	ptr->parent = newroot;

	ptr->balance += -3;
	newroot->balance += -2;
	ptr = newroot;
}

template<typename KeyType>
bool AVLTree<KeyType>::Insert(AVLNode *& ptr, const KeyType x, AVLNode * pa)
{
	bool res = false;
	if (ptr == NULL)
	{
		ptr = _Buynode();
		new (&ptr->key) KeyType(x); //ptr->key = x;
		ptr->balance = 0;
		ptr->parent = pa;
		res = true;
	}
	else if (x < ptr->key)
	{
		res = Insert(ptr->leftchild, x, ptr);
		if (res )
		{
			ptr->balance = Depth(ptr->rightchild) - Depth(ptr->leftchild);
			if (ptr->balance < -1)
			{
				if (ptr->leftchild->balance == -1)
					RotateRight(ptr);
				else
					LeftBalance(ptr);
			}
		}
	}
	else if (x > ptr->key)
	{
		res = Insert(ptr->rightchild, x, ptr);
		if (res)
		{
			ptr->balance = Depth(ptr->rightchild) - Depth(ptr->leftchild);
			if (ptr->balance > 1)
			{
				if (ptr->rightchild->balance == 1)
					RotateLeft(ptr);
				else
					RightBalance(ptr);
			}
		}
	}
	return res;
}

template<typename KeyType>
void AVLTree<KeyType>::InOrder(AVLNode *ptr)
{
	if (ptr == NULL)return;
	InOrder(ptr->leftchild);
	cout << ptr->key << " ";
	InOrder(ptr->rightchild);
}