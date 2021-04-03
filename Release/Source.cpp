#include <iostream>
#include <string>
#include <fstream> 

using namespace std;

#define nouth 0
#define west  1
#define south 2
#define east  3
#define turnleft(dir) ((dir+1)%4)
#define turnright(dir) ((dir+3)%4)

ofstream outfile("output.txt");

class grid
{
public:
	grid(int x = 0, int y = 0) : X(x), Y(y), next(0) {
		bwall[0] = 0; bwall[1] = 0; bwall[2] = 0; bwall[3] = 0;
	}
	bool bwall[4];
	grid* next;
	int X, Y;
};

string inttostring_bytarget(int num, string target)
{
	string ret = "";
	int B = target.length();
	if (num == 0) return "0";
	while (num > 0)
	{
		ret = target[num % B] + ret;
		num /= B;
	}
	return ret;
}

void drawmap(grid* map, int row, int columns)
{
	int i = 0;
	int maplen = row * columns;
	while (i < maplen)
	{
		if ((i % columns == 0) && (i > 0))
		{
			cout << endl;
			outfile << endl;
		}
		bool up = (i >= columns) ? map[i - columns].bwall[south] : 0;
		bool down = (i + columns < row* columns) ? map[i + columns].bwall[nouth] : 0;
		bool left = (i % columns > 0) ? map[i - 1].bwall[east] : 0;
		bool right = (i % columns < columns - 1) ? map[i + 1].bwall[west] : 0;
		int V = (map[i].bwall[nouth] || up) + (map[i].bwall[south] || down) * 2 + (map[i].bwall[west] || left) * 4 + (map[i].bwall[east] || right) * 8;
		V = 15 - V;
		string S = inttostring_bytarget(V, "0123456789abcdef");
		cout << S;
		outfile << S;
		i++;
	}
	cout << endl;
	outfile << "\nЭтап готов.\n";
	outfile << endl;
}

int main(int argc, char* argv[])
{
	ifstream infile("input.txt");
	int N = 0;
	infile >> N;
	int I = 1;
	outfile << "Становка на лабиринт\n";
	outfile << "\n";
	while (N-- > 0)
	{
		string entrance_to_exit, exit_to_entrance;
		infile >> entrance_to_exit >> exit_to_entrance;
		int p = 1;
		int q = 1;
		int Lenp = entrance_to_exit.length();
		int Lenq = exit_to_entrance.length();
		grid entrance(0, 0);
		grid* node = &entrance;
		int max_eage[4] = { 0 };
		int curdir = south;
		int pX = 0;
		int pY = 0;
		bool setexit_lock = false;
		int exitX = 0, exitY = 0, exitDir = curdir;
		while (1)
		{
			char C;
			if (p < Lenp - 1) 
			{
				C = entrance_to_exit[p];
				p++;
			}
			else if (q < Lenq - 1) 
			{
				if (!setexit_lock)
				{
					exitX = pX;
					exitY = pY;
					exitDir = curdir;
					setexit_lock = true;
					curdir = turnright(turnright(curdir));
				}
				C = exit_to_entrance[q];
				q++;
			}
			else
				break;
			if ('W' == C)
			{
				switch (curdir) {
				case nouth: pY--;
					break;
				case south: pY++;
					if (pY > max_eage[south])
						max_eage[south] = pY;
					break;
				case west: pX--;
					if (pX < max_eage[west])
						max_eage[west] = pX;
					break;
				case east: pX++;
					if (pX > max_eage[east])
						max_eage[east] = pX;
					break;
				}
				grid* newnode = new  grid(pX, pY);
				node->next = newnode;
				node = newnode;
			}
			else if ('L' == C)
			{
				curdir = turnleft(curdir);
			}
			else if ('R' == C)
			{
				node->bwall[curdir] = 1;
				node->bwall[turnleft(curdir)] = 1;
				curdir = turnright(curdir);
			}
		}
		int rows = max_eage[south] - max_eage[nouth] + 1;
		int columns = max_eage[east] - max_eage[west] + 1;
		int dx = 0 - max_eage[west];
		grid* map = new grid[rows * columns];
		grid* nextnode = &entrance;
		while (nextnode)
		{
			int position = nextnode->X + dx + nextnode->Y * columns;
			for (int i = 0; i < 4; i++)
			{
				map[position].bwall[i] = map[position].bwall[i] || nextnode->bwall[i];
			}
			grid* t = nextnode;
			nextnode = nextnode->next;
			if (t != &entrance) delete t;
		}
		int k;
		for (k = 0; k < rows; k++)
		{
			map[k * columns].bwall[west] = 1;
			map[(k + 1) * columns - 1].bwall[east] = 1;
		}
		for (k = 0; k < columns; k++)
		{
			map[k].bwall[nouth] = 1;
			map[rows * columns - 1 - k].bwall[south] = 1;
		}
		map[entrance.X + dx].bwall[nouth] = 0;
		map[exitX + dx + exitY * columns].bwall[exitDir] = 0;
		cout << "Case #" << I << ": " << endl;
		outfile << "Case #" << I << ": " << endl;
		I++;
		drawmap(map, rows, columns);		
		delete[]map;
	}
	cout << "file output gotov, svezhii lezhit i zdet";
	outfile << "\n А уж лабиринт тем более.";
	return 0;
}