// UMBC - CMSC 341 - Spring 2022 - Proj2
//file name: fleet.cpp
//author name: Gurnoor Hira
//prof name: Allgood
#include "fleet.h"

const int NEGATIVE_ONE = -1;
Fleet::Fleet(){
    m_root = nullptr;
}
Fleet::~Fleet(){
    clear();
}
void Fleet::insert(const Ship& ship){
    //if ship is already in fleet or ship id is invalid number then don't insert
    if(ship.getID() <= 0 || findShip(ship.getID())){
        cout << "invalid ID, cant insert " << endl;
        return;
    }
    m_root = recursiveInsert(m_root, ship.getID(),ship);

}

void Fleet::clear(){
    if(m_root == nullptr){ //if already cleared
        cout << "tree is clear" << endl;
        return;
    }
    else{
        recursiveDestructor(m_root);
    }
}

void Fleet::remove(int id){
    if(m_root == nullptr || findShip(id) == false){ //validate if removal is possible 
        cout << "INVALID -- CAN'T REMOVE" << endl;
        return;
    }

    if(id != m_root->getID()){
        recursiveRemove(m_root,id);
    }
    //if the root is ship that needs to be removed
    Ship* tempShip = recursiveMin(m_root->getRight(),1);
    //update root with temp ship's data
    m_root->setID(tempShip->getID());
    m_root->setState(tempShip->getState());
    m_root->setType(tempShip->getType());

    //delete tempShip because its data has been copied over so it is not needed anymore
    tempShip = nullptr;
    delete tempShip;

    //rebalance and update tree
    updateHeight(m_root);
    m_root = rebalance(m_root);
    return;
}

void Fleet::updateHeight(Ship* aShip){
    int left = 0;
    int right = 0;
    if(aShip == nullptr){
        return;
    }
    //getting the heights of the left and right subtrees
    if(aShip->getLeft() == nullptr){
        left = NEGATIVE_ONE;
    }
    else{
        left = aShip->getLeft()->getHeight();
    }
    if(aShip->getRight() == nullptr){
        right = NEGATIVE_ONE;
    }
    else{
        right = aShip->getRight()->getHeight();
    }
    //set height to bigger +1
    if(left > right){
        aShip->setHeight(left + 1);
    }
    else{
        aShip->setHeight(right + 1);
    }
}


int Fleet::checkImbalance(Ship* aShip){
    if(aShip == nullptr){
        return NEGATIVE_ONE;
    }
    int left = NEGATIVE_ONE;
    int right = NEGATIVE_ONE;
    //if ship has a right get right's height
    if(aShip->getRight() != nullptr){
        right = aShip->getRight()->getHeight();
    }
    //if ship has a left get left's right
    if(aShip->getLeft() != nullptr){
        left = aShip->getLeft()->getHeight();
    }
    return(left - right);
}


Ship* Fleet::rebalance(Ship* aShip){
    if(aShip == nullptr){
        return nullptr;
    }
    // right rotation
    else if((checkImbalance(aShip) > 1) && ((checkImbalance(aShip->getLeft()) >= 0))){
        return rotateRight(aShip);
    }
    //left rotation
    if((checkImbalance(aShip) < NEGATIVE_ONE) && (checkImbalance(aShip->getRight()) <= 0)){
        return rotateLeft(aShip);
    }
     //rotate left then right
    else if((checkImbalance(aShip) > 1) && (checkImbalance(aShip->getLeft()) <= 0)){
        aShip->setLeft(rotateLeft(aShip->getLeft()));
        return rotateRight(aShip);
    } 
    //rotate right then left
    else if((checkImbalance(aShip) < NEGATIVE_ONE) && (checkImbalance(aShip->getRight()) >= 0)){
        aShip->setRight(rotateRight(aShip->getRight()));
        return rotateLeft(aShip);
    }
    //in case of no imbalanced --> no rotation
    else{
        return aShip;
    }

}

void Fleet::dumpTree() const {
    //function provided
    dump(m_root);
}

void Fleet::dump(Ship* aShip) const{
    //function provided
    if (aShip != nullptr){
        cout << "(";
        dump(aShip->m_left);//first visit the left child
        cout << aShip->m_id << ":" << aShip->m_height;//second visit the node itself
        dump(aShip->m_right);//third visit the right child
        cout << ")";
    }
}

void Fleet::listShips() const {
    if(m_root == nullptr){
        cout << "tree empty can't list ships" << endl;
        return;
    }
    recursiveListShips(m_root);
    
}

bool Fleet::setState(int id, STATE state){
    if(findShip(id)){
        Ship *temp = m_root;
        //find id and set state to parameter fiven
        while(temp != nullptr){
            if(id == temp->getID()){
                temp->setState(state);
                return true;
            }
            else if(id > temp->getID()){
                temp = temp->getRight();
            }
            temp = temp->getLeft();
            
        }
    }
    cout << "can't set state, ship not found" << endl;
    return false;
}

void Fleet::removeLost(){
    if(m_root == nullptr){
        cout << "can't remove lost ships -- tree is empty" << endl;
        return;
    }
    recursiveLostRemove(m_root); //update root
    recursiveLostRemove(m_root);//remove missed nodes after updated

    
}

bool Fleet::findShip(int id) const {
    if(m_root == nullptr){
        return false;
    }
    else{
        Ship* temp = m_root;
        //traverse through tree to look for ship
        while(temp != nullptr){
            if(id == temp->getID()){
                return true;
            }
            else if(id > temp->getID()){
                temp = temp->getRight();
            }
            else{
                temp = temp->getLeft();
            }
        }
        return false;
    }
    return false;
}

Ship* Fleet::recursiveInsert(Ship* s,int id,const Ship& sh){
    if(s == nullptr){
        //if ship to insert is null create new ship and set state and value
        Ship* newS = new Ship(id);
        newS->setState(sh.getState());
        newS->setType(sh.getType());
        return newS;
    }
    
    else if(s->getID() < id){
        //if ships ID is less than id then set right
        s->setRight(recursiveInsert(s->getRight(),id,sh));
        updateHeight(s); //update height of tree
        return rebalance(s);//rebalance tree
    }
    else if(s->getID() > id){
        //if ships id is greater than id then set left
        s->setLeft(recursiveInsert(s->getLeft(),id,sh));
        updateHeight(s); ///update height of tree
        return rebalance(s); //rebalance tree
    }

    return s;
}

Ship* Fleet::rotateLeft(Ship* s){
    Ship* a = s->getRight();
    Ship* b = a->getLeft();
    //rotate left
    a->setLeft(s);
    s->setRight(b);
    updateHeight(s);
    updateHeight(a);
    return a; //return rotated node

}

Ship* Fleet::rotateRight(Ship* s){
    Ship* a = s->getLeft();
    Ship* b = a->getRight();
    //rotate right
    a->setRight(s);
    s->setLeft(b);
    updateHeight(s);
    updateHeight(a);
    return a;  //return rotated node
}
Ship* Fleet::recursiveRemove(Ship* ship, int id){
    Ship *shipTemp;
    if(ship == nullptr){
        return nullptr;
    }
    //if id greater than ship id, traverse to right
    else if(id > ship->getID()){
        ship->setRight(recursiveRemove(ship->getRight(),id));
    }
    //if id less than ship id, traverse to left
    else if(id < ship->getID()){
        ship->setLeft(recursiveRemove(ship->getLeft(),id));
    }
    //if 2 childred
    else if(ship->getLeft() && ship->getRight()){
        //find lowest val of right and copy its value over to node to delete
        shipTemp = recursiveMin(ship->getRight(),0);
        ship->setID(shipTemp->getID());
        ship->setState(shipTemp->getState());
        ship->setType(shipTemp->getType());
        ship->setRight(recursiveRemove(ship->getRight(),ship->getID()));
    }
    else{
        shipTemp = ship;
        if(ship->getLeft() == nullptr){
            ship = ship->getRight();
        }
        else if(ship->getRight() == nullptr){
            ship = ship->getLeft();
        }
        delete shipTemp;
    }
    updateHeight(ship);
    //update root to check imbalalences
    if(ship != nullptr && ship->getID() == m_root->getID()){
        m_root = rebalance(m_root);
    }
    //rebalance ship then return
    ship = rebalance(ship);
    return ship;
}

Ship* Fleet::recursiveMin(Ship* s, int root){
    if(root == 1 && s->getLeft() != nullptr){ //checking to see if we are working with root
        if(root == 1 && s->getLeft()->getLeft() == nullptr){
            Ship* shipTemp = s->getLeft();
            s->getLeft()->setLeft(nullptr);
            return shipTemp;
        }
    }
    if(s->getLeft() == nullptr || s == nullptr){
        return s;
    }
    return recursiveMin(s->getLeft(),root);
}

void Fleet::recursiveListShips(Ship* s) const{
    if(s == nullptr){
        return;
    }
    //traversal -- in order
    recursiveListShips(s->getLeft());
    cout << s->getID() << ":" << s->getStateStr() << ":" << s->getTypeStr() << endl;
    recursiveListShips(s->getRight());
}

void Fleet::recursiveLostRemove(Ship* s){
    if(s == nullptr){
        return;
    }
    //post order traversal
    recursiveLostRemove(s->getLeft());
    recursiveLostRemove(s->getRight());
    //removes whenever state is lost
    if(s->getStateStr() == "LOST"){
        cout << " removing lost ship " << s->getID() << endl;
        remove(s->getID());
    }
}
void Fleet::recursiveDestructor(Ship* s){
    if(s == nullptr){
        return;
    }
    //delete subtrees
    recursiveDestructor(s->getRight());
    recursiveDestructor(s->getLeft());
    
    //delete ship 
    delete s;
}