#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <Windows.h>
using namespace cv;
using namespace std;

//地圖大小
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
// 檢查遊戲勝負
int checkGameResult()
{
	// 判斷是否有連線
	for (int i = 0; i < 3; i++)
	{
		// 三條 直
		if (board[i] == board[i + 3] &&
			board[i + 3] == board[i + 6] &&
			board[i] != 0)
			return board[i];
		// 三條 橫
		if (board[i * 3] == board[i * 3 + 1] &&
			board[i * 3 + 1] == board[i * 3 + 2] &&
			board[i * 3] != 0)
			return board[i * 3];
	}
	// 兩條 斜線
	if ((board[4] == board[0] && board[4] == board[8] && board[4] != 0) ||
		 board[4] == board[2] && board[4] == board[6] && board[4] != 0)
		return board[4];
	// 平手
	if ((board[0] != 0) && (board[1] != 0) && (board[2] != 0) && (board[3] != 0) && (board[4] != 0) &&
		(board[5] != 0) && (board[6] != 0) && (board[7] != 0) && (board[8] != 0))
		return 3;
	// 遊戲未結束
	return 0;
}

// 繪製玩家棋子
void paintShape(int player, int board_num)
{
	int pos_x = 100 + 200 * ((board_num) % 3);
	int pos_y = 100 + 200 * ((board_num ) / 3);
	// 畫 O
	if (player == 1)
	{
		circle(img, Point(pos_x, pos_y), 50, Scalar(255, 0, 0), -1);
		circle(img, Point(pos_x, pos_y), 40, Scalar(255, 255, 255), -1);
	}
	// 畫 X
	else if (player == 2)
	{
		line(img, Point(pos_x - 50, pos_y - 50), Point(pos_x + 50, pos_y + 50), Scalar(0, 255, 0), 3);
		line(img, Point(pos_x + 50, pos_y - 50), Point(pos_x - 50, pos_y + 50), Scalar(0, 255, 0), 3);
	}
}

int minimax(int board[], int player, int depth)
{
	// X的一方為 Max 、 Max結果越大對 X 越好
	// O的一方為 Min 、 Min結果越小對 O 越好
	int maxNum = 1;
	int minNum = -1;
	// 檢查勝負
	int MINMAX = checkGameResult();
	// 當O贏 局面為 -1 分
	if (MINMAX == 1)
		return -1;
	// 當X贏 局面為 1 分
	else if (MINMAX == 2)
		return 1;
	// 沒有結果、平局 局面為 0 分
	else if(MINMAX == 3)
		return 0;

	// 開始對空格進行填入
	int result;
	for (int i = 0; i < 9; i++)
	{
		if (board[i] == 0)
		{
			// 玩家為 X
			if (player == 2)
			{
				// 填入 X
				board[i] = 2;
				// 輪到玩家 O (depth往下一層)
				result = minimax(board, 1, depth + 1);
				// 改回空格
				board[i] = 0;
				// 結果大於-1的話存到 minNum
				if (result > minNum)
				{
					minNum = result;
					// 將 X 最後填入的格子存到 posmin
					if (depth == 0)
						posmin = i;
				}
			}
			// 玩家為 O
			else if (player == 1)
			{
				// 填入 O
				board[i] = 1;
				// 輪到玩家 X (depth往下一層)
				result = minimax(board, 2, depth + 1);
				// 改回空格
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
	// 判斷勝負
	int result = checkGameResult();
	// 更改 X 為玩家或電腦
	if (event == EVENT_RBUTTONDOWN)
		mode = !mode;
	// 遊戲未結束
	if (result == 0)
	{
		// 換玩家下
		if (event == EVENT_LBUTTONDOWN)
		{
			//選擇板子
			for (int i = 0; i < 9; i++)
			{
				// 上下左右邊界
				int lx, rx, ly, ry;
				lx = 200 * (i % 3);
				ly = 200 * (i / 3);
				rx = 200 + 200 * (i % 3);
				ry = 200 + 200 * (i / 3);
				// 判斷是否在邊界內
				if (x > lx && x<rx && y>ly && y < ry && board[i] == 0)
				{
					board[i] = (GameRound % 2) + 1;
					GameRound++;
				}
			}
		}
		// 換電腦下
		else if (((GameRound % 2) + 1) == 2 && mode == true)
		{
			posmax = -1;
			posmin = -1;
			minimax(board, 2, 0);
			board[posmin] = (GameRound % 2) + 1;
			GameRound++;
		}
	}


	// 繪製玩家棋子
	for (int i = 0; i < 9; i++)
	{
		if (board[i] != 0)
			paintShape(board[i], i);
	}
	// 繪製垂直線
	line(img, Point(range / 3, 0), Point(range / 3, range), Scalar(0, 0, 0), 3);
	line(img, Point(range / 3 * 2, 0), Point(range / 3 * 2, range), Scalar(0, 0, 0), 3);
	// 繪製水平線
	line(img, Point(0, range / 3), Point(range, range / 3), Scalar(0, 0, 0), 3);
	line(img, Point(0, range / 3 * 2), Point(range, range / 3 * 2), Scalar(0, 0, 0), 3);

	// 顯示輪到誰、贏家資訊
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

	// 顯示回合資訊
	sprintf(inform, "Round=%d", GameRound);
	putText(img, inform, Point(650, 200), 0, 1, Scalar(0, 0, 0), 3);

	// 顯示玩家資訊
	sprintf(inform, "O = player1");
	putText(img, inform, Point(650, 300), 0, 1, Scalar(0, 0, 0), 3);
	if(mode) sprintf(inform, "X = computer");
	else sprintf(inform, "X = player2");
	putText(img, inform, Point(650, 400), 0, 1, Scalar(0, 0, 0), 3);

	// 重新開始
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
