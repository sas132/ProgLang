#ifndef MAIN
#define MAIN

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <ctype.h>

#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401

class LexerParse
{
private:
	int lookahead;
	int lineCnt;
	int numLexeme;
	int currentVar;
	bool begun;
	bool ended;
	std::ifstream input;
	std::string varNames[20];
	int varTypes[20];
	int lookup(std::string value);
	void setArrays();

public:
	int lexan();
	void insert(std::string name, int type);
	void AssignStmt();
	void expression();
	void term();
	void factor();
	void match(int t);
	void readLine(std::string filename);
	void print();
//	int main(int argc, char** argv);
};

#endif
