#include "AdaptiveHuffman.h"

int main()
{
	AdaptiveHuffman* tree = new AdaptiveHuffman();

	tree->load_rawtext("input.txt");

	cout << endl << endl;
	cout << "Raw text from input file: " << endl << tree->get_buffer();

	tree->encode();

	cout << endl << endl;
	cout << "Encoded bit string: " << endl << tree->get_buffer();

	tree->write_encoded_file("encoded.huff");

	AdaptiveHuffman* another = new AdaptiveHuffman();

	another->load_encoded_file("encoded.huff");

	cout << endl << endl;
	cout << "Decoded bit string from encoded file: " << endl << another->get_buffer();

	another->decode();

	cout << endl << endl;
	cout << "Decoded text from encoded file: " << endl << another->get_buffer();
	cout << endl << endl;

	another->write_rawtext("output.txt");
	return 0;
}