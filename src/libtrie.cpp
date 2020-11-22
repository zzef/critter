#include <string>
#include <iostream>

#define width 93


int store(std::string str,fstream& file) {

	for(size_t i = 0; i<str.length(); i++) {
		yyu
	}

}

bool check(std::string str,fstream& file) {

}

int main(int argc, char* argv[]) {

	fstream trie_enc;
	trie_enc.open("trie", ios::binary);

	if (!trie_enc.is_open()) {
		std::cout << "Couldn't open the file" << std::endl; 
		return(1);
	}

	store("their",trie_enc);
	store("there",trie_enc);
	store("this",trie_enc);
	store("that",trie_enc);
	store("does",trie_enc);
	store("did",trie_enc);

	trie_enc.close();


	return(0);

}
