#pragma once

/*
1）每个结点要么是红的，要么是黑的。
2）根结点是黑的。
3）最长路径 小于 最短路径的二倍
4）如果一个结点是红的，那么它的俩个儿子都是黑的。
5）对每个结点，从该结点到其子孙结点的所有路径上包含相同数目的黑结点。
*/


enum Colour {
	RED,
	BLACK,
};

template<class K,class V>
struct RBTreeNode {
	RBTreeNode<K, V>* _left;
	RBTreeNode<K, V>* _right;
	RBTreeNode<K, V>* _parent;

	K _key;
	V _value;

	Colour _col;

	RBTreeNode(const K& key, const V& value, Colour col = RED)
		:_right(NULL)
		, _left(NULL)
		, _parent(NULL)
		, _key(key)
		, _value(value)
		, _col(col)
	{}
};

template<class K,class V>
class RBTree {
	typedef RBTreeNode<K, V> Node;
public:
	RBTree()
		:_root(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		//1. tree empty
		if (_root == NULL)
		{
			_root = new Node(key,value,BLACK);
			return true;
		}
		//2 树 不空,先找到位置
		Node* parent = NULL;
		Node* cur = _root;
		while (cur)
		{
			if (key < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (key > cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
				return false;
		}
		cur = new Node(key, value, RED);//默认插入RED,不会破坏性质5
		if (key > parent->_key)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}

		//3,调整
		// 插入节点的父节点是黑色，不用调整
		//否则根据叔叔节点的颜色调整
		bool isRotate = false;
		Node* ppNode = NULL;
		while (cur != _root && parent->_col == RED)
		{
			Node* grandfather = parent->_parent;
			if (parent == grandfather->_left)
			{
				Node* uncle = grandfather->_right;
				//插入节点的父节点和叔叔节点都是红色
				//将 两者的颜色置黑，祖父节点置红
				//向上迭代 变更颜色
				if (uncle && uncle->_col == RED)//case 1
				{
					uncle->_col = parent->_col = BLACK;
					grandfather->_col = RED;
					cur = grandfather;
					parent = cur->_parent;
				}
				//黑叔时
				//cur为左孩子，LL	
				//cur为右孩子  LR
				else
				{
					if (cur == parent->_right)
					{
						_RotateR(parent);
						swap(cur, parent);
					}
					grandfather->_col = RED;
					parent->_col = BLACK;
					_RotateL(grandfather);
				}
			}

			else
			{
				Node *uncle = grandfather->_left;

				if (uncle && uncle->_col == RED)  //case1
				{
					parent->_col = uncle->_col = BLACK;
					grandfather->_col = RED;
					cur = grandfather;
					parent = cur->_parent;
				}
				else
				{
					if (cur == parent->_left)  //case3->case2
					{
						_RotateL(parent);
						swap(cur, parent);
					}

					grandfather->_col = RED;
					parent->_col = BLACK;
					_RotateR(grandfather);
				}
			}
		}
		_root->_col = BLACK;
	}

protected:
	void _InOrder(Node *root) 
	{
		if (NULL == _root) 
			return;

		_InOrder(root->_left);
		cout << root->_key << " ";
		_InOrder(root->_right);
	}

	void _RotateL(Node *& parent)
	{
		Node* cur = parent->_left;
		Node* curR = cur->_right;
		parent->_left = curR;
		if (curR != NULL)
			curR->_parent = parent;

		cur->_right = parent;
		cur->_parent = parent->_parent;
		parent->_parent = cur;

		if (cur->_parent == NULL)
			_root = cur;
		else
		{
			if (cur->_key < cur->_parent->_key)
				cur->_parent->_left = cur;
			else
				cur->_parent->_right = cur;
		}
	}

	void _RotateR(Node *& parent)
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;

		parent->_right = subRL;
		if (subRL != NULL)
			subRL->_parent = parent;

		subR->_left = parent;
		subR->_parent = parent->_parent;
		parent->_parent = subR;

		if (subR->_parent == NULL)
			_root = subR;
		else
		{
			if (subR->_key < subR->_parent->_key)
				subR->_parent->_left = subR;
			else
				subR->_parent->_right = subR;
		}
	}


	void _RotateR(Node *& parent)
	{
		Node *subR = parent->_right;
		Node *subRL = subR->_left;

		parent->_right = subRL;
		if (subRL != NULL)
			subRL->_parent = parent;

		subR->_left = parent;
		subR->_parent = parent->_parent;
		parent->_parent = subR;

		if (subR->_parent == NULL)
			_root = subR;
		else
		{
			if (subR->_key < subR->_parent->_key)
				subR->_parent->_left = subR;
			else
				subR->_parent->_right = subR;
		}
	}

private:
	Node *_root;
};