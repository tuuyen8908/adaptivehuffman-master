#include "AdaptiveHuffman.h"

AdaptiveHuffman::AdaptiveHuffman()
{
	nyt = new Node("NYT", 0);
	nyt->set_parent(NULL);
	root = nyt;
	nodes.push_back(nyt);
}

void AdaptiveHuffman::insert(char sym)
{
	Node* tba = NULL;

	if (!is_known(sym))
	{
		Node* inner = new Node("", 1);
		Node* fresh = new Node(string(1, sym), 1);

		inner->set_left(nyt);
		inner->set_right(fresh);
		inner->set_parent(nyt->get_parent());

		if (nyt->get_parent() != NULL)
			nyt->get_parent()->set_left(inner);
		else
			root = inner;

		nyt->set_parent(inner);
		fresh->set_parent(inner);

		nodes.insert(nodes.begin() + 1, inner);
		nodes.insert(nodes.begin() + 1, fresh);

		knownsym.push_back(sym);

		tba = inner->get_parent();
	}
	else
	{
		tba = find_node(sym);
	}

	while (tba != NULL)
	{
		Node* bignode = find_bignode(tba->get_frequency());

		if (tba != bignode && tba->get_parent() != bignode && bignode->get_parent() != tba)
			swap_node(tba, bignode);

		tba->set_frequency(tba->get_frequency() + 1);
		tba = tba->get_parent();
	}
}

void AdaptiveHuffman::encode()
{
	string result;
	result.append("0");

	for (char c : buffer)
	{
		result.append(get_code(c));
		insert(c);
	}

	buffer.clear();
	buffer = result;
}

void AdaptiveHuffman::decode()
{
	string result = "";
	string sym;
	Node* p = root;

	sym = get_by_asc(0);

	result += sym;

	insert(sym[0]);
	p = root;

	for (int i = 9; i < buffer.size(); i++)
	{
		if (buffer[i] == '0')
			p = p->get_left();
		else
			p = p->get_right();

		sym = p->get_symbol();

		if (sym != "")
		{
			if (sym == "NYT")
			{
				sym = get_by_asc(i);
				i += 8;
			}

			result += sym;

			insert(sym[0]);
			p = root;
		}
	}

	buffer.clear();
	buffer = result;
}

bool AdaptiveHuffman::is_known(char sym)
{
	if (find(knownsym.begin(), knownsym.end(), sym) != knownsym.end())
		return true;

	return false;
}

Node* AdaptiveHuffman::find_node(char sym)
{
	for (Node* node : nodes)
	{
		if (node->get_symbol() != "" && node->get_symbol() == string(1, sym))
			return node;
	}

	return NULL;
}

Node* AdaptiveHuffman::find_bignode(int freq)
{
	Node* node;

	for (vector<Node*>::reverse_iterator i = nodes.rbegin(); i != nodes.rend(); i++)
	{
		node = *i;

		if (node->get_frequency() == freq)
			break;
	}

	return node;
}

void AdaptiveHuffman::swap_node(Node* src, Node* tgt)
{
	int srcpos = find(nodes.begin(), nodes.end(), src) - nodes.begin();
	int tgtpos = find(nodes.begin(), nodes.end(), tgt) - nodes.begin();

	nodes.at(srcpos) = tgt;
	nodes.at(tgtpos) = src;

	Node* psrc = src->get_parent();
	Node* ptgt = tgt->get_parent();

	if (psrc != ptgt)
	{
		if (psrc->get_left() == src)
			psrc->set_left(tgt);
		else
			psrc->set_right(tgt);

		if (ptgt->get_left() == tgt)
			ptgt->set_left(src);
		else
			ptgt->set_right(src);
	}
	else
	{
		psrc->set_left(tgt);
		psrc->set_right(src);
	}

	src->set_parent(ptgt);
	tgt->set_parent(psrc);
}

string AdaptiveHuffman::get_code(char sym)
{
	tempcode = "";

	get_code_by_tree(root, string(1, sym), "");

	string result = tempcode;
	
	if (tempcode == "")
	{
		get_code_by_tree(root, "NYT", "");
		result = tempcode;
		result += to_binary(sym);
	}

	return result;
}

void AdaptiveHuffman::get_code_by_tree(Node* node, string sym, string code)
{
	if (node->get_left() == NULL && node->get_right() == NULL)
	{
		if (node->get_symbol() != "" && node->get_symbol() == sym)
			tempcode = code;
	}
	else
	{
		if (node->get_left() != NULL)
			get_code_by_tree(node->get_left(), sym, code + "0");
		if (node->get_right() != NULL)
			get_code_by_tree(node->get_right(), sym, code + "1");
	}
}

string AdaptiveHuffman::to_binary(char sym)
{
	string result = "";

	for (int i = 0; i < 8; i++)
	{
		if (sym % 2 == 0)
			result = "0" + result;
		else
			result = "1" + result;

		sym /= 2;
	}

	return result;
}

string AdaptiveHuffman::get_by_asc(int index)
{
	int asc = 0;
	int temp = 0;

	for (int i = 7; i >= 0; i--)
	{
		temp = buffer[++index] - 48;
		asc += temp * pow(2, i);
	}

	return string(1, asc);
}

void AdaptiveHuffman::load_rawtext(string path)
{
	ifstream input;

	input.open(path);

	stringstream buff;
	buff << input.rdbuf();

	buffer.clear();

	for (char byte : buff.str())
		buffer.append(string(1, byte));

	input.close();
}

void AdaptiveHuffman::load_encoded_file(string path)
{
	ifstream input;
	string temp;

	input.open(path, ios::binary);

	stringstream buff;
	buff << input.rdbuf();

	buffer.clear();

	int rest = (int) buff.str().at(0);

	for (int i = 1; i < buff.str().size(); i++)
	{
		temp.clear();
		temp = bitset<8>(buff.str().at(i)).to_string();

		if (rest > 0 && i == buff.str().size()-1)
			buffer.append(temp.substr(0, temp.size() - (8 - rest)));
		else
			buffer.append(temp);
	}

	input.close();
}

void AdaptiveHuffman::write_rawtext(string path)
{
	ofstream output;

	output.open(path, ofstream::out);

	for (char byte : buffer)
		output << byte;

	output.close();
}


void AdaptiveHuffman::write_encoded_file(string path)
{
	ofstream output;
	vector<bitset<8>> bytes;
	bitset<8> b;

	int nofbytes = ceil(buffer.size() / 8);
	int rest = buffer.size() % 8;

	for (int i = 0; i < nofbytes; i++)
	{
		b.reset();
		bytes.push_back(b);

		for (int j = 0; j < 8; j++)
			bytes.back().set(7 - j, (buffer.at(i * 8 + j) == '1') ? 1 : 0);
	}

	if (rest > 0)
	{
		b.reset();
		bytes.push_back(b);

		for (int i = 0; i < rest; i++)
		{
			bytes.back().set(7 - i, (buffer.at(nofbytes * 8 + i) == '1') ? 1 : 0);
		}
	}

	output.open(path, ofstream::out | ofstream::binary);

	output << (char) rest;

	for (bitset<8> byte : bytes)
	{
		output << (char) byte.to_ulong();
	}

	output.close();
}

string AdaptiveHuffman::get_buffer()
{
	return buffer;
}