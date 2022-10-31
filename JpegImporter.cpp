#include "JpegImporter.h"

static const int zigZagToRec[64] // последовательное обращение к источнику
{
	0,  1,  8,  16, 9,  2,  3,  10,
	17, 24, 32, 25, 18, 11, 4,  5,
	12, 19, 26, 33, 40, 48, 41, 34,
	27, 20, 13, 6,  7,  14, 21, 28,
	35, 42, 49, 56, 57, 50, 43, 36,
	29, 22, 15, 23, 30, 37, 44, 51,
	58, 59, 52, 45, 38, 31, 39, 46,
	53, 60, 61, 54, 47, 55, 62, 63
};

jpeg_importer::BinaryTree::BinaryTree(char* tableLevel, char* table) : root(new node(-1, nullptr))
{

	int offset = 0;
	int curLevel = 0;
	countOfValue = 0;

	node* current = root;

	for (int i = 0; i < levelCount; i++)
	{
		countOfValue += tableLevel[i];
		for (int j = 0; j < tableLevel[i]; j++)
		{
			while (i != curLevel || !(current->left == nullptr || current->right == nullptr))
			{
				if (current->left == nullptr)
				{
					current->left = new node(-1, current);
					current = current->left;
					curLevel++;
				}
				else if (current->left->visited == false)
				{
					current = current->left;
					curLevel++;
				}
				else
				{
					if (current->right == nullptr)
					{
						current->right = new node(-1, current);
						current = current->right;
						curLevel++;
					}
					else if (current->right->visited == false)
					{
						current = current->right;
						curLevel++;
					}
					else
					{
						current->visited = true;
						current = current->root;
						curLevel--;
					}
				}
			}
			if (current->left == nullptr)
			{
				current->left = new node((unsigned char)table[offset + j], current);
				current->left->lief = true;
				current->left->visited = true;
			}
			else if (current->right == nullptr)
			{
				current->right = new node((unsigned char)table[offset + j], current);
				current->right->lief = true;
				current->right->visited = true;
			}
		}
		offset += tableLevel[i];
	}
}

int jpeg_importer::BinaryTree::Get(BitReader& byte)
{
	node* current = root;
	while (current->lief != true)
	{
		int nextB = byte.ReadNextBit();
		if (nextB == 0)
		{
			current = current->left;
		}
		else if (nextB == 1)
		{
			current = current->right;
		}
		if (current == nullptr)
		{
			assert(false);
		}
	}
	int result = current->value;
	return result;
}

int jpeg_importer::BitReader::ReadNextBit()
{
	char input[1];
	if (byte_pointer == 7)
	{
		in->read(input, 1);

		if ((unsigned char)input[0] == 0xff)
		{
			in->read(input, 1);
			if (input[0] == 0x00)
			{
				input[0] = 0xff;
			}
			else
			{
				throw JpegExeption("Format error: unknow byte 0xFF in raw data");
			}

		}
		byte = std::bitset<8>(input[0]);
		byte_pointer = 0;
	}
	else
	{
		byte_pointer++;
	}
	return byte[7 - byte_pointer];
}

int jpeg_importer::FindMarker()
{
	char input[1];
	in.read(input, 1);
	while (input[0] != marker)
	{
		in.read(input, 1);
	}
	in.read(input, 1);
	return (unsigned char)input[0];
}

void jpeg_importer::StartProcess()
{
	char input = (char)FindMarker();
	if (input != marker_SOI)
	{
		throw JpegExeption("Format error: no start marker");
	}
	ProcessFile();
}

void jpeg_importer::ProcessFile()
{

	bool endOfFile = false;

	while (!endOfFile)
	{

		char input = (char)FindMarker();

		//Process Marker
		switch (input)
		{
			char d_size[2];
			unsigned int size;

		case marker_SOF0:
			in.read(d_size, 2);
			size = char2ToUint(d_size[0], d_size[1]) - 2;
			ProcessMarkerSOF0(size);
			break;

		case marker_SOF1:
		case marker_SOF2:
		case marker_SOF3:
		case marker_SOF5:
		case marker_SOF6:
		case marker_SOF7:
		case marker_JPG:
		case marker_SOF9:
		case marker_SOF10:
		case marker_SOF11:
		case marker_SOF13:
		case marker_SOF14:
		case marker_SOF15:
			throw JpegExeption("Unsupported format");
			break;

		case marker_DHT:
			in.read(d_size, 2);
			size = char2ToUint(d_size[0], d_size[1]) - 2;
			ProcessMarkerDHT(size);
			break;

		case marker_DAC:
			throw JpegExeption("Unsupported marker");
			break;

		case marker_EOI:
			Decode();
			Pack();
			endOfFile = true;
			break;

		case marker_SOS:
			in.read(d_size, 2);
			size = char2ToUint(d_size[0], d_size[1]) - 2;
			ProcessMarkerSOS(size);
			break;

		case marker_DQT:
			in.read(d_size, 2);
			size = char2ToUint(d_size[0], d_size[1]) - 2;
			ProcessMarkerDQT(size);
			break;

		case marker_DNL:
			throw JpegExeption("Unsupported marker");
			break;

		case marker_DRI:
			in.read(d_size, 2);
			size = char2ToUint(d_size[0], d_size[1]) - 2;
			ProcessMarkerDRI(size);
			break;

		case marker_EXP:
			throw JpegExeption("Unsupported marker");
			break;

			//case APP:
			//case JPGn:

		case marker_COM:
			in.read(d_size, 2);
			size = char2ToUint(d_size[0], d_size[1]) - 2;
			ProcessMarkerCOM(size);
			break;

		case marker_TEM:
			throw JpegExeption("Unsupported marker");
			break;

			//case RES:

		default:
			if (input >= marker_from_APPn && input <= marker_to_APPn)
			{
				in.read(d_size, 2);
				size = char2ToUint(d_size[0], d_size[1]) - 2;
				char* garb = new char[size];
				in.read(garb, size);
				delete[] garb;
				break;

			}
			else if (input >= marker_from_JPGn && input <= marker_to_JPGn)
			{
				throw JpegExeption("Unsupported marker");
				break;

			}
			else if (input >= marker_from_RESn && input <= marker_to_RESn)
			{
				throw JpegExeption("Unsupported marker");
				break;

			}
			else
			{
				std::string err = "Error: unknow marker. Marker" + std::to_string((int)input);
				throw JpegExeption(err);
				break;
			}
			break;
		}
	}

	clearCreationInfo();
}

//Маркер [FF C0]: SOF0 — Baseline DCT
void jpeg_importer::ProcessMarkerSOF0(unsigned int size)
{
	char* input = new char[size];
	in.read(input, size);

	precision = (unsigned int)input[0];
	Height = char2ToUint(input[1], input[2]);
	Weight = char2ToUint(input[3], input[4]);
	CountOfChanel = (unsigned int)input[5];

	if (CountOfChanel != 3 || precision != 8)
	{
		throw JpegExeption("Unknow format");
	}

	chanels = new ChanelInfo[CountOfChanel];

	hSamplingMax = 0;
	vSamplingMax = 0;

	for (int i = 0; i < CountOfChanel; i++)
	{
		//_Chanels.push_back();
		int horSize = (uint)input[6 + i * 3 + 1] / 16;
		int vertSize = (uint)input[6 + i * 3 + 1] % 16;

		hSamplingMax = std::max(hSamplingMax, horSize);
		vSamplingMax = std::max(vSamplingMax, vertSize);

		chanels[i].ident = (unsigned int)input[6 + i * 3 + 0];
		chanels[i].hor_size = horSize;
		chanels[i].vert_size = vertSize;

		chanels[i]._table_quantization = table_quantization[input[6 + i * 3 + 2]];
	}

	int x_nod = std::gcd(std::gcd(chanels[0].hor_size, chanels[1].hor_size), chanels[2].hor_size);
	int y_nod = std::gcd(std::gcd(chanels[0].hor_size, chanels[1].hor_size), chanels[2].hor_size);

	for (int i = 0; i < CountOfChanel; i++)
	{
		hSamplingMax /= x_nod;
		vSamplingMax /= y_nod;
		chanels[i].hor_size /= x_nod;
		chanels[i].vert_size /= y_nod;
	}

	mcuCount = std::ceil((float)Weight / (hSamplingMax * 8)) * std::ceil((float)Height / (vSamplingMax * 8));
	Mcu = new int*** [mcuCount];
	for (int i = 0; i < mcuCount; i++)
	{
		Mcu[i] = new int** [3];
	}
	delete[] input;
}

//Маркер [FF C4]: DHT (таблица Хаффмана)
void jpeg_importer::ProcessMarkerDHT(unsigned int size)
{
	char* input = new char[size];
	in.read(input, size);
	int readedByte = 0;

	while (readedByte < size)
	{
		BinaryTree* tree = new BinaryTree(input + readedByte + 1, input + readedByte + 17);

		if (input[readedByte] / 16 == 0)
		{
			table_huffman_DC[(int)(input[readedByte]) % 16] = tree;
		}
		else
		{
			table_huffman_AC[(int)(input[readedByte]) % 16] = tree;
		}
		readedByte += 17 + tree->countOfValue;

	}
	delete[] input;
}

//Маркер[FF DB]: DQT — таблица квантования
void jpeg_importer::ProcessMarkerDQT(unsigned int size)
{
	char* input = new char[size];

	assert(size % 65 == 0);

	int readedByteCount = 0;

	while (readedByteCount < size)
	{
		in.read(input, 1);
		readedByteCount += 1;

		unsigned char length = ((unsigned char)input[0] / 16);
		unsigned char identifier = ((unsigned char)input[0] % 16);

		if (length == 1)
		{
			throw JpegExeption("Unsupported format");
		}

		in.read(input, 64);
		readedByteCount += 64;

		int* result = new int[64];
		for (int i = 0; i < 64; i++)
		{
			result[zigZagToRec[i]] = (unsigned char)input[i];
		}

		table_quantization.insert({ identifier, result });
	}
	delete[] input;
}

void jpeg_importer::ProcessMarkerDRI(unsigned int size)
{
	assert(size == 2);
	char input[2];
	in.read(input, 2);
	RestartInterval = char2ToUint(input[0], input[1]);
}

void jpeg_importer::ProcessMarkerCOM(unsigned int size)
{
	char* input = new char[size];
	in.read(input, size);
	Comment = std::string(input);
	delete[] input;
}

//Маркер [FF DA]: SOS (Start of Scan)
void jpeg_importer::ProcessMarkerSOS(unsigned int size)
{
	char* input = new char[size];
	in.read(input, size);

	for (int i = 0; i < CountOfChanel; i++)
	{
		if (chanels[i].ident != input[1 + i * 2])
		{
			throw JpegExeption("Unknow format: index orders do not match");
		}

		chanels[i].table_huffman_DC = table_huffman_DC[(unsigned char)input[2 + i * 2] / 16];
		chanels[i].table_huffman_AC = table_huffman_AC[(unsigned char)input[2 + i * 2] % 16];
	}
	BitReader byte(in);
	int* dc_prev = new int[CountOfChanel]();
	for (int i = 0; i < mcuCount; i++)
	{
		for (int j = 0; j < CountOfChanel; j++)
		{
			Mcu[i][j] = new int* [chanels[j].hor_size * chanels[j].vert_size];
			for (int k = 0; k < chanels[j].hor_size * chanels[j].vert_size; k++)
			{
				Mcu[i][j][k] = new int[64]();

				//READ DC

				int size_val = chanels[j].table_huffman_DC->Get(byte);

				int DC_coff = 0; // если рамер 0, то DC = 0
				if (size_val != 0)
				{
					DC_coff = bitStreamToInt(byte, size_val % 16);
				}

				Mcu[i][j][k][zigZagToRec[0]] = dc_prev[j] + DC_coff;
				dc_prev[j] = Mcu[i][j][k][zigZagToRec[0]];

				//READ AC

				for (int elem = 1; elem < 64; elem++)
				{
					size_val = chanels[j].table_huffman_AC->Get(byte);

					if (size_val == 0)
					{
						break; //оставить нули
					}
					for (int m = 0; m < size_val / 16; m++)
					{
						elem++; //нули
					}

					assert(elem < 64);

					Mcu[i][j][k][zigZagToRec[elem]] = bitStreamToInt(byte, size_val % 16);
				}

			}

		}

		if (i == RestartInterval - 1 && i != mcuCount - 1 && RestartInterval != 0)
		{
			char mark = (char)FindMarker();
			if (mark >= marker_from_RSTn && mark <= marker_to_RSTn)
			{
				for (int ch = 0; ch < CountOfChanel; ch++)
				{
					dc_prev[ch] = 0;
				}
				byte.Revert();
			}
			else
			{
				throw JpegExeption("Restart intervals invalid");
			}
		}
	}

	delete[] input;
}

void jpeg_importer::Decode()
{
	static const float PI = acos(-1);

	for (int i = 0; i < mcuCount; i++)
	{
		for (int j = 0; j < CountOfChanel; j++)
		{
			for (int k = 0; k < chanels[j].hor_size * chanels[j].vert_size; k++)
			{

				for (int elem = 0; elem < 64; elem++)
				{
					Mcu[i][j][k][elem] *= (int)(unsigned char)chanels[j]._table_quantization[elem];
				}

				float* result = InverseFourierTransform(Mcu[i][j][k]);

				for (int elem = 0; elem < 64; elem++)
				{
					Mcu[i][j][k][elem] = (int)result[elem];
				}
				delete[] result;
			}
		}
	}
}

float* jpeg_importer::InverseFourierTransform(int* resTable)
{
	static const float PI = acos(-1);
	float* result = new float[64];

	for (int elem = 0; elem < 64; elem++)
	{
		int u = elem / 8; // v
		int v = elem % 8;
		result[elem] = 0;

		for (int x = 0; x < 7; x++)
		{
			float c_x = (x == 0) ? 1 / sqrt(2) : 1;
			for (int y = 0; y < 7; y++)
			{
				float c_y = (y == 0) ? 1 / sqrt(2) : 1;
				result[elem] += c_x * c_y * resTable[x * 8 + y] * cos((2 * u + 1) * x * PI / 16) * cos((2 * v + 1) * y * PI / 16);
			}
		}
		result[elem] *= (float)1 / 4;
		result[elem] = std::min(std::max(0, (int)result[elem] + 128), 255);
	}
	return result;
}

void jpeg_importer::Pack()
{
	Data = new unsigned char[Weight * Height * CountOfChanel];

	int mcuXsize = hSamplingMax * 8;
	int mcuYsize = vSamplingMax * 8;

	for (int i = 0; i < Weight * Height * CountOfChanel; i += 3)
	{
		int row = i / 3 / Weight;
		int col = i / 3 - row * Weight;

		int mcuNum = row / mcuYsize + col / mcuXsize;

		int mcuRow = row % mcuYsize;
		int mcuCol = col % mcuXsize;

		for (int j = 0; j < CountOfChanel; j++)
		{
			int mcuRowSized = std::floor((float)mcuRow * chanels[j].vert_size / vSamplingMax);
			int mcuColSized = std::floor((float)mcuCol * chanels[j].hor_size / hSamplingMax);

			int tableNum = mcuColSized / 8 + mcuRowSized / 8;

			Data[i + j] = Mcu[mcuNum][j][tableNum][mcuColSized % 8 + mcuRowSized % 8];
		}
	}

	YCbCrToRGB();

}

void jpeg_importer::YCbCrToRGB()
{
	for (int i = 0; i < Weight * Height; i += 3)
	{
		int R = round(Data[i] + 1.402 * (float)(Data[i + 2] - 128));
		int G = round(Data[i] - 0.34414 * (float)(Data[i + 1] - 128) - 0.71414 * (float)(Data[i + 2] - 128));
		int B = round(Data[i] + 1.772 * (float)(Data[i + 1] - 128));

		R = std::min(std::max(0, R), 255);
		G = std::min(std::max(0, G), 255);
		B = std::min(std::max(0, B), 255);

		Data[i] = (unsigned char)R;
		Data[i + 1] = (unsigned char)G;
		Data[i + 2] = (unsigned char)B;
	}

}

void jpeg_importer::clearCreationInfo()
{
	delete[] chanels;
	for (auto l : table_huffman_DC)
	{
		delete l.second;
	}
	for (auto l : table_huffman_AC)
	{
		delete l.second;
	}
	for (auto l : table_quantization)
	{
		delete[] l.second;
	}
	table_huffman_DC.clear();
	table_huffman_AC.clear();
	table_quantization.clear();
}

unsigned int jpeg_importer::char2ToUint(char ch1, char ch2)
{
	return (int)((uint8_t)ch1 / 16 * pow(16, 3) + (uint8_t)ch1 % 16 * pow(16, 2) + (uint8_t)ch2);
}

int jpeg_importer::char2ToInt(char ch1, char ch2)
{
	return (int)((int8_t)ch1 / 16 * pow(16, 3) + (int8_t)ch1 % 16 * pow(16, 2) + (int8_t)ch2);
}

int jpeg_importer::bitStreamToInt(BitReader& bits, int count)
{
	assert(count > 0);
	int first = bits.ReadNextBit();
	int value = first * pow(2, count - 1);

	for (int m = 1; m < count; m++)
	{
		value += bits.ReadNextBit() * pow(2, count - 1 - m);
	}

	value = first == 1 ? value : value - 2 * pow(2, count - 1) + 1;
	return value;
}