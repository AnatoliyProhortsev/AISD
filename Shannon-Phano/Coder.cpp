#include "Coder.h"

int Coder::NodeComparator(const void* elem1, const void* elem2)
{
	const Node a = *(Node*)elem1;
	const Node b = *(Node*)elem2;

	if (a.frequency < b.frequency)
		return -1;					//���������� �� �����������
	else if (a.frequency > b.frequency)
		return 1;
	else
		return 0;
}

void Coder::EncodeShannonAlgorithm(int li, int ri)
{
	if (li == ri) //���� ����� ���������� ���������
		return;
	else if (ri - li == 1) //���� ���������� ������� �� ���� ��������
	{
		codes[table[li].symb] += '0';
		codes[table[ri].symb] += '1';

		operationsCount += 5;
	}
	else
	{
		int m = getMedian(li, ri); //������ ���������� ������� ������� ���������

		for (int i = li; i <= ri; i++)
		{
			if (i > m)
				codes[table[i].symb] += '1';
			else
				codes[table[i].symb] += '0';

			operationsCount += 3;
		}

		EncodeShannonAlgorithm(li, m); //���������� ����� �� �������(������������)
		EncodeShannonAlgorithm(m + 1, ri); //������
	}
}

int Coder::getMedian(int li, int ri) //������� ���������� ���������� ������� � �������� ����������
{
	int sumOne = 0;

	for (int i = li; i < ri; i++)
	{
		sumOne += table[i].frequency;
		operationsCount += 2;
	}

	int sumTwo = table[ri].frequency;
	int m = ri;
	operationsCount += 3;

	double d = 0;

	do {
		d = abs(sumOne - sumTwo);
		m -= 1;
		sumOne -= table[m].frequency;
		sumTwo += table[m].frequency;
		operationsCount += 10;
	} while (abs(sumOne - sumTwo) <= d);

	return m;
}

bool Coder::EncodeFile(const std::string& inputFileName, const std::string& outputFileName)
{
	std::map<char, int> frequences; //���-�� ��������� ������� ����������� �������

	std::ifstream inputFile(inputFileName, std::ios::in);
	if (!inputFile)
	{
		std::cout << "Error: cannot open file " << inputFileName << '\n';
		return false;
	}
	
	//������� ������ ��������

	char symb;

	while (inputFile.get(symb))
	{
		frequences[symb]++;
		if (inputFile.fail())
		{
			std::cout << "Error during reading input file. Aborting.\n ";
			inputFile.close();
			return false;
		}
		operationsCount += 2;
	}

	//�������� ������� �����

	tSize = (int)frequences.size();

	table = new Node[tSize];
	operationsCount += 5;

	if (!table)
	{
		std::cout << "\nError during creating the table. Aborting.\n";
		return false;
	}	

	std::map<char, int>::iterator it;
	int i;

	for (it = frequences.begin(), i = 0; it != frequences.end(); ++it, ++i)
	{
		table[i].symb = (*it).first;
		table[i].frequency = (*it).second;
		operationsCount += 6;
	}

	qsort(table, tSize, sizeof(Node), NodeComparator);	 //���������� �������

	EncodeShannonAlgorithm(0, tSize - 1);	//��������� ������� �������-����

	std::cout << '\n';

	std::ofstream outputFile(outputFileName, std::ios::out);

	if (!outputFile.is_open())
	{
		std::cout << "Error: cannot open file " << outputFileName << '\n';
		return false;
	}
		
	outputFile << tSize<<'\n';

	std::cout << "Table of frequences:\n\n";

	//����� ������� �����

	for (i = 0; i < tSize; i++)
	{
		std::cout << table[i].symb << ' ' << table[i].frequency << ' ' << codes[table[i].symb].c_str() << '\n';
		outputFile << table[i].symb << ' ' << table[i].frequency << ' ' << codes[table[i].symb].c_str() << '\n';
		if (outputFile.fail())
		{
			std::cout << "Error during writing output file. Aborting.\n ";
			outputFile.close();
			return false;
		}

		operationsCount += 5;
	}
	std::cout << "\nEncoded text:\n\n";

inputFile.clear(); //������� ����� ����� �����
inputFile.seekg(0);
operationsCount += 2;

//������ ����� � ����

while (inputFile.get(symb))
{
	std::cout << codes[symb].c_str();
	outputFile << codes[symb].c_str();
	if (inputFile.fail() || outputFile.fail())
	{
		std::cout << "Error during writing codes in file. Aborting.\n ";
		inputFile.close();
		outputFile.close();
		return false;
	}
	operationsCount += 6;
}

std::cout << "\n\nEncoded text and table successfully written in " << outputFileName;
std::cout << '\n';

inputFile.close();
outputFile.close();
operationsCount += 2;

return true;
}

bool Coder::DecodeFile(const std::string& inputFileName, const std::string& outputFileName)
{
	std::ifstream inputFile(inputFileName, std::ios::in);
	if (!inputFile)
	{
		std::cout << "\nError: cannot open file " << inputFileName << '\n';
		return false;
	}

	std::map<char, int> frequences; //���-�� ��������� ������� ����������� �������

	//���������� ������� �����

	inputFile >> tSize;
	if (inputFile.fail())
	{
		std::cout << "\nError during reading size of codes. Aborting.\n ";
		inputFile.close();
		return false;
	}
	char ch, code[128];
	int freq, minLen = 128;
	inputFile.get();
	for (int i = 0; i < tSize; i++)
	{
		inputFile.get(ch);
		inputFile.get();
		inputFile >> freq;
		inputFile.get();
		inputFile >> code;

		codes[ch] = code;
		if (codes[ch].length() < minLen)	//����������� ����������� ����� ����
			minLen = codes[ch].length();

		inputFile.get();
		if (inputFile.fail())
		{
			std::cout << "\nError during reading the codes. Aborting.\n ";
			inputFile.close();
			return false;
		}
	}


	std::ofstream outputFile(outputFileName, std::ios::out);
	if (!outputFile.is_open())
	{
		std::cout << "\nError: cannot open file " << outputFileName << '\n';
		return false;
	}

	std::string accum = "";
	std::map<char, std::string>::iterator it;

	std::cout << "\nDecoded text:\n\n";

	//������ ��������������� ������

	while (inputFile.get(ch))
	{
		if (inputFile.fail() || outputFile.fail())
		{
			std::cout << "\nError during writing text in file. Aborting.\n ";
			inputFile.close();
			outputFile.close();
			return false;
		}
		accum += ch;
		if ((accum.length() >= minLen))
		{
			for (it = codes.begin(); it != codes.end(); it++)
			{
				if (!strcmp((*it).second.c_str(), accum.c_str()))	//��������� ���������������� ������ ����� � ��������
				{
					accum = "";
					std::cout << (*it).first;
					outputFile << (*it).first;
				}
			}
		}
	}

	std::cout << "\n\nText successfully written in " << outputFileName;

	std::cout << '\n';

	outputFile.close();
	inputFile.close();
}

unsigned Coder::getOpCount()
{
	return operationsCount;
}

Coder::~Coder()
{
	codes.clear();

	if(table)
		delete[] table;
}
