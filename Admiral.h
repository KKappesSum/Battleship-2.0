// Admiral.h

#ifndef ADMIRAL_H
#define ADMIRAL_H
#include <string>
#include <iostream>
#include <stdexcept>
#include <cctype>
#include <vector>
#include "Grid.h"
#include "Ship.h"
using namespace std;

class Admiral
{
	protected:
		Grid m_board;	/* a Grid to store the Admiral's game map. */
		int m_numShips;	/* the number of Ships in Admiral. */
		vector<Ship*> m_fleet;	/* an vector of pointers to Ships that belong to the Admiral. */
		int m_numAfloat;	/* the number of ships that are still afloat. */

	public:
		/** A constructor that takes no parameters.
		 *
		 */
		Admiral();

		/** A constructor that takes one parameter.
		 * @param numShips the number of Ships the Admiral will have.
		 */
		Admiral(int numShips);

		/** The destructor.  Manually deletes each pointer contained by m_fleet.
		 *
		 */
		~Admiral();

		// --- getters ---

		/** Gets the Admiral's game board.
		 * @return a reference to m_board.
		 */
		Grid* getBoard();

		/** Gets the Admiral's total number of Ships.
		 * @return the number of Ships, aka the size of the m_fleet array.
		 */
		int getNumShips() const;

		/** Gets the Admiral's fleet of Ships.
		 * @return a reference to the fleet vector.
		 */
		vector<Ship*>* getFleet();

		/** Gets the number of the Admiral's Ships that are still afloat.
		 * @return the number of Ships afloat.
		 */
		int getNumAfloat() const;

		// --- setters ---

		/** Sets the number of Ships that Admiral has.
		 * @param numShips the number of Ships.
		 */
		void setNumShips(const int numShips);

		/** Sets the number of Ships that are still afloat; initially, the same number as the total number of Ships.
		 * @param num the number of Ships.
		 */
		void setNumAfloat(const int num);

		// --- helpers ---

		/** Constructs a Ship object with size and coordinates, adds it to the m_fleet vector, and assigns the Ship's coordinates to m_board.
		 * @param size the size of the Ship.
		 * @param coordsArr a pointer to an array of coordinates that the new Ship will occupy.
		 */
		void addShip(const int size, const string* coordsArr);

		/** Decrements the number of Ships still afloat; this function is called when a Ship sinks.
		 * @return the number of Ships still afloat.
		 */
		int decNumAfloat();

		/** Checks the given coordinate in its m_board, updates the game board and the relevant Ship if necessary.
		 * @param coord - the coordinate to try.
		 * @return the content of the coord location, which indicates the result of the shot.
		 */
		string incomingShot(const string coord);

		/** Finds the index of the Ship in the fleet that occupies the given coordinate.
		 * @return the index of the relevant Ship in m_fleet, or -1 if the Ship is not found.
		 */
		int findShipbyCoord(const string coord) const;

		void loadShip(int size, string* coordsArr, bool status, int hits);

		/**
		 * dummy fire method to please C++ inheritence
		 */
		virtual string fire() {}

		/** Remove the last Ship that was added and clear that Ship's coordinates from the Grid.
		 * @pre the Admiral has at least one Ship.
		 * @post Removes and deletes the last Ship that was added and clears that Ship's coordinates from the Grid.
		 */
		void removeLastShip();

};
#endif
