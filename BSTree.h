#pragma once
#include <iostream>
using namespace std;
#include <memory>
#include <cassert>

template<typename KeyType>
class BSTree
{
protected:
	struct BstNode
	{
		BstNode *leftchild;
		BstNode *rightchild;
		BstNode *parent;
		KeyType key;
	};
public:
	typedef BstNode* PBstNode;
	BSTree(KeyType value = -1) :root(NULL), RefValue(value) {}
	~BSTree();
	bool Insert(const KeyType &kx);
	BstNode * Find(const KeyType &kx);
	BstNode * Search(const KeyType &kx);
	bool Remove(const KeyType &kx);
	void NiceInOrder();
	void ResNiceInOrder();
private:
	static BstNode * _Buynode();
	static void _Freenode(BstNode *p);
	static void DestoryTree(BstNode *ptr);
	static bool Insert(BstNode **ptr, KeyType kx);
	static bool Remove(BstNode **ptr, KeyType kx);
	static BstNode * Find(BstNode *ptr, KeyType kx);
	static BstNode * Search(BstNode *ptr, KeyType kx);
	static BstNode * First(BstNode *ptr);
	static BstNode * Next(BstNode *ptr);
	static BstNode * Last(BstNode *ptr);
	static BstNode * Prev(BstNode *ptr);
private:
	BstNode * root;
	KeyType RefValue;
};

template<typename KeyType>
BSTree<KeyType>::~BSTree()
{
	DestoryTree(root);
}

template<typename KeyType>
bool BSTree<KeyType>::Insert(const KeyType &kx)
{
	if (kx != RefValue)
		return Insert(&root, kx);
	else
		return false;
}

template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::Find(const KeyType &kx)
{
	return Find(root, kx);
}

template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::Search(const KeyType &kx)
{
	if (kx != RefValue)
		return Search(root, kx);
	else
		return NULL;
}

template<typename KeyType>
bool BSTree<KeyType>::Remove(const KeyType &kx)
{
	if (kx != RefValue)
		return Remove(&root, kx);
	else
		return false;
}

template<typename KeyType>
void BSTree<KeyType>::NiceInOrder()
{
	for (BstNode *p = First(root); p != NULL; p = Next(p))
	{
		cout << p->key << " ";
	}
	cout<<endl;
}

template<typename KeyType>
void BSTree<KeyType>::ResNiceInOrder()
{
	for (BstNode *p = Last(root); p != NULL; p = Prev(p))
	{
		cout << p->key << " ";
	}
	cout << endl;
}

template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::_Buynode()
{
	BstNode *p = (BstNode *)malloc(sizeof(BstNode));
	assert(p != NULL);

	memset(p, 0, sizeof(p));
	return p;
}

template<typename KeyType>
void BSTree<KeyType>::_Freenode(BstNode * p)
{
	if (p != NULL)
		free(p);
}

template<typename KeyType>
void BSTree<KeyType>::DestoryTree(BstNode * ptr)
{
	if (ptr != NULL)
	{
		DestoryTree(ptr->leftchild);
		DestoryTree(ptr->rightchild);
		_Freenode(ptr);
	}
}

template<typename KeyType>
bool BSTree<KeyType>::Insert(BstNode ** ptr, KeyType kx)
{
	if(ptr == NULL) return false;
	BstNode *p = *ptr;
	BstNode *pa = NULL;
	while (p != NULL && p->key != kx)
	{
		pa = p;
		p = kx < p->key ? p->leftchild : p->rightchild;
	}
	if (p != NULL)
		return false;
	p = _Buynode();
	p->key = kx;
	p->leftchild = p->rightchild = NULL;
	p->parent = pa;
	if (pa == NULL)
		*ptr = p;
	else
	{
		if (p->key < pa->key)
			pa->leftchild = p;
		else
			pa->rightchild = p;
	}
	return true;
}

template<typename KeyType>
bool BSTree<KeyType>::Remove(BstNode ** ptr, KeyType kx)
{
	if (ptr == NULL || *ptr == NULL) return false;
	BstNode *p = Find(*ptr, kx);
	if (p == NULL) return false;
	BstNode *pa = p->parent;
	if (p->leftchild != NULL && p->rightchild != NULL)//两个分支
	{
		BstNode *child = Next(p);
		p->key = child->key;
		p = child;
		pa = p->parent;
	}
	BstNode *child = p->leftchild != NULL ? p->leftchild : p->rightchild;
	if (child != NULL) child->parent = pa;
	if (pa != NULL)
	{
		if (pa->leftchild == p)
			pa->leftchild = child;
		else
			pa->rightchild = child;
	}
	else
	{
		*ptr = child;
	}
	_Freenode(p);
	return true;
}

template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::Find(BstNode * ptr, KeyType kx)
{
	if (ptr == NULL) return NULL;
	while (ptr != NULL && ptr->key != kx)
	{
		ptr = kx < ptr->key ? ptr->leftchild : ptr->rightchild;
	}
	return ptr;
}

template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::Search(BstNode * ptr, KeyType kx)
{
	if (ptr != NULL && ptr->key != kx)
	{
		if (ptr->key < kx)
			ptr = Search(ptr->rightchild, kx);
		else
			ptr = Search(ptr->leftchild, kx);
	}
	return ptr;
}

template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::First(BstNode * ptr)
{
	while (ptr != NULL && ptr->leftchild != NULL)
	{
		ptr = ptr->leftchild;
	}
	return ptr;
}

template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::Next(BstNode * ptr)
{
	if (ptr == NULL) return NULL;
	BstNode *parent = NULL;
	if (ptr->rightchild == NULL)
	{
		parent = ptr->parent;
		while (parent != NULL && parent->rightchild == ptr)
		{
			ptr = parent;
			parent = ptr->parent;
		}
		ptr = parent;
	}
	else
	{
		ptr = First(ptr->rightchild);
	}
	return ptr;
}

template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::Last(BstNode * ptr)
{
	while (ptr != NULL && ptr->rightchild != NULL)
	{
		ptr = ptr->rightchild;
	}
	return ptr;
}

template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::Prev(BstNode * ptr)
{
	if (ptr == NULL) return NULL;
	BstNode *parent = ptr->parent;
	if (ptr->leftchild != NULL)
	{
		ptr = Last(ptr->leftchild);
	}
	else
	{
		while (parent != NULL && parent->leftchild == ptr)
		{
			ptr = parent;
			parent = ptr->parent;
		}
		ptr = parent;
	}
	return ptr;
}


#if 0
template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::Next(BstNode * ptr)
{
	if (ptr == NULL) return NULL;
	BstNode *parent = NULL;
	if (ptr->rightchild != NULL)
	{
		ptr = ptr->rightchild;
		while (ptr->leftchild != NULL)
		{
			ptr = ptr->leftchild;
		}
	}
	else
	{
		parent = ptr->parent;
		while (parent != NULL && parent->rightchild == ptr)
		{
			ptr = parent;
			parent = ptr->parent;
		}
		ptr = parent;
	}
	return ptr;
}
template<typename KeyType>
typename BSTree<KeyType>::BstNode * BSTree<KeyType>::Prev(BstNode * ptr)
{
	if (ptr == NULL) return NULL;
	BstNode *parent = ptr->parent;
	if (ptr->leftchild != NULL)
	{
		ptr = ptr->leftchild;
		while (ptr->rightchild != NULL)
		{
			ptr = ptr->rightchild;
		}
	}
	else
	{
		while (parent != NULL && parent->leftchild == ptr)
		{
			ptr = parent;
			parent = ptr->parent;
		}
		ptr = parent;
	}
	return ptr;
}
#endif