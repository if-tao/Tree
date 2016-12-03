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
	ElemType data[M+1];
	BNode *  sub[M+1];
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

void Destory(BNode *&p)
{
	if (p != NULL)
	{
		for (int i = 0; i <= p->num; ++i)
		{
			Destory(p->sub[i]);
		}
		_Freenode(p);
		p = NULL;
	}
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

BNode * Create_Root(ElemType x, BNode *left, BNode *right)
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

ElemType Move_Item(BNode *ptr, BNode *s, int pos)
{
	for (int i = 0, j = pos + 1; j <= ptr->num; ++i, ++j)
	{
		s->data[i] = ptr->data[j];
		s->sub[i] = ptr->sub[j];
		if(s->sub[i] != NULL)
			s->sub[i]->parent = s;
	}
	ptr->num = MIN;
	s->num = MIN;
	s->parent = ptr->parent;
	return s->data[0];
}

BNode *Splice(BNode *ptr)
{
	BNode *s = _Buynode();
	ElemType x = Move_Item(ptr, s, MIN);
	if (ptr->parent == NULL)
	{
		return Create_Root(x, ptr, s);
	}
	ptr = ptr->parent;
	ptr->data[0] = x;
	int pos = ptr->num;
	while(x.key < ptr->data[pos].key)
		--pos;
	Insert_Item(ptr, pos, x, s);
	if (ptr->num > MAX)
	{
		return Splice(ptr);
	}
	return NULL;
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
		*ptree = Create_Root(x, NULL, NULL);
		return true;
	}
	Result res = Find(*ptree, x.key);
	if (res.pnode == NULL || res.tag) return false;
	BNode *ptr = res.pnode;
	int    pos = res.index;
	Insert_Item(ptr, pos, x, NULL);
	if (ptr->num > MAX)
	{
		BNode *newroot = Splice(ptr);
		if (newroot != NULL)
		{
			*ptree = newroot;
		}
	}
	return true;
}

BNode *Prev(BNode *ptr)
{
	while (ptr != NULL && ptr->sub[ptr->num] != NULL)
	{
		ptr = ptr->sub[ptr->num];
	}
	return ptr;
}

BNode *Next(BNode *ptr)
{
	while (ptr != NULL && ptr->sub[0] != NULL)
	{
		ptr = ptr->sub[0];
	}
	return ptr;
}

void Del_Branch_Item(BNode *ptr, int pos)
{
	for (int i = pos; i < ptr->num; ++i)
	{
		ptr->data[i] = ptr->data[i + 1];
		ptr->sub[i] = ptr->sub[i + 1];
	}
	ptr->sub[ptr->num] = NULL;
	ptr->num -= 1;
}

void Del_Leaf_Item(BNode *ptr, int pos)
{
	Del_Branch_Item(ptr, pos);
}

void RMove_Breach(BNode *pleft, BNode *ptr, BNode *pa, int pos)
{
	ptr->data[0] = pa->data[pos];
	ptr->sub[0] = pleft->sub[pleft->num];
	if (ptr->sub[0] != NULL)
		ptr->sub[0]->parent = ptr;
	for (int i = ptr->num + 1; i > 0; --i)
	{
		ptr->data[i] = ptr->data[i - 1];
		ptr->sub[i] = ptr->sub[i - 1];
	}
	pa->data[pos] = pleft->data[pleft->num];
	ptr->num += 1;
	pleft->num -= 1;
}

void RMove_Leaf(BNode *pleft, BNode *ptr, BNode *pa, int pos)
{
	RMove_Breach(pleft, ptr, pa, pos);
}

void LMove_Breach(BNode *pright, BNode *ptr, BNode *pa, int pos)
{
	ptr->num += 1;
	ptr->data[ptr->num] = pa->data[pos + 1];
	ptr->sub[ptr->num] = pright->sub[0];
	if (ptr->sub[ptr->num] != NULL)
		ptr->sub[ptr->num]->parent = ptr;
	pa->data[pos + 1] = pright->data[1];
	Del_Leaf_Item(pright, 0);
}

void LMove_leaf(BNode *pright, BNode *ptr, BNode *pa, int pos)
{
	LMove_Breach(pright, ptr, pa, pos);
}

void LMerge_Breach(BNode *pleft, BNode *ptr, BNode *pa, int pos)
{
	ptr->data[0] = pa->data[pos];
	for (int i = pleft->num + 1, j = 0; j <= ptr->num; ++i, ++j)
	{
		pleft->data[i] = ptr->data[j];
		pleft->sub[i] = ptr->sub[j];
		if (pleft->sub[i] != NULL)
			pleft->sub[i]->parent = pleft;
	}
	pleft->num = pleft->num + ptr->num + 1;
	Del_Branch_Item(pa, pos);
}

void LMerge_Leaf(BNode *pleft, BNode *ptr, BNode *pa, int pos)
{
	LMerge_Breach(pleft, ptr, pa, pos);
}

void RMerge_Breach(BNode *pright, BNode *ptr, BNode *pa, int pos)
{
	LMerge_Breach(ptr, pright, pa, pos + 1);
}

void RMerge_Leaf(BNode *pright, BNode *ptr, BNode *pa, int pos)
{
	RMerge_Breach(pright, ptr, pa, pos);
}

BNode * Adjust_Leaf(BNode *ptr)
{
	BNode *pa = ptr->parent;
	BNode *pleft = NULL, *pright = NULL;
	int pos = pa->num;
	while (pos >= 0 && pa->sub[pos] != ptr) --pos;
	if (pos < 0) { exit(1); }
	if (pos > 0)
	{
		pleft = pa->sub[pos - 1];
	}
	if (pos < pa->num)
	{
		pright = pa->sub[pos + 1];
	}

	if (pleft != NULL && pleft->num > MIN)
	{
		RMove_Leaf(pleft, ptr, pa, pos);
		//Insert_Item(ptr, 0, pa->data[pos], NULL);
		//pa->data[pos] = pleft->data[pleft->num];
		//Del_Leaf_Item(pleft, pleft->num);
	}
	else if (pright != NULL && pright->num > MIN)
	{
		LMove_leaf(pright, ptr, pa, pos);
		//Insert_Item(ptr, ptr->num, pa->data[pos + 1], NULL);
		//pa->data[pos + 1] = pright->data[1];
		//Del_Leaf_Item(pright, 1);
	}
	else if (pleft != NULL)
	{
		LMerge_Leaf(pleft, ptr, pa, pos);
		_Freenode(ptr);
		ptr = pleft;
		//Insert_Item(pleft, pleft->num, pa->data[pos], ptr->sub[0]);
		//for (int i = 1; i <= ptr->num; ++i)
		//{
		//	Insert_Item(pleft, pleft->num, ptr->data[i], ptr->sub[i]);
		//}
		//_Freenode(ptr);
		//Del_Branch_Item(pa, pos);
		//if (pa->parent == NULL && pa->num == 0)
		//{
		//	pleft->parent = NULL;
		//	_Freenode(pa);
		//	return pleft;
		//}
	}
	else if (pright != NULL)
	{
		RMerge_Leaf(pright, ptr, pa, pos);
		_Freenode(pright);
		pright = NULL;
		//Insert_Item(pright, 0, pa->data[pos+1], NULL);
		//for (int i = 1; i <= ptr->num; ++i)
		//{
		//	Insert_Item(pright, i-1, ptr->data[i], ptr->sub[i]);
		//}
		//_Freenode(ptr);
		//Del_Branch_Item(pa, pos);
		//if (pa->parent == NULL && pa->num == 0)
		//{
		//	pright->parent = NULL;
		//	_Freenode(pa);
		//	return pright;
		//}
	}

	if (pa->parent == NULL && pa->num <= 0)
	{
		ptr->parent = NULL;
		_Freenode(pa);
		return ptr;
	}
	else if (pa->parent != NULL && pa->num < MIN)
	{
		return Adjust_Leaf(pa);
	}
	return NULL;
}

bool Remove(BTree *ptree, ElemType x)
{
	if(ptree == NULL || *ptree == NULL) return false;
	Result res = Find(*ptree, x.key);
	if (res.pnode == NULL || !res.tag) return false;
	BNode *ptr = res.pnode;
	int pos = res.index;
	BNode *pre = Prev(ptr->sub[pos-1]);
	BNode *pnx = Next(ptr->sub[pos]);
	if (pre != NULL && pre->num > MIN)
	{
		ptr->data[pos] = pre->data[pre->num];
		ptr = pre;
		pos = pre->num;
	}
	else if (pnx != NULL && pnx->num > MIN)
	{
		ptr->data[pos] = pnx->data[1];
		ptr = pnx;
		pos = 1;
	}
	else if (pre != NULL)
	{
		ptr->data[pos] = pre->data[pre->num];
		ptr = pre;
		pos = pre->num;
	}
	else if (pnx != NULL)
	{
		ptr->data[pos] = pnx->data[1];
		ptr = pnx;
		pos = 1;
	}

	Del_Leaf_Item(ptr, pos);
	if (ptr->parent != NULL && ptr->num < MIN)
	{
		BNode *newroot = Adjust_Leaf(ptr);
		if (newroot != NULL)
		{
			*ptree = newroot;
		}
	}
	else if (ptr->parent == NULL && ptr->num == 0)
	{
		_Freenode(ptr);
		*ptree = NULL;
	}
	return true;
}
#endif