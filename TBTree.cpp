#include <iostream>
using namespace std;
#include <cassert>
#include <memory>

#include <vld.h>

typedef char ElemType;
#define END '#'
typedef enum {LINK=0,THREAD=1} PointerTag;

class ThreadBinaryTree
{
public:
	struct BiThrNode
	{
		BiThrNode *leftchild;
		BiThrNode *rightchild;
		PointerTag Ltag, Rtag;
		ElemType data;
	};
public:
	ThreadBinaryTree(ElemType *ptr = NULL);
	~ThreadBinaryTree();
	void ClearThread();
	void MakeInOrderTree();
	void InOrder();
	void MakePreOrderTree();
	void PreOrder();
	void MakePastOrderTree();
	void PastOrder();
private:
	static BiThrNode* _BuyNode();
	static void _FreeNode(BiThrNode *q);
	static BiThrNode * CreateTree(ElemType * * const pstr);
	static void DestoryTree(BiThrNode *&p);
	static void ClearThread(BiThrNode *&p);
	static void MakeInOrderTree(BiThrNode *p, BiThrNode *&pre);
	static BiThrNode *InFirst(BiThrNode *ptr);
	static BiThrNode *InNext(BiThrNode *ptr);
	static void InOrder(BiThrNode *ptr);
	static void MakePreOrderTree(BiThrNode *p, BiThrNode *&pre);
	static void PreOrder(BiThrNode *ptr);
	static void MakePastOrderTree(BiThrNode *p, BiThrNode *&pre);
	static void PastOrder(BiThrNode *ptr);
	static BiThrNode * FindParent(BiThrNode *p, BiThrNode *child);
private:
	BiThrNode *root;
};

//int main()
//{
//	ElemType *sp = "ABC##DE##F##G#H##";
//	ThreadBinaryTree root(sp);
//	root.MakeInOrderTree();
//	root.InOrder();
//
//	root.ClearThread();
//
//	root.MakePreOrderTree();
//	root.PreOrder();
//
//	root.ClearThread();
//
//	root.MakePastOrderTree();
//	root.PastOrder();
//	return 0;
//}

ThreadBinaryTree::ThreadBinaryTree(ElemType * ptr)
{
	root = CreateTree(&ptr);
}

ThreadBinaryTree::~ThreadBinaryTree()
{
	DestoryTree(root);
}

void ThreadBinaryTree::ClearThread()
{
	ClearThread(root);
}

void ThreadBinaryTree::MakeInOrderTree()
{
	BiThrNode *pre = NULL;
	MakeInOrderTree(root, pre);
	pre->rightchild = NULL;
	pre->Rtag = THREAD;
}

void ThreadBinaryTree::InOrder()
{
	InOrder(root);
	cout << endl;
}

void ThreadBinaryTree::MakePreOrderTree()
{
	BiThrNode *pre = NULL;
	MakePreOrderTree(root,pre);
	pre->rightchild = NULL;
	pre->Rtag = THREAD;
}

void ThreadBinaryTree::PreOrder()
{
	PreOrder(root);
	cout << endl;
}

void ThreadBinaryTree::MakePastOrderTree()
{
	BiThrNode *pre = NULL;
	MakePastOrderTree(root,pre);
}

void ThreadBinaryTree::PastOrder()
{
	PastOrder(root);
}

ThreadBinaryTree::BiThrNode * ThreadBinaryTree::_BuyNode()
{
	BiThrNode *s = new BiThrNode;
	assert(NULL != s);
	memset(s, 0, sizeof(BiThrNode));
	return s;
}

void ThreadBinaryTree::_FreeNode(BiThrNode * q)
{
	delete q;
}

ThreadBinaryTree::BiThrNode * ThreadBinaryTree::CreateTree(ElemType ** const pstr)
{
	BiThrNode * s = NULL;
	if (pstr != NULL && *pstr != NULL && **pstr != END)
	{
		s = _BuyNode();
		s->data = **pstr;
		s->leftchild = CreateTree(&(++(*pstr)));
		s->rightchild = CreateTree(&(++(*pstr)));
	}
	return s;
}

void ThreadBinaryTree::DestoryTree(BiThrNode *& p)
{
	if (p != NULL)
	{
		if (p->Ltag != THREAD)
		{
			DestoryTree(p->leftchild);
		}
		if (p->Rtag != THREAD)
		{
			DestoryTree(p->rightchild);
		}
		_FreeNode(p);
	}
	p = NULL;
}

void ThreadBinaryTree::ClearThread(BiThrNode *& p)
{
	if (p != NULL)
	{
		if (p->Ltag == THREAD)
		{
			p->leftchild = NULL;
			p->Ltag = LINK;
		}
		if (p->Rtag == THREAD)
		{
			p->rightchild = NULL;
			p->Rtag = LINK;
		}
		ClearThread(p->leftchild);
		ClearThread(p->rightchild);
	}
}

void ThreadBinaryTree::MakeInOrderTree(BiThrNode * p, BiThrNode *& pre)
{
	if (p != NULL)
	{
		MakeInOrderTree(p->leftchild, pre);

		if (p->leftchild == NULL)
		{
			p->Ltag = THREAD;
			p->leftchild = pre;
		}
		if (pre != NULL && pre->rightchild == NULL)
		{
			pre->Rtag = THREAD;
			pre->rightchild = p;
		}
		pre = p;
		MakeInOrderTree(p->rightchild, pre);
	}
}

ThreadBinaryTree::BiThrNode * ThreadBinaryTree::InFirst(BiThrNode * ptr)
{
	while (ptr!= NULL && ptr->Ltag != THREAD)
	{
		ptr = ptr->leftchild;
	}
	return ptr;
}

ThreadBinaryTree::BiThrNode * ThreadBinaryTree::InNext(BiThrNode * ptr)
{
	if (ptr == NULL) return ptr;
	if (ptr->Rtag == THREAD)
	{
		return ptr->rightchild;
	}
	else
	{
		return InFirst(ptr->rightchild);
	}
}

void ThreadBinaryTree::InOrder(BiThrNode * ptr)
{
	for (BiThrNode *p = InFirst(ptr); p != NULL; p = InNext(p))
	{
		cout << p->data << " ";
	}
}

void ThreadBinaryTree::MakePreOrderTree(BiThrNode * p, BiThrNode *& pre)
{
	if (p != NULL)
	{
		if (p->leftchild == NULL)
		{
			p->leftchild = pre;
			p->Ltag = THREAD;
		}
		if (pre != NULL && pre->rightchild == NULL)
		{
			pre->rightchild = p;
			pre->Rtag = THREAD;
		}
		pre = p;
		if (p->Ltag != THREAD)
		{
			MakePreOrderTree(p->leftchild, pre);
		}
		if (p->Rtag != THREAD)
		{
			MakePreOrderTree(p->rightchild, pre);
		}	
	}
}

void ThreadBinaryTree::PreOrder(BiThrNode * ptr)
{
	BiThrNode *p = ptr;
	while (p != NULL)
	{
		cout << p->data << " ";
		if (p->Ltag == LINK)
		{
			p = p->leftchild;
		}
		else
		{
			p = p->rightchild;
		}
	}
}

void ThreadBinaryTree::MakePastOrderTree(BiThrNode * p, BiThrNode *& pre)
{
	if (p != NULL)
	{
		MakePastOrderTree(p->leftchild, pre);
		MakePastOrderTree(p->rightchild, pre);

		if (p->leftchild == NULL)
		{
			p->leftchild = pre;
			p->Ltag = THREAD;
		}
		if (pre != NULL && pre->rightchild == NULL)
		{
			pre->rightchild = p;
			pre->Rtag = THREAD;
		}
		pre = p;
	}
}

void ThreadBinaryTree::PastOrder(BiThrNode * ptr)
{
	if (ptr != NULL)
	{
		if (ptr->Ltag == THREAD)
		{
			PastOrder(ptr->leftchild);
		}
		else
		{
			PastOrder(ptr->rightchild);
		}
		cout << ptr->data << " ";
	}
}

ThreadBinaryTree::BiThrNode * ThreadBinaryTree::FindParent(BiThrNode * p, BiThrNode * child)
{
	BiThrNode * s = NULL;
	if (p != NULL && child != NULL && p != NULL)
	{
		if ((p->Ltag == LINK && p->leftchild == child) 
			|| (p->Rtag == LINK && p->rightchild == child))
			s = p;
		else
		{
			if (p->Ltag == LINK)
			{
				s = FindParent(p->leftchild, child);
			}
			if (s == NULL && p->Rtag == LINK)
			{
				s = FindParent(p->rightchild, child);
			}
		}
	}
	return s;
}

