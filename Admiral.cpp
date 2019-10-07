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
}

// --- getters ---
Grid Admiral::getBoard() const
{
	return(m_board);
}

int Admiral::getNumShips() const
{
	return(m_numShips);
}

vector<Ship*> Admiral::getFleet() const
{
	return(m_fleet);
}

int Admiral::getNumAfloat() const
{
	return(m_numAfloat);
}

// string Admiral::getPowerUps() const

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

int Admiral::decNumAfloat()
{
	m_numAfloat--;
	return(m_numAfloat);
}

int Admiral::incomingShot(const string coord)
{
	
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
