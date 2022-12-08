#include <map>
#include <queue>
#include <ctime>
#include <cstdio>
#include <random>
#include <utility>
#include <windows.h>
#include <algorithm>
using namespace std;
mt19937 myrand(time(0)), game_data(202105200);
const int Goal = 12345678;
const int wx[] = {-1, 0, 1, 0}, wy[] = {0, 1, 0, -1};
int Row, Col, counter_id;
int lev, rate, playing, tot_step;
int grade[1060], counter_num[10], num[15], Operation[50];
int g[5][5];
bool Lock[1060];

struct Game
{
	int data, step;
	unsigned long long key;
	bool operator < ( const Game & tmp) const
	{
		return step < tmp.step;
	}
}game[1060];

pair <int, int> Init_graph(int X)
{
	pair <int, int> tmp;
	for (int i = 2; i >= 0; --i)
	{
		for (int j = 2; j >= 0; --j)
		{
			g[i][j] = X % 10;
			X /= 10;
			if (!g[i][j])
				tmp = make_pair(i, j);
		}
	}
	return tmp;
}

void Move(short y, short x)
{
	HANDLE place;
	place = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(place, pos);
}

namespace Press
{
	bool Ctrl()
	{ return (GetKeyState(VK_CONTROL)&0x8000); }
	
	bool Alt()
	{ return (GetKeyState(VK_MENU)&0x8000); }
	
	bool Shitt()
	{ return (GetKeyState(VK_SHIFT)&0x8000); }
	
	bool Left()
	{ return (GetKeyState(VK_LEFT)&0x8000); }
	
	bool Right()
	{ return (GetKeyState(VK_RIGHT)&0x8000); }
	
	bool Up()
	{ return (GetKeyState(VK_UP)&0x8000); }
	
	bool Down()
	{ return (GetKeyState(VK_DOWN)&0x8000); }
	
	bool Esc()
	{ return (GetKeyState(27) < 0); }
	
	bool Enter()
	{ return (GetKeyState(13) < 0); }
}

namespace Set_color
{
	void Init()
	{ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); }
	
	void Red()
	{ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED); }
	
	void Red_back()
	{ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_RED); }
	
	void Blue_back()
	{ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_BLUE); }
	
	void Blue()
	{ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE); }
	
	void Green()
	{ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN); }
	
	void Green_back()
	{ SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),BACKGROUND_GREEN); }
	
	void Yellow()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	}
	
	void Yellow_blue()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_BLUE);
	}
	
	void Purple()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
	}
	
	void Purple_back()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE);
	}
	
	void Cyan()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	
	void Cyan_back()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
		BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE);
	}
}

string Num_str(int x)
{
	if (x == 0) return "";
	if (x == 1) return "壹 1";
	if (x == 2) return "贰 2";
	if (x == 3) return "叁 3";
	if (x == 4) return "肆 4";
	if (x == 5) return "伍 5";
	if (x == 6) return "陆 6";
	if (x == 7) return "柒 7";
	if (x == 8) return "捌 8";
	return "玖";
}
void Print_game_board()
{
	system("mode con cols=60 lines=22");
	Move(0, 0);
	Set_color :: Cyan();
	printf("+=========================================================+\n");
	printf("+                                                         +\n");
	printf("+=========================================================+\n");
	printf("+                                +       ↑↓←→把某个方 +\n");
	printf("+                                +   块往对应方向移动或将 +\n");
	printf("+  ############################# +   题解翻页 <题解模式>。+\n");
	printf("+  ##       ##       ##       ## +========================+\n");
	printf("+  ##       ##       ##       ## +  Esc 退出   Ctrl 重置  +\n");
	printf("+  ##       ##       ##       ## +========================+\n");
	printf("+  ############################# +         题解说明       +\n");
	printf("+  ##       ##       ##       ## +   您可以在题库页面选   +\n");
	printf("+  ##       ##       ##       ## +   择对应关卡并按下Alt  +\n");
	printf("+  ##       ##       ##       ## +   键锁定关卡并查看题   +\n");
	printf("+  ############################# +   解，锁定后，您无法   +\n");
	printf("+  ##       ##       ##       ## +   刷新对应关卡成绩。   +\n");
	printf("+  ##       ##       ##       ## +========================+\n");
	printf("+  ##       ##       ##       ## +目    1 2  +  Std   %2d  +\n", game[playing].step);
	printf("+  ############################# +标: 3 4 5  +  Step %3d  +\n", tot_step);
	printf("+                                +    6 7 8  +            +\n");
	printf("+=========================================================+\n");
	printf(" 注意：只能把方块移动到空旷的区域，选择的箭头表示移动的方向 "); 
	Move(1, 15);
	Set_color :: Yellow();
	printf("数字华容道 Digit Puzzle");
	Move(3, 15);
	Set_color :: Purple();
	printf("第 %d 关", playing);
	int tmp = 0;
	Move(18, 5);
	Set_color :: Yellow();
	printf("Developed by zhaoxi zheng");
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
		{
			const int nx = 7 + 4 * i, ny = 6 + 9 * j;
			if (g[i][j] == i * 3 + j)
			{
				++tmp;
				Set_color :: Green_back();
				for (int k = -1; k <= 1; ++k)
				{
					Move(nx + k, ny - 2);
					printf("        ");
				}
			}
			Move(nx, ny);
			Set_color :: Yellow();
			printf("%s", Num_str(g[i][j]).c_str());
		}
	Move(4, 5);
	printf("完成度 ");
	for (int i = 1; i <= tmp; ++i)
		printf("★");
}

void Solution(int l, unsigned long long tmp)
{
	tot_step = 0;
	int k = game[l].step;
	pair <int, int> tmp2 = Init_graph(game[l].data);
	int lstx = tmp2.first, lsty = tmp2.second, it = 0;
	int x = lstx, y = lsty;
	for (int i = k; i >= 1 ; --i)
	{
		Operation[i] = tmp % 4;
		tmp >>= 2ull;
	}
	Print_game_board();
	while (it <= k)
	{
		if (Press :: Right())
		{
			++it;
			++tot_step;
			x = lstx + wx[Operation[it]];
			y = lsty + wy[Operation[it]];
			swap(g[x][y], g[lstx][lsty]);
			lstx = x;
			lsty = y;
			Print_game_board();
			Sleep(100);
		}
		if (Press :: Left() && it > 0)
		{
			--tot_step;
			x = lstx - wx[Operation[it]];
			y = lsty - wy[Operation[it]];
			swap(g[x][y], g[lstx][lsty]);
			--it;
			lstx = x;
			lsty = y;
			Print_game_board();
			Sleep(100);
		}
	}
}

namespace Initialize_game
{
	int cnt = 0;
	int op[50];
	map <int, int> dis;
	map <int, unsigned long long> ans;
	map <int, bool> vis;
	queue <int> q;
	void BFS(int X)
	{
		dis[X] = 1;
		q.push(X);
		while (!q.empty())
		{
			int node = q.front(), tmp = node; q.pop();
			pair <int, int> tmp2 = Init_graph(tmp);
			int x = tmp2.first, y = tmp2.second;
			for (int i = 0; i < 4; ++i)
			{
				int nx = x + wx[i], ny = y + wy[i], to = 0;
				if (nx < 0 || nx > 2 || ny < 0 || ny > 2) continue;
				swap(g[x][y], g[nx][ny]);
				for (int i = 0; i < 3; ++i)
					for (int j = 0; j < 3; ++j)
						to = (to << 3) + (to << 1) + g[i][j];
				if (dis[to] == 0)
				{
					dis[to] = dis[node] + 1;
					ans[to] = (ans[node] << 2ull) + (unsigned long long)i;
					q.push(to);	
				}
				swap(g[nx][ny], g[x][y]);
			}
		}
	}
	int Transfer(int X)
	{
		if (X == 0) return 2;
		if (X == 1) return 3;
		if (X == 2) return 0;
		return 1;
	}
	void Get_data()
	{
		for (int i = 0; i <= 8; ++i)
			num[i] = i;
		BFS(Goal);
		while (cnt < 1050)
		{
			int cur = 0;
			shuffle(num, num + 9, game_data);
			for (int i = 8; i >= 0; --i)
				cur = (cur << 3) + (cur << 1) + num[i];
			unsigned long long tmp = ans[cur];
			int k = dis[cur] - 1;
			if (k == -1 || vis[cur]) continue;
			vis[cur] = 1;
			for (int i = 1; i <= k; ++i)
			{
				op[i] = Transfer((int)(tmp % 4ull));
				tmp >>= 2ull;
			}
			tmp = 0ull;
			for (int i = 1; i <= k; ++i)
				tmp = tmp * 4 + (unsigned long long)op[i];
			game[++cnt] = (Game){cur, k, tmp}; 
		}
		sort(game + 1, game + 1051);
	}
}

namespace IO_user_info
{
	const int Hash_mul = 9901U, Hash_mod = 2003U, Hash_xor = 2017U;
	const char FilePath[] = "C:\\ProgramData\\Digit Puzzle.txt";
	int Hash(int x)
	{ return (x + ((int)(myrand() % Hash_mod) + 1) * Hash_mul) ^ Hash_xor; }
	
	int Hash_back(int x)
	{ return (x ^ Hash_xor) % Hash_mul; }
	
	void Get_user_info()
	{
		if (FILE * file = fopen(FilePath, "r"))
		{
			fscanf(file, "%d %d ", &rate, &lev);
			rate = Hash_back(rate);
			lev = Hash_back(lev);
			for (int i = 1; i < lev; ++i)
			{
				fscanf(file, "%d ", &grade[i]);
				grade[i] = Hash_back(grade[i]);
			}
			for (int i = 1; i < lev; ++i)
			{
				int tmp;
				fscanf(file, "%d ", &tmp);
				Lock[i] = (bool)Hash_back(tmp);
			}
			fclose(file);
		}
		else
		{
			rate = 0;
			lev = 1;
		}
	}
	
	void Save_user_info()
	{
		FILE * file = fopen(FilePath, "w");
		fprintf(file, "%d %d ", Hash(rate), Hash(lev));
		for (int i = 1; i < lev; ++i)
			fprintf(file, "%d ", Hash(grade[i]));
		for (int i = 1; i < lev; ++i)
			fprintf(file, "%d ", Hash((int)Lock[i]));
		fclose(file);
	}
}

void To_problem_set();

void Print_lev()
{
	Move(3, 45);
	if (rate <= 10)
	{
		Set_color :: Init();
		printf("Newbie");
	}
	else if (rate <= 30)
	{
		Set_color :: Green();
		printf("Pupil");
	}
	else if (rate <= 50)
	{
		Set_color :: Cyan();
		printf("specialist");	
	}
	else if (rate <= 100)
	{
		Set_color :: Blue();
		printf("Expert");
	}
	else if (rate <= 300)
	{
		Set_color :: Purple();
		printf("Candidate master");	
	}
	else if (rate <= 800)
	{
		Set_color :: Red();
		printf("Master");
	}
	else
	{
		Set_color :: Yellow();
		printf("   Grandmaster  ");
	}
	Row = 8; Col = 35;
	Move(Row, Col);
	Set_color :: Init();
	printf("    Newbie : 0 ~ 10   ");
	Move(++Row, Col);
	Set_color :: Green();
	printf("    Pupil :  11 ~ 30   ");
	Move(++Row, Col);
	Set_color :: Cyan();
	printf(" Specialist : 31 ~ 50 ");
	Move(++Row, Col);
	Set_color :: Blue();
	printf("   Expert : 51 ~ 100    ");
	Move(++Row, Col);
	Set_color :: Purple();
	printf("Candidate master 101 ~300");
	Move(++Row, Col);
	Set_color :: Red();
	printf("   Master : 301 ~ 800   ");
	Move(++Row, Col);
	Set_color :: Yellow();
	printf("  Grandmaster : > 800  ");
}

void Print_home_page()
{
	system("mode con cols=66 lines=20");
	Move(0, 0);
	Set_color :: Cyan();
	printf("+===============================================================+\n");
	printf("+       游戏版本：闯关版        +            用户信息           +\n");
	printf("+  ###########################  +                               +\n");
	printf("+  #       ##       ##       #  + 用户等级                      +\n");
	printf("+  #       ##  壹 1 ##  贰 2 #  +===============================+\n");
	printf("+  #       ##       ##       #  +          用户等级说明         +\n");
	printf("+  ###########################  +    用户等级与用户积分有关：   +\n");
	printf("+  #       ##       ##       #  +===============================+\n");
	printf("+  #  叁 3 ##  肆 4 ##  伍 5 #  +                               +\n");
	printf("+  #       ##       ##       #  +                               +\n");
	printf("+  ###########################  +                               +\n");
	printf("+  #       ##       ##       #  +                               +\n");
	printf("+  #  陆 6 ##  柒 7 ##  捌 8 #  +                               +\n");
	printf("+  #       ##       ##       #  +                               +\n");
	printf("+  ###########################  +                               +\n");
	printf("+===============================+===============================+\n");
	printf("+    数字华容道 Digit Puzzle    +        按下 → 前往题库       +\n");
	printf("+   Developed by zhaoxi zheng   +  press → to the problem set  +\n");
	printf("+===============================================================+\n");
	Move(2, 33);
	Set_color :: Yellow();
	printf(" 闯关数 : %3d", lev - 1);
	Set_color :: Green();
	printf("    得分 : %4d", rate);
	Print_lev();
}

void To_home_page()
{
	system("cls");
	Move(0, 0);
	Print_home_page();
	while (1)
	{
		if (Press :: Esc())
		{
			IO_user_info :: Save_user_info();
			exit(0);
		}
		if (Press :: Right())
		{
			To_problem_set();
			break;
		}
		Sleep(100);
	}
}

void Print_problem_set(int page)
{
	system("cls");
	Move(0, 0);
	Set_color :: Cyan();
	for (int i = 1; i <= 15; ++i)
	{
		for (int j = 1; j <= 113; ++j)
			printf("#");
		printf("\n");
		for (int j = 1; j <= 7; ++j)
		{
			Set_color :: Cyan(); 
			printf("#");
			int tmp = page * 105 + (i - 1) * 7  + j;
			if (Lock[tmp]) Set_color :: Yellow_blue();
			else Set_color :: Yellow();
			printf("%04d|", page * 105 + (i - 1) * 7  + j);
			for (int k = 1; k <= grade[tmp]; ++k)
				printf("★");
			for (int k = grade[tmp] + 1; k <= 5; ++k)
				printf("☆");
		}
		Set_color :: Cyan();
		printf("#\n");
	}
	for (int j = 1; j <= 113; ++j)
		printf("#");
	Set_color :: Init();
}

void Print_counter()
{
	Row = 31; Col = 0;
	Move(Row, Col);
	string s1 = "# ", s2 = "# ", s3 = "# ", s4 = "# ", s5 = "# ";
	for (int i = 0; i <= 4; ++i)
	{
		if (i == 1)
		{
			s1 += "# ";
			s2 += "# ";
			s3 += "# ";
			s4 += "# ";
			s5 += "# ";
		}
		if (counter_num[i] == 0)
		{
			s1 += "%%%%% ";
			s2 += "%   % ";
			s3 += "%   % ";
			s4 += "%   % ";
			s5 += "%%%%% ";
		}
		else if (counter_num[i] == 1)
		{
			s1 += "  %   ";
			s2 += "  %   ";
			s3 += "  %   ";
			s4 += "  %   ";
			s5 += "  %   ";
		}
		else if (counter_num[i] == 2)
		{
			s1 += "%%%%% ";
			s2 += "    % ";
			s3 += "%%%%% ";
			s4 += "%     ";
			s5 += "%%%%% ";
		}
		else if (counter_num[i] == 3)
		{
			s1 += "%%%%% ";
			s2 += "    % ";
			s3 += "%%%%% ";
			s4 += "    % ";
			s5 += "%%%%% ";
		}
		else if (counter_num[i] == 4)
		{
			s1 += "%   % ";
			s2 += "%   % ";
			s3 += "%%%%% ";
			s4 += "    % ";
			s5 += "    % ";
		}
		else if (counter_num[i] == 5)
		{
			s1 += "%%%%% ";
			s2 += "%     ";
			s3 += "%%%%% ";
			s4 += "    % ";
			s5 += "%%%%% ";
		}
		else if (counter_num[i] == 6)
		{
			s1 += "%%%%% ";
			s2 += "%     ";
			s3 += "%%%%% ";
			s4 += "%   % ";
			s5 += "%%%%% ";
		}
		else if (counter_num[i] == 7)
		{
			s1 += "%%%%% ";
			s2 += "    % ";
			s3 += "    % ";
			s4 += "    % ";
			s5 += "    % ";
		}
		else if (counter_num[i] == 8)
		{
			s1 += "%%%%% ";
			s2 += "%   % ";
			s3 += "%%%%% ";
			s4 += "%   % ";
			s5 += "%%%%% ";
		}
		else if (counter_num[i] == 9)
		{
			s1 += "%%%%% ";
			s2 += "%   % ";
			s3 += "%%%%% ";
			s4 += "    % ";
			s5 += "%%%%% ";
		}
	}
	s1 += "# ";
	s2 += "# ";
	s3 += "# ";
	s4 += "# ";
	s5 += "# ";
	Set_color :: Purple();
	for (int i = 1; i <= 35; ++i)
		printf("#");
	printf("\n");
	printf("%s\n%s\n%s\n%s\n%s\n", s1.c_str(), s2.c_str(), s3.c_str(), s4.c_str(), s5.c_str());
	for (int i = 1; i <= 35; ++i)
		printf("#");
	if (counter_id == 0)
		Move(Row + 6, 2);
	else Move(Row + 6, counter_id * 6 + 4);
	Set_color :: Yellow();
	for (int i = 1; i <= 5; ++i)
		printf("#");
	Col = 36;
	Move(Row, 36);
	for (int i = 1; i <= 77; ++i)
		printf("#");
	printf("\n");
	Move(++Row, Col); 
	printf("#           关卡选择器使用方法说明：                       #  Digit Puzzle  #\n");
	Move(++Row, Col); 
	printf("#    第一个数字表示显示关卡页面的编号。                    #   数字华容道   #\n");
	Move(++Row, Col); 
	printf("#     后面四个数字组成的四位数表示关卡编号（前端补零）。   #                #\n");
	Move(++Row, Col); 
	printf("#    ← → 控制光标的移动，↑ ↓ 调节数字的大小。          #  Developed by  #\n");
	Move(++Row, Col); 
	printf("# Esc 退出游戏 Enter 确认选择的关卡  Alt 锁住题目并查看解析#  zhaoxi zheng  #\n");
	Move(++Row, Col); 
	for (int i = 1; i <= 77; ++i)
		printf("#");
	printf("\n");
	Set_color :: Init();
}

bool Finish()
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			if (g[i][j] != i * 3 + j)
				return 0;
	return 1;
}

void Play_game()
{
	int tmp = 0;
	counter_num[0] = lev / 105;
	Print_problem_set(counter_num[0]);
	Print_counter();
	while (true)
	{
		while (true)
		{
			if (Press :: Alt())
			{
				tmp = -3;
				break;	
			}
			if (Press :: Esc())
			{
				system("cls");
				IO_user_info :: Save_user_info();
				exit(0);
			}
			if (Press :: Enter())
			{
				tmp = -2;
				break;
			}
			if (Press :: Left())
			{
				if (!counter_id)
				{
					tmp = -1;
					break;
				}
				--counter_id;
				Print_counter();
			}
			if (Press :: Right())
			{
				if (counter_id != 4)
				{
					++counter_id;
					Print_counter();
				}
			}
			if (Press :: Up())
			{
				counter_num[counter_id] = (counter_num[counter_id] + 1) % 10;
				if (counter_id == 0 && counter_num[counter_id] >= 0 && counter_num[counter_id] < 9)
					Print_problem_set(counter_num[0]);
				Print_counter();
			}
			if (Press :: Down())
			{
				counter_num[counter_id] = (counter_num[counter_id] + 9) % 10;
				if (counter_id == 0 && counter_num[counter_id] >= 0 && counter_num[counter_id] <= 9)
					Print_problem_set(counter_num[0]);
				Print_counter();
			}
			Sleep(100);
		}
		playing = counter_num[1] * 1000 + counter_num[2] * 100 + counter_num[3] * 10 + counter_num[4];
		if (tmp == -1) break;
		if (playing > lev)
		{
			Move(38, 0);
			printf("该关卡未解锁\n");
		}
		else if (playing > 1050 || playing <= 0)
		{
			Move(38, 0);
			printf("该关卡不存在\n");
		}
		else break;
	}
	if (tmp == -2)
	{
		pair <int, int> st = Init_graph(game[playing].data);
		int x = st.first, y = st.second;
		int lstx = x, lsty = y;
		tot_step = 0;
		Print_game_board();
		while (!Finish())
		{
			while (true)
			{
				if (Press :: Down())
					if (x > 0) {--x, ++tot_step; break;}
				if (Press :: Up())
					if (x < 2) {++x, ++tot_step; break;}
				if (Press :: Right())
					if (y > 0) {--y, ++tot_step; break;}
				if (Press :: Left())
					if (y < 2) {++y, ++tot_step; break;}
				if (Press :: Ctrl())
				{
					st = Init_graph(game[playing].data);
					tot_step = 0;
					x = lstx = st.first;
					y = lsty = st.second;
					break;
				}
				if (Press :: Esc())
				{
					To_problem_set();
					return;
				}
			}
			swap(g[x][y], g[lstx][lsty]);
			lstx = x;
			lsty = y;
			Print_game_board();
			Sleep(50);
		}
		system("cls");
		Set_color :: Yellow();
		Move(10, 15);
		printf("-----------------------------");
		Move(11, 15);
		printf("|  恭喜通关，即将返回关卡库 |");
		Move(12, 15);
		printf("-----------------------------");
		Sleep(500);
		Set_color :: Init();
		int point;
		if (tot_step * 4 <= game[playing].step * 5) point = 5;
		else if (tot_step <= game[playing].step * 2) point = 4;
		else if (tot_step <= game[playing].step * 3) point = 3;
		else if (tot_step <= game[playing].step * 5) point = 2;
		else point = 1;
		if (playing == lev) ++lev;
		if (point > grade[playing] && Lock[playing] == 0)
		{
			rate += (point - grade[playing]);
			grade[playing] = point;
		}
		IO_user_info :: Save_user_info();
		To_problem_set();
		return;
	}
	else if (tmp == -3)
	{
		Lock[playing] = 1;
		IO_user_info :: Save_user_info();
		Solution(playing, game[playing].key);
		To_problem_set();
		return;
	}
	else if (tmp == -1) To_home_page();
}

void Print_problem_set();

void To_problem_set()
{
	system("mode con cols=115 lines=40");
	Play_game();
}

void Login()
{
	IO_user_info :: Get_user_info();
	if (Press :: Esc())
	{
		IO_user_info :: Save_user_info();
		return;
	}
	To_home_page();
}

void Hide()
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0}; 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

int main()
{
	Initialize_game :: Get_data();
	Hide();
	Login();
	return 0;
}
