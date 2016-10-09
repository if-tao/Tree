#include <iostream>
using namespace std;
#include <cassert>
#include <memory>
#include <stack>
#include <queue>

#include <vld.h>


#define END '#'	

template<typename Type>
class BinaryTree
{
private:
	struct BtNode
	{
		BtNode* leftchild;
		BtNode* rightchild;
		Type data;
	};
public:
	typedef BtNode * PBtNode;
	BinaryTree(Type *ptr = NULL,Type endvalue = '#');
	~BinaryTree();
	BinaryTree(const BinaryTree &p);
	BinaryTree& operator=(const BinaryTree &p);
	BtNode * GetRoot();
	void ClearBTree();
	void PreOrder() const;
	void InOrder() const;
	void PastOrder() const;
	void NicePreOrder() const;
	void NiceInOrder() const;
	void NicePastOrder() const;
	void NiceLevelOrder() const;
	void PrintLevelItem(int k) const;
	int Size() const;
	int Depth() const;
	bool IsFullBTree() const;
	bool IsCompleteBTree() const;
	void CreateTreePI(Type *ps, Type *is);
	void CreateTreeIL(Type *is, Type *ls);
	BtNode *FindValue(Type x) const;
	BtNode *FindParent(BtNode *child) const;
	BtNode *NearParent(BtNode *child1, BtNode *child2) const;
	bool operator==(const BinaryTree &p);
	bool operator!=(const BinaryTree &p);
	bool FindThePath(stack<BtNode *> &st, BtNode *object);
private:
	static BtNode* _BuyNode();
	static void _FreeNode(BtNode *q);
	static BtNode* CreateTree(Type *& pstr);
	static void DestoryTree(BtNode *&p);
	static void PreOrder(BtNode *p);
	static void InOrder(BtNode *p);
	static void PastOrder(BtNode *p);
	static void NicePreOrder(BtNode *p);
	static void NiceInOrder(BtNode *p);
	static void NicePastOrder(BtNode *p);
	static void NiceLevelOrder(BtNode *p);
	static void PrintLevelItem(BtNode *p, int k);
	static int Size(BtNode *p);
	static int Depth(BtNode *p);
	static bool IsFullBTree(BtNode *p);
	static bool IsCompleteBTree(BtNode *p);
	static int FindIs(Type *ptr, int len, Type x);
	static BtNode * CreateTreePI(Type *ps, Type *is, int n);
	static BtNode * CreateTreeIL(Type *is, Type *ls, int n);
	static BtNode * FindValue(BtNode *ptr, Type x);
	static BtNode * FindParent(BtNode *ptr, BtNode *child);
	static BtNode * NearParent(BtNode *root, BtNode *p1, BtNode *p2);
	static void Copy(BtNode *&desc, BtNode *src);
	static bool Equal(const BtNode* desc, const BtNode* src);
	static bool FindThePath(stack<BtNode *> &st, BtNode *ptr, BtNode *object);
private:
	BtNode *root;
	Type EndValue;
};



template<typename Type>
BinaryTree<Type>::BinaryTree(Type * ptr,Type endvalue)
{
	root = CreateTree(ptr);
	EndValue = endvalue;
}

template<typename Type>
BinaryTree<Type>::~BinaryTree()
{
	DestoryTree(root);
}

template<typename Type>
BinaryTree<Type>::BinaryTree(const BinaryTree & p)
{
	Copy(root, p.root);
	EndValue = p.EndValue;
}

template<typename Type>
BinaryTree<Type> & BinaryTree<Type>::operator=(const BinaryTree & p)
{
	if (this != &p)
	{
		DestoryTree(root);
		Copy(root, p.root);
	}
	return *this;
}

template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::GetRoot()
{
	return root;
}

template<typename Type>
void BinaryTree<Type>::ClearBTree()
{
	DestoryTree(root);
}

template<typename Type>
void BinaryTree<Type>::PreOrder() const
{
	PreOrder(root);
	cout << endl;
}

template<typename Type>
void BinaryTree<Type>::InOrder() const
{
	InOrder(root);
	cout << endl;
}

template<typename Type>
void BinaryTree<Type>::PastOrder() const
{
	PastOrder(root);
	cout << endl;
}

template<typename Type>
void BinaryTree<Type>::NicePreOrder() const
{
	NicePreOrder(root);
	cout << endl;
}

template<typename Type>
void BinaryTree<Type>::NiceInOrder() const
{
	NiceInOrder(root);
	cout << endl;
}

template<typename Type>
void BinaryTree<Type>::NicePastOrder() const
{
	NicePastOrder(root);
	cout << endl;
}

template<typename Type>
void BinaryTree<Type>::NiceLevelOrder() const
{
	NiceLevelOrder(root);
	cout << endl;
}

template<typename Type>
void BinaryTree<Type>::PrintLevelItem(int k) const
{
	PrintLevelItem(root, k);
	cout << endl;
}

template<typename Type>
int BinaryTree<Type>::Size() const
{
	return Size(root);
}

template<typename Type>
int BinaryTree<Type>::Depth() const
{
	return Depth(root);
}

template<typename Type>
bool BinaryTree<Type>::IsFullBTree() const
{
	return IsFullBTree(root);
}

template<typename Type>
bool BinaryTree<Type>::IsCompleteBTree() const
{
	return IsCompleteBTree(root);
}

template<typename Type>
void BinaryTree<Type>::CreateTreePI(Type * ps, Type * is)
{
	int a = strlen(ps);
	int b = strlen(is);
	int len = a < b ? a : b;
	root = CreateTreePI(ps, is, len);
}

template<typename Type>
void BinaryTree<Type>::CreateTreeIL(Type * is, Type * ls)
{
	int a = strlen(is);
	int b = strlen(ls);
	int len = a < b ? a : b;
	root = CreateTreeIL(is, ls, len);
}

template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::FindValue(const Type x) const
{
	return FindValue(root,x);
}

template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::FindParent(BtNode * child) const
{
		return FindParent(root,child);
}

template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::NearParent(BtNode * child1, BtNode * child2) const
{
	return NearParent(root,child1,child2);
}

template<typename Type>
bool BinaryTree<Type>::operator==(const BinaryTree & p)
{
	return Equal(root, p.root);
}

template<typename Type>
bool BinaryTree<Type>::operator!=(const BinaryTree & p)
{
	//return !(*this == p);
	return !Equal(root,p.root);
}

template<typename Type>
bool BinaryTree<Type>::FindThePath(stack<BtNode*>& st, BtNode * object)
{
	return FindThePath(st, root, object);
}



/*
/////////////////////////////////////////////////////////////
以下函数均为static 静态函数 即具体实现方法
*/

template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::_BuyNode()
{
	BtNode *s = new BtNode;
	assert(NULL != s);
	memset(s, 0, sizeof(BtNode));
	return s;
}

template<typename Type>
void BinaryTree<Type>::_FreeNode(BtNode * q)
{
	delete q;
}

template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::CreateTree(Type *& pstr)
{
	BtNode * s = NULL;
	if (pstr != NULL && *pstr != END && *pstr != '\0')
	{
		s = _BuyNode();
		s->data = *pstr;
		s->leftchild = CreateTree(++pstr);
		s->rightchild = CreateTree(++pstr);
	}
	return s;
}

template<typename Type>
void BinaryTree<Type>::DestoryTree(BtNode *& p)
{
	if (p != NULL)
	{
		DestoryTree(p->leftchild);
		DestoryTree(p->rightchild);
		_FreeNode(p);
	}
	p = NULL;
}

template<typename Type>
void BinaryTree<Type>::PreOrder(BtNode * p)
{
	if (p != NULL)
	{
		cout << p->data << " ";
		PreOrder(p->leftchild);
		PreOrder(p->rightchild);
	}
}

template<typename Type>
void BinaryTree<Type>::InOrder(BtNode * p)
{
	if (p != NULL)
	{
		InOrder(p->leftchild);
		cout << p->data << " ";
		InOrder(p->rightchild);
	}
}

template<typename Type>
void BinaryTree<Type>::PastOrder(BtNode * p)
{
	if (p != NULL)
	{
		PastOrder(p->leftchild);
		PastOrder(p->rightchild);
		cout << p->data << " ";
	}
}

template<typename Type>
void BinaryTree<Type>::NicePreOrder(BtNode * p)
{
	if (p == NULL) return;
	stack<BtNode*> st;

	st.push(p);
	while (!st.empty())
	{
		p = st.top(); st.pop();
		cout << p->data << " ";
		if (p->rightchild != NULL)
			st.push(p->rightchild);
		if (p->leftchild != NULL)
			st.push(p->leftchild);
	}
}

template<typename Type>
void BinaryTree<Type>::NiceInOrder(BtNode * p)
{
	if (p == NULL) return;
	stack<BtNode*> s;

	while (!s.empty() || p != NULL)
	{
		while (p != NULL)
		{
			s.push(p);
			p = p->leftchild;
		}

		p = s.top(); s.pop();
		cout << p->data << " ";
		p = p->rightchild;
	}
}

template<typename Type>
void BinaryTree<Type>::NicePastOrder(BtNode * p)
{
	if (p == NULL) return;
	stack<BtNode*> s;

	BtNode* over = NULL;
	while (!s.empty() || p != NULL)
	{
		while (p != NULL)
		{
			s.push(p);
			p = p->leftchild;
		}
		p = s.top(); s.pop();
		if (p->rightchild != NULL && p->rightchild != over)
		{
			s.push(p);
			p = p->rightchild;
		}
		else
		{
			cout << p->data << " ";
			over = p;
			p = NULL;
		}
	}
}

template<typename Type>
void BinaryTree<Type>::NiceLevelOrder(BtNode * p)
{
	if (p == NULL) return;

	queue<BtNode*> que;
	while (!que.empty() || p != NULL)
	{
		if (p != NULL)
		{
			cout << p->data << " ";
			que.push(p->leftchild);
			que.push(p->rightchild);
			p = NULL;
		}
		else
		{
			p = que.front();
			que.pop();
		}
	}
}

template<typename Type>
void BinaryTree<Type>::PrintLevelItem(BtNode * p, int k)
{
	if (p == NULL || k < 0) return;
	
	if (k == 0)
	{
		cout << p->data << " ";
	}
	else
	{
		PrintLevelItem(p->leftchild, k-1);
		PrintLevelItem(p->rightchild, k-1);
	}
}

template<typename Type>
int BinaryTree<Type>::Size(BtNode * p)
{
	int num = 0;
	if (p != NULL)
	{
		num = 1 + Size(p->leftchild) + Size(p->rightchild);
	}
	return num;
}

template<typename Type>
int BinaryTree<Type>::Depth(BtNode * p)
{
	int dep = 0;
	if (p != NULL)
	{
		int a = Depth(p->leftchild);
		int b = Depth(p->rightchild);
		dep = a > b ? a + 1 : b + 1;
	}
	return dep;
}

template<typename Type>
bool BinaryTree<Type>::IsFullBTree(BtNode * p)
{
	if (p == NULL || (p->leftchild == NULL && p->rightchild == NULL))
		return true;
	else if (p->leftchild != NULL && p->rightchild != NULL
		&& (Depth(p->leftchild) == Depth(p->rightchild)))
	{
		return IsFullBTree(p->leftchild) && IsFullBTree(p->rightchild);
	}
	else
		return false;
}

template<typename Type>
bool BinaryTree<Type>::IsCompleteBTree(BtNode * p)
{
	if (p != NULL)
	{
		if (p->leftchild != NULL)
		{
			int n = Depth(p->leftchild) - Depth(p->rightchild);
			if (n == 0)
			{
				return IsFullBTree(p->leftchild) && IsCompleteBTree(p->rightchild);
			}
			else if (n == 1)
			{
				return IsCompleteBTree(p->leftchild) && IsFullBTree(p->rightchild);
			}
			else
			{
				return false;
			}
		}
		else if (p->rightchild != NULL)
		{
			return false;
		}
	}
	return true;
}

template<typename Type>
int BinaryTree<Type>::FindIs(Type * ptr, int len, Type x)
{
	for (int i = 0; i < len; ++i)
	{
		if (ptr[i] == x)
		{
			return i;
		}
	}
	return -1;
}

template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::CreateTreePI(Type * ps, Type * is, int n)
{
	BtNode * root = NULL;

	if (n > 0 && ps != NULL && is != NULL)
	{
		Type *p = ps;
		Type *i = is;
		root = _BuyNode();
		root->data = ps[0];

		int num = FindIs(is, n, ps[0]);
		if (num == -1)
		{
			_FreeNode(root);
			return NULL;
		}

		root->leftchild = CreateTreePI(ps + 1, is, num);
		root->rightchild = CreateTreePI(ps + num + 1, is + num + 1, n - num - 1);
	}
	return root;
}

template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::CreateTreeIL(Type * is, Type * ls, int n)
{
	BtNode *root = NULL;

	if (n > 0 && is != NULL && ls != NULL)
	{
		Type *i = is;
		Type *l = ls;
		root = _BuyNode();
		root->data = ls[n - 1];

		int num = FindIs(is, n, ls[n - 1]);
		if (num == -1)
		{
			_FreeNode(root);
			return NULL;
		}

		root->leftchild = CreateTreeIL(is, ls, num);
		root->rightchild = CreateTreeIL(is + num + 1, ls + num, n - num - 1);
	}
	return root;
}

template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::FindValue(BtNode * ptr, Type x)
{
	BtNode * s = ptr;
	if (ptr != NULL && ptr->data != x)
	{
		s = FindValue(ptr->leftchild, x);
		if (s == NULL)
			s = FindValue(ptr->rightchild, x);
	}
	return s;
}

template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::FindParent(BtNode * ptr, BtNode * child)
{
	BtNode * s = NULL;
	if (ptr != NULL && child != NULL && ptr != child)
	{
		if (ptr->leftchild == child || ptr->rightchild == child)
			s = ptr;
		else
		{
			s = FindParent(ptr->leftchild, child);
			if (s == NULL)
				s = FindParent(ptr->rightchild, child);
		}

	}
	return s;
}

//template<typename Type>
//typename BinaryTree<Type>::BtNode * BinaryTree<Type>::NearParent(BtNode * root, BtNode * p1, BtNode * p2)
//{
//	if (root == NULL || p1 == NULL || p2 == NULL)
//	{
//		return NULL;
//	}
//	else if (p1 == p2)
//	{
//		return FindParent(root, p1);
//	}
//	else
//	{
//		stack<BtNode*>s1;
//		stack<BtNode*>s2;
//
//		BtNode *p = NULL;
//		while ((p = FindParent(root, p1)) != NULL)
//		{
//			s1.push(p);
//			p1 = p;
//		}
//
//		while ((p = FindParent(root, p2)) != NULL)
//		{
//			s2.push(p);
//			p2 = p;
//		}
//
//		while (!s1.empty() && !s2.empty() && s1.top() == s2.top())
//		{
//			p = s1.top();
//			s1.pop();
//			s2.pop();
//		}
//
//		return p;
//	}
//}

/*
改进版本，同样需要两个栈，不过总共遍历两次树就可以
*/
template<typename Type>
typename BinaryTree<Type>::BtNode * BinaryTree<Type>::NearParent(BtNode * root, BtNode * p1, BtNode * p2)
{
	if (root == NULL || p1 == NULL || p2 == NULL)
		return NULL;
	else if (p1 == p2)
		return FindParent(root, p1);
	else
	{
		stack<BtNode *> st1;
		stack<BtNode *> st2;
		FindThePath(st1, root, p1);
		FindThePath(st2, root, p2);

		while (st1.size() > st2.size())
			st1.pop();
		while (st1.size() < st2.size())
			st2.pop();
		while (!st1.empty())
		{
			if (st1.top() != st2.top())
			{
				st1.pop(); st2.pop();
			}
			else
				return st1.top();
		}
		return NULL;
	}
}

/*
拷贝函数
*/
template<typename Type>
void BinaryTree<Type>::Copy(BtNode * &desc, BtNode * src)
{
	if (src == NULL) desc = NULL;
	else
	{
		desc = _BuyNode();
		desc->data = src->data;
		Copy(desc->leftchild, src->leftchild);
		Copy(desc->rightchild, src->rightchild);
	}
}

/*
判等函数，判断每个节点是否相同
*/
template<typename Type>
bool BinaryTree<Type>::Equal(const BtNode* desc, const BtNode* src)
{
	if (desc != NULL && src != NULL)
	{
		return desc->data == src->data
			&& Equal(desc->leftchild, src->leftchild)
			&& Equal(desc->rightchild, src->rightchild);
	}
	else
	{
		return desc == src;
	}
	//return (desc == NULL && src == NULL) || 
	//(desc != NULL && src != NULL 
	//&& desc->data == src->data
	//&& Equal(desc->leftchild,src->leftchild)
	//&& Equal(desc->rightchild,src->rightchild) )
}

/*
找到根节点得到目的节点的所有经过节点并记录在栈中
*/
template<typename Type>
bool BinaryTree<Type>::FindThePath(stack<BtNode*>& st, BtNode *ptr, BtNode * object)
{
	if (ptr == NULL || ptr == object)
		return ptr == object;
	else
	{
		st.push(ptr);
		if ( !FindThePath(st,ptr->leftchild,object) )
		{
			while (st.top() != ptr)
			{
				st.pop();
			}
			if (!FindThePath(st, ptr->rightchild, object))
			{
				while (st.top() != ptr)
				{
					st.pop();
				}
				return false;
			}
			else return true;

			return false;
		}
		else return true;
	}
}


/*
##################################################
					迭代器
##################################################
*/

template<typename Type>
class TreeIterator
{
protected:
	BinaryTree<Type> &tree;
	typename BinaryTree<Type>::PBtNode _Ptr;
public:
	TreeIterator(BinaryTree<Type> &t) :tree(t), _Ptr(NULL) {}
	virtual void First() = 0;
	Type &operator*() { return _Ptr->data; }
	const Type & operator*() const { return _Ptr->data; }
	bool IsDone() const { return _Ptr == NULL; }
	virtual void operator++() = 0;
};

template<typename Type>
class PreIterator : public TreeIterator<Type>
{
protected:
	stack<typename BinaryTree<Type>::PBtNode> st;
public:
	PreIterator(BinaryTree<Type> &t) :TreeIterator<Type>(t) {}
	virtual void First()
	{
		_Ptr = NULL;
		if (tree.GetRoot() != NULL)
		{
			st.push(tree.GetRoot());
			operator++();
		}
	}
	virtual void operator++()
	{
		if (!st.empty())
		{
			_Ptr = st.top(); st.pop();
			if (_Ptr->rightchild != NULL)
				st.push(_Ptr->rightchild);
			if (_Ptr->leftchild != NULL)
				st.push(_Ptr->leftchild);
		}
		else
			_Ptr = NULL;
	}
};

//template <typename Type>
//struct StkNode
//{
//	typename BinaryTree<Type>::PBtNode pnode;
//	int popnum;
//public:
//	StkNode(typename BinaryTree<Type>::PBtNode p = NULL) :pnode(p), popnum(0) {}
//};
//
//template<typename Type>
//class PastIterator : public TreeIterator<Type>
//{
//protected:
//	stack<StkNode<Type> > st;
//public:
//	PastIterator(BinaryTree<Type> &t) :TreeIterator<Type>(t) {}
//	virtual void First()
//	{
//		_Ptr = NULL;
//		if (tree.GetRoot() != NULL)
//		{
//			st.push(StkNode<Type>(tree.GetRoot()));
//			operator++();
//		}
//	}
//	virtual void operator++()
//	{
//		if (st.empty())
//		{
//			_Ptr = NULL;
//			return;
//		}
//		StkNode<Type> p;
//		for ( ; ; )
//		{
//			p = st.top(); st.pop();
//			if (++p.popnum == 3)
//			{
//				_Ptr = p.pnode;
//				return;
//			}
//			else
//			{
//				st.push(p);
//				if (p.popnum == 1 && p.pnode->leftchild != NULL)
//				{
//					st.push(StkNode<Type>(p.pnode->leftchild));
//				}
//				else if (p.popnum == 2 && p.pnode->rightchild != NULL)
//				{
//					st.push(StkNode<Type>(p.pnode->rightchild));
//				}
//			}
//		}
//	}
//};
//
//template<typename Type>
//class InIterator : public TreeIterator<Type>
//{
//protected:
//	stack<StkNode<Type> > st;
//public:
//	InIterator(BinaryTree<Type> &t):TreeIterator<Type>(t){}
//	virtual void First()
//	{
//		_Ptr = NULL;
//		if (tree.GetRoot() != NULL)
//		{
//			st.push(StkNode<Type>(tree.GetRoot()));
//			operator++();
//		}
//	}
//
//	virtual void operator++()
//	{
//		if (st.empty())
//		{
//			_Ptr = NULL;
//			return;
//		}
//		StkNode<Type> p;
//		for (; ;)
//		{
//			p = st.top(); st.pop();
//			if (++p.popnum == 2)
//			{
//				_Ptr = p.pnode;
//				if (p.pnode->rightchild != NULL)
//					st.push(StkNode<Type>(p.pnode->rightchild));
//				return;
//			}
//			else
//			{
//				st.push(p);
//				if (p.pnode->leftchild != NULL)
//					st.push(StkNode<Type>(p.pnode->leftchild));
//			}
//		}
//	}
//};

template<typename Type>
class InIterator : public TreeIterator<Type>
{
protected:
	stack<typename BinaryTree<Type>::PBtNode> st;
public:
	InIterator(BinaryTree<Type> &t) :TreeIterator(t) {}
	virtual void First()
	{
		_Ptr = NULL;
		if (tree.GetRoot() != NULL)
		{
			st.push(tree.GetRoot());
			operator++();
		}
	}

	virtual void operator++()
	{
		if (!st.empty())
		{
			BinaryTree<Type>::PBtNode p = st.top(); st.pop();
			while (!st.empty() || p != NULL)
			{
				while (p != NULL)
				{
					st.push(p);
					p = p->leftchild;
				}
				if (!st.empty())
					p = st.top(); st.pop();
				_Ptr = p;
				st.push(p->rightchild);
				return;
			}
		}
		_Ptr = NULL;
	}
};

template<typename Type>
class PastIterator : public TreeIterator<Type>
{
protected:
	stack<typename BinaryTree<Type>::PBtNode> st;
public:
	PastIterator(BinaryTree<Type> &t) :TreeIterator(t) {}
	virtual void First()
	{
		_Ptr = NULL;
		if (tree.GetRoot() != NULL)
		{
			st.push(tree.GetRoot());
			operator++();
		}
	}

	virtual void operator++()
	{
		if (!st.empty())
		{
			BinaryTree<Type>::PBtNode p = st.top(); st.pop();
			BinaryTree<Type>::PBtNode over = NULL;
			if (!st.empty())
			{
				over = st.top(); st.pop();
			}
			while (!st.empty() || p != NULL)
			{
				while (p != NULL)
				{
					st.push(p);
					p = p->leftchild;
				}
				p = st.top(); st.pop();
				if (p->rightchild != NULL && p->rightchild != over)
				{
					st.push(p);
					p = p->rightchild;
				}
				else
				{
					_Ptr = p;
					st.push(p);//over;
					st.push(NULL);
					return;
				}
			}
		}
		_Ptr = NULL;
	}
};

template<typename Type>
class LevelIterator : public TreeIterator<Type>
{
protected:
	queue<typename BinaryTree<Type>::PBtNode> que;
public:
	LevelIterator(BinaryTree<Type> &t) :TreeIterator(t) {}
	virtual void First()
	{
		_Ptr = NULL;
		if (tree.GetRoot() != NULL)
		{
			que.push(tree.GetRoot());
			operator++();
		}
	}

	virtual void operator++()
	{
		if (que.empty())
		{
			_Ptr = NULL;
			return;
		}
		BinaryTree<Type>::PBtNode p = NULL;
		p = que.front(); que.pop();
		_Ptr = p;
		if (p->leftchild != NULL)
		{
			que.push(p->leftchild);
		}
		if (p->rightchild != NULL)
		{
			que.push(p->rightchild);
		}
	}
};

//template<typename Type>
//class PreIterator : public TreeIterator<Type>
//{
//public:
//	PreIterator(BinaryTree<Type> &t) :TreeIterator(t) {}
//	void First();
//	void operator++();
//private:
//	typename BinaryTree<Type>::PBtNode PreNext(typename BinaryTree<Type>::PBtNode p);
//};
//
//template<typename Type>
//void PreIterator<Type>::First()
//{
//	_Ptr = tree.GetRoot();
//}
//
//template<typename Type>
//void PreIterator<Type>::operator++()
//{
//	_Ptr = PreNext(_Ptr);
//}
//
//template<typename Type>
//typename BinaryTree<Type>::PBtNode PreIterator<Type>::PreNext(typename BinaryTree<Type>::PBtNode p)
//{
//	typename BinaryTree<Type>::PBtNode ret = p;
//	if (p == NULL)
//		ret = p;
//	else if (p->leftchild != NULL)
//	{
//		ret = p->leftchild;
//	}
//	else if (p->rightchild != NULL)
//	{
//		ret = p->rightchild;
//	}
//	else
//	{
//		typename BinaryTree<Type>::PBtNode parent = tree.FindParent(p);
//		if (parent == NULL) return NULL;
//		if (parent->rightchild != p && parent->rightchild != NULL)
//		{
//			ret = parent->rightchild;
//		}
//		else
//		{
//			while (parent != NULL && (parent->rightchild == p || parent->rightchild == NULL))
//			{
//				p = parent;
//				parent = tree.FindParent(parent);
//			}
//			if (parent != NULL)
//			{
//				ret = parent->rightchild;
//			}
//			else
//				ret = NULL;
//		}
//	}
//	return ret;
//}