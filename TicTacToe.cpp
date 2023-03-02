#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Windows.h>
using namespace cv;
using namespace std;

//�a�Ϥj�p
int range = 600;
Mat img(range, range + 300, CV_8UC3, Scalar(255, 255, 255));


int board[9] = { 0,0,0,0,0,0,0,0,0 };
int GameRound = 0;
bool mode = true;
int posmax, posmin;

void init() 
{
	for (int i = 0; i < 9; i++)
		board[i] = 0;
	GameRound = 0;
}
// �ˬd�C���ӭt
int checkGameResult()
{
	// �P�_�O�_���s�u
	for (int i = 0; i < 3; i++)
	{
		// �T�� ��
		if (board[i] == board[i + 3] &&
			board[i + 3] == board[i + 6] &&
			board[i] != 0)
			return board[i];
		// �T�� ��
		if (board[i * 3] == board[i * 3 + 1] &&
			board[i * 3 + 1] == board[i * 3 + 2] &&
			board[i * 3] != 0)
			return board[i * 3];
	}
	// ��� �׽u
	if ((board[4] == board[0] && board[4] == board[8] && board[4] != 0) ||
		 board[4] == board[2] && board[4] == board[6] && board[4] != 0)
		return board[4];
	// ����
	if ((board[0] != 0) && (board[1] != 0) && (board[2] != 0) && (board[3] != 0) && (board[4] != 0) &&
		(board[5] != 0) && (board[6] != 0) && (board[7] != 0) && (board[8] != 0))
		return 3;
	// �C��������
	return 0;
}

// ø�s���a�Ѥl
void paintShape(int player, int board_num)
{
	int pos_x = 100 + 200 * ((board_num) % 3);
	int pos_y = 100 + 200 * ((board_num ) / 3);
	// �e O
	if (player == 1)
	{
		circle(img, Point(pos_x, pos_y), 50, Scalar(255, 0, 0), -1);
		circle(img, Point(pos_x, pos_y), 40, Scalar(255, 255, 255), -1);
	}
	// �e X
	else if (player == 2)
	{
		line(img, Point(pos_x - 50, pos_y - 50), Point(pos_x + 50, pos_y + 50), Scalar(0, 255, 0), 3);
		line(img, Point(pos_x + 50, pos_y - 50), Point(pos_x - 50, pos_y + 50), Scalar(0, 255, 0), 3);
	}
}

int minimax(int board[], int player, int depth)
{
	// X���@�謰 Max �B Max���G�V�j�� X �V�n
	// O���@�謰 Min �B Min���G�V�p�� O �V�n
	int maxNum = 1;
	int minNum = -1;
	// �ˬd�ӭt
	int MINMAX = checkGameResult();
	// ��OĹ ������ -1 ��
	if (MINMAX == 1)
		return -1;
	// ��XĹ ������ 1 ��
	else if (MINMAX == 2)
		return 1;
	// �S�����G�B���� ������ 0 ��
	else if(MINMAX == 3)
		return 0;

	// �}�l��Ů�i���J
	int result;
	for (int i = 0; i < 9; i++)
	{
		if (board[i] == 0)
		{
			// ���a�� X
			if (player == 2)
			{
				// ��J X
				board[i] = 2;
				// ���쪱�a O (depth���U�@�h)
				result = minimax(board, 1, depth + 1);
				// ��^�Ů�
				board[i] = 0;
				// ���G�j��-1���ܦs�� minNum
				if (result > minNum)
				{
					minNum = result;
					// �N X �̫��J����l�s�� posmin
					if (depth == 0)
						posmin = i;
				}
			}
			// ���a�� O
			else if (player == 1)
			{
				// ��J O
				board[i] = 1;
				// ���쪱�a X (depth���U�@�h)
				result = minimax(board, 2, depth + 1);
				// ��^�Ů�
				board[i] = 0;
				if (result < maxNum)
				{
					maxNum = result;
					if (depth == 0)
						posmax = i;
				}
			}
		}
	}
	if (player == 1)
		return maxNum;
	else
		return minNum;
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	img.setTo(Scalar(255, 255, 255));
	// �P�_�ӭt
	int result = checkGameResult();
	// ��� X �����a�ιq��
	if (event == EVENT_RBUTTONDOWN)
		mode = !mode;
	// �C��������
	if (result == 0)
	{
		// �����a�U
		if (event == EVENT_LBUTTONDOWN)
		{
			//��ܪO�l
			for (int i = 0; i < 9; i++)
			{
				// �W�U���k���
				int lx, rx, ly, ry;
				lx = 200 * (i % 3);
				ly = 200 * (i / 3);
				rx = 200 + 200 * (i % 3);
				ry = 200 + 200 * (i / 3);
				// �P�_�O�_�b��ɤ�
				if (x > lx && x<rx && y>ly && y < ry && board[i] == 0)
				{
					board[i] = (GameRound % 2) + 1;
					GameRound++;
				}
			}
		}
		// ���q���U
		else if (((GameRound % 2) + 1) == 2 && mode == true)
		{
			posmax = -1;
			posmin = -1;
			minimax(board, 2, 0);
			board[posmin] = (GameRound % 2) + 1;
			GameRound++;
		}
	}


	// ø�s���a�Ѥl
	for (int i = 0; i < 9; i++)
	{
		if (board[i] != 0)
			paintShape(board[i], i);
	}
	// ø�s�����u
	line(img, Point(range / 3, 0), Point(range / 3, range), Scalar(0, 0, 0), 3);
	line(img, Point(range / 3 * 2, 0), Point(range / 3 * 2, range), Scalar(0, 0, 0), 3);
	// ø�s�����u
	line(img, Point(0, range / 3), Point(range, range / 3), Scalar(0, 0, 0), 3);
	line(img, Point(0, range / 3 * 2), Point(range, range / 3 * 2), Scalar(0, 0, 0), 3);

	// ��ܽ���֡BĹ�a��T
	char inform[1024];
	if (result == 1)
		sprintf(inform, "O WIN!!");
	else if (result == 2)
		sprintf(inform, "X WIN!!");
	else if (result == 3)
		sprintf(inform, "NO WINNER");
	else if ((GameRound % 2) + 1 == 1)
		sprintf(inform, "O's turn");
	else
		sprintf(inform, "X's turn");
	putText(img, inform, Point(650, 100), 0, 1, Scalar(0, 0, 0), 3);

	// ��ܦ^�X��T
	sprintf(inform, "Round=%d", GameRound);
	putText(img, inform, Point(650, 200), 0, 1, Scalar(0, 0, 0), 3);

	// ��ܪ��a��T
	sprintf(inform, "O = player1");
	putText(img, inform, Point(650, 300), 0, 1, Scalar(0, 0, 0), 3);
	if(mode) sprintf(inform, "X = computer");
	else sprintf(inform, "X = player2");
	putText(img, inform, Point(650, 400), 0, 1, Scalar(0, 0, 0), 3);

	// ���s�}�l
	sprintf(inform, "RESTART");
	int left = 640, right = 790, top = 470, bottom = 510;
	line(img, Point(left, bottom), Point(right, bottom), Scalar(0, 0, 0), 3);
	line(img, Point(left, top), Point(right, top), Scalar(0, 0, 0), 3);
	line(img, Point(left, top), Point(left, bottom), Scalar(0, 0, 0), 3);
	line(img, Point(right, top), Point(right, bottom), Scalar(0, 0, 0), 3);
	putText(img, inform, Point(650, 500), 0, 1, Scalar(0, 0, 0), 3);
	if(event == EVENT_LBUTTONDOWN)
	{
		if (x > left && x<right && y>top && y < bottom)
			init();
	}
	imshow("TicTacToe Game", img);
}

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE);

	//Create a window
	namedWindow("TicTacToe Game");
	//set the callback function for any mouse event
	setMouseCallback("TicTacToe Game", CallBackFunc, NULL);

	waitKey(0);
	return 0;
}
