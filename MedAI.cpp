#include "MedAI.h"

MedAI::MedAI(Grid* grid)
{
    opponentBoard = grid;
    counter = 0;
    prevShot = "";
    hit = false;
    prevShipAfloat = getNumShips();
    direction = 0;
    current = 0;
}

MedAI::MedAI(Grid* grid, int numShips)
{
    opponentBoard = grid;
    counter = 0;
    prevShot = "";
    hit = false;
    prevShipAfloat = getNumShips();
    direction = 0;
    current = 0;
    m_numShips = numShips;
    m_numAfloat = numShips;
}

void MedAI::updateMiss()
{
    cout<<"\nEntered Update Miss\n";
    string fire = "";
    if (getNumAfloat() != prevShipAfloat)
    {
        hit = false;
        counter = 0;
        prevShot = "";
        prevShipAfloat = getNumShips();
        direction = 0;
        current = 0;
    }
    else if (direction == current)
    {
        int r = stoi(prevShot.substr(0,1));
        int c = stoi(prevShot.substr(1,2));
        if (direction ==1)
        {
            direction = 3;
            fire = to_string(r) + ":" + to_string(c-counter);
            prevShot = fire;
        
        }
        else if (direction ==2)
        {
            direction =4;
            fire = to_string(r+counter) + ":" + to_string(c);
            prevShot = fire;
            
        }
        else if(direction == 3)
        {
            direction =1;
            fire =  to_string(r) + ":" + to_string(c+counter);
            prevShot = fire;

        }
        else 
        {
            direction = 2;
            fire = to_string(r-counter) + ":" + to_string(c);
            prevShot = fire;
        }
    }
    cout<<"Exit Updated Miss";
}

string MedAI::checkDirection()
{
    cout<<"\nEntered Chect Direction\n";
    string fire = opponentBoard->randCoor(true);
    if (opponentBoard->getCoor(fire) == "2" || opponentBoard->getCoor(fire) == "3" ||  opponentBoard->getCoor(fire) == "4" || opponentBoard->getCoor(fire) == "5")
    {
        counter = 1;
        hit = true;
        prevShot = fire;
        string val = opponentBoard->getCoor(fire);
        int r = stoi(prevShot.substr(0,1));
        int c = stoi(prevShot.substr(1,2));
        if (c!=8)
        {
            string temp = to_string(r) + ":" + to_string(c+1);
            if (opponentBoard->getCoor(temp) == val)
            {
                direction = 1;
            }
        }
        if (c!=0)
        {
            string temp = to_string(r) + ":" + to_string(c-1);
            if (opponentBoard->getCoor(temp) == val)
            {
                direction = 3;
            }
        }
        if (r!=8)
        {
            string temp = to_string(r+1) + ":" + to_string(c);
            if (opponentBoard->getCoor(temp) == val)
            {
                direction = 4;
            }
        }
        if (r!=0)
        {
            string temp = to_string(r-1) + ":" + to_string(c);
            if (opponentBoard->getCoor(temp) == val)
            {
                direction = 2;
            }
        }
        
    }
    else if (opponentBoard->getCoor(fire) == "1")
    {
        hit = false;
        prevShipAfloat -= 1;
    }
    cout<<"\nExited Check Direction\n";
    return(fire);
}

string MedAI::fire()
{
    cout<<"\nEntered Fire\n";
    if(!hit)
    {
        string fire = checkDirection();
        return fire;
    }
    else
    {
        string fire = "";
        //need to set current to 0 in constructor 
        int r = stoi(prevShot.substr(0,1));
        int c = stoi(prevShot.substr(1,2));
        if (current ==  0)
        {
            if (c!=8)
            {
                current =1;
            }
            else if (r!=0)
            {
                current = 2;
            }
            else
            {
                current = 3;
            }
        }
        if (current == direction)
        {
            if (current ==1)
            {
                if(c==8)
                {   
                    current = 3;
                    fire = to_string(r) + ":" + to_string(c-counter);
                    counter ++;
                    prevShot = fire;
                    return fire;
                }
                fire = to_string(r) + ":" + to_string(c+1);
                counter ++;
                prevShot = fire;
                return fire;
            }
            if (current == 2)
            {
                if (r == 0)
                {
                    current = 4;
                    fire = to_string(r+counter) + ":" + to_string(c);
                    counter ++;
                    prevShot = fire;
                    return fire;
                }
                fire = to_string(r-1) + ":" + to_string(c);
                counter ++;
                prevShot = fire;
                return fire;
            }
            if (current == 3)
            {
                if (c== 0)
                {
                    current = 1;
                    fire =  to_string(r) + ":" + to_string(c+counter);
                    counter ++;
                    prevShot = fire;
                    return fire;
                }
                fire =  to_string(r) + ":" + to_string(c-1);
                counter ++;
                prevShot = fire;
                return fire;
            }
            if (current == 4)
            {
                if (r == 8)
                {
                    current = 2;
                    fire = to_string(r-counter) + ":" + to_string(c);
                    counter ++;
                    prevShot = fire;
                    return fire;
                }
                fire = to_string(r+1) + ":" + to_string(c);    
                counter ++;
                prevShot = fire;
                return fire;       
            }
        }
        if (current == 1)
        {
            fire = to_string(r) + ":" + to_string(c+1);
            if (r != 0)
            {
                current = 2;
            }
            else if (c!= 0)
            {
                current = 3;
            }
            else 
            {
                current = 4;
            }
            return fire;   
        }
        if (current ==2)
        {
            fire = to_string(r-1) + ":" + to_string(c);
            if(c!= 0)
            {
                current = 3;
            }
            else if (r!= 8)
            {
                current = 4;
            }
            else
            {
                current = 1;
            }
            return fire;
        }
        if (current == 3)
        {
            fire = to_string(r) + ":" + to_string(c-1);
            if (r!= 8)
            {
                current = 4;
            }
            else if (c!= 8)
            {
                current = 1;
            }
            else
            {
                current = 2;
            }
            return fire; 
        }
        if (current == 4)
        {
            fire = to_string(r+1) + ":" + to_string(c);
            if (c!= 8)
            {
                current = 1;
            }
            else if (r!= 0)
            {
                current = 2;
            }
            else
            {
                current = 3;
            }
            return fire; 
        }
        if (current >= 5 || current <= 0)
        {
            cout << "Error something went wrong in fire for medium AI";
        }
    }
    cout<<"\nExited Fire\n";
}


MedAI::~MedAI()
{

}
