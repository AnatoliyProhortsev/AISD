#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <fstream>

class Coder
{
private:
	struct Node
	{
		char symb;
		int frequency;
	};

	int tSize; //������ �������(���-�� ��������)
	Node* table; //������� ������ ������������� ��������
	std::map<char, std::string> codes; //��� ��� ������� �������

	static int NodeComparator(const void* elem1, const void* elem2); //����������� ���������� ��� ��������� ������
	int getMedian(int li, int ri);
	void EncodeShannonAlgorithm(int li, int ri);
public:
	bool EncodeFile(const std::string& inputFileName, const std::string& outputFileName);
	bool DecodeFile(const std::string& inputFileName, const std::string& outputFileName);
	~Coder();
};

