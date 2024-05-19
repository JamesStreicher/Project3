//James Streicher
//CS 3100 
//Project 3

#include "Sequence.h"   //include header file 

//Constructor of Sequence object w/ its size (num of nodes) as parameter
//paramter is an unsigned int (can't be negative)
Sequence::Sequence(size_type sz)
{
    numElts = sz;   //set the number of Elements data member to be the size that is declared
    
    //EDGE CASE: if creating an empty list, head and tail point to null
    if (sz == 0){   
        head = nullptr;
        tail = nullptr;
    }

    //EDGE CASE: If the size of sequence being created is 1, want the head and tail 
    //pointing to the same node
    if (sz == 1) {          

        //by default, prev and next of a SequenceNode are null 
        //due to constructor in SequenceNode class 
        head = new SequenceNode(); 
        tail = head;
    
    //When size of sequence to be created is more than 1
    } else if (sz > 1) {

        head = new SequenceNode();      //start by initializing a head node
        
        //initialize 2 node pointers to add nodes to Sequence and to  
        //set next and prev to correct nodes
        SequenceNode* currNode = head;
        SequenceNode* nextNode = head;

        //for loop adding additional nodes in the sequence according to sz variable
        //(i < sz - 1) b/c head we already have head pointing to the first node
        for(size_type i = 0; i < sz - 1; i++) {
            nextNode = new SequenceNode();  //create new node
            currNode->next = nextNode;      //point the current last node to newNode
            nextNode->prev = currNode;      //point the new last node to old last node
            currNode = nextNode;            //set the current to point to the new last node
        }


        //the last node (which currNode is pointing to) will have the tail pointing to it
        tail = currNode;
        tail->next = nullptr;
    
    }
}

//Constructor of Sequence object by creating a deep copy of another object
//parameter is a constant reference to the object being copied (so that it can't get changed)
Sequence::Sequence(const Sequence& s) 
{
    numElts = s.numElts;      //contruct this numElts to be the same as the copied sequence's numElts

    if (numElts == 0) {       //if empty sequence, head and tail pointers are null, good to go
        return;
    }

    head = new SequenceNode();       //create new node that's the head of this Sequence
    SequenceNode* curr = s.head;     //initialize a pointer pointing to the head of what we're copying

    head->elt = curr->elt;          //copying over first node of Sequence& s
    tail = head;

    //this while loop is essentially the same as the for loop in other constructor, 
    //it's (curr->next != nullptr) because head node is already created
    while (curr->next != nullptr) {
        tail->next = new SequenceNode(); //tail->next node initialized with null prev and next pointers

        //set new last node's prev pointer to point to where tail currently is
        tail->next->prev = tail;        
        tail = tail->next;          //move tail pointer to new tail of this Sequence
        curr = curr->next;          //move the current node pointing to next node to be copied over
        tail->elt = curr->elt;      //copy over element value
    }

}

//Destructor method calls on clear(), as it does everything the Destructor needs to (and some)
Sequence::~Sequence()
{
    clear();
}

//operator = override, clears Sequence and replaces it with a deep copy of Sequence s
//returns a Sequence reference (so Sequence a = b = c = d etc. is possible)
//parameter is a constant Sequence reference
Sequence& Sequence::operator=(const Sequence& s)
{
    //this first part is almost exactly the same as clear() method, 
    //just doesn't set head and tail to null
    SequenceNode* curr = this->head;
    while (curr != nullptr) {
        SequenceNode *killMeNext = curr->next;
        delete curr;
        curr = killMeNext;
    }
    this->numElts = 0;

    //EDGE CASE: if the Sequence to copy is empty, set head and tail to null, return
    if (s.numElts == 0) {
        this->head = nullptr;
        this->tail = nullptr;
        return *this;
    }

    //Sequence > 1 being copied
    curr = s.head;     //initialize a pointer pointing to the head of what we're copying

    this->head = new SequenceNode(curr->elt);   //initilize head and copy element of s.head
    this->tail = this->head;

    //loop that calls on push_back method, adding a copy of 
    //the next node of s to this->tail until fully copied over
    while (curr != nullptr) {
        push_back(curr->elt);
        curr = curr->next;
    }

    return *this;
}

//operator [] override, returns the value of element in the node at index "position"
Sequence::value_type& Sequence::operator[](size_type position)
{
    //invalid call, out of bounds, throw exception
    if (position >= this->numElts) {
        throw exception();
    } 

    SequenceNode * curr = this->head;   //you get the deal by now

    //iterate current node so it's pointing to the node we want to grab
    for (int i = 0; i < position; i++) {
        curr = curr->next;
    }

    //at the right node, return its element
    return curr->elt;
}

//push_back adds on a new node with given value as its elt to the tail end of Sequence
//parameter is a constant reference to value_type (basically int) value
void Sequence::push_back(const value_type& value)
{
    //EDGE CASE: If pushing back onto an empty sequence, need to point head and tail to the new node
    if (this->numElts == 0) {
        this->head = new SequenceNode(value);   //create the head node
        this->tail = this->head;
        numElts++;                              //update the numElts
        return;                                 //done in this case
    }

    //push_back onto nonempty Sequence, already has head pointing to a node, so won't mess with that
    this->tail->next = new SequenceNode(value);     //tail is a new node that will hold the given value
    this->tail->next->prev = this->tail;            //point new node's prev to current tail node
    this->tail = this->tail->next;                  //move tail pointer to the true last node (next is alreday null)
    this->numElts++;                                //update counter of Elts
}

//pops off the tail node of the Sequence
void Sequence::pop_back()
{
    //invalid call, can't get rid of what isn't there, throw exception
    if (numElts == 0) {
        throw exception();
    }

    //EDGE CASE: If clearing out the whole list, release the memory of this node and
    //set head and tail to nullptr, numElts to 0
    if (numElts == 1) {
        delete tail;
        head = nullptr;
        tail = nullptr;
        numElts--;
        return;
    }

    //If remaining Sequence isn't empty
    tail = tail->prev;      //move tail pointer back one node
    delete tail->next;      //relelase the memory of old tail
    tail->next = nullptr;   //set new tail next to null pointer

    numElts--;              //decrease numElts by 1
}

//inserts a node with Element "value" at index "position"
//parameters are an unsigned int position and an int value
void Sequence::insert(size_type position, value_type value)
{
    //if the position if out of bounds, throw an exception
    if (position >= this->numElts) {
        throw exception();
    }

    //EDGE CASE1: if the element is being inserted at the end, just call 
    //push_back(value) to add node with value to the end of the sequence
    if (position == this->numElts - 1) {
        push_back(value);               //push_back creates the node and adds it to the end
        return;
    }

    //otherwise a new node needs to be created in this method
    SequenceNode * insert = new SequenceNode(value);

    //EDGE CASE2: if the new node is being added to the front, 
    if (position == 0) {
        insert->next = head;        //point new node->next to head
        head->prev = insert;        //point current head->prev to new node insert
        head = insert;              //set head pointer to new node
        return;
    }
    
    //SequenceNode pointer needs to be pointing at the index where insert will be inserted
    SequenceNode * curr = head;
    for (int i = 0; i < position; i++) {
        curr = curr->next;
    }

    //point the surrounding nodes to the new node insert to insert the node
    curr->prev->next = insert;      //set prev node pointing its next pointer to insert
    insert->prev = curr->prev;      //point insert's prev pointer back at that node    
    curr->prev = insert;            //curr is pointing to node that's after inserted node, 
                                    //so point its prev pointer to insert
    insert->next = curr;            //insert should now point to its next node curr

    numElts++;                      //added a node, increase numElts by 1
}


const Sequence::value_type& Sequence::front() const
{
    if (this->numElts == 0) {
        throw exception();
    } else {
        return this->head->elt;
    }
}

const Sequence::value_type& Sequence::back() const
{
    if (this->numElts == 0) {
        throw exception();
    } else {
        return this->tail->elt;
    }
}

bool Sequence::empty() const
{
    if (numElts > 0) {     //should probably actually check the elements
        return false;
    } else {
        return true;
    }
}

Sequence::size_type Sequence::size() const
{
    return this->numElts;
}

//All items in the sequence deleted and the memory associated with 
//the sequence is released. Left with empty sequence (unlike Destructor)
void Sequence::clear()
{
    // if (this->numElts == 0) {    might not be necessary
    //     return;
    // }

    //initialize a pointer to the head, go through all nodes and "delete" them
    //need a second pointer because the node that's being deleted has a pointer 
    //to it, so that pointer needs to know where to go next after a node is deleted
    SequenceNode* curr = this->head;
    while (curr != nullptr) {
        SequenceNode *killMeNext = curr->next;
        delete curr;
        curr = killMeNext;
    }

    //clean it up
    this->head = nullptr;      
    this->tail = nullptr;
    this->numElts = 0;

}

//deletes "count" number of nodes starting at index "position", memory of all deleted nodes 
//gets released, pointers are set accordingly
//parameters are unsigned int position, unsigned int count
void Sequence::erase(size_type position, size_type count)
{
    //If trying to delete nodes that are out of bounds, throw exception
    if (position + count > this->numElts) {
        throw exception();
    }

    //if parameters are valid but count is 0, nothing gets removed, don't do nothing
    if (count == 0) {
        return;
    }

    //SequenceNode pointer needs to be pointing at index given by position variable
    SequenceNode * eraserNode = this->head;     //I call this eraserHead

    //for loop to set eraserNode at the start of where it'll "erase"
    for (int i = 0; i < position; i++) {
        eraserNode = eraserNode->next;        
    }

    //initialze another pointer node
    SequenceNode * endOfErase = eraserNode;

    //sending endOfErase to last node to be erased
    //this is so prev and next pointers can be set up accurately after erasing range of nodes
    for (int i = 1; i < count; i++) {
        endOfErase = endOfErase->next;        
    }

    //EDGE CASE: if using erase to delete entire list, clear and return
    if (count == numElts) {     //only way this block would hit is if position == 0
        clear();
        return;
    }

    //DECAPITATED: if erasing at least head node, need to point this.head to the node after endOfErase
    if (position == 0) {
        this->head = endOfErase->next;
        this->head->prev = nullptr;
    } else {
        //otherwise set up next pointer to point to node after the range of to-be-erased nodes
        eraserNode->prev->next = endOfErase->next; //if endOfErase is tail, sets eraserNode->prev->next to nullptr
    }

    //NO MORE TAIL: if erasing at least tail node, need to point this.tail to the node before eraserNode
    if (position + count == numElts) {
        this->tail = eraserNode->prev;
        //tail->next has already been set to nullptr in previous block's else case
    } else {
        //otherwise set first node after endOfErase's prev pointer to point to eraseNode->prev
        endOfErase->next->prev = eraserNode->prev; 
    }


    //deleting the number of nodes, i>1 avoids seg fault if tail node is being deleted
    for (int i = count; i > 1; i--) {
        SequenceNode *killMeNext = eraserNode->next;  //if this is called while eraserNode = tail, seg fault
        delete eraserNode;
        eraserNode = killMeNext;
        numElts--;
    }

    delete endOfErase;    //delete the final node 
    numElts--;

}

// Place code for printing sequence here (well not here, inside the method)
//Prints out the sequence in the form of "elt, elt, elt, ..., elt"
void Sequence::print(ostream& os) const
{
    SequenceNode * curr = this->head;
    while (curr->next) {
        os << curr->elt << ", ";
        curr = curr->next;
    }

    os << curr->elt;
}

// Don't modify, do the output in the print() method
ostream& operator<<(ostream& os, const Sequence& s)
{
    s.print(os);
    return os;
}

