#include "Node.h"

Node::Node()
{
}

Node::Node(string sym, int freq)
{
	symbol = sym;
	frequency = freq;

	left = NULL;
	right = NULL;
	parent = NULL;
}

Node* Node::get_left()
{
	return left;
}

void Node::set_left(Node* node)
{
	left = node;
}

Node* Node::get_right()
{
	return right;
}

void Node::set_right(Node* node)
{
	right = node;
}

Node* Node::get_parent()
{
	return parent;
}

void Node::set_parent(Node* node)
{
	parent = node;
}

string Node::get_symbol()
{
	return symbol;
}

void Node::set_symbol(string sym)
{
	symbol = sym;
}

int Node::get_frequency()
{
	return frequency;
}

void Node::set_frequency(int freq)
{
	frequency = freq;
}