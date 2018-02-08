#include "stdio.h" 
#include <fstream> 
#include <iostream> 
#include <string> 
#include <conio.h>
using namespace std;

// �� float � ��������� �������������
// ������ float � int �� 4 �����,�.�. �����
// �������������� ���������� ���������� ���� float ��� ���������� ���������� ���� int
// ����� ������ ��������� �������� "������� �" - & - � ��������� ��� �������� � ������� �����

void BinaryToFloat(bool mas[32],bool flag);

void FloatToBinary(float var, bool flag){
	float *p = &var;
	long *number = (long*)p;
	bool *mas = new bool[32];
	for (int i = 31; i >= 0; i--){
		long mask = 1 << i;
		if ((mask & *number) == 0)
			mas[31 - i] = 0;
		else
			mas[31 - i] = 1;
	}
	// ����� ����������� �������
	for (int i = 0; i < 32; i++){
		if ((i == 1) || (i == 9)) cout << "|";
		cout << mas[i];
	}
	if (flag == 0) return;
	int choise;
	cout << endl << "������ ���������� �������� �������� � �����������? " << endl << "���� ��,������� 1" << endl << "��� ������ � ������� ���� ������� ���-������"<<endl<<"��� ������ �� ��������� ������� enter" << endl;
	choise = _getch();
	switch (choise){
		case '1':{
		BinaryToFloat(mas,0); 
		system("pause");
		break;
	}
	case 13:exit(0);
	default:break;
	}
}
// �� ��������� ������������� � float 
void BinaryToFloat(bool mas[32],bool flag){
	int mask = 0;
	for (int i = 0; i<32; i++) {
		mask <<= 1;
		mask |= mas[i];
	}
	int *_flt = &mask;
	float *flt = (float*)_flt;
	cout << *flt << endl;
	if (flag == 0) return;
	int choise;
	cout << "������ ���������� �������� �������� � �����������? " << endl << "���� ��,������� 1" << endl << "��� ������ � ������� ���� ������� ���-������" << endl << "��� ������ �� ��������� ������� enter" << endl;
	choise = _getch();
	switch (choise){
	case '1':{
		FloatToBinary(*flt,0);
		system("pause");
		break;
	}
	case 13:exit(0);
	default:break;
	}
}
void main()
{	
	setlocale(LC_ALL, "Russian");
	float var;	
	int choise=0;
	while (true){
		cout << "�������� ���� �� ������������ ���������:" << endl << "1) �������� �������� ������������� ����� ���� float" << endl << "2) �������� ����� ���� float �� ��� ��������� �������������" << endl<<"��� ������ ������� enter"<<endl;
		choise=_getch();		
		switch (choise){
		case '1':{
			cout << "������� ���������� ���� float:" << endl;
			cin >> var;
			FloatToBinary(var,1);
			break;
		}
		case '2':{
			bool mas[32];
			cout << "������� �������� �������������" << endl;
			string bin;
			cin >> bin;
			if (bin.length() < 32){
				cout << "���� �������� ������������� �� ������" << endl;
				break;
			}
			for (int i = 0; i < 32; i++) mas[i] = bin[i]-'0';
			BinaryToFloat(mas,1);
			break;
		}
		case 13: exit(0); break;
		default: {
			cout << "�� ������� �������������� �������,���������� ��� ���" << endl;
			break;
		}
		}	
		system("cls");
	}	
	return;
}