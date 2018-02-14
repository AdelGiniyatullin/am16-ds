#include <iostream>
#include <fstream>
#include <vector>

char koi8[256];
char win1251[256];
char utf8[65536];
unsigned char codesKoiToWin[256];
unsigned char codesWinToKoi[256];
unsigned short int codesKoiToUTF[256]; // ���� �������� � ����, ������� ����� ����� �����
unsigned short int codesWinToUTF[256];

int IdentifyEncoding (const std::vector<unsigned char> input); // 0 - unknown, 1 - koi8, 2 - win1251
unsigned char most_frequent_symbol    (const std::vector<unsigned char> input);

std::vector<unsigned short int> * ConvertFromKoi8ToUTF8 (const std::vector<unsigned char> koi_input);
std::vector<unsigned short int> * ConvertFromWin1251ToUTF8 (const std::vector<unsigned char> win_input);
std::vector<unsigned char> * ConvertFromWin1251ToKoi8 (const std::vector<unsigned char> win_input);
std::vector<unsigned char> * ConvertFromKoi8ToWin1251 (const std::vector<unsigned char> koi_input);

void convert_to_UTF_and_print_into_file (const std::vector<unsigned char> input, std::ofstream & out, int encoding);

int main ()
{
	/////------------------------------| ������������� ����� |------------------------------/////

	//system ("chcp 1251"); // �������� ��� ������� � �����.

	for (int i = 0; i < 256; ++i) {
		koi8[i] = 0;
		win1251[i] = 0;
	}
	for (int i = 0; i < 65536; ++i) {
		utf8[i] = 0;
	}

	koi8[0xA3] = '�';	koi8[0xB3] = '�';

	koi8[0xC0] = '�'; 	koi8[0xC1] = '�';	koi8[0xC2] = '�';	koi8[0xC3] = '�';
	koi8[0xC4] = '�';	koi8[0xC5] = '�';	koi8[0xC6] = '�';	koi8[0xC7] = '�';
	koi8[0xC8] = '�';	koi8[0xC9] = '�';	koi8[0xCA] = '�';	koi8[0xCB] = '�';
	koi8[0xCC] = '�';	koi8[0xCD] = '�';	koi8[0xCE] = '�';	koi8[0xCF] = '�';
	koi8[0xD0] = '�';	koi8[0xD1] = '�';	koi8[0xD2] = '�';	koi8[0xD3] = '�';
	koi8[0xD4] = '�';	koi8[0xD5] = '�';	koi8[0xD6] = '�';	koi8[0xD7] = '�';
	koi8[0xD8] = '�';	koi8[0xD9] = '�';	koi8[0xDA] = '�';	koi8[0xDB] = '�';
	koi8[0xDC] = '�';	koi8[0xDD] = '�';	koi8[0xDE] = '�';	koi8[0xDF] = '�';

	koi8[0xE0] = '�';	koi8[0xE1] = '�';	koi8[0xE2] = '�';	koi8[0xE3] = '�';
	koi8[0xE4] = '�';	koi8[0xE5] = '�';	koi8[0xE6] = '�';	koi8[0xE7] = '�';
	koi8[0xE8] = '�';	koi8[0xE9] = '�';	koi8[0xEA] = '�';	koi8[0xEB] = '�';
	koi8[0xEC] = '�';	koi8[0xED] = '�';	koi8[0xEE] = '�';	koi8[0xEF] = '�';
	koi8[0xF0] = '�';	koi8[0xF1] = '�';	koi8[0xF2] = '�';	koi8[0xF3] = '�';
	koi8[0xF4] = '�';	koi8[0xF5] = '�';	koi8[0xF6] = '�';	koi8[0xF7] = '�';
	koi8[0xF8] = '�';	koi8[0xF9] = '�';	koi8[0xFA] = '�';	koi8[0xFB] = '�';
	koi8[0xFC] = '�';	koi8[0xFD] = '�';	koi8[0xFE] = '�';	koi8[0xFF] = '�';

	win1251[0xA8] = '�'; win1251[0xB8] = '�';
	for (int i = 0; i < 32; ++i) {
		win1251[0xC0 + i] = '�' + i - 1;
		win1251[0xE0 + i] = '�' + i - 1;
	}

	utf8[0xD001] = '�'; utf8[0xD191] = '�';
	for (int i = 0; i < 16; ++i) {
		utf8[0xD090 + i] = '�' + i - 1;
		utf8[0xD0B0 + i] = '�' + i - 1;
	}
	for (int i = 0; i < 16; ++i) {
		utf8[0xD090 + i + 16] = '�' + i;
		utf8[0xD180 + i] = '�' + i; // �.�. ��������� ����� ����� �������, ������ ��� �����
	}

	for (int i = 0; i < 0xC0; ++i) { // ������� ASCII � ��������� ����� ���������� �� ������, �.�. �� �� ���.
		codesKoiToWin[i] = i;
		codesWinToKoi[i] = i;
	}
	for (int i_win = 0x80; i_win <= 0xFF; ++i_win) { // ���������� ������� �������� KOI � Win
		for (int i_koi = 0x80; i_koi <= 0xFF; ++i_koi) {
			if (koi8[i_koi] == win1251[i_win]) {
				codesKoiToWin[i_koi] = i_win;
				codesWinToKoi[i_win] = i_koi;
			}
		}
	}
	
	for (int i = 0; i < 0xC0; ++i) { // ������� ASCII � ��������� ����� ���������� �� ������, �.�. �� �� ���.
		codesWinToUTF[i] = i;
		codesKoiToUTF[i] = i;
	}
	for (int i_win = 0x80; i_win <= 0xFF; ++i_win) { // ���������� ������� �������� � UTF-8
		for (int i_utf = 0xD090; i_utf <= 0xD18F; ++i_utf) {
			if (utf8[i_utf] == win1251[i_win]) {
				codesWinToUTF[i_win] = i_utf;
				codesKoiToUTF[codesWinToKoi[i_win]] = i_utf;
			}
		}
	}
	codesKoiToUTF[0xA3] = 0xD191; codesKoiToUTF[0xB3] = 0xD001;
	codesWinToUTF[0xB8] = 0xD191; codesWinToUTF[0xA8] = 0xD001; // ����� �

	/////------------------------------| ������������� ����� ��������� |------------------------------/////

	std::ifstream text ("disp.txt", std::ifstream::binary);
	if (!text.is_open()) {
		throw std::runtime_error ("��� ����� �� ������");
	}
	std::ofstream output ("result.txt", std::ofstream::binary);
	std::vector<unsigned char> input;
	std::string fromfile;
	//std::cout << "done\n";

	char c;
	while (text.get (c)) {
		fromfile.push_back (c); // ������ � ������
	}
	//std::cout << std::endl;
	const char * chars = fromfile.data ();
	const unsigned char* data_bytes = reinterpret_cast<const unsigned char*>(chars); // ��������� ��� ������ � unsigned char

	//std::cout << "done\n";
	for (int i = 0; i < fromfile.size (); ++i) {
		input.push_back (data_bytes[i]); // ��� ������������� ���� �������.
		//std::cout << koi8[codesWinToKoi[data_bytes[i]]] << win1251[data_bytes[i]] << std::endl;
	}

	std::vector<unsigned char> * converted;
	std::vector<unsigned char> * new_inputKoiToWin;
	std::vector<unsigned char> * new_inputWinToKoi;
	int encoding = IdentifyEncoding (input);
	if (encoding != 0) { // ����� ���������� ���������
		convert_to_UTF_and_print_into_file (input, output, encoding);
	}
	else {
		new_inputKoiToWin = &input;
		new_inputWinToKoi = &input;
		for (int i = 0; i < 3; ++i) {
			converted = ConvertFromKoi8ToWin1251 (*new_inputKoiToWin);
			if (i > 0) { // ������� input ������� �� �����
				delete new_inputKoiToWin;
			}
			new_inputKoiToWin = converted;
			encoding = IdentifyEncoding (*new_inputKoiToWin);
			if (encoding != 0) {
				convert_to_UTF_and_print_into_file (*new_inputKoiToWin, output, encoding);
				if (i > 0) {
					delete new_inputKoiToWin;
				}
				break;
			}

			converted = ConvertFromWin1251ToKoi8 (*new_inputWinToKoi);
			if (i > 0) { // ������� input ������� �� �����
				delete new_inputWinToKoi;
			}
			new_inputWinToKoi = converted;
			encoding = IdentifyEncoding (*new_inputWinToKoi);
			if (encoding != 0) {
				convert_to_UTF_and_print_into_file (*new_inputWinToKoi, output, encoding);
				if (i > 0) {
					delete new_inputWinToKoi;
				}
				break;
			}
		}
	}

	text.close ();
	output.close ();
	std::cin >> c;

	return 0;
}

std::vector<unsigned char> * ConvertFromKoi8ToWin1251 (const std::vector<unsigned char> koi_input) {
	std::vector<unsigned char> * result = new std::vector<unsigned char>;
	for (int i = 0; i < koi_input.size (); ++i) {
		result->push_back (codesKoiToWin[koi_input[i]]);
	}
	return result;
}

std::vector<unsigned char> * ConvertFromWin1251ToKoi8 (const std::vector<unsigned char> win_input)
{
	std::vector<unsigned char> * result = new std::vector<unsigned char>;
	for (int i = 0; i < win_input.size (); ++i) {
		result->push_back (codesWinToKoi[win_input[i]]);
	}
	return result;
}

std::vector<unsigned short int> * ConvertFromKoi8ToUTF8 (const std::vector<unsigned char> koi_input)
{
	std::vector<unsigned short int> * result = new std::vector<unsigned short int>;
	for (int i = 0; i < koi_input.size (); ++i) {
		result->push_back (codesKoiToUTF[koi_input[i]]);
	}
	return result;
}

std::vector<unsigned short int> * ConvertFromWin1251ToUTF8 (const std::vector<unsigned char> win_input)
{
	std::vector<unsigned short int> * result = new std::vector<unsigned short int>;
	for (int i = 0; i < win_input.size (); ++i) {
		result->push_back (codesWinToUTF[win_input[i]]);
	}
	return result;
}

int IdentifyEncoding (const std::vector<unsigned char> input) { // 0 - unknown, 1 - koi8, 2 - win1251
	unsigned char mfs = most_frequent_symbol (input);
	if ('�' == koi8[mfs]) {
		//std::cout << 1 << ' ' << mfs << ";\n";
		return 1;
	}
	else if ('�' == win1251[mfs]) {
		//std::cout << 2 << ' ' << mfs << ";\n";
		return 2;
	}
	else {
		//std::cout << 0 << ' ' << mfs << ";\n";
		return 0;
	}
}

unsigned char most_frequent_symbol (const std::vector<unsigned char> input) {
	int symb_count[256];
	unsigned char most_frequent_one = 0xC0;
	int amount;

	for (int i = 0; i < 0xFF; ++i) {
		symb_count[i] = 0;
	}
	for (int i = 0; i < input.size (); ++i) {
		if (input[i] < 0x80) { // ������� ASCII ��� �� ����������
			continue;
		}
		++symb_count[input[i]];
	} // �������� ����������

	for (int i = 0xC0; i < 0xFF; ++i) {
		if (symb_count[i] > symb_count[most_frequent_one]) {
			most_frequent_one = i;
		}
	}
	return most_frequent_one;
}

void convert_to_UTF_and_print_into_file (const std::vector<unsigned char> input, std::ofstream & out, int encoding)
{
	std::vector<unsigned short int> * converted;
	switch (encoding) {
		case 1:
			converted = ConvertFromKoi8ToUTF8 (input);
			break;
		case 2:
			converted = ConvertFromWin1251ToUTF8 (input);
			break;
		default:
			return; // �������� ����� �� ����, ������ ��� ������
	}
	for (int i = 0; i < converted->size (); ++i) {
		if ((*converted)[i] <= 0xFF ) { // ������������ == ASCII
			out.put ((char)(*converted)[i]);
		}
		else { // ������������ �������
			out.put ((char)(((*converted)[i] & 0xFF00) >> 8)); // ������ ����
			out.put ((char)(((*converted)[i] & 0x00FF)     )); // ������ ����
		}
	}
	delete converted;
}