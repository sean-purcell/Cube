#include <fstream>
#include <iostream>
#include <string>
#include <functional>
#include <vector>

#include "assembler.h"
#include "cube.h"

/* tree filenames */
static const char htolfn[] = "htol.txt";
static const char ltolfn[] = "ltol.txt";

static Trie root_hl;
static Trie root_ll;

Trie construct_trie(const char* fname) {
	std::ifstream in;
	in.open(fname);

	std::string line;
	while((in >> line, line) != "") {
		std::cout << line << std::endl;
	}
	return Trie();
}

struct pair {
	char a, b;
};

const static struct pair pairs_hl[] = {
	{ 'D', 'd' },
	{ 'U', 'u' },
	{ 'F', 'f' },
	{ 'R', 'r' },
	{ 'B', 'b' },
	{ 'L', 'l' },
};

const static struct pair pairs_ll[] = {
	{ 'r', 'l' },
	{ 'c', 'w' },
};

/* hl->hl */
std::string remove_undos_hl(std::string moves) {
	for(int i = 0; i + 1 < moves.size(); i++) {
		for(int j = 0; j < sizeof(pairs_hl)/sizeof(pairs_ll[0]); j++) {
			if((moves[i] == pairs_hl[j].a && moves[i+1] == pairs_hl[j].b) ||
				(moves[i+1] == pairs_hl[j].a && moves[i] == pairs_hl[j].b)) {
					moves = moves.substr(0, i) + moves.substr(i + 2);
					i = i - 1;
					goto cont;
			}
		}
		cont:;
	}
	return moves;
}

/* ll->ll */
std::string remove_undos_ll(std::string moves) {
	for(int i = 0; i + 1 < moves.size(); i++) {
		for(int j = 0; j < sizeof(pairs_ll)/sizeof(pairs_ll[0]); j++) {
			if((moves[i] == pairs_ll[j].a && moves[i+1] == pairs_ll[j].b) ||
				(moves[i+1] == pairs_ll[j].a && moves[i] == pairs_ll[j].b)) {
					moves = moves.substr(0, i) + moves.substr(i + 1);
					goto cont;
			}
		}
		cont:;
	}
	return moves;
}

/* hl->hl & ll->ll */
std::string remove_fours(std::string moves) {
	for(int i = 0; i + 4 <= moves.size(); i++) {
		/* test if theres a contiguous block of 4 chars */
		bool same = true;
		for(int j = i + 1; same && (j < i + 4); j++) {
			same = moves[j] == moves[i];
		}
		if(same) {
			moves = moves.substr(0, i) + moves.substr(i+4);
			/* i must be set back in case removing moves in the middle made a new set */
			i = i - 4 < -1 ? -1 : i - 4;
		}
	}
	return moves;
}

/* hl->hl & ll->ll */
std::string basic_opt(std::string moves, bool hl) {
	while(1) {
		std::string orig = moves;
		moves = remove_fours(moves);
		moves = hl ? remove_undos_hl(moves) : remove_undos_ll(moves);
		if(moves == orig) {
			return moves;
		}
	}
}

/* hl -> ll */
std::string assembler_O0(std::string in) {
	std::function<void(Cube&)> ops[128];
	ops[(int)'D'] = &Cube::D;
	ops[(int)'d'] = &Cube::d;
	ops[(int)'U'] = &Cube::U;
	ops[(int)'u'] = &Cube::u;
	ops[(int)'F'] = &Cube::F;
	ops[(int)'f'] = &Cube::f;
	ops[(int)'R'] = &Cube::R;
	ops[(int)'r'] = &Cube::r;
	ops[(int)'B'] = &Cube::B;
	ops[(int)'b'] = &Cube::b;
	ops[(int)'L'] = &Cube::L;
	ops[(int)'l'] = &Cube::l;
	Cube c;
	for(int i = 0; i < in.size(); i++) {
		ops[(int)in[i]](c);
	}
	return c.hist;
}

/* hl -> ll */
std::string assemble(std::string hlm) {
	return basic_opt(assembler_O0(basic_opt(hlm, true)), false);
}
