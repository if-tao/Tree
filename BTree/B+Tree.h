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
		return Insert(root, first, kx, px);
	}

	bool Remove(KeyType kx)
	{
		return Remove(root, first, kx);
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

	static bool Insert(BNode *&root,BNode *&first,KeyType kx,Record *px)
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

/*
***************************************************************
						Remove
*/
	static void Del_Branch_Item(BNode *ptr, int pos)
	{
		for (int i = pos; i < ptr->num; ++i)
		{
			ptr->key[i] = ptr->key[i + 1];
			ptr->sub[i] = ptr->sub[i + 1];
		}
		ptr->sub[ptr->num] = NULL;
		ptr->num -= 1;
	}
	
	static void Del_Leaf_Item(BNode *ptr, int pos)
	{
		for(int i = pos+1; i < ptr->num; ++i)
		{
			ptr->key[i-1] = ptr->key[i];
			ptr->recptr[i-1] = ptr->recptr[i];
		}
		ptr->num -= 1;

		if (pos == 0 && ptr->parent != NULL && ptr->prev != NULL)
		{
			KeyType kx = ptr->key[0];
			BNode *pa = ptr->parent;
			int index = pa->num;
			while (pa != NULL)
			{
				while (index >= 0 && pa->sub[index] != ptr) --index;
				if (index != 0) break;
				ptr = pa;
				pa = ptr->parent;
				index = pa->num;
			}
			pa->key[pos] = kx;
		}
	}

	static void RMove_Leaf(BNode *pleft, BNode *ptr)
	{
		for(int i = ptr->num; i > 0; --i)
		{
			ptr->key[i] = ptr->key[i-1];
			ptr->recptr[i] = ptr->recptr[i-1];
		}
		ptr->key[0] = pleft->key[pleft->num-1];
		ptr->recptr[0] = pleft->recptr[pleft->num-1];
		ptr->num += 1;
		pleft->num -= 1;
		KeyType kx = ptr->key[0];

		while(ptr->parent != pleft->parent)
		{
			ptr = ptr->parent;
			pleft = pleft->parent;
		}
		BNode *pa = ptr->parent;
		int pos = pa->num;
		while (pos >= 0 && pa->sub[pos] != ptr) --pos;
		pa->key[pos] = kx;
	}

	static void LMove_Leaf(BNode *pright, BNode *ptr)
	{
		ptr->key[ptr->num] = pright->key[0];
		ptr->recptr[ptr->num] = pright->recptr[0];
		ptr->num += 1;
		pright->num -= 1;
		for(int i = 0; i < pright->num ; ++i)
		{
			pright->key[i] = pright->key[i+1];
			pright->recptr[i] = pright->recptr[i+1];
		}
		KeyType kx = pright->key[0];
		
		while(pright->parent != ptr->parent)
		{
			pright = pright->parent;
			ptr = ptr->parent;
		}
		BNode *pa = ptr->parent;
		int pos = pa->num;
		while(pos >= 0 && pa->sub[pos] != ptr) --pos;
		pa->key[pos+1] = kx;
	}

	static void LMerge_Leaf(BNode *pleft,BNode *ptr,BNode *pa,int pos)
	{
		for(int i = pleft->num, j = 0; j < ptr->num; ++i, ++j)
		{
			pleft->key[i] = ptr->key[j];
			pleft->recptr[i] = ptr->recptr[j];
		}
		pleft->num += ptr->num;
		Del_Branch_Item(pa, pos+1);
	}

	static void RMerge_Leaf(BNode *pright, BNode *ptr, BNode *pa, int pos)
	{
		if (pos == pa->num)
		{
			pa = pright->parent;
			pos = pos == pa->num ? 0 : pos+1;
		}
		LMerge_Leaf(ptr, pright, pa, pos); 
	}

	static BNode* Adjust_Leaf(BNode *ptr)
	{
		BNode *pa = ptr->parent;
		BNode *pleft = ptr->prev;
		BNode *pright = ptr->next;
		int pos = pa->num;
		while (pos >= 0 && pa->sub[pos] != ptr) --pos;
		if (pos < 0) { exit(-1); }

		if(pleft != NULL && pos != 0 && pleft->num > LEAFMIN)
		{
			RMove_Leaf(pleft,ptr);
		}
		else if(pright != NULL && pos != pa->num && pright->num > LEAFMIN)
		{
			LMove_Leaf(pright,ptr);
		}
		else if(pleft != NULL && pos != 0)
		{
			LMerge_Leaf(pleft, ptr,pa,pos);
			pleft->next = ptr->next;
			if (ptr->next != NULL)
				ptr->next->prev = pleft;
			_Freenode(ptr);
			ptr = pleft;
		}
		else if(pright != NULL && pos != pa->num)
		{
			RMerge_Leaf(pright, ptr, pa, pos);
			ptr->next = pright->next;
			if (pright->next != NULL)
				pright->next->prev = ptr;
			_Freenode(pright);
			pright = NULL;
		}

		if (pa->parent == NULL && pa->num <= 0)
		{
			ptr->parent = NULL;
			_Freenode(pa);
			return ptr;
		}
		else if (pa->parent != NULL && pa->num < BRCHMIN)
		{
			return Adjust_Breach(pa);
		}
		return NULL;
	}

	static void RMove_Breach(BNode *pleft, BNode *ptr, BNode *pa, int pos)
	{
		ptr->key[0] = pa->key[pos];
		ptr->sub[0] = pleft->sub[pleft->num];
		if (ptr->sub[0] != NULL)
			ptr->sub[0]->parent = ptr;
		for (int i = ptr->num + 1; i > 0; --i)
		{
			ptr->key[i] = ptr->key[i - 1];
			ptr->sub[i] = ptr->sub[i - 1];
		}
		pa->key[pos] = pleft->key[pleft->num];
		ptr->num += 1;
		pleft->num -= 1;
	}

	static void LMove_Breach(BNode *pright, BNode *ptr, BNode *pa, int pos)
	{
		ptr->num += 1;
		ptr->key[ptr->num] = pa->key[pos + 1];
		ptr->sub[ptr->num] = pright->sub[0];
		if (ptr->sub[ptr->num] != NULL)
			ptr->sub[ptr->num]->parent = ptr;
		pa->key[pos + 1] = pright->key[1];
		Del_Leaf_Item(pright, 0);
	}

	static void LMerge_Breach(BNode *pleft, BNode *ptr, BNode *pa, int pos)
	{
		ptr->key[0] = pa->key[pos];
		for (int i = pleft->num + 1, j = 0; j <= ptr->num; ++i, ++j)
		{
			pleft->key[i] = ptr->key[j];
			pleft->sub[i] = ptr->sub[j];
			if (pleft->sub[i] != NULL)
				pleft->sub[i]->parent = pleft;
		}
		pleft->num = pleft->num + ptr->num + 1;
		Del_Branch_Item(pa, pos);
	}

	static void RMerge_Breach(BNode *pright, BNode *ptr, BNode *pa, int pos)
	{
		LMerge_Breach(ptr, pright, pa, pos + 1);
	}

	static BNode *Adjust_Breach(BNode *ptr)
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

		if (pleft != NULL && pleft->num > BRCHMIN)
		{
			RMove_Breach(pleft, ptr, pa, pos);
		}
		else if (pright != NULL && pright->num > BRCHMIN)
		{
			LMove_Breach(pright, ptr, pa, pos);
		}
		else if (pleft != NULL)
		{
			LMerge_Breach(pleft, ptr, pa, pos);
			_Freenode(ptr);
			ptr = pleft;
		}
		else if (pright != NULL)
		{
			RMerge_Breach(pright, ptr, pa, pos);
			_Freenode(pright);
			pright = NULL;
		}

		if (pa->parent == NULL && pa->num <= 0)
		{
			ptr->parent = NULL;
			_Freenode(pa);
			return ptr;
		}
		else if (pa->parent != NULL && pa->num < BRCHMIN)
		{
			return Adjust_Breach(pa);
		}
		return NULL;
	}

	static bool Remove(BNode *&root,BNode *&first,KeyType kx)
	{
		if(root == NULL) return false;
		Result res = FindFromRoot(root,kx);
		if(res.pnode == NULL || !res.tag) return false;
		BNode *ptr = res.pnode;
		int pos = res.index;

		Del_Leaf_Item(ptr, pos);
		if(ptr->parent != NULL && ptr->num < LEAFMIN)
		{
			BNode *newroot = Adjust_Leaf(ptr);
			if (newroot != NULL)
			{
				root = newroot;
			}
		}
		else if(ptr->parent == NULL && ptr->num == 0)
		{
			_Freenode(ptr);
			first = root = NULL;
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