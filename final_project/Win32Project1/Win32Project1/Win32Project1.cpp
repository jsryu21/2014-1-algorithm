// Win32Project1.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Win32Project1.h"
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <limits>
#include <stack>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <set>
#include <iterator>
#include <fstream>
#include <string>
#include <sstream>

#define MAX_LOADSTRING 100
#define RECT_SIZE 10
#define MAX_POINT_X 150
#define MAX_POINT_Y 150
#define MIN_POINT_X -150
#define MIN_POINT_Y -150
#define X_OFFSET 50
#define Y_OFFSET 50
#define BRUTAL_X_OFFSET X_OFFSET
#define BRUTAL_Y_OFFSET Y_OFFSET
#define GRAHAM_X_OFFSET (X_OFFSET + (MAX_POINT_X + abs(min(MIN_POINT_X, 0))) + X_OFFSET)
#define GRAHAM_Y_OFFSET Y_OFFSET
#define MAX_POINT_COUNT 100

using namespace std;

// ���� ����:
HINSTANCE hInst;								// ���� �ν��Ͻ��Դϴ�.
TCHAR szTitle[MAX_LOADSTRING];					// ���� ǥ���� �ؽ�Ʈ�Դϴ�.
TCHAR szWindowClass[MAX_LOADSTRING];			// �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	// Console ����� �α� ���
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "conout$", "w", stdout); //returns 0
	// input ���� ������ ���� �ʱ�ȭ
	srand(static_cast<unsigned int>(time(NULL)));
	MSG msg;
	HACCEL hAccelTable;

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// �⺻ �޽��� �����Դϴ�.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// Console Stream Close
	if (stream != nullptr)
	{
		fclose(stream);
	}

	return (int)msg.wParam;
}



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WIN32PROJECT1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

struct Point
{
	Point()
	: X(0)
	, Y(0)
	, Angle(0)
	, Distance(0)
	{
	}
	bool operator<(const Point& rhs) const
	{
		if (X < rhs.X)
		{
			return true;
		}
		else if (X > rhs.X)
		{
			return false;
		}
		else
		{
			return Y < rhs.Y;
		}
	}
	bool operator==(const Point& rhs) const
	{
		return X == rhs.X && Y == rhs.Y;
	}
	int X;
	int Y;
	double Angle; // BasePoint ���� ����
	double Distance; // BasePoint ���� �Ÿ�
};

class ConvexHull
{
public:
	ConvexHull()
		: Points(std::vector< Point >())
		, BrutalResult(std::vector< std::pair< Point, Point > >())
		, GrahamResult(std::vector< Point >())
		, BrutalPoints(std::set< Point >())
		, GrahamPoints(std::set< Point >())
		, Count(0)
	{
	}

	void CreateNewInput()
	{
		// �� Input ����
		Count = rand() % (MAX_POINT_COUNT - 3);
		Count += 3;
		Points.clear();
		for (int i = 0; i < Count; ++i)
		{
			Point temp;
			temp.X = rand() % (MAX_POINT_X - MIN_POINT_X);
			temp.Y = rand() % (MAX_POINT_Y - MIN_POINT_Y);
			temp.X += MIN_POINT_X;
			temp.Y += MIN_POINT_Y;
			Points.push_back(temp);
		}
		BrutalResult.clear();
		GrahamResult.clear();
		assert(Points.size() == static_cast<unsigned int>(Count));
		std::cout << "Input Size : " << Count << std::endl;
		for (int i = 0; i < Count; ++i)
		{
			std::cout << i << "	: (" << Points[i].X << ", " << Points[i].Y << ")" << std::endl;
		}
	}

	void PointSetToTextFile(string strFile, std::set< Point > setPoints, double dTime)
	{
		////	Point Set �� File �� Output ����

		ofstream outFile(strFile);
		if (outFile.is_open())
		{
			std::set< Point >::iterator IterBegin = setPoints.begin();
			std::set< Point >::iterator IterEnd = setPoints.end();
			for(std::set< Point >::iterator IterPos = IterBegin; IterPos != IterEnd;
				++IterPos )
			{
				outFile << "<" + to_string(IterPos->X )+ ", " + to_string(IterPos->Y) + ">\n";
			}

			outFile << "Elapsed Time : " + to_string(dTime) + "Clocks";

			outFile.close();
		}
	}

	void PointVectorToTextFile(string strFile, std::vector< Point > vPoints)
	{
		////	Point Vector �� File �� Output ����

		ofstream outFile(strFile);
		if (outFile.is_open())
		{
			std::vector< Point >::iterator IterBegin = vPoints.begin();
			std::vector< Point >::iterator IterEnd = vPoints.end();
			for(std::vector< Point >::iterator IterPos = IterBegin; IterPos != IterEnd;
				++IterPos )
			{
				outFile << "<" + to_string(IterPos->X )+ ", " + to_string(IterPos->Y) + ">\n";
			}

			outFile.close();
		}
	}

	void CreateInputTxt()
	{
		////	�� Input ���� �� ���Ϸ� ���

		PointVectorToTextFile("Input.txt", Points);
	}

	std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		split(s, delim, elems);
		return elems;
	}

	inline std::string trim(std::string& s,const std::string& drop)
	{
		std::string r=s.erase(s.find_last_not_of(drop)+1);
		return r.erase(0,r.find_first_not_of(drop));
	}

	void LoadInputTxt()
	{
		////	Input.txt ���Ͽ��� ���� �о�ͼ� �˰����� ������

		Count = 0;

		string line;
		ifstream inFile ("Input.txt");
		if (inFile.is_open())
		{
			while ( getline (inFile, line) )
			{
				Count++;

				line = trim(line, "<");
				line = trim(line, ">");
				std::vector<std::string> point = split(line, ',');

				std::vector< std::string >::iterator IterBegin = point.begin();
				std::vector< std::string >::iterator IterEnd = IterBegin++;
				
				string x = IterBegin->c_str();
				x = trim(x, " ");

				string y = IterEnd->c_str();
				y = trim(y, " ");

				Point p;
				p.Y = atoi(x.c_str());
				p.X = atoi(y.c_str());

				Points.push_back(p);
			}

			inFile.close();
		}

		BrutalResult.clear();
		GrahamResult.clear();
	}

	void EliminateDuplicationPoints()
	{
		////	������ �ߺ��� ����

		Sort();
		auto last = std::unique(Points.begin(), Points.end());
		Points.erase(last, Points.end());

		Count = Points.size();
	}

	void DoBrutalAlgorithm()
	{
		////	Brutal Algorithm �� ���� (������ �̿��� ���Ѵ�)

		clock_t start_time, end_time;
		start_time = clock();

		for (int i = 0; i < Count; ++i)
		{
			const Point& p = Points[i];

			for (int j = 0; j < Count; ++j)
			{
				if (i == j)
				{
					continue;
				}

				const Point& q = Points[j];

				if (p == q)
					continue;

				bool bSuccess = true;
				short sSign = 0;

				for (int k = 0; k < Count; ++k)
				{
					if (i == k || j == k)
					{
						continue;
					}

					const Point& r = Points[k];

					if (p == r || q == r)
						continue;

					double crossProduct = (p.X - q.X) * (r.Y - q.Y) - (p.Y - q.Y) * (r.X - q.X);

					short sSignCur = 0;

					if (crossProduct > 0)
					{
						sSignCur = 1;
					}
					else if (crossProduct < 0)
					{
						sSignCur = -1;
					}

					if (sSign == 0)
					{
						sSign = sSignCur;
					}
					else if ((sSign > 0 && sSignCur < 0) || (sSign < 0 && sSignCur > 0))
					{
						bSuccess = false;
						break;
					}
				}

				if (bSuccess)
				{
					for (int k = 0; k < Count; ++k)
					{
						////	���� ���� ���� �� ���� �� ���� �̾��ֱ� ���ؼ�

						if (i == k || j == k)
						{
							continue;
						}

						const Point& r = Points[k];

						if (p == r || q == r)
							continue;

						double crossProduct = (p.X - q.X) * (r.Y - q.Y) - (p.Y - q.Y) * (r.X - q.X);

						if (crossProduct == 0)
						{
							double fLengthPQ = sqrt(pow(p.X - q.X, 2) + pow(p.Y - q.Y, 2));
							double fLengthPR = sqrt(pow(r.X - p.X, 2) + pow(r.Y - p.Y, 2));
							double fLengthRQ = sqrt(pow(r.X - q.X, 2) + pow(r.Y - q.Y, 2));

							if (fLengthPQ > fLengthPR && fLengthPQ > fLengthRQ)
							{
								if (BrutalPoints.find(r) != BrutalPoints.end())
									BrutalPoints.erase(r);
							}
							else
							{
								bSuccess = false;
								break;
							}
						}
					}

					if (bSuccess)
					{
						BrutalResult.push_back(std::make_pair(p, q));
						BrutalPoints.insert(p);
						BrutalPoints.insert(q);
					}
				}
			}
		}

		end_time = clock();
		std::cout << "Brutal Algorithm : " << static_cast<double>((end_time - start_time)) << " clocks" << std::endl;

		////	���Ϸ� ����

		PointSetToTextFile("Output_Brutal.txt", BrutalPoints, static_cast<double>((end_time - start_time)));
	}
	void DoGrahamAlgorithm()
	{
		////	Graham Algorhtim �� �����Ѵ�

		clock_t start_time, end_time;
		start_time = clock();

		std::stack< Point > stack;

		////	Sorting �Ͽ� Base �� ���� ��

		Sort();

		stack.push(Points[0]);
		stack.push(Points[1]);

		for (int i = 2; i < Count; i++)
		{
			Point p1 = Points[i];
			Point p2 = stack.top();
			stack.pop();
			Point p3 = stack.top();

			////	CounterClockWise ���� Ȯ���Ͽ� ������ �ȳ����� �����Ѵ�

			int checkPoints = CounterClockwise(p3, p2, p1);

			// counter-clockwise
			if (checkPoints > 0)
			{
				stack.push(p2);
				stack.push(p1);
			}

			// collinear
			if (checkPoints == 0)
				stack.push(p1);

			// clockwise
			else
				i--;
		}

		// end of the hull

		while (stack.empty() == false)
		{
			GrahamResult.push_back(stack.top());
			GrahamPoints.insert(stack.top());
			stack.pop();
		}

		end_time = clock();
		std::cout << "Graham Algorithm : " << static_cast<double>((end_time - start_time)) << " clocks" << std::endl;

		////	���Ϸ� ����Ѵ�

		PointSetToTextFile("Output_Graham.txt", GrahamPoints, static_cast<double>((end_time - start_time)));
	}

	void CheckIdenticalResult()
	{
		////	Brutal �� Graham �� �� ����� �´��� üũ�ϴ� �Լ�

		std::vector< Point > diff;
		std::set_difference(BrutalPoints.begin(), BrutalPoints.end(), GrahamPoints.begin(), GrahamPoints.end(), std::inserter(diff, diff.begin()));
		if (diff.size() == 0 && BrutalPoints.size() == GrahamPoints.size())
		{
			std::cout << "Result is identical." << std::endl;
		}
		else
		{
			std::vector< Point >::iterator IterBegin = diff.begin();
			std::vector< Point >::iterator IterEnd = diff.end();
			for(std::vector< Point >::iterator IterPos = IterBegin; IterPos != diff.end();
				++IterPos )
			{
				std::cout << "	: (" << IterPos->X << ", " << IterPos->Y << ")" << std::endl;
			}

			std::cout << "Result is not identical." << std::endl;
		}
	}
private:
	void Sort()
	{
		////	Sorting �ϴµ�, Graham ���� MinX, MinY �� ���� ���� ���� Base ������ ���, �������� Base �κ����� ����, �Ÿ� ������ �����Ѵ�

		int nMinX = INT_MAX;
		int nMinY = INT_MAX;
		int nResIdx = 0;

		for (int i = 0; i < Count; i++)
		{
			if (nMinY > Points[i].Y)
			{
				nResIdx = i;
				nMinX = Points[i].X;
				nMinY = Points[i].Y;
			}
			else if (nMinY == Points[i].Y)
			{
				if (nMinX > Points[i].X)
				{
					nResIdx = i;
					nMinX = Points[i].X;
				}
			}
		}

		Point BasePoint = Points[nResIdx];

		for (int i = 0; i < Count; ++i)
		{
			if (Points[i] == BasePoint)
			{
				Points[i].Angle = std::numeric_limits<double>::lowest();
				Points[i].Distance = 0;
			}
			else
			{
				Points[i].Angle = atan2(Points[i].Y - BasePoint.Y, Points[i].X - BasePoint.X);
				Points[i].Distance = (Points[i].X - BasePoint.X) * (Points[i].X - BasePoint.X) + (Points[i].Y - BasePoint.Y) * (Points[i].Y - BasePoint.Y);
			}
		}

		std::sort(Points.begin(), Points.end(), [BasePoint, this](Point& p1, Point& p2)
		{
			if (AreSame(p1.Angle, p2.Angle))
			{
				if (AreSame(p1.Distance, p2.Distance))
				{
					return false;
				}
				return p1.Distance < p2.Distance;
			}
			return p1.Angle < p2.Angle;
		});

		//for (int i = 0; i < Count; ++i)
		//{
		//	std::cout << i << "	: (" << Points[i].X << ", " << Points[i].Y << ")	Angle : " << Points[i].Angle << std::endl;
		//}
	}

	int CounterClockwise(const Point& p1, const Point& p2, const Point& p3)
	{
		////	�ݽð�������� Ȯ��

		return (p2.X - p1.X) * (p3.Y - p1.Y) - (p2.Y - p1.Y) * (p3.X - p1.X);
	}

	bool AreSame(double a, double b)
	{
		return fabs(a - b) < DBL_EPSILON;
	}

public:
	std::vector< Point > Points;
	std::vector< std::pair< Point, Point > > BrutalResult;
	std::vector< Point > GrahamResult;
	std::set< Point > BrutalPoints;
	std::set< Point > GrahamPoints;
	int Count;
};

//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND	- ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT	- �� â�� �׸��ϴ�.
//  WM_DESTROY	- ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static ConvexHull* c = nullptr;
	RECT wndRect;
	RECT rect;
	HBRUSH hbrush = CreateSolidBrush(RGB(0, 255, 0));
	TCHAR str1[] = _T("Brutal Algorithm");
	TCHAR str2[] = _T("Graham Algorithm");

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �޴� ������ ���� �м��մϴ�.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_NEW_INPUT:
			if (c != nullptr)
			{
				free(c);
			}
			c = new ConvexHull();
			c->CreateNewInput();
			c->CreateInputTxt();
			c->EliminateDuplicationPoints();
			c->DoBrutalAlgorithm();
			c->DoGrahamAlgorithm();
			c->CheckIdenticalResult();
			InvalidateRect(hWnd, NULL, true);
			break;
		case ID_LOAD_INPUT:
			if (c != nullptr)
			{
				free(c);
			}
			c = new ConvexHull();
			c->LoadInputTxt();
			c->EliminateDuplicationPoints();
			c->DoBrutalAlgorithm();
			c->DoGrahamAlgorithm();
			c->CheckIdenticalResult();
			InvalidateRect(hWnd, NULL, true);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
		GetClientRect(hWnd, &wndRect);
		if (c != nullptr)
		{
			//Draw Brutal Algorithm
			for (int i = 0; i < c->Count; i++)
			{
				rect.left = c->Points[i].X + BRUTAL_X_OFFSET + abs(min(MIN_POINT_X, 0));
				rect.right = rect.left + RECT_SIZE;
				rect.top = wndRect.bottom - (c->Points[i].Y + BRUTAL_Y_OFFSET + abs(min(MIN_POINT_Y, 0)));
				rect.bottom = rect.top + RECT_SIZE;
				FillRect(hdc, &rect, hbrush);
			}
			MoveToEx(hdc, BRUTAL_X_OFFSET + abs(min(MIN_POINT_X, 0)), wndRect.bottom - BRUTAL_Y_OFFSET, NULL);
			LineTo(hdc, BRUTAL_X_OFFSET + abs(min(MIN_POINT_X, 0)), wndRect.bottom - (BRUTAL_Y_OFFSET + (MAX_POINT_Y - MIN_POINT_Y)));
			MoveToEx(hdc, BRUTAL_X_OFFSET, wndRect.bottom - (BRUTAL_Y_OFFSET + abs(min(MIN_POINT_Y, 0))), NULL);
			LineTo(hdc, BRUTAL_X_OFFSET + (MAX_POINT_X - MIN_POINT_X), wndRect.bottom - (BRUTAL_Y_OFFSET + abs(min(MIN_POINT_Y, 0))));
			for (auto it = c->BrutalResult.cbegin(); it != c->BrutalResult.cend(); ++it)
			{
				MoveToEx(hdc, it->first.X + BRUTAL_X_OFFSET + abs(min(MIN_POINT_X, 0)), wndRect.bottom - (it->first.Y + BRUTAL_Y_OFFSET + abs(min(MIN_POINT_Y, 0))), NULL);
				LineTo(hdc, it->second.X + BRUTAL_X_OFFSET + abs(min(MIN_POINT_X, 0)), wndRect.bottom - (it->second.Y + BRUTAL_Y_OFFSET + abs(min(MIN_POINT_Y, 0))));
			}
			//Draw Graham Scan Algorithm
			for (int i = 0; i < c->Count; i++)
			{
				rect.left = c->Points[i].X + GRAHAM_X_OFFSET + abs(min(MIN_POINT_X, 0));
				rect.right = rect.left + RECT_SIZE;
				rect.top = wndRect.bottom - (c->Points[i].Y + GRAHAM_Y_OFFSET + abs(min(MIN_POINT_Y, 0)));
				rect.bottom = rect.top + RECT_SIZE;
				FillRect(hdc, &rect, hbrush);
			}
			MoveToEx(hdc, GRAHAM_X_OFFSET + abs(min(MIN_POINT_X, 0)), wndRect.bottom - GRAHAM_Y_OFFSET, NULL);
			LineTo(hdc, GRAHAM_X_OFFSET + abs(min(MIN_POINT_X, 0)), wndRect.bottom - (GRAHAM_Y_OFFSET + (MAX_POINT_Y - MIN_POINT_Y)));
			MoveToEx(hdc, GRAHAM_X_OFFSET, wndRect.bottom - (GRAHAM_Y_OFFSET + abs(min(MIN_POINT_Y, 0))), NULL);
			LineTo(hdc, GRAHAM_X_OFFSET + (MAX_POINT_X - MIN_POINT_X), wndRect.bottom - (GRAHAM_Y_OFFSET + abs(min(MIN_POINT_Y, 0))));
			int size = c->GrahamResult.size();
			for (int i = 0; i < size; i++)
			{
				const Point& p1 = c->GrahamResult[i];
				const Point& p2 = c->GrahamResult[(i + 1) % size];
				MoveToEx(hdc, p1.X + GRAHAM_X_OFFSET + abs(min(MIN_POINT_X, 0)), wndRect.bottom - (p1.Y + GRAHAM_Y_OFFSET + abs(min(MIN_POINT_Y, 0))), NULL);
				LineTo(hdc, p2.X + GRAHAM_X_OFFSET + abs(min(MIN_POINT_X, 0)), wndRect.bottom - (p2.Y + GRAHAM_Y_OFFSET + abs(min(MIN_POINT_Y, 0))));
			}
		}

		TextOut(hdc,
			BRUTAL_X_OFFSET + 90, BRUTAL_Y_OFFSET + 250 ,
			str1, _tcslen(str1));

		TextOut(hdc,
			GRAHAM_X_OFFSET + 90, GRAHAM_Y_OFFSET + 250 ,
			str2, _tcslen(str2));

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}