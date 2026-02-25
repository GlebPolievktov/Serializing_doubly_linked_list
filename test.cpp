#include <iostream>
#include <string>
int main()
{
    std::string str = "node_data;3";
    size_t pos = str.rfind(';');
    std::string data = str.substr(0, pos);
    int randindex = std::stoi(str.substr(pos + 1));
    std::cout << pos << " " << data << " " <<  randindex;
}