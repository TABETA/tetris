#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <d2d1.h>
#include <numeric>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <conio.h>
#include <thread>

using namespace std;


int conv1to2(int col, int row, int width)
{
	return col + row * width;

}

class Point
{
public:
	Point() :col_(), row_()
	{}
	Point(int c, int r) :col_(c), row_(r)
	{
	}
	Point(const Point& p) :col_(p.col_), row_(p.row_)
	{
	}
	void drop()
	{
		row_ += 1;
	}
	int col_;
	int row_;
};

class Block : public Point
{
public:
	static const int w = 2;
	Block() :Point(3, 0), val_(vector<vector<char> >(2,vector<char>(w)))
	{
		char dat[][w] =
		{
			{ 'x', 'x' },
			{ 'x', 'x' },
		};
		for (int i = 0; i < 2; ++i)
		{
			val_[i] = vector<char>(dat[i], dat[i] + w);
		}
	}
	Block(const Block& b) :Point(b), val_(b.val_)
	{
	}
	bool drop(size_t hight)
	{
		if (row_ + val_.size() + 1 > hight)
		{
			return false;
		}
		Point::drop();
		return true;
	}
	bool turnLeft()
	{
		return true;
	}
	bool turnRight()
	{
		return true;
	}
	bool moveLeft()
	{
		if (col_ <= 0)
		{
			return false;
		}
		--col_;
		return true;
	}
	bool moveRight(size_t width)
	{
		if (col_ + val_[0].size() >= width)
		{
			return false;
		}
		++col_;
		return true;
	}
	char get(int col, int row)const
	{
		return val_[row][col];
	}
	vector<vector<char> > val_;
};
class Cell
{
public:
	Cell() :b_(), val_(15, vector<char>(10, ' '))
	{
	}
	Cell(const Block& b) :Cell()
	{
		for (size_t c = 0; c < b.val_[0].size(); ++c)
		{
			for (size_t r = 0; r < b.val_.size(); ++r)
			{
				val_[b.row_+r][b.col_+c] = b.get(c, r);
			}
		}
	}

	Cell(const Cell& c1, const Cell& c2) :b_(), val_(15, vector<char>(10, ' '))
	{
		for (size_t c = 0; c < val_[0].size(); ++c)
		{
			for (size_t r = 0; r < val_.size(); ++r)
			{
				val_[r][c] = c1.val_[r][c] + c2.val_[r][c] - ' ';
			}
		}
	}
	void operator()()
	{
		time_t start = clock();
		while (true)
		{
			if (clock() - start > 0.064 * CLOCKS_PER_SEC)
			{
				system("cls");
				Cell temp2(*this, b_);
				temp2.draw_();
				start = clock();
			}
		}
	}
	void draw_()
	{
		for (size_t r = 0; r < val_.size(); ++r)
		{
			string b = "++";
			b.insert(1, string(val_[r].begin(), val_[r].end()));
			cout << b << endl;
		}
		string b(val_[0].size() + 2, '+');
		cout << b << endl;
	}
	void ml()
	{
		Block b(b_);
		if (!b.moveLeft())
		{
			return;
		}
		Cell c(b);
		if (!isCollide(c))
		{
			b_ = b;
		}
	}
	void mr()
	{
		Block b(b_);
		if (!b.moveRight(val_[0].size()))
		{
			return;
		}
		Cell c(b);
		if (!isCollide(c))
		{
			b_ = b;
		}
	}
	bool drop()
	{
		Block b(b_);
		if (!b.drop(val_.size()))
		{
			combine(b_);
			return false;
		}
		Cell c(b);
		if (isCollide(c))
		{
			combine(b_);
			return false;
		}
		else
		{
			b_ = b;
			return true;
		}
	}
	bool add(const Block& b)
	{
		b_ = b;
		return true;
	}
private:
	Block b_;
	vector<vector<char> > val_;
	void pack(int r)
	{
		for (int j = r; j >= 0; --j)
		{
			if (j == 0)
			{
				fill(val_[j].begin(), val_[j].end(), ' ');
			}
			else
			{
				copy(val_[j - 1].begin(), val_[j - 1].end(), val_[j].begin());
			}
		}
	}
	bool banish()
	{
		bool result = false;
		for (int r = val_.size() - 1; r >= 0; --r)
		{
			if (all_of(val_[r].begin(), val_[r].end(), [](char c){ return c == 'x'; }))
			{
				pack(r);
				result = true;
			}
		}
		return result;
	}
	void combine(const Cell& cell)
	{
		for (int r = val_.size()-1; r >= 0; --r)
		{
			for (int c = 0; c < val_[0].size(); ++c)
			{
				val_[r][c] += cell.val_[r][c] - ' ';
			}
		}
		while (banish())
		{
		}
	}
	bool isCollide(const Cell& cell)
	{
		for (size_t r = 0; r < val_.size(); ++r)
		{
			for (size_t c = 0; c < val_[0].size(); ++c)
			{
				if (val_[r][c] != ' ' && cell.val_[r][c] != ' ')
				{
					return true;
				}
			}
		}
		return false;
	}

};


#include <time.h>


int main()
{
	Cell cell;

	Block b;
	cell.add(b);
	time_t start = clock();
	thread draw(ref(cell));
	while(true)
	{
		if (kbhit())
		{
			switch (getch())
			{
			case 'a':
				cell.ml();
				break;
			case 'd':
				cell.mr();
				break;
			case 's':
				if (!cell.drop())
				{
					Block b;
					cell.add(b);
				}
				break;
			}
		}
		if (
			(clock() - start)
			> (0.4 * CLOCKS_PER_SEC)
			)
		{
			if (!cell.drop())
			{
				Block b;
				cell.add(b);
			}
			start = clock();
		}
	}
	draw.join();
	return 0;
}
