#pragma once

#include <iostream>
#include <cassert>
using namespace std;


const int M = 5;
const int BRCHMIN = M / 2;
const int BRCHMAX = M - 1;
const int LEAFMIN = M / 2 + 1;
const int LEAFMAX = M;

template<typename KeyType,typename Record>
class BTree
{
private:
	typedef enum { LEAF = 0, BRCH = 1 } NodeType;
	struct BNode
	{
		NodeType utype;
		int num;
		BNode *parent;
		KeyType key[M + 1];
		union
		{
			struct
			{
				Record *recptr[M + 1];
				BNode *prev;
				BNode *next;
			};
			BNode *sub[M + 1];
		};
	};
	typedef struct
	{
		BNode *pnode;
		int index;
		bool tag;
	}Result;

	BNode *root;
	BNode *first;
public:
	BTree() : root(NULL), first(NULL) {}
	~BTree()
	{
		Destory(root);
	}

	bool Insert(KeyType kx, Record *px)
	{
		return Insert(root, kx, px);
	}

	void InOrder()
	{
		BNode *p = first;
		while (p != NULL)
		{
			cout << p->num << " : ";
			for (int i = 0; i < p->num; ++i)
			{
				cout << p->key[i] << " ";
			}
			cout<<endl;
			p = p->next;
		}
	}
private:
	static BNode *_Buynode()
	{
		BNode *s = (BNode *)malloc(sizeof(BNode));
		assert(s != NULL);
		memset(s, 0, sizeof(BNode));
		return s;
	}

	static void _Freenode(BNode *p)
	{
		if (p != NULL)
			free(p);
	}

	static void Destory(BNode *&p)
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

	static BNode *BuyBrch()
	{
		BNode *res = _Buynode();
		res->utype = BRCH;
		return res;
	}

	static BNode *BuyLeaf()
	{
		BNode *res = _Buynode();
		return res;
	}

	static void Insert_Leaf(BNode *ptr, int pos, KeyType kx, Record *px)
	{
		for (int i = ptr->num-1; i > pos; --i)
		{
			ptr->key[i + 1] = ptr->key[i];
			ptr->recptr[i + 1] = ptr->recptr[i];
		}
		ptr->key[pos+1] = kx;
		ptr->recptr[pos+1] = px;
		ptr->num += 1;
	}

	static void Insert_Brch(BNode *ptr, int pos, KeyType kx, BNode *right)
	{
		for (int i = ptr->num; i > pos; --i)
		{
			ptr->key[i + 1] = ptr->key[i];
			ptr->sub[i + 1] = ptr->sub[i];
		}
		ptr->key[pos + 1] = kx;
		ptr->sub[pos + 1] = right;
		if (right != NULL)
			right->parent = ptr;
		ptr->num += 1;
	}

	static BNode * CreateRoot(KeyType x, BNode *left, BNode *right)
	{
		BNode *res = BuyBrch();
		res->key[1] = x;
		res->num = 1;
		res->sub[0] = left;
		left->parent = res;
		res->sub[1] = right;
		right->parent = res;
		return res;
	}

	static KeyType Move_Leaf(BNode *src, BNode *des, int pos)
	{
		for (int i = pos, j = 0; i < src->num; ++i, ++j)
		{
			des->key[j] = src->key[i];
			des->recptr[j] = src->recptr[j];
		}
		src->num = pos;
		des->num = pos;
		des->parent = src->parent;

		des->next = src->next;
		if (des->next != NULL)
			des->next->prev = src;
		src->next = des;
		des->prev = src;
		
		return des->key[0];
	}

	static BNode *Splice_Leaf(BNode *ptr)
	{
		BNode *s = BuyLeaf();
		KeyType x = Move_Leaf(ptr, s, LEAFMIN);

		if (ptr->parent == NULL)
		{
			return CreateRoot(x, ptr, s);
		}
		ptr = ptr->parent;
		ptr->key[0] = x;
		int pos = ptr->num;
		while(x < ptr->key[pos]) --pos;
		Insert_Brch(ptr, pos, x, s);
		if (ptr->num > BRCHMAX)
		{
			return Splice_Brch(ptr);
		}
		return NULL;
	}

	static KeyType Move_Brch(BNode *src, BNode *des, int pos)
	{
		for (int i = pos+1, j = 0; i <= src->num; ++i, ++j)
		{
			des->key[j] = src->key[i];
			des->sub[j] = src->sub[i];
			if (des->sub[j] != NULL)
				des->sub[j]->parent = des;
		}
		src->num = pos;
		des->num = pos;
		des->parent = src->parent;
		return des->key[0];
	}

	static BNode *Splice_Brch(BNode *ptr)
	{
		BNode *s = BuyBrch();
		KeyType x = Move_Brch(ptr, s, BRCHMIN);
		if (ptr->parent == NULL)
		{
			return CreateRoot(x, ptr, s);
		}
		ptr = ptr->parent;
		ptr->key[0] = x;
		int pos = ptr->num;
		while (x < ptr->key[pos]) --pos;
		Insert_Brch(ptr, pos, x, s);
		if (ptr->num > BRCHMAX)
		{
			return Splice_Brch(ptr);
		}
		return NULL;
	}

	bool Insert(BNode *&root, KeyType kx,Record *px)
	{
		if (root == NULL)
		{
			first = root = BuyLeaf();
			Insert_Leaf(root, -1, kx, px);
			return true;
		}
		Result res = FindFromRoot(root, kx);
		if (res.pnode == NULL || res.tag) return false;
		BNode *ptr = res.pnode;
		int    pos = res.index;

		Insert_Leaf(ptr, pos, kx, px);
		if (ptr->num > LEAFMAX)
		{
			BNode *newroot = Splice_Leaf(ptr);
			if (newroot != NULL)
			{
				root = newroot;
			}
		}
		return true;
	}

	static Result FindFromRoot(BNode *ptr,KeyType kx)
	{
		Result res = { NULL,-1,false };
		while (ptr != NULL && ptr->utype == BRCH)
		{
			ptr->key[0] = kx;
			int i = ptr->num;
			while(kx < ptr->key[i]) --i;
			res.pnode = ptr;
			res.index = i;
			ptr = ptr->sub[i];
		}
		if (ptr != NULL)
		{
			int i = ptr->num-1;
			while(i > -1 && kx < ptr->key[i]) --i;
			res.pnode = ptr;
			res.index = i;
			if (i >= 0 && ptr->key[i] == kx)
			{
				res.tag = true;
			}
		}
		return res;
	}

	static Result FindFromLeaf(BNode *ptr,KeyType kx)
	{
		Result res = { NULL,-1,false };
		while (ptr != NULL && ptr->next != NULL 
			&& ptr->key[ptr->num - 1] < kx
			&& ptr->next->key[0] < kx)
		{
			ptr = ptr->next;
		}
		if (ptr != NULL)
		{
			int i = ptr->num - 1;
			while(i > -1 && kx < ptr->key[i]) --i;
			res.pnode = ptr;
			res.index = i;
			if (i >= 0 && ptr->key[i] == kx)
			{
				res.tag = true;
			}
		}
		return res;
	}
};

#if 0
#define M 5
#define BRCHMIN (M/2)
#define BRCHMAX (M-1)
#define LEAFMIN (M/2+1)
#define LEAFMAX (M)

typedef char KeyType;
typedef struct {} Record;

typedef enum {LEAF = 0,BRCH = 1} NodeType;

struct BNode
{
	NodeType utype;
	int num;
	BNode *parent;

	KeyType key[M + 1];

	union
	{
		struct 
		{
			Record *recptr[M + 1];
			BNode *prev;
			BNode *next;
		};
		BNode *sub[M + 1];
	};
};

typedef struct
{
	BNode *pnode;
	int index;
	bool tag;
}Result;

Result FindRoot(BNode *ptr, KeyType kx)
{
	Result res = { NULL,-1,false };
	while(ptr != NULL)
	{
		int i = ptr->utype == 0 ? ptr->num-1 : ptr->num;
		while (kx < ptr->key[i] && i > 0) --i;
		if (ptr->utype == 0)
		{
			if (ptr->key[i] != kx)
			{
				res.tag = false;
			}
			res.pnode = ptr;
			res.index = i;
			break;
		}
		else
		{
			ptr = ptr->sub[i];
		}
	}
	return res;
}

Result FindFirst(BNode *ptr, KeyType kx)
{
	Result res = { NULL,-1,false };
	while (ptr != NULL)
	{
		int i = 0;
		while (i < ptr->num)
		{
			if (kx > ptr->key[i])
				++i;
			else
				break;
		}
		if (i == ptr->num)
		{
			ptr = ptr->next;
		}
		else
		{
			if (ptr->key[i] == kx)
			{
				res.tag = true;
			}
			res.pnode = ptr;
			res.index = i;
			break;
		}
	}
	return res;
}

#endif