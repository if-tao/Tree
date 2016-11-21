#pragma once

#include <iostream>

using namespace std;

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