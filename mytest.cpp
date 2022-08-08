// UMBC - CMSC 341 - Spring 2022 - Proj2
//file name: mytest.cpp
//author name: Gurnoor Hira
//prof name: Allgood
#include "fleet.h"
#include "fleet.cpp"
#include <random>
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else{ //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
class Tester{
    public:
    /******************************************
    * Test function declarations go here! *
    ******************************************/
   Ship* getRoot(const Fleet &f);
   bool testAvlTree(Ship* s);
   bool treeBalanced(Ship* s);

   //functions to help make sure tree is balanced
   int getMax(int first, int second);
   int treeHeight(Ship* s); //get height of subtrees

};
int main(){
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Tester tester;
    {
        Fleet fleet;
        int fleetSize = 10;
        int tempID = 0;
        int ID = 0;
        int duplicate = 0;
        int remove = 0;
        for(int i=0;i<fleetSize;i++){
            ID = idGen.getRandNum();

            //for duplicate insertion
            if(i == 2){
                duplicate = ID;
            }
            if (i == fleetSize / 2){
                tempID = ID;//we store this ID for later use
            }
            Ship ship(ID,static_cast<SHIPTYPE>(typeGen.getRandNum()));
            if(i == 3 || i == 1 || i ==2){ //setting 3 ships to lost to test remove lost
                ship.setState(LOST);
            }
            if(i == 4){
                remove = ID; 
            }
            fleet.insert(ship);
        }
        cout << "\nDump after inserting " << fleetSize << " nodes:\n\n";
        fleet.dumpTree();
        cout << "\n\nList of Ships after inserting " << fleetSize << " nodes:\n";
        fleet.listShips();
        cout << endl;

        cout << "TESTING IF TREE AVL: " <<endl; //test if tree is binary search tree 
        Ship* root = tester.getRoot(fleet);
        if(tester.testAvlTree(root)){
            cout << "TREE AVL -- PASSED" << endl;
        }
        else{
            cout << "TREE NOT AVL -- FAIL" << endl;
        }
        cout << "CHECK IF TREE BALANCED: " << endl; //test if tree is balanced after insertion
        if(tester.treeBalanced(root)){
            cout << "TREE IS BALANCED -- PASS" <<endl;
        }
        else{
            cout << "TREE NOT BALANCED -- FAIL" << endl;
        }
        cout <<"TESTING INSERT DUPLICATE -- ERROR CASE" << endl; //error case insert test
        Ship newShip(duplicate,static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet.insert(newShip);
        cout << endl;
        cout << "TESTING INSERT EDGE CASE, INSERTING WITH VALUE ZERO" << endl; //edge case insert test
        Ship sh(0,static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet.insert(sh);
        cout << endl;
        cout <<"TESTING INSERT -- ERROR CASE" << endl; //error case insert test
        Ship shi(-1,static_cast<SHIPTYPE>(typeGen.getRandNum()));
        fleet.insert(shi);
        cout << endl;
        cout << "TESTING REMOVE: "<< endl;
        fleet.remove(tempID); //testing remove normal case
        cout << "\nDump after removing the node with ID: " << tempID << "\n\n";
        fleet.dumpTree();
        cout << "\n\nList of Ships after removing the node with ID: " << tempID << "\n";
        fleet.listShips();
        cout << endl;
        if(fleet.findShip(tempID)){ //checking if ship was actually removed or not
            cout <<"REMOVE DID NOT WORK -- FAIL" << endl;
        }
        else{
            cout << "REMOVE WORKED-- PASS" << endl;
        }
        cout << endl;
        cout <<"REMOVING SAME ID AGAIN, ERROR CASE: " << endl; //test duplication for remove
        fleet.remove(tempID);
        cout << endl;
        cout <<"TEST REMOVING LOST: " <<endl;
        fleet.removeLost(); //testing remove lost, 3 ships were set to lost so all 3 will be removed 
        cout <<endl;
        cout <<"list ships after removing lost" <<endl;
        fleet.listShips();
        cout << endl;
        cout << "CHECK IF AVL TREE: " << endl; //check if tree is BST
        root = tester.getRoot(fleet);
        if(tester.testAvlTree(root)){
            cout << "TREE IS AVL-- PASS" << endl;
        }
        else{
            cout << "TREE NOT AVL -- FAIL" << endl;
        }
        cout << "TEST IF TREE BALANCED: " << endl; //check is tree is balanced
        if(tester.treeBalanced(root)){
            cout << "TREE BALANCED -- PASS" << endl;
        }
        else{
            cout << "TREE NOT BALANCED -- FAIL" << endl;
        }
        cout << endl;
        cout << "TEST EVERYTHING WITH ALOT OF INSERTIONS: " << endl; //test with a lot of nodes
        Fleet bigFleet;
        for(int i=50000;i<50500;i++){ //insert 500 ships
            Ship ship(i,CARGO,ALIVE);
            bigFleet.insert(ship);
        }
        root = tester.getRoot(bigFleet);
        cout << "TEST IF BIG FLEET IS AVL: " << endl; //check if BST
        if(tester.testAvlTree(root)){
            cout << "PASSED -- IS AVL" << endl;
        }
        else{
            cout << "FAILED -- NOT AVL" << endl;
        }
        cout << "TEST IF BIG FLEET IS BALANCED: " << endl; //check if balanced
        if(tester.treeBalanced(root)){
            cout << "PASS -- BALANCED" << endl;
        }
        else{
            cout << "FAIL -- NOT BALANCED: " << endl;
        }
        for(int i = 50100; i < 50400; i++){ //remove 300 ships
            bigFleet.remove(i);
        }
        cout << "TEST AFTER REMOVING ALOT OF NODES: " << endl; 
        root = tester.getRoot(bigFleet);
        cout << "TEST IF BIG FLEET IS AVL: " << endl;
        if(tester.testAvlTree(root)){
            cout << "PASSED -- IS AVL" << endl;
        }
        else{
            cout << "FAILED -- NOT AVL" << endl;
        }
        cout << "TEST IF BIG FLEET IS BALANCED: " << endl;
        if(tester.treeBalanced(root)){
            cout << "PASS -- BALANCED" << endl;
        }
        else{
            cout << "FAIL -- NOT BALANCED: " << endl;
        }
        cout << "TEST TIME OF INSERTION OF MANY NODES -- MUST BE LOG(N): " << endl;
        clock_t start, stop;
        Fleet bigFleet2;
        double time = 0.0;
        start = clock(); ///start ticking

        for(int i=50000;i<51000;i++){ 
            Ship ship(i,CARGO,ALIVE);
            bigFleet2.insert(ship);//insert 1000 ships
        }
        stop = clock(); //stop ticking
        time = stop - start; //calculate time
        cout << "Inserting 1000 nodes took "<< time <<" seconds" << endl;
        start = clock(); //start again
        for(int i=51000;i<52000;i++){
            Ship ship(i,CARGO,ALIVE);
            bigFleet2.insert(ship); //insert another 1000 for total of 2000
        }
        stop = clock(); //stop again
        double time2 = stop - start;
        cout << "Inserting 2000 nodes took " << time2 << " seconds" << endl;
        if((1.1 - 0.4) < time2 / time < (1.1 + 0.4)){ //equation to see if time ratio is sufficient for 0 log(n)
            cout << "INSERTION IS 0(LOG N)" << endl;
        }
        else{
            cout << "INSERTION NOT O LOG(N)" << endl;
        }
        cout << endl;
        cout << "TEST REMOVE TIME -- MUST BE O(LOG N): " << endl;
        clock_t start1, stop1;
        double removeTime1 = 0.0;
        double removeTime2 = 0.0;
        start1 = clock(); //start ticking
        for(int i = 50000; i < 50500; i++){
            bigFleet2.remove(i); //remove 500 ships
        }
        stop1 = clock(); //stop ticking
        removeTime1 = stop1 - start1;
        cout << "removing 500 nodes took " << removeTime1 << " seconds" << endl;
        start1 = clock(); //start again
        for(int i = 50500; i < 51000; i++){
            bigFleet2.remove(i); //remove another 500 for total of 1000
        }
        stop1 = clock(); //stop again
        removeTime2 = stop1 - start1;
        cout << "removing 1000 nodes took " << removeTime2 << " seconds" << endl;
        if((1.1 - 0.4) < removeTime2 / removeTime1 < (1.1 + 0.4)){ //equation to see if time ratio is sufficient for 0 log(n)
            cout << "REMOVAL IS 0(LOG N)" << endl;
        }
        else{
            cout << "REMOVAL NOT O LOG(N)" << endl;
        }

    }
        
    return 0;
}

bool Tester::testAvlTree(Ship* s){
    if(s == nullptr){
        return true;
    }
    //right is less than current node then false
    if(s->getRight() != nullptr && s->getRight()->getID() < s->getID()){
        return false;
    }
    //if left is bigger than current node then false
    if(s->getLeft() != nullptr && s->getLeft()->getID() > s->getID()){
        return false;
    }
    return true;
}

bool Tester::treeBalanced(Ship* s){
    if(s == nullptr){
        return true;
    }
    int right;
    int left;
    right = treeHeight(s->getRight()); //right tree height
    left = treeHeight(s->getLeft()); //left tree height
    if(abs(left - right) <= 1 && treeBalanced(s->getLeft()) && treeBalanced(s->getRight())){
        return true;
    }
    else{
        return false;
    }
    
}
int Tester::getMax(int first, int second){
    if(first >= second){
        return first;
    }
    else{
        return second;
    }
}
int Tester::treeHeight(Ship* s){ 
    if(s == nullptr){
        return 0;
    }
    return max(treeHeight(s->getLeft()),treeHeight(s->getRight())) + 1;
}

Ship* Tester::getRoot(const Fleet &f){
    return f.m_root;
}





