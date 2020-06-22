#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "dsexceptions.h"
#include <algorithm>
#include <iostream> 
using namespace std;

// AvlTree class
// Original Author Mark Allen Weiss
// Modified by Dylan Pranga for Grand Valley State University
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS********************************************************
// void insert( x )					 --> Insert x							| DEPRECIATED
// void remove( x )					 --> Remove x (unimplemented)			| DEPRECIATED
// NOTE: Internal remove method modified. Public remove method depreciated.
// bool contains( x )				 --> Return true if x is present		| DEPRECIATED FOR SECURITY
// int findMin( )					 --> Return smallest item
// int findMax( )					 --> Return largest item
// boolean isEmpty( )				 --> Return true if empty; else false
// void makeEmpty( )				 --> Remove all items
// void printTree( )				 --> Print tree in sorted order			| DEPRECIATED
//
// void insert(id, pass, msg, att)	 --> Inserts node with given data into AVL tree.
// string tryRetrieve(id, pass)		 --> Returns message, otherwise RequestException.
// ******************ERRORS*******************************************************************
// Throws UnderflowException as warranted.
// Throws RequestException children:
// MethodDepreciatedException
// MessageDestructedException
// UIDNotFoundException
// InvalidPasswordException

template <typename Comparable>
class AvlTree
{
public:
	AvlTree() : root{ nullptr }
	{ }

	AvlTree(const AvlTree & rhs) : root{ nullptr }
	{
		root = clone(rhs.root);
	}

	AvlTree(AvlTree && rhs) : root{ rhs.root }
	{
		rhs.root = nullptr;
	}

	~AvlTree()
	{
		makeEmpty();
	}

	/**
	 * Deep copy.
	 */
	AvlTree & operator=(const AvlTree & rhs)
	{
		AvlTree copy = rhs;
		std::swap(*this, copy);
		return *this;
	}

	/**
	 * Move.
	 */
	AvlTree & operator=(AvlTree && rhs)
	{
		std::swap(root, rhs.root);

		return *this;
	}

	/**
	 * Find the smallest item in the tree.
	 * Throw UnderflowException if empty.
	 */
	const int & findMin() const
	{
		if (isEmpty())
			throw UnderflowException{ };
		return findMin(root)->id;
	}

	/**
	 * Find the largest item in the tree.
	 * Throw UnderflowException if empty.
	 */
	const int & findMax() const
	{
		if (isEmpty())
			throw UnderflowException{ };
		return findMax(root)->id;
	}

	/**
	 * Returns true if x is found in the tree.
	 * DEPRECIATED FOR SECURITY
	 */
	bool contains(const int & x) const
	{
		throw MethodDepreciatedException{ };
		return contains(x, root);
	}

	/**
	 * Test if the tree is logically empty.
	 * Return true if empty, false otherwise.
	 */
	bool isEmpty() const
	{
		return root == nullptr;
	}

	/**
	 * Print the tree contents in sorted order.
	 * DEPRECIATED
	 */
	void printTree() const
	{
		throw MethodDepreciatedException{ };

		if (isEmpty())
			cout << "Empty tree" << endl;
		else
			printTree(root);
	}

	/**
	 * Make the tree logically empty.
	 */
	void makeEmpty()
	{
		makeEmpty(root);
	}

	/**
	 * Insert x into the tree; duplicates are ignored.
	 * DEPRECIATED
	 */
	void insert(const int & x)
	{
		throw MethodDepreciatedException{ };
		insert(x, root);
	}

	/**
	 * Insert x into the tree; duplicates are ignored.
	 * DEPRECIATED
	 */
	void insert(int && x)
	{
		throw MethodDepreciatedException{ };
		insert(std::move(x), root);
	}

	/**
	 * Insert top secret data into the tree; duplicates are ignored.
	 */
	void insert(const int & id, const string & pass, const string & msg, const int & att)
	{
		insert(id, pass, msg, att, root);
	}

	/**
	 * Remove x from the tree. Nothing is done if x is not found.
	 * DEPRECIATED FOR SECURITY
	 */
	void remove(const int & x)
	{
		throw MethodDepreciatedException{ };
		remove(x, root);
	}

	string tryRetrieve(const int & id, const string & pass) throw()
	{
		try {
			AvlNode * msgNode = tryRetrieve(id, pass, root);

			if (msgNode == nullptr)
			{
				return "";
			}
			else
			{
				string msg = msgNode->message;
				remove(id, root);
				return msg;
			}
		}
		catch (MessageDestructedException ex) {
			remove(id, root);
			throw ex;
		}
		catch (RequestException ex) {
			throw ex;
		}
	}

private:
	struct AvlNode
	{
		int id;
		string password;
		int attempts;
		string message;
		AvlNode   *left;
		AvlNode   *right;
		int       height;

		AvlNode(const int & uid, string pass, string msg, int att, AvlNode *lt, AvlNode *rt, int h = 0)
			: id{ uid }, password{ pass }, message{ msg }, attempts{ att }, left{ lt }, right{ rt }, height{ h } { }
		//DEPRECIATED
		AvlNode(const int & ele, AvlNode *lt, AvlNode *rt, int h = 0)
			: id{ ele }, left{ lt }, right{ rt }, height{ h } { }
		//DEPRECIATED
		AvlNode(int && ele, AvlNode *lt, AvlNode *rt, int h = 0)
			: id{ std::move(ele) }, left{ lt }, right{ rt }, height{ h } { }
	};

	AvlNode *root;

	//Used to decrement a node's attempts, and throws exception if threshold is met.
	void failedAttempt(AvlNode * & t) throw()
	{
		t->attempts -= 1;
		if (t->attempts <= 0)
		{
			throw MessageDestructedException();
		}
	}

	//Used to attempt to retrieve a message, given an ID and password.
	AvlNode * tryRetrieve(const int & id, const string & password, AvlNode * & t) throw()
	{
		//If the UID is unable to be found...
		if (t == nullptr)
		{
			throw UIDNotFoundException();
		}

		//If the ID's match...
		if (t->id == id)
		{
			//Ensure the passwords are correct.
			if (password.compare(t->password) == 0)
			{
				return t;
			}
			else
			{
				//If not, fail to retrieve and decrement attempts.
				failedAttempt(t);
				throw InvalidPasswordException(to_string(t->attempts));
			}
		}

		//Continue searching right.
		if (t->id < id)
		{
			return tryRetrieve(id, password, t->right);
		}

		//Continue searching left.
		return tryRetrieve(id, password, t->left);
	}

	/**
	 * Internal method to insert into a subtree.
	 * x is the item to insert.
	 * t is the node that roots the subtree.
	 * Set the new root of the subtree.
	 * DEPRECIATED
	 */
	void insert(const int & x, AvlNode * & t)
	{
		throw MethodDepreciatedException{ };

		if (t == nullptr)
			t = new AvlNode{ x, nullptr, nullptr };
		else if (x < t->id)
			insert(x, t->left);
		else if (t->id < x)
			insert(x, t->right);

		balance(t);
	}

	/**
	 * Internal method to insert into a subtree.
	 * x is the item to insert.
	 * t is the node that roots the subtree.
	 * Set the new root of the subtree.
	 * DEPRECIATED
	 */
	void insert(int && x, AvlNode * & t)
	{
		throw MethodDepreciatedException{ };

		if (t == nullptr)
			t = new AvlNode{ std::move(x), nullptr, nullptr };
		else if (x < t->id)
			insert(std::move(x), t->left);
		else if (t->id < x)
			insert(std::move(x), t->right);

		balance(t);
	}

	/**
	 * Internal method to insert into a subtree.
	 * id is the ID of the message.
	 * pwd is the password to access the message.
	 * msg is the message.
	 * att is the allowed attempts.
	 * t is the node that roots the subtree.
	 * Set the new root of the subtree.
	 */
	void insert(const int & id, const string & pwd, const string & msg, const int & att, AvlNode * & t)
	{
		if (t == nullptr)
			t = new AvlNode{ id, pwd, msg, att, nullptr, nullptr };
		else if (id < t->id)
			insert(id, t->left);
		else if (t->id < id)
			insert(id, t->right);

		balance(t);
	}

	/**
	 * Internal method to remove from a subtree.
	 * id is the id of the message to remove.
	 * t is the node that roots the subtree.
	 * Set the new root of the subtree.
	 */
	void remove(const int & id, AvlNode * & t)
	{
		if (t == nullptr)
			return;   // Item not found; do nothing

		if (id < t->id)
			remove(id, t->left);
		else if (t->id < id)
			remove(id, t->right);
		else if (t->left != nullptr && t->right != nullptr) // Two children
		{
			t->id = findMin(t->right)->id;
			remove(t->id, t->right);
		}
		else
		{
			AvlNode *oldNode = t;
			t = (t->left != nullptr) ? t->left : t->right;
			delete oldNode;
		}

		balance(t);
	}

	static const int ALLOWED_IMBALANCE = 1;

	// Assume t is balanced or within one of being balanced
	void balance(AvlNode * & t)
	{
		if (t == nullptr)
			return;

		if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
			if (height(t->left->left) >= height(t->left->right))
				rotateWithLeftChild(t);
			else
				doubleWithLeftChild(t);
		else
			if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
				if (height(t->right->right) >= height(t->right->left))
					rotateWithRightChild(t);
				else
					doubleWithRightChild(t);

		t->height = max(height(t->left), height(t->right)) + 1;
	}

	/**
	 * Internal method to find the smallest item in a subtree t.
	 * Return node containing the smallest item.
	 */
	AvlNode * findMin(AvlNode *t) const
	{
		if (t == nullptr)
			return nullptr;
		if (t->left == nullptr)
			return t;
		return findMin(t->left);
	}

	/**
	 * Internal method to find the largest item in a subtree t.
	 * Return node containing the largest item.
	 */
	AvlNode * findMax(AvlNode *t) const
	{
		if (t != nullptr)
			while (t->right != nullptr)
				t = t->right;
		return t;
	}


	/**
	 * Internal method to test if an item is in a subtree.
	 * x is item to search for.
	 * t is the node that roots the tree.
     * DEPRECIATED
	 */
	bool contains(const int & x, AvlNode *t) const
	{
		throw MethodDepreciatedException{ };

		if (t == nullptr)
			return false;
		else if (x < t->id)
			return contains(x, t->left);
		else if (t->id < x)
			return contains(x, t->right);
		else
			return true;    // Match
	}
	/****** NONRECURSIVE VERSION*************************
		bool contains( const int & x, AvlNode *t ) const
		{
			while( t != nullptr )
				if( x < t->id )
					t = t->left;
				else if( t->id < x )
					t = t->right;
				else
					return true;    // Match

			return false;   // No match
		}
	*****************************************************/

	/**
	 * Internal method to make subtree empty.
	 */
	void makeEmpty(AvlNode * & t)
	{
		if (t != nullptr)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullptr;
	}

	/**
	 * Internal method to print a subtree rooted at t in sorted order.
	 */
	void printTree(AvlNode *t) const
	{
		if (t != nullptr)
		{
			printTree(t->left);
			cout << t->id << endl;
			printTree(t->right);
		}
	}

	/**
	 * Internal method to clone subtree.
	 */
	AvlNode * clone(AvlNode *t) const
	{
		if (t == nullptr)
			return nullptr;
		else
			return new AvlNode{ t->id, clone(t->left), clone(t->right), t->height };
	}
	// Avl manipulations
	/**
	* Return the height of node t or -1 if nullptr.
	*/
	int height(AvlNode *t) const
	{
		return t == nullptr ? -1 : t->height;
	}

	int max(int lhs, int rhs) const
	{
		return lhs > rhs ? lhs : rhs;
	}

	/**
	 * Rotate binary tree node with left child.
	 * For AVL trees, this is a single rotation for case 1.
	 * Update heights, then set new root.
	 */
	void rotateWithLeftChild(AvlNode * & k2)
	{
		AvlNode *k1 = k2->left;
		k2->left = k1->right;
		k1->right = k2;
		k2->height = max(height(k2->left), height(k2->right)) + 1;
		k1->height = max(height(k1->left), k2->height) + 1;
		k2 = k1;
	}

	/**
	 * Rotate binary tree node with right child.
	 * For AVL trees, this is a single rotation for case 4.
	 * Update heights, then set new root.
	 */
	void rotateWithRightChild(AvlNode * & k1)
	{
		AvlNode *k2 = k1->right;
		k1->right = k2->left;
		k2->left = k1;
		k1->height = max(height(k1->left), height(k1->right)) + 1;
		k2->height = max(height(k2->right), k1->height) + 1;
		k1 = k2;
	}

	/**
	 * Double rotate binary tree node: first left child.
	 * with its right child; then node k3 with new left child.
	 * For AVL trees, this is a double rotation for case 2.
	 * Update heights, then set new root.
	 */
	void doubleWithLeftChild(AvlNode * & k3)
	{
		rotateWithRightChild(k3->left);
		rotateWithLeftChild(k3);
	}

	/**
	 * Double rotate binary tree node: first right child.
	 * with its left child; then node k1 with new right child.
	 * For AVL trees, this is a double rotation for case 3.
	 * Update heights, then set new root.
	 */
	void doubleWithRightChild(AvlNode * & k1)
	{
		rotateWithLeftChild(k1->right);
		rotateWithRightChild(k1);
	}
};

#endif