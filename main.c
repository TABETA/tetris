#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>


static const int Cell_row_ = 15;
static const int Cell_col_ = 10;

typedef struct Cell
{
	char val_[Cell_row_][Cell_col_];

}Cell;

void Cell_initialize(Cell* self)
{
	for(int c = 0; c  < Cell_col_; ++c)
	{
		for(int r = 0; r < Cell_row_; ++r)
		{
			self->val_[r][c] = ' ';
		}
	}
}

void Cell_draw(Cell* self)
{
	for(int c = 0; c  < Cell_col_; ++c)
	{
		putchar('|');
		for(int r = 0; r < Cell_row_; ++r)
		{
			putchar(self->val_[r][c]);
		}
		puts("|");
	}
	putchar('+');
	for(int r = 0; r < Cell_row_; ++r)
	{
		putchar('-');
	}
	puts("+\n");
	sleep(1);
	system("clear");
}

void Cell_drop(Cell* self)
{
	for(int c = Cell_col_-1; c  > 0; --c)
	{
		for(int r = 0; r < Cell_row_; ++r)
		{
			self->val_[r][c] = self->val_[r-1][c];
		}
	}
	for(int r = 0; r < Cell_row_; ++r)
	{
		self->val_[r][0] = ' ';
	}
}

int main()
{

	Cell cell;
	Cell_initialize(&cell);

	while(true)
	{
		Cell_drop(&cell);
		Cell_draw(&cell);
	}

	return 0;
}
