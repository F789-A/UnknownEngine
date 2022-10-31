#pragma once

#include <iostream>
#include <filesystem>
#include <fstream>

#include <bitset>
#include <cmath>
#include <map>
#include <stack>

#include <algorithm>
#include <numeric>
#include <cassert> 
#include <string>

using uint = unsigned int;

//Ќедоработано, пока не использовать

//TODO: , изменить ќƒ ѕ; убрать дыры с count_of_chanels; поддержка разных цветовых схем
// добаление progressive mode, добавление пор€дка обработки

//Support: Baseline DCT only Huffman coding and 8-bit sample precision;
//ƒобавить Progressive DCT

class jpeg_importer
{
private:
	static const char marker = 0xFF;

	static const char marker_SOF0 = 0xC0;	// Baseline DCT
	static const char marker_SOF1 = 0xC1;	// Extended sequential DCT
	static const char marker_SOF2 = 0xC2;	// Progressive DCT
	static const char marker_SOF3 = 0xC3;	// Lossless (sequential)

	static const char marker_SOF5 = 0xC5;	// Differential sequential DCT
	static const char marker_SOF6 = 0xC6;	// Differential progressive DCT
	static const char marker_SOF7 = 0xC7;	// Differential lossless (sequential)

	static const char marker_JPG = 0xC8;	// Reserved for JPEG extensions
	static const char marker_SOF9 = 0xC9;	// Extended sequential DCT
	static const char marker_SOF10 = 0xCA;	// Progressive DCT
	static const char marker_SOF11 = 0xCB;	// Lossless (sequential)

	static const char marker_SOF13 = 0xCD;	// Differential sequential DCT
	static const char marker_SOF14 = 0xCE;	// Differential progressive DCT
	static const char marker_SOF15 = 0xCF;	// Differential lossless (sequential)

	static const char marker_DHT = 0xC4;	// Define Huffman table(s)

	static const char marker_DAC = 0xCC;	// Define arithmetic coding conditioning(s)

	static const char marker_from_RSTn = 0xD0; // Restart with modulo 8 count УmФ from
	static const char marker_to_RSTn = 0xD7;	// Restart with modulo 8 count УmФ to

	static const char marker_SOI = 0xD8;	// Start of image
	static const char marker_EOI = 0xD9;	// End of image
	static const char marker_SOS = 0xDA;	// Start of scan
	static const char marker_DQT = 0xDB;	// Define quantization table(s)

	static const char marker_DNL = 0xDC;	// Define number of lines
	static const char marker_DRI = 0xDD;	// Define restart interval
	static const char marker_DHP = 0xDE;	// Define hierarchical progression
	static const char marker_EXP = 0xDF;	// Expand reference component(s)

	static const char marker_from_APPn = 0xE0; // Reserved for application segments from
	static const char marker_to_APPn = 0xEF;	// Reserved for application segments to

	static const char marker_from_JPGn = 0xF0; // Reserved for JPEG extensions from
	static const char marker_to_JPGn = 0xFD;	// Reserved for JPEG extensions to

	static const char marker_COM = 0xFE;	// Comment

	static const char marker_TEM = 0x01;	// For temporary private use in arithmetic coding

	static const char marker_from_RESn = 0x02; // Reserved from
	static const char marker_to_RESn = 0xBF;	// Reserved to

	//class 

	class BitReader
	{
	public:
		std::ifstream* in;

		std::bitset<8> byte;
		int byte_pointer = 7;

		BitReader(std::ifstream& in) : in(&in) {}

		int ReadNextBit();
		void Revert()
		{
			byte_pointer = 7;
		}
	};

	class JpegExeption : std::exception
	{
	private:
		std::string info;
	public:
		JpegExeption(std::string&& info) noexcept : info(std::move(info)) {}
		JpegExeption(const std::string& info) noexcept : info(info) {}
		~JpegExeption() noexcept = default;
	};

	/*class JpegMarker
	{
		unsigned char ident;
		int number;
	};*/

	class BinaryTree
	{
	private:

		class node
		{
		public:
			int value;
			bool lief;
			bool visited;
			node* left;
			node* right;
			node* root;

			node(int value, node* root) : value(value), root(root), left(nullptr), right(nullptr), lief(false), visited(false) {}

			node operator=(const node&) = delete;
			node(const node&) = delete;

			~node()
			{
				if (left != nullptr)
				{
					delete left;
				}
				if (right != nullptr)
				{
					delete right;
				}
			}
		};

		static const int levelCount = 16;

	public:
		node* root;
		int countOfValue;
		BinaryTree(char* tableLevel, char* table);
		int Get(BitReader& byte);

		BinaryTree operator=(const BinaryTree& a) = delete;
		BinaryTree(const BinaryTree& a) = delete;

		~BinaryTree()
		{
			delete root;
		}
	};

	class ChanelInfo
	{
	public:

		int ident;
		int vert_size;
		int hor_size;

		int* _table_quantization;

		BinaryTree* table_huffman_DC;
		BinaryTree* table_huffman_AC;

		ChanelInfo() {};

		~ChanelInfo() = default;
	};

	std::ifstream in;

	int hSamplingMax;
	int vSamplingMax;
	int mcuCount;
	int precision;
	ChanelInfo* chanels;

	std::vector<ChanelInfo> _Chanels;

	int**** Mcu; // «аменить на ветор

	std::vector<std::vector<std::vector<std::array<int, 64>>>> _Mcu;

	int RestartInterval;

	std::map<unsigned char, int*> table_quantization;

	std::map<int, BinaryTree*> table_huffman_DC;
	std::map<int, BinaryTree*> table_huffman_AC;

	void StartProcess();
	void ProcessFile();
	int FindMarker();
	void ProcessMarkerCOM(unsigned int size);
	void ProcessMarkerDQT(unsigned int size);
	void ProcessMarkerSOF0(unsigned int size);
	void ProcessMarkerDHT(unsigned int size);
	void ProcessMarkerSOS(unsigned int size);
	void ProcessMarkerDRI(unsigned int size);
	void Decode();
	void Pack();
	void clearCreationInfo();
	void YCbCrToRGB();
	float* InverseFourierTransform(int* resTable);

	unsigned int char2ToUint(char ch1, char ch2);
	int char2ToInt(char ch1, char ch2);
	int bitStreamToInt(BitReader& bits, int count);


public:
	unsigned char* Data;
	int Height;
	int Weight;
	int CountOfChanel;
	std::string Comment;

	jpeg_importer(std::filesystem::path input) : RestartInterval(0)
	{
		in = std::ifstream(input.c_str(), std::ios::binary | std::ios::in);

		StartProcess();
	}

	jpeg_importer operator=(const jpeg_importer&) = delete;
	jpeg_importer(const jpeg_importer&) = delete;

	~jpeg_importer()
	{
		delete[] Data;
	}
};