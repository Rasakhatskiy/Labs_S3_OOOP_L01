//! AVL Tree class.
/*!
* \file TreeAVL.hpp
* \author Maksym Rasakhatskyi
* \version 1.0 15/09/20
*/

#pragma once
#include "NodeTreeAVL.hpp"
#include "../PriorQueue.hpp"

#include <algorithm>

template <typename T>
class TreeAVL : public PriorQueue<T>
{
private:
	NodeTreeAVL<T>* root;

	/*! Right rotates subtree rooted with given node.
	* \param[in] node Subtree root.
	* \returns New root of rotated subtree.
	*/
	NodeTreeAVL<T>* rotateRight(NodeTreeAVL<T>* node);

	/*! Left rotates subtree rooted with given node.
	* \param[in] node Subtree root.
	* \returns New root of rotated subtree.
	*/
	NodeTreeAVL<T>* rotateLeft(NodeTreeAVL<T>* node);

	/*! Recursive function to insert a value in the subtree rooted with node.
	* \param[in] target Where to insert.
	* \param[in] value Item that contains tree.
	* \param[in] priority Priority of item on tree.
    * \returns New root of the subtree. 
	*/
	NodeTreeAVL<T>* insertNode(NodeTreeAVL<T>* target, T value, int priority);

	/*! Recursive function to delete a node with given data from subtree
	* \param[in] target Where to delete.
	* \param[in] value Item that contains tree.
	* \param[in] priority Priority of item on tree.
	* \returns New root of the subtree. 	
	*/
	NodeTreeAVL<T>* deleteNode(NodeTreeAVL<T>* target, T value, int priority);
public:
	/*! Creates empty tree with NULL root. */
	TreeAVL();

	/*! Pushes value with priority to AVL Tree.
	* \param[in] value Value to insert.
	* \param[in] priority Priority in queue.
	*/
	void push(T value, unsigned int priority) override;

	/*! Pops element from queue.
	* Gets element from queue tree with most priority. 
	* Removes it from queue.
	*/
	T pop() override;

	/*! Gets element from queue.
	* Gets element from queue tree with most priority. 
	* Doesn't touch elements.
	*/
	T predict() override;

	/*! Gets node height.
	* \param node[int] Node with height.
	* \returns 0 if node is NULL or it's height.
	*/
	int getNodeHeight(NodeTreeAVL<T>* node);

	/*! Get the balance factor of this ancestor node.
	* \param node[int] Node with balance.
	* \returns Balance factor of this ancestor node.
    */
	int getNodeBalance(NodeTreeAVL<T>* node);
};

template<typename T>
inline NodeTreeAVL<T>* TreeAVL<T>::rotateRight(NodeTreeAVL<T>* node)
{
	auto newNode = node->left;

	//swap
	auto buffer = newNode->right;
	newNode->right = node;
	node->left = buffer;

	//recalculate heights  
	node->height = 
		std::max(
			node->left->getHeight(),
			node->right->getHeight()) 
		+ 1;

	newNode->height = 
		std::max(
			newNode->left->getHeight(),
			newNode->right->getHeight()) 
		+ 1;

	//new root  
	return newNode;
}

template<typename T>
inline NodeTreeAVL<T>* TreeAVL<T>::rotateLeft(NodeTreeAVL<T>* node)
{
	auto newNode = node->right;

	//swap
	auto buffer = newNode->left;
	newNode->left = node;
	node->right = buffer;

	// recalculate heights  
	node->height = 
		std::max(
			node->left->getHeight(),
			node->right->getHeight()) 
		+ 1;
	newNode->height = std::max(height(newNode->left),
		height(newNode->right)) + 1;

	// Return new root  
	return newNode;
}

template<typename T>
inline NodeTreeAVL<T>* TreeAVL<T>::deleteNode(NodeTreeAVL<T>* target, T value, int priority)
{
	if (!target)
		return target;

	if (key < target->priority)
	{
		target->left = deleteNode(target->left, value, priority);
	}
	else if (priority > target->priority)
	{
		target->right = deleteNode(target->right, value, priority);
	}
	else
	{
		if (!target->left ||
			!target->right)
		{
			auto temp = target->left ?
				target->left :
				target->right;

			// No child case  
			if (!temp)
			{
				temp = target;
				target = NULL;
			}
			else
			{
				*target = *temp;
			}
			free(temp);
		}
		else
		{
			auto temp = minValueNode(target->right);

			target->key = temp->key;
			target->value = temp->value;
 
			target->right = deleteNode(
				target->right,
				temp->value,
				temp->key);
		}
	}

	// If the tree had only one node 
	// then return  
	if (target == NULL)
		return target;

	// STEP 2: UPDATE HEIGHT OF THE CURRENT NODE  
	target->height = 1 + max(height(target->left),
		height(target->right));

	// STEP 3: GET THE BALANCE FACTOR OF  
	// THIS NODE (to check whether this  
	// node became unbalanced)  
	int balance = getBalance(target);

	// If this node becomes unbalanced,  
	// then there are 4 cases  

	// Left Left Case  
	if (balance > 1 &&
		getBalance(target->left) >= 0)
		return rightRotate(target);

	// Left Right Case  
	if (balance > 1 &&
		getBalance(target->left) < 0)
	{
		target->left = leftRotate(target->left);
		return rightRotate(target);
	}

	// Right Right Case  
	if (balance < -1 &&
		getBalance(target->right) <= 0)
		return leftRotate(target);

	// Right Left Case  
	if (balance < -1 &&
		getBalance(target->right) > 0)
	{
		target->right = rightRotate(target->right);
		return leftRotate(target);
	}

	return target;
}

template<typename T>
inline TreeAVL<T>::TreeAVL()
{
	root = nullptr;
}

template<typename T>
inline void TreeAVL<T>::push(T value, unsigned int priority)
{
	if (!root)
		root = new NodeTreeAVL<T>(value, priority);
	else
		root = insertNode(root, value, priority);
}

template<typename T>
inline NodeTreeAVL<T>* TreeAVL<T>::insertNode(NodeTreeAVL<T>* target, T value, int priority)
{
	if (!target)
		return(newNode(value, priority));

	if (priority < target->priority)
		target->left = insertNode(target->left, value, priority);
	else 
		target->right = insertNode(target->right, value, priority);


	// Update height of this ancestor node
	target->height = max(
		getNodeHeight(target->left),
		getNodeHeight(target->right));

	// Get the balance factor of this ancestor node to check whether this node became unbalanced
	int balance = getNodeBalance(target);

	// Left Left Case  
	if (balance > 1 && 
		priority < target->left->priority)
		return rightRotate(target);

	// Right Right Case  
	if (balance < -1 && 
		priority > target->right->priority)
		return leftRotate(target);

	// Left Right Case  
	if (balance > 1 && 
		priority > target->left->priority)
	{
		target->left = leftRotate(target->left);
		return rightRotate(target);
	}

	// Right Left Case  
	if (balance < -1 && 
		priority < target->right->priority)
	{
		target->right = rightRotate(target->right);
		return leftRotate(target);
	}

	return target;
}

template<typename T>
inline T TreeAVL<T>::pop()
{
	if (!root) return NULL;
	auto value = root->value;
	root = deleteNode(root, root->value, root->priority);
	return value;
}

template<typename T>
inline T TreeAVL<T>::predict()
{
	if (!root) return NULL;
	else return root->value;
}

template<typename T>
inline int TreeAVL<T>::getNodeHeight(NodeTreeAVL<T>* node)
{
	if (!node) return 0;
	else return node->height;
}

template<typename T>
inline int TreeAVL<T>::getNodeBalance(NodeTreeAVL<T>* node)
{
	if (!node) return 0;
	else return getNodeHeight(node->left) - getNodeHeight(node->right);
}