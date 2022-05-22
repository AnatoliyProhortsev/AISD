#include "Coder.h"

#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>

int main()
{
    setlocale(LC_ALL, "Russian");

    std::string inputFileName;
    std::string outputFileName;
    char ch;
   
    std::cout << "Choose an option\n\ne - Encode text\nd - Decode text\n\n";
    std::cout << "Your choise: ";
    std::cin >> ch;
    std::cout << '\n';

    Coder coder;

    switch (ch)
    {
    case 'e':
        std::cout << "Enter input text file name: ";
        std::cin >> inputFileName;
        std::cout << "Enter output text file name: ";
        std::cin >> outputFileName;
        if (!coder.EncodeFile(inputFileName, outputFileName))
        {
            std::cout << "\nExitting program.\n";
            return -1;
        }
        std::cout << "\nOperations done: "<<coder.getOpCount()<<'\n';
        break;
    case 'd':
        std::cout << "Enter input text file name: ";
        std::cin >> inputFileName;
        std::cout << "Enter output text file name: ";
        std::cin >> outputFileName;
        if(!coder.DecodeFile(inputFileName, outputFileName))
        {
            std::cout << "\nExitting program.\n";
            return -1;
        }
        break;
    default:
        std::cout << "Invalid option. Exitting programm\n";
        return -1;
    }

    return 0;
}

