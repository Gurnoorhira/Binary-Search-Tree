// UMBC - CMSC 341 - Spring 2022 - Proj2
//file name: fleet.h
//author name: Gurnoor Hira
//prof name: Allgood
#ifndef FLEET_H
#define FLEET_H
#include <iostream>
using namespace std;
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
enum STATE {ALIVE, LOST};
enum SHIPTYPE {CARGO, TELESCOPE, COMMUNICATOR, FUELCARRIER, ROBOCARRIER};
const int MINID = 10000;
const int MAXID = 99999;
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0
#define DEFAULT_TYPE CARGO
#define DEFAULT_STATE ALIVE
class Ship{
    public:
    friend class Grader;
    friend class Tester;
    friend class Fleet;
    Ship(int id, SHIPTYPE type = DEFAULT_TYPE, STATE state = DEFAULT_STATE)
        :m_id(id),m_type(type), m_state(state) {
            m_left = nullptr;
            m_right = nullptr;
            m_height = DEFAULT_HEIGHT;
        }
    Ship(){
        m_id = DEFAULT_ID;
        m_type = DEFAULT_TYPE;
        m_state = DEFAULT_STATE;
        m_left = nullptr;
        m_right = nullptr;
        m_height = DEFAULT_HEIGHT;
    }
    int getID() const {return m_id;}
    STATE getState() const {return m_state;}
    string getStateStr() const {
        string text = "";
        switch (m_state)
        {
        case LOST:text = "LOST";break;
        case ALIVE:text = "ALIVE";break;
        default:text = "UNKNOWN";break;
        }
        return text;
    }
    SHIPTYPE getType() const {return m_type;}
    string getTypeStr() const {
        string text = "";
        switch (m_type)
        {
        case CARGO:text = "CARGO";break;
        case TELESCOPE:text = "TELESCOPE";break;
        case COMMUNICATOR:text = "COMMUNICATOR";break;
        case FUELCARRIER:text = "FUELCARRIER";break;
        case ROBOCARRIER:text = "ROBOCARRIER";break;
        default:text = "UNKNOWN";break;
        }
        return text
        ;
    }
    int getHeight() const {return m_height;}
    Ship* getLeft() const {return m_left;}
    Ship* getRight() const {return m_right;}
    void setID(const int id){m_id=id;}
    void setState(STATE state){m_state=state;}
    void setType(SHIPTYPE type){m_type=type;}
    void setHeight(int height){m_height=height;}
    void setLeft(Ship* left){m_left=left;}
    void setRight(Ship* right){m_right=right;}
    private:
    int m_id;
    SHIPTYPE m_type;
    STATE m_state;
    Ship* m_left;//the pointer to the left child in the BST
    Ship* m_right;//the pointer to the right child in the BST
    int m_height;//the height of node in the BST
};
class Fleet{
    public:
    friend class Grader;
    friend class Tester;
    Fleet();
    ~Fleet();
    void insert(const Ship& ship);
    void clear();
    void remove(int id);
    void dumpTree() const;
    void listShips() const;
    bool setState(int id, STATE state);
    void removeLost();//remove all lost ships from the tree
    bool findShip(int id) const;//returns true if the ship is in tree
    private:
    Ship* m_root;//the root of the BST

    void dump(Ship* aShip) const;//helper for recursive traversal
    void updateHeight(Ship* aShip);
    int checkImbalance(Ship* aShip);
    Ship* rebalance(Ship* aShip);

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************
    Ship* recursiveInsert(Ship* s,int ID,const Ship& sh);
    Ship* rotateLeft(Ship* s);
    Ship* rotateRight(Ship* s);
    Ship* recursiveRemove(Ship* ship, int id);
    Ship* recursiveMin(Ship* s, int root);
    void recursiveListShips(Ship* s) const;
    void recursiveLostRemove(Ship* s);
    void recursiveDestructor(Ship* s);
};
#endif