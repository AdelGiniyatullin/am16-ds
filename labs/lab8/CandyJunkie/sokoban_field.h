#pragma once
#include <iostream>
#include <fstream>
#include <string>

#define MAX_SIZE 10

enum MOVE {
	RIGHT, UP, LEFT, DOWN, MOVE_MAX
};

enum POS {
	X, Y, POS_MAX
};

enum LEGEND {
	EMPTY, WALL, PLAYER, BOX, PLACE, PLAYER_ON_PLACE, BOX_ON_PLACE
};

class sokoban_field {
	static int width; // ������ <= 10
	static int height; // ������ ���� <= 10. 
	static LEGEND ** field;
	int player_x;
	int player_y;
	static int box_amount;
	int ** box;
	sokoban_field * parent;
	sokoban_field * childs[MOVE_MAX]; 
	static bool older_steps[MAX_SIZE][MAX_SIZE][MAX_SIZE][MAX_SIZE][MAX_SIZE][MAX_SIZE][MAX_SIZE][MAX_SIZE]; // ��� ������������. ������ ��� ������������.
public:
	sokoban_field ();
	/* ---------------------------------------------------------------------
	 * ������ �����: 2 ����� ����� ������, �������� ������ ����: ���-�� ����� � ��������. (��� <= 10)
	 * �� ��������� ������ ��������� �������� ������ ����:
	 * @ - �����.
	 * # - �����.
	 * _ - ����� ��� ����
	 * 0 - ����
	 * ! - ����� �� ����� ��� ����
	 * 1 - ���� �� ����� ��� ����
	 * ������ ������ ���� ������, ��� ���� ��� ���.
	 * ������� ��������� ���������� ���� ������ �� ����� ��� �����.
	 * ���� ��� �� ��������� � ������������� ��������� ��������, �� � ��� ��������.
	 * --------------------------------------------------------------------*/
	sokoban_field (std::ifstream & in);
	sokoban_field (const sokoban_field & prev, MOVE move);
	~sokoban_field ();
	bool can_move (MOVE move) const;
	bool is_finish () const;
	bool check_older (int newpos_x, int newpos_y, int boxnum, MOVE move) const;
	sokoban_field * go (MOVE move);
	void print_from_root ();
};

sokoban_field::sokoban_field ()
{
	player_x = -1;
	player_y = -1;
	box = new int * [3]; // �������� ������� 3. 4 ����� - �� �������.
	for (int i = 0; i < 3; ++i) {
		box[i] = new int [2];
		box[i][0] = 0;
		box[i][1] = 0;
	}
	parent = NULL;
	for (int m = RIGHT; m < MOVE_MAX; ++m) {
		childs[(MOVE)m] = NULL;
	}
}

#include "sokoban_field.tpp"