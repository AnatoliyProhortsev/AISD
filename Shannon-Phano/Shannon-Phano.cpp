#include "Coder.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

#define NL '\n'

int main()
{
    setlocale(0, "");
    std::string inputFileName;
    std::string outputFileName;
    char ch;
   
    std::cout << "Choose an option\ne - Encode text\nd - Decode text\n";
    std::cout << "Your choise: ";
    //std::cin >> ch;
    ch = 'd';

    Coder coder;

    switch (ch)
    {
    case 'e':
        /*std::cout << "Enter input text file name: ";
        std::cin >> inputFileName;
        std::cout << "Enter output text file name: ";
        std::cin >> outputFileName;*/
        coder.EncodeFile("english.txt", "encoded.txt");
        break;
    case 'd':
        /*std::cout << "Enter input text file name: ";
        std::cin >> inputFileName;
        std::cout << "Enter output text file name: ";
        std::cin >> outputFileName;*/
        coder.DecodeFile("encoded.txt", "decoded.txt");
        break;
    default:
        std::cout << "Invalid option. Exitting programm\n";
        return -1;
    }

    return 0;
}

