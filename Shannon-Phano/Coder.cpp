#include "Coder.h"
#define _CRT_SECURE_NO_DEPRECATE

int Coder::NodeComparator(const void* elem1, const void* elem2)
{
	const Node a = *(Node*)elem1;
	const Node b = *(Node*)elem2;

	if (a.frequency < b.frequency)
		return -1;					//Сортировка по возрастанию
	else if (a.frequency > b.frequency)
		return 1;
	else
		return 0;
}

void Coder::EncodeShannonAlgorithm(int li, int ri)
{
	if (li == ri) //Если концы промежутка совпадают
		return;
	else if (ri - li == 1) //Если промежуток состоит из двух символов
	{
		codes[table[li].symb] += '0';
		codes[table[ri].symb] += '1';
	}
	else
	{
		int m = getMedian(li, ri); //Расчёт медианного значения данного интервала

		for (int i = li; i <= ri; i++)
		{
			if (i > m)
				codes[table[i].symb] += '1';
			else
				codes[table[i].symb] += '0';
		}

		EncodeShannonAlgorithm(li, m); //Промежуток слева от медианы(включительно)
		EncodeShannonAlgorithm(m + 1, ri); //Справа
	}
}

int Coder::getMedian(int li, int ri) //Функция нахождения медианного значения в заданном промежутке
{
	int sumOne = 0;

	for (int i = li; i < ri; i++)
	{
		sumOne += table[i].frequency;
	}

	int sumTwo = table[ri].frequency;
	int m = ri;

	double d = 0;

	do {
		d = abs(sumOne - sumTwo);
		m -= 1;
		sumOne -= table[m].frequency;
		sumTwo += table[m].frequency;
	} while (abs(sumOne - sumTwo) <= d);

	return m;
}

bool Coder::EncodeFile(const std::string& inputFileName, const std::string& outputFileName)
{
	std::map<char, int> frequences; //Кол-во вхождений каждого уникального символа

	std::ifstream inputFile(inputFileName, std::ios::in);
	if (!inputFile)
	{
		std::cout << "Error: cannot open file " << inputFileName << '\n';
		return false;
	}
		
	char symb;

	while (inputFile.get(symb))
	{
		frequences[symb]++;
	}

	tSize = (int)frequences.size();

	table = new Node[tSize];
	if (!table)
		return false;

	std::map<char, int>::iterator it;

	int i;
	for (it = frequences.begin(), i = 0; it != frequences.end(); ++it, ++i)
	{
		table[i].symb = (*it).first;
		table[i].frequency = (*it).second;
	}

	qsort(table, tSize, sizeof(Node), NodeComparator);

	EncodeShannonAlgorithm(0, tSize - 1);

	std::cout << '\n';

	std::ofstream outputFile(outputFileName, std::ios::out);
	if (!outputFile.is_open())
	{
		std::cout << "Error: cannot open file " << outputFileName << '\n';
		return false;
	}
		
	outputFile << tSize<<'\n';

	std::cout << "\nTable of frequences:\n";

	for (i = 0; i < tSize; i++)
	{
		std::cout << table[i].symb << ' ' << table[i].frequency << ' ' << codes[table[i].symb].c_str() << '\n';
		outputFile << table[i].symb << ' ' << table[i].frequency << ' ' << codes[table[i].symb].c_str() << '\n';
	}
	std::cout << "\nText and table successfully written in "<<outputFileName<<"\n\nEncoded text:\n\n";

	inputFile.clear(); //Очистка флага конца файла
	inputFile.seekg(0);

	while (inputFile.get(symb)) //Запись кодов в файл
	{
		std::cout << codes[symb].c_str();
		outputFile << codes[symb].c_str();
	}

	std::cout << '\n';

	inputFile.close();
	outputFile.close();

	return true;
}

bool Coder::DecodeFile(const std::string& inputFileName, const std::string& outputFileName)
{
	std::map<char, int> frequences; //Кол-во вхождений каждого уникального символа

	std::ifstream inputFile(inputFileName, std::ios::in);
	if (!inputFile)
	{
		std::cout << "Error: cannot open file " << inputFileName << '\n';
		return false;
	}

	inputFile >> tSize;
	char ch, code[128];
	int freq;
	inputFile.get();
	for (int i = 0; i < tSize; i++)
	{
		inputFile.get(ch);
		inputFile.get();
		inputFile >> freq;
		inputFile.get();
		inputFile >> code;
		codes[ch] = code;
		inputFile.get();
	}

	std::ofstream outputFile(outputFileName, std::ios::out);
	if (!outputFile.is_open())
	{
		std::cout << "Error: cannot open file " << outputFileName << '\n';
		return false;
	}

	std::string accum = "";
	std::map<char, std::string>::iterator it;

	std::cout << "\n\nText successfully written in " << outputFileName << "\n\nDecoded text:\n\n";

	while (inputFile.get(ch))
	{
		accum += ch;
		for (it = codes.begin(); it != codes.end(); it++)
		{
			if (!strcmp((*it).second.c_str(), accum.c_str()))
			{
				accum = "";
				std::cout << (*it).first;
				outputFile << (*it).first;
			}
		}
	}

	std::cout << '\n';

	outputFile.close();
	inputFile.close();
}

Coder::~Coder()
{
	codes.clear();

	if(table)
		delete[] table;
}
