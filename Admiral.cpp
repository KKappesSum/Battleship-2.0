// Admiral.cpp

#include "Admiral.h"

Admiral::Admiral()
{
	m_numShips = 0;
	m_numAfloat = 0;
}

Admiral::Admiral(int numShips)
{
	m_numShips = numShips;
	m_numAfloat = numShips;
}

Admiral::~Admiral()
{
	Ship* tempPtr = nullptr;
	for(vector<Ship*>::iterator it = m_fleet.begin(); it != m_fleet.end(); ++it)
	{
		tempPtr = *it;
		delete tempPtr;
	}
}

// --- getters ---
Grid* Admiral::getBoard()
{
	Grid* ptr = &m_board;
	return(ptr);
}

int Admiral::getNumShips() const
{
	return(m_numShips);
}

vector<Ship*>* Admiral::getFleet()
{
	vector<Ship*>* ptr = &m_fleet;
	return(ptr);
}

int Admiral::getNumAfloat() const
{
	return(m_numAfloat);
}

// --- setters ---
void Admiral::setNumShips(const int numShips)
{
	m_numShips = numShips;
}

void Admiral::setNumAfloat(const int num)
{
	m_numAfloat = num;
}

void Admiral::addShip(const int size, const string* coordsArr)
{
	m_fleet.push_back(new Ship(size));
	m_fleet.back()->setCoords(coordsArr, size);
	m_board.readShip(coordsArr, size);
}

void Admiral::loadShip(int size, string* coordsArr, bool status, int hits)
{
	m_fleet.push_back(new Ship(size, coordsArr, status, hits));
}



int Admiral::decNumAfloat()
{
	m_numAfloat--;
	return(m_numAfloat);
}

string Admiral::incomingShot(const string coord)
{
	string origStr = m_board.getCoor(coord);
	char orig = origStr.at(0);
	if(isdigit(orig) != 0)	// check if the target is a Ship
	{
		m_board.setCoor(coord, "X");
		int shipIndex = findShipbyCoord(coord);
		m_fleet[shipIndex]->incNumHits();
		if(!(m_fleet[shipIndex]->getStatus()))
		{
			int numAfloat = decNumAfloat();
			return("sunk");
		}
		else
		{
			return("hit");
		}
	}
	else if(orig == '~' || orig == 'O')	// shot is a miss
	{
		m_board.setCoor(coord, "O");
		return("miss");
	}
	else if(orig == 'X')
	{
		return("X");
	}
	else
	{
		return(origStr);
	}
}

int Admiral::findShipbyCoord(const string coord) const
{
	int foundIndex = -1;
	for(int shipIndex = 0; shipIndex < m_numShips; shipIndex++)
	{
		int shipSize = m_fleet.at(shipIndex)->getSize();
		string* shipCoords = m_fleet.at(shipIndex)->getCoords();
		for(int coordIndex = 0; coordIndex < shipSize; coordIndex++)
		{
			if(shipCoords[coordIndex] == coord)
			{
				foundIndex = shipIndex;
				return(foundIndex);
			}
		}
	}
	return(foundIndex);
}

void Admiral::removeLastShip()
{
	Ship* tempPtr = m_fleet.back();
	string* coordsPtr = tempPtr->getCoords();
	int size = tempPtr->getSize();

	for(int i = 0; i < size; i++)
	{
		m_board.setCoor(coordsPtr[i], "~");
	}
	delete[] tempPtr;
	m_fleet.pop_back();
}