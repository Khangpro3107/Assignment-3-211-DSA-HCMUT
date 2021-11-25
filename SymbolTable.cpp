#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
	ifstream fname;
	fname.open(filename);
	string cmd;
	string tokens[5];
	for (int i = 0; i < 5; i++) tokens[i] = "";
	getline(fname, cmd);
	if (!split_config(cmd, tokens)) throw InvalidInstruction(cmd);
	if (cmd[0] == 'L') {
		int table_size = stoi(tokens[1]);
		this->hash_const1 = stoi(tokens[2]);
		if (table_size > 999999 || hash_const1 > 999999) throw InvalidInstruction(cmd);
		this->table = new Symbol[table_size];
		this->capacity = table_size;
		this->mode = linear;
	}
	else if (cmd[0] == 'Q') {
		int table_size = stoi(tokens[1]);
		this->hash_const1 = stoi(tokens[2]);
		this->hash_const2 = stoi(tokens[3]);
		if (table_size > 999999 || hash_const1 > 999999 || hash_const2 > 999999) throw InvalidInstruction(cmd);
		this->table = new Symbol[table_size];
		this->capacity = table_size;
		this->mode = quad;
	}
	else if (cmd[0] == 'D') {
		int table_size = stoi(tokens[1]);
		this->hash_const1 = stoi(tokens[2]);
		if (table_size > 999999 || hash_const1 > 999999) throw InvalidInstruction(cmd);
		this->table = new Symbol[table_size];
		this->capacity = table_size;
		this->mode = db;
	}
	for (int i = 0; i < 5; i++) tokens[i] = "";
	while (getline(fname, cmd)) {
		if (split(cmd, tokens)) {
			switch (cmd[0]) {
			case 'I': {
				insert_symbol(cmd, tokens);
				break;
			}
			case 'A': {
				assign(cmd, tokens);
				break;
			}
			case 'C': {
				call(cmd, tokens);
				break;
			}
			case 'L': {
				look_up(cmd, tokens);
				break;
			}
			case 'B': {
				begin();
				break;
			}
			case 'E': {
				end();
				break;
			}
			case 'P': {
				print();
				break;
			}
			}
		}
		else throw InvalidInstruction(cmd);
	}
	if (unclosed()) throw UnclosedBlock(this->current_scope);
}