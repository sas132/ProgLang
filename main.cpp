#include <iostream>
#include <string>
#include <fstream>

void readLine(std::string fileName)
{
	std::cout << "reading " << fileName <<std::endl;
	std::ifstream input;
	input.open(fileName);

	if(input.is_open()){
	std::string line;
	getline(input, line);
	std::cout << "test1\n";
	std::cout << line << std::endl;
	std::cout << "test2\n";}
	else{
		std::cout << "error\n";}
}

int main(int argc, char** argv)
{
	if(argc == 2)
	{
		readLine(argv[1]);
	}
}
