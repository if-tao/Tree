#ifndef _RBTREE_H_
#define _RBTREE_H_



#include <iostream>
#include <memory>

using namespace std;

template<class _Ty>
class R_BTree
{
protected:
	enum _Redbl{_Red=0,_Black=1};
	struct _Node;
	typedef struct _Node * _Nodeptr;
	struct _Node
	{
		_Nodeptr _Left,_Parent,_Right;
		_Ty _Value;
		_Redbl _Color;
	};

	typedef struct _Node *& _Nodepref;
	typedef _Redbl  &       _Rbref;
	typedef _Ty     &       _Vref;
	static _Rbref _Color(_Nodeptr _P)
	{
		return _P->_Color;
	}
	static _Vref  _Value(_Nodeptr _P)
	{
		return _P->_Value;
	}
	static _Nodepref _Left(_Nodeptr _P)
	{
		return _P->_Left;
	}
	static _Nodepref _Right(_Nodeptr _P)
	{
		return _P->_Right;
	}
	static _Nodepref _Parent(_Nodeptr _P)
	{
		return _P->_Parent;
	}
public:

	class iterator
	{
	private:
		typedef typename R_BTree::_Node _Node;
		typedef typename R_BTree::_Nodeptr _Nodeptr;
		R_BTree * _Tree;
		typename R_BTree::_Nodeptr _Ptr;
	public:
		iterator(R_BTree* tree = NULL,_Nodeptr _ptr=NULL) :_Tree(tree), _Ptr(_ptr) {}
		_Node operator*() const { return *_Ptr; }
		void operator++() { _Ptr = _Tree->_Next(_Ptr); }
		bool operator==(const iterator &it)
		{
			return this->_Tree == it._Tree && this->_Ptr == it._Ptr;
		}
		bool operator!=(const iterator &it)
		{
			return !(*this == it);
		}
	};
	iterator begin()
	{
		return iterator(this,_Left(_Head));
	}
	iterator end()
	{
		return iterator(this,_Nil);
	}
	//class const_iterator
	//{};
	//const_iterator begin() const;
	//const_iterator end() const;


	void Show(_Node &node)
	{
		cout << "Value : " << node._Value;
		cout << "; Color: " << (node._Color == _Red ? "Red  " : "Black");
		cout << "; Parent: " << (node._Parent)->_Value << endl;
	}


	R_BTree():_Head(NULL),_Nil(NULL) 
	{
		_Init();
	}
	~R_BTree()
	{
		_Destory(_Root());
		_Freenode(_Head); _Head = NULL;
		_Freenode(_Nil); _Nil = NULL;
	}
	void Insert(const _Ty &_V)
	{
		_Nodeptr _X = _Root();
		_Nodeptr _Y = _Head;
		while(_X != _Nil && _Value(_X) != _V)
		{
			_Y = _X;
			_X = _V < _Value(_X) ? _Left(_X):_Right(_X);
		}
		if(_X != _Nil) return ;
		_Nodeptr _Z = _Insert(_X,_Y,_V);

		if (_Left(_Head) == NULL || _Value(_Left(_Head)) > _V)
		{
			if(_Right(_Head) == NULL)
				_Right(_Head) = _Z;
			_Left(_Head) = _Z;
		}
		else if(_Value(_Right(_Head)) < _V)
		{
			_Right(_Head) = _Z;
		}
	}

	void InOrder()
	{
		for (_Nodeptr _P = _First(_Root()); _P != _Nil; _P = _Next(_P))
		{
			cout << "Value : " << _Value(_P);
			cout << "; Color: " << (_Color(_P) == _Red ? "Red  " : "Black");
			cout << "; Parent: " << _Value(_Parent(_P))<<endl;
		}
		cout << endl;
	}

	void Delete(const _Ty &_V)
	{
		_Nodeptr _X = _Root();
		_Nodeptr _Y = _Nil;
		while (_X != _Nil && _Value(_X) != _V)
		{
			_Y = _X;
			_X = _V < _Value(_X) ? _Left(_X) : _Right(_X);
		}
		if (_X == _Nil) return ;
		if (_Left(_X) != _Nil && _Right(_X) != _Nil)
		{
			_Y = _Next(_X);
			_Value(_X) = _Value(_Y);
		}
		else
		{
			_Y = _X;
		}
		_Delete(_Y);
	}
private:
	_Nodeptr _Root() const { return _Parent(_Head); }
	_Nodepref _Root() { return _Parent(_Head); }

	void _Delete(_Nodeptr _Y)
	{
		_Nodeptr _Z = _Left(_Y) != _Nil ? _Left(_Y) : _Right(_Y);
		if (_Y == _Root())
		{
			_Root() = _Z;
		}
		else if(_Y == _Left(_Parent(_Y)))
		{
			_Left(_Parent(_Y)) = _Z;
		}
		else
		{
			_Right(_Parent(_Y)) = _Z;
		}
		_Parent(_Z) = _Parent(_Y);

		if (_Color(_Y) == _Black)
		{
			_DeleteColor(_Z);
		}

		_Freenode(_Y);
	}
	
	void _DeleteColor(_Nodeptr _X)
	{
		_Nodeptr _Y = _Nil;
		while (_X != _Root() && _Color(_X) == _Black)
		{
			if (_X == _Left(_Parent(_X)))
			{
				_Y = _Right(_Parent(_X));
				if (_Color(_Y) == _Red)
				{
					_Color(_Y) = _Black;
					_Color(_Parent(_X)) = _Red;
					_Lrotate(_Parent(_X));
					_Y = _Right(_Parent(_X));
				}
				if (_Color(_Left(_Y)) == _Black && _Color(_Right(_Y)) == _Black)
				{
					_Color(_Y) = _Red;
					_X = _Parent(_X);
				}
				else
				{
					if (_Color(_Right(_Y)) == _Black)
					{
						_Color(_Left(_Y)) = _Black;
						_Color(_Y) = _Red;
						_Rrotate(_Y);
						_Y = _Parent(_Parent(_X));
					}
					_Color(_Y) = _Color(_Parent(_X));
					_Color(_Parent(_X)) = _Black;
					_Color(_Right(_Y)) = _Black;
					_Lrotate(_Parent(_X));
					_X = _Root();
				}
			}
			else
			{
				_Y = _Left(_Parent(_X));
				if (_Color(_Y) == _Red)
				{
					_Color(_Y) = _Black;
					_Color(_Parent(_X)) = _Red;
					_Rrotate(_Parent(_X));
					_Y = _Left(_Parent(_X));
				}
				if (_Color(_Right(_Y)) == _Black && _Color(_Left(_Y)) == _Black)
				{
					_Color(_Y) = _Red;
					_X = _Parent(_X);
				}
				else
				{
					if (_Color(_Left(_Y)) == _Black)
					{
						_Color(_Right(_Y)) = _Black;
						_Color(_Y) = _Red;
						_Lrotate(_Y);
						_Y = _Parent(_Parent(_X));
					}
					_Color(_Y) = _Color(_Parent(_X));
					_Color(_Parent(_X)) = _Black;
					_Color(_Left(_Y)) = _Black;
					_Rrotate(_Parent(_X));
					_X = _Root();
				}
			}
		}//while end
		_Color(_X) = _Black;
	}

	void _Destory(_Nodeptr _P)
	{
		if (_P == _Nil) return;
		if (_Left(_P) != _Nil)
			_Destory(_Left(_P));
		if(_Right(_P) != _Nil)
			_Destory(_Right(_P));
		
		_Freenode(_P);
		_P = NULL;
	}

	_Nodeptr _First(_Nodeptr _P)
	{
		while (_P != _Nil && _Left(_P) != _Nil)
		{
			_P = _Left(_P);
		}
		return _P;
	}

	_Nodeptr _Next(_Nodeptr _P)
	{
		if (_P == _Nil) return _P;
		_Nodeptr _PA = _Nil;
		if (_Right(_P) == _Nil)
		{
			_PA = _Parent(_P);
			while (_PA != _Head && _Right(_PA) == _P)
			{
				_P = _PA;
				_PA = _Parent(_P);
			}
			_P = _PA == _Head ? _Nil : _PA;
		}
		else
		{
			_P = _First(_Right(_P));
		}
		return _P;
	}

	_Nodeptr _Insert(_Nodeptr _X,_Nodeptr _Y,const _Ty &_V)
	{
		_Nodeptr _Z = _Buynode(_Y,_Red);
		if(_Y == _Head)
		{
			_Parent(_Head) = _Z;
		}else
		{
			if(_V < _Value(_Y))
				_Left(_Y) = _Z;
			else
				_Right(_Y) = _Z;
		}
		_Left(_Z) = _Right(_Z) = _Nil;
		new(&_Value(_Z)) _Ty(_V); // 
		_X = _Z;
		
		for(;_X != _Root() && _Color(_Parent(_X)) == _Red;)
		{
			if(_Parent(_X) == _Left(_Parent(_Parent(_X)))) // left
			{
				_Y = _Right(_Parent(_Parent(_X)));
				if(_Color(_Y) == _Red)
				{
					_Color(_Parent(_X)) = _Black;
					_Color(_Y) = _Black;
					_Color(_Parent(_Parent(_X))) = _Red;
					_X = _Parent(_Parent(_X));
				}else
				{
					if(_X == _Right(_Parent(_X)))
					{
						_X = _Parent(_X);
						_Lrotate(_X);
					}
					_Color(_Parent(_X)) = _Black;
					_Color(_Parent(_Parent(_X))) = _Red;
					_Rrotate(_Parent(_Parent(_X)));
				}
			}
			else // right
			{
				_Y = _Left(_Parent(_Parent(_X)));
				if(_Color(_Y) == _Red)
				{
					_Color(_Parent(_X)) = _Black;
					_Color(_Y) = _Black;
					_Color(_Parent(_Parent(_X))) = _Red;
					_X = _Parent(_Parent(_X));
				}else
				{
					if(_X == _Left(_Parent(_X)))
					{
						_X = _Parent(_X);
						_Rrotate(_X);
					}
					_Color(_Parent(_X)) = _Black;
					_Color(_Parent(_Parent(_X))) = _Red;
					_Lrotate(_Parent(_Parent(_X)));
				}
			}
		}//for end
		_Color(_Root()) = _Black;
		return _Z;
	}

	void _Rrotate(_Nodeptr _X)
	{
		_Nodeptr _Y = _Left(_X);
		_Left(_X) = _Right(_Y);
		if(_Right(_Y) != _Nil)
		{
			_Parent(_Right(_Y)) = _X;
		}
		_Parent(_Y) = _Parent(_X);
		if(_X == _Root())
		{
			_Root() = _Y;
		}else if(_X == _Right(_Parent(_X)))
		{
			_Right(_Parent(_X)) = _Y;
		}else 
		{
			_Left(_Parent(_X)) = _Y;
		}
		_Right(_Y) = _X;
		_Parent(_X) = _Y;
	}

	void _Lrotate(_Nodeptr _X)
	{
		_Nodeptr _Y = _Right(_X);
		_Right(_X) = _Left(_Y);
		if(_Left(_Y) != _Nil)
		{
			_Parent(_Left(_Y)) = _X;
		}
		_Parent(_Y) = _Parent(_X);
		if(_X == _Root())
		{
			_Root() = _Y;
		}else if(_X == _Left(_Parent(_X)))
		{
			_Left(_Parent(_X)) = _Y;
		}else
		{
			_Right(_Parent(_X)) = _Y;
		}
		_Left(_Y) = _X;
		_Parent(_X) = _Y;
	}
	void _Init()
	{
		_Nil  = _Buynode(NULL,_Black);

		_Head = _Buynode(_Nil,_Red);
	}
	static _Nodeptr _Buynode(_Nodeptr _Parg,_Redbl _Carg)
	{
		_Nodeptr _S = (_Nodeptr)malloc(sizeof(_Node));
		if(NULL == _S) exit(1);
		memset(_S,0,sizeof(_Node));
		_Parent(_S) = _Parg;
		_Color(_S) = _Carg;
		return _S;
	}
	static void _Freenode(_Nodeptr _X)
	{
		if (_X != NULL)
			free(_X);
	}
	_Nodeptr _Head;
	_Nodeptr _Nil;
};
#endif 


#if 0
typedef enum { RED = 0, BLACK = 1 } Color;
typedef int KeyType;
typedef struct _RBnode
{
	KeyType key;
	struct _RBnode *leftchild;
	struct _RBnode *rightchild;
	struct _RBnode *parent;
	Color	color;
}RBnode;

RBnode* _RealBuynode();
RBnode* _Buynode(KeyType &x, RBnode *NIL);
void _Freenode(RBnode *&p, RBnode *NIL);
void Init(RBnode *&NIL);
bool Insert(KeyType &val, RBnode *NIL);
void InsertAdjust(RBnode *&kx, RBnode *NIL);
void LeftRotate(RBnode *kx, RBnode *NIL);
void RightRotate(RBnode *kx, RBnode *NIL);


int main()
{
	int ar[] = { 1,2,11,4,14,15,6,7,8 };
	int len = sizeof(ar) / sizeof(ar[0]);

	RBnode *NIL = NULL;
	Init(NIL);

	for (int i = 0; i < len; ++i)
	{
		Insert(ar[i], NIL);
	}

	return 0;
}

bool Insert(KeyType &val,RBnode *NIL)
{
	if (NIL == NULL || NIL->parent == NULL) return false;
	RBnode *kx = _Buynode(val, NIL);

	RBnode *p = NIL->parent->parent;//root
	RBnode *pa = NIL;//root->parent
	while (p != NIL)
	{
		pa = p;
		if (kx->key < p->key)
			p = p->leftchild;
		else
			p = p->rightchild;
	}
	
	kx->parent = pa;
	if (pa == NIL)
		NIL->parent->parent = kx;
	else if (kx->key < pa->key)
		pa->leftchild = kx;
	else
		pa->rightchild = kx;
	
	InsertAdjust(kx,NIL);
	
	if(kx->key < NIL->parent->leftchild->key)
		NIL->parent->leftchild = kx;
	else if(kx->key > NIL->parent->rightchild->key)
		NIL->parent->rightchild = kx;

	return true;
}

void InsertAdjust(RBnode *&kx, RBnode *NIL)
{
	while (kx->parent->color == RED)
	{
		if (kx->parent == kx->parent->parent->leftchild)
		{
			RBnode *ptr = kx->parent->parent->rightchild;
			if (ptr->color == RED)
			{
				kx->parent->color = BLACK;
				ptr->color = BLACK;
				ptr->parent->color = RED;
				kx = ptr->parent;//new RED
			}
			else
			{
				if (kx == kx->parent->rightchild)
				{
					kx = kx->parent;
					LeftRotate(kx, NIL);
				}
				kx->parent->color = BLACK;
				kx->parent->parent->color = RED;
				RightRotate(kx->parent->parent, NIL);
			}
		}
		else
		{
			RBnode *ptr = kx->parent->parent->leftchild;
			if (ptr->color == RED)
			{
				kx->parent->color = BLACK;
				ptr->color = BLACK;
				ptr->parent->color = RED;
				kx = ptr->parent;//new RED
			}
			else
			{
				if (kx == kx->parent->leftchild)
				{
					kx = kx->parent;
					RightRotate(kx, NIL);
				}
				kx->parent->color = BLACK;
				kx->parent->parent->color = RED;
				LeftRotate(kx->parent->parent, NIL);
			}
		}
	}
	NIL->parent->parent->color = BLACK;//root->color
}

void LeftRotate(RBnode *kx,RBnode *NIL)
{
	RBnode *newroot = kx->rightchild;
	newroot->parent = kx->parent;
	kx->rightchild = newroot->leftchild;
	if(newroot->leftchild != NIL)
		newroot->leftchild->parent = kx;

	if(kx->parent == NIL)//NIL->parent == head
		NIL->parent->parent = newroot;
	else if(kx == kx->parent->leftchild)
		kx->parent->leftchild = newroot;
	else
		kx->parent->rightchild = newroot;

	newroot->leftchild = kx;
	kx->parent = newroot;
}

void RightRotate(RBnode *kx, RBnode *NIL)
{
	RBnode *newroot = kx->leftchild;
	kx->leftchild = newroot->rightchild;
	newroot->parent = kx->parent;
	if(newroot->rightchild != NIL)
		newroot->rightchild->parent = kx;

	if(kx->parent == NIL)
		NIL->parent->parent = newroot;
	else if(kx == kx->parent->rightchild)
		kx->parent->rightchild = newroot;
	else
		kx->parent->leftchild = newroot;

	newroot->rightchild = kx;
	kx->parent = newroot;
}

void Init(RBnode *&NIL)
{
	if (NIL != NULL) return;
	NIL = _RealBuynode();
	NIL->key = (KeyType)(-1);
	NIL->color = BLACK;

	RBnode *head = _RealBuynode();
	head->color = RED;
	head->key = (KeyType)(-1);
	head->leftchild = head->rightchild = head->parent = NIL;

	NIL->parent = head;
}

void _Freenode(RBnode *&p,RBnode *NIL)
{
	if (p != NULL)
		free(p);
	p = NIL;
}

RBnode* _RealBuynode()
{
	RBnode *res = (RBnode *)malloc(sizeof(RBnode));
	assert(res != NULL);
	memset(res, 0, sizeof(RBnode));
	return res;
}

RBnode* _Buynode(KeyType &x,RBnode *NIL)
{
	RBnode *res = _RealBuynode();
	res->key = x;
	res->color = RED;
	res->leftchild = res->rightchild = res->parent = NIL;
	return res;
}

#endif