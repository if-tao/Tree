#ifndef _B_TREE_H_
#define _B_TREE_H_

#include <iostream>
#include <cassert>
#include <memory>
using namespace std;

typedef char KeyType;
typedef struct {} Record;
typedef struct
{
	KeyType key;
	Record	*recptr;
}ElemType;

#define M 5
#define MIN (M/2)
#define MAX (M-1)

typedef struct BNode
{
	int num;
	BNode *parent;
	ElemType data[M];
	BNode *  sub[M];
}BNode,*BTree;

typedef struct
{
	BNode *pnode;
	int index;
	bool tag;
}Result;

BNode * _Buynode()
{
	BNode * res = (BNode*)malloc(sizeof(BNode));
	assert(res != NULL);
	memset(res, 0, sizeof(BNode));
	return res;
}
void _Freenode(BNode *p)
{
	if (p != NULL)
		free(p);
}

Result Find(BNode *ptr, KeyType kx)
{
	Result res = { NULL,-1,false };
	while (ptr != NULL)
	{
		ptr->data[0].key = kx;
		int i = ptr->num;
		while(kx < ptr->data[i].key)
			--i;
		res.pnode = ptr;
		res.index = i;
		if (i > 0 && ptr->data[i].key == kx)
		{
			res.tag = true;
			break;
		}
		ptr = ptr->sub[i];
	}
	return res;
}


Result Search(BNode *ptr, KeyType kx)
{
	Result res = { NULL,-1,false };
	if (ptr != NULL)
	{
		ptr->data[0].key = kx;
		int i = ptr->num;
		while(kx < ptr->data[i].key)
			--i;
		res.pnode = ptr;
		res.index = i;
		if (i > 0 && ptr->data[i].key == kx)
		{
			res.tag = true;
		}
		else if (ptr->sub[i] != NULL)
		{
			res = Search(ptr->sub[i], kx);
		}
	}
	return res;
}

BNode * CreateRoot(ElemType x, BNode *left, BNode *right)
{
	BNode *s = _Buynode();
	s->parent = NULL;
	s->num = 1;
	s->data[1] = x;
	s->sub[0] = left;
	if(left != NULL)
		left->parent = s;
	s->sub[1] = right;
	if(right != NULL)
		right->parent = s;
	return s;
}

void Insert_Item(BNode *ptr, int pos, ElemType x, BNode *right)
{
	for (int i = ptr->num; i > pos; --i)
	{
		ptr->data[i + 1] = ptr->data[i];
		ptr->sub[i + 1] = ptr->sub[i];
	}
	ptr->data[pos+1] = x;
	ptr->sub[pos + 1] = right;
	if (right != NULL)
		right->parent = ptr;
	ptr->num += 1;
}
void Move_Item(BNode *ptr, BNode *s, int pos)
{
	int j = 1;
	for (int i = pos + 1; i <= ptr->num; ++i, ++j)
	{
		s->data[j] = ptr->data[i];
		s->sub[j] = ptr->sub[i];
		if(s->sub[j] != NULL)
			s->sub[j]->parent = s;
	}
	ptr->num = pos;
	s->num = j - 1;
	s->parent = ptr->parent;
}
BNode* Splice(BNode *ptr, int pos, ElemType x, BNode *right)
{	
	BNode * p = _Buynode();
	int tmp = pos > MIN ? MIN + 1 : MIN;
	Move_Item(ptr, p, tmp);
	if (pos <= MIN)
	{
		Insert_Item(ptr, pos, x, right);
	}
	else
	{
		Insert_Item(p, pos - tmp, x, right);
	}
	if (right != NULL)
		p->sub[0] = ptr->sub[tmp];
	ptr->num -= 1;

	if (ptr->parent == NULL)
	{
		return CreateRoot(ptr->data[MIN + 1], ptr, p);
	}
	x = ptr->data[MIN + 1];
	ptr = ptr->parent;

	ptr->data[0] = x;
	pos = ptr->num;
	while (x.key < ptr->data[pos].key)
		--pos;
	if (ptr->num < MAX)
	{
		Insert_Item(ptr, pos, x, p);
		return NULL;
	}
	else
	{
		return Splice(ptr, pos, x, p);
	}
}
void InOrder(BNode *p)
{
	if (p != NULL)
	{
		InOrder(p->sub[0]);
		for (int i = 1; i <= p->num; ++i)
		{
			cout << p->data[i].key << " ";
			InOrder(p->sub[i]);
		}
	}

}

bool Insert(BTree *ptree, ElemType x)
{
	if (ptree == NULL) return false;
	if (*ptree == NULL)
	{
		*ptree = CreateRoot(x, NULL, NULL);
		return true;
	}
	Result res = Find(*ptree, x.key);
	if (res.pnode == NULL || res.tag) return false;
	BNode *ptr = res.pnode;
	int    pos = res.index;
	if (ptr->num < MAX)
	{
		Insert_Item(ptr, pos, x, NULL);
	}
	else
	{
		BNode *newroot = Splice(ptr, pos, x, NULL);
		if (newroot != NULL)
		{
			*ptree = newroot;
		}
	}
	return true;
}

#endif