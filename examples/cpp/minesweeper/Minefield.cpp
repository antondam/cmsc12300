/*
 * Minefield.cpp
 *
 *  Created on: May 3, 2013
 *      Author: borja
 */

#include<algorithm>
using namespace std;

#include "Minefield.h"

MinefieldEntry::MinefieldEntry(EntryType t)
{
	this->type = t;
	this->adjacentMines = 0;
}

void MinefieldEntry::setType(EntryType t)
{
	this->type = t;
}

EntryType MinefieldEntry::getType()
{
	return this->type;
}

int MinefieldEntry::operator++()
{
	return adjacentMines++;
}

int MinefieldEntry::operator++(int a)
{
	return ++adjacentMines;
}


Minefield::Minefield(): rows(0), cols(0), field(NULL)
{
	// TODO Auto-generated constructor stub

}

Minefield::Minefield(int rows, int cols): rows(rows), cols(cols)
{
	this->field = new MinefieldEntry[rows*cols];
}

Minefield::Minefield(const Minefield &mf)
{
	cout << "COPY" << endl;
	this->rows = mf.rows;
	this->cols = mf.cols;
	this->field = new MinefieldEntry[rows*cols];
	copy(&mf.field[0], &mf.field[rows*cols], &this->field[0]);
}


Minefield::~Minefield()
{
	delete[] field;
}

int Minefield::getNumRows()
{
	return rows;
}

int Minefield::getNumCols()
{
	return cols;
}

void Minefield::solve_range(int x_min, int x_max, int y_min, int y_max, int radius, EntryType entries)
{
	for (int x=x_min; x < x_max; x++)
		for (int y=y_min; y < y_max; y++)
			if (entries == MINE && field[i(x,y)].getType() == MINE)
				processEntry(x, y, MINE, radius);
			else if (entries == EMPTY && field[i(x,y)].getType() == EMPTY)
				processEntry(x,y, EMPTY, radius);
}

void Minefield::solve(int radius, EntryType entries)
{
	solve_range(0, rows, 0, cols, radius, entries);
}


std::ostream& operator<<(std::ostream &os, const Minefield &mf)
{
	for (int x=0; x < mf.rows; x++)
	{
		for (int y=0; y < mf.cols; y++)
		{
			MinefieldEntry &entry = mf.field[x*mf.cols + y];
			switch(entry.type)
			{
			case MINE:
				os << "X";
				break;
			case EMPTY:
				os << entry.adjacentMines;
				break;
			}
			os << " ";
		}
		os << endl;
	}

	return os;
}

std::istream& operator>>(std::istream &is, Minefield &mf)
{
	is >> mf.rows;
	is >> mf.cols;

	if (mf.rows == 0 && mf.cols == 0)
	{
		is.setstate(ios::eofbit);
		return is;
	}

	delete[] mf.field;

	mf.field = new MinefieldEntry[mf.rows * mf.cols];

	for(int i=0; i < mf.rows*mf.cols; i++)
	{
		int x;
		is >> x;
		mf.field[i].setType(x?MINE:EMPTY);
	}

	return is;
}

void Minefield::processEntry(int x, int y, EntryType type, int radius)
{
	int x_min, x_max, y_min, y_max;

	x_min = max(0, x-radius);
	x_max = min(x+radius, rows - 1);

	y_min = max(0, y-radius);
	y_max = min(y+radius, cols -1);

	for(int x2=x_min; x2<=x_max; x2++)
		for(int y2=y_min; y2<=y_max; y2++)
			if(!(x2==x && y2==y))
			{
				if(type == MINE)
					field[i(x2,y2)]++;
				if(type == EMPTY && field[i(x2,y2)].getType()==MINE)
					field[i(x,y)]++;
			}
}

