#include <stdlib.h>
#include <string>

using namespace std;

class Node
{
	private:
		Node* left;
		Node* right;
		Node* parent;
		string symbol;
		int frequency;

	public:
		Node();
		Node(string sym, int freq);

		Node* get_left();
		Node* get_right();
		Node* get_parent();
		string get_symbol();
		int get_frequency();

		void set_left(Node* node);
		void set_right(Node* node);
		void set_parent(Node* node);
		void set_symbol(string sym);
		void set_frequency(int freq);
};