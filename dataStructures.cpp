// Alex Olson		CS202		Assignment 1
// dataStructures.cpp	4/7/2017
/*
    This file contains the "building block" data structures that are
    used elsewhere in the program (as well as some basic function
    implementations). The building blocks come in two general flavors
    for use elsewhere in the program, and those are LLL and DLL. The
    LLL is known as the list class, which HAS nodes , and the DLL is
    known as the edge class, which HAS DLLnodes. Finally, this file
    also contains the checkpoint class which is used as a building
    block for constructing the maze (along with the edge class).
 */


#include "data.h"

#ifndef MAX
    #define MAX 1000
#endif


/********************************************************************
                            Misc Functions
********************************************************************/
//Welcome user and explain the program
//INPUT: void
//OUTPUT: void
void welcome(void)
{
    for (int i = 0; i < 50; ++ i) cout << endl;	//Clear screen
    cout << "\t\t*** Welcome to multiplayer maze game ***"
	 << "\n\tIn this game, you can work in teams to solve a small maze"
	 << "\n\tThe game is over when two players reach the finish"
	 << "\n\tThere is also a chance for a reward at each checkpoint"
	 << "\n\n\t\t\tGood Luck!!!\n\n";
    return;
}


//Determines if a user wishes to repeat an action
//INPUT: void
//OUTPUT: true = repeat, false = no repeat
bool again(void)
{
    char selection;				//User selection 

    cout << "\n Again? (y|n): ";
    cin >> selection;				//Get user selection
    cin.ignore(MAX, '\n');
    selection = toupper(selection);		//Refine selection
    if (selection == 'Y')			//If user wants to repeat
	return true;				//Return true for repeat
    else					//If user does not repeat
	return false;				//Return false for no repeat
}
    

//Let user determine how many players are playing the game
//INPUT: void
//OUTPUT: number of players in the game
int number_of_players(void)
{
    int size = 0;				//Number of players in game
    do 
    {	
	cout << "\n Enter the number of players: ";
	cin >> size;				//Get number of players playing
	cin.ignore(MAX, '\n');
	if (size < 2)				//Check for proper player size
	    cerr << "\n ***** There must be at least 2 players\n";
    } while (size < 2);				//Repeat until there are at least 2 players
    return size;				//Return number of players
}


/********************************************************************
                            data CLASS
********************************************************************/
//Data Default Constructor
//INPUT: void
data::data(void):info(NULL), num(-1)
{

}


//Data Char Constructor
//INPUT: Data name to copy
data::data(const char * data_to_add):num(-1)
{    
    if (data_to_add)
    {
	info = new char [strlen(data_to_add) + 1];  //Dynamic memory
	strcpy(info, data_to_add);
    }
    else
	info = NULL;
}


//Data Object Constructor
//INPUT: data to copy
data::data(const data & data_to_add):num(-1)
{
    if (data_to_add.info)
    {
	info = new char [strlen(data_to_add.info) + 1];//Dynamic memory
	strcpy(info, data_to_add.info);
    }
    else
	info = NULL;
}


//Data Destructor
data::~data(void)
{
    if (info) delete [] info;
    info = NULL;
}


//New info to update
//INPUT: data to add
//OUTPUT: void
void data::update(void)
{
    char temp[50];				    //Temp array storage
    if (info) delete [] info;			    //Clear out any garbage
    info = NULL;				    //Set info to NULL
    cout << "\n Player name: ";
    cin.get(temp, MAX, '\n');			    //Get player name
    cin.ignore(MAX, '\n');
    info = new char [strlen(temp) + 1];		    //Allocate a new array
    strcpy(info, temp);				    //Copy new data
    return;					    //Return
}
void data::update(int updated_data)
{
    num = updated_data;				    //Update stored number
    return;					    //Return
}


//Display Data
//INPUT: void
//OUTPUT: void
void data::display(void) const
{
    if (info)					    //If there is info
	cout << " " << info;			    //Output info
    if (num >= 0)				    //Or if there's a number	
	cout << " " << num;			    //Output number
    return;					    //Return
}
void data::displayer(void) const
{
    if (num >= 0)				    //If there is a number
	cout << " " << num + 1;			    //Display the offset
    return;					    //Return
}

//Compare Data (char or int)
//INPUT: data to compare
//OUTPUT: positive if match is larger, negative if match is smaller
int data::compare(const char * match) const
{
    return strcmp(match, info);			    //Return strcmp result
}
int data::compare(const int match) const	    
{
    return match - num;				    //Return intcmp result
}


//Retrieve data
//INPUT: void
//OUTPUT: returns the stored data
char * data::retrieve(void) const
{
    return info;				    //Return stored data
}


//Retrieve number
//INPUT: void
//OUTPUT: number retrieved
int data::get_num(void)
{
    return num;					    //Return stored number
}



/********************************************************************
                                node CLASS
********************************************************************/
//Node Default Constructor
//INPUT: void
node::node():next(NULL)
{

}


//Node Char Constructor
//INPUT: name of node
node::node(const char * data_to_add):
    data(data_to_add), next(NULL)
{

}


//Node Object Constructor
//INPUT: node to copy
node::node(const node & data_to_add):
    data(data_to_add.info)
{
    connect_next(data_to_add.next);
}


//Traverse to next node
//INPUT: void
//OUTPUT: returns the next node by reference
node *& node::go_next(void)
{
    return next;				    //Next node in list
}


//Connect next node
//INPUT: node to connect next to
//OUTPUT: void
void node::connect_next(node * connection)
{   
    next = connection;				    //Connect to next node
    return;					    //Return
}



/********************************************************************
                            list CLASS
********************************************************************/
//List Default Constructor
//INPUT: void
list::list(void):head(NULL)
{

}


//List Char Constructor
//INPUT: name of list member
list::list(const char * item_to_add):head(NULL)
{
    if(add(item_to_add))
	head->connect_next(NULL);
}


//List Object Constructor
//INPUT: list to copy
list::list(const list & list_to_add):head(NULL)
{
    node * current = list_to_add.head;
    while(current)
    {
	add(current->retrieve());		    //Add current's data to list
	head->connect_next(current->go_next());	
	current = current->go_next();
    }
}


//List Destructor
list::~list()
{
    remove_all(head);				    //Recursively deallocate list
}


//Add an item, or number, to a linear linked list
//INPUT: item to add to the list
//OUTPUT: failure = false, success = true
bool list::add(const char * item_to_add)
{
    if(!item_to_add)				    //If nothing to add
	return false;				    //Return false
    node * temp = new node(item_to_add);	    //Create a new node with item to add
    temp->connect_next(head);			    //Connect new item to head
    head = temp;				    //Shift new item into head of list
    return true;				    //Return success
}
bool list::add(int item_to_add)
{
    if(item_to_add < 0)				    //If nothing to add
	return false;				    //Return false
    node * temp = new node;			    //Create a new node with item to add
    temp->update(item_to_add);			    //Add number to head
    temp->connect_next(head);			    //Connect new item to head
    head = temp;				    //Shift new item into head of list
    return true;				    //Return success
}


//[Wrapper] Remove an instances of an item from the list
//INPUT: item to remove from the list
//OUTPUT: failure = false, success = true
bool list::remove(const char * item_to_remove)
{
    if(!item_to_remove)				    //If nothing to remove
	return false;				    //Return failure
    if(!head->compare(item_to_remove))		    //Case 1: Removing from head
    {
	node * next = head->go_next();		    //Hold onto next node in list
	delete head;				    //Remove head
	head = next;				    //Shift the list back
    }
    remove(item_to_remove, head);		    //Case 2: Removing elsewhere
    return true;				    //Return success
}


//Remove item [Wrapper]
//INPUT: item to remove (number)
//OUTPUT: something removed = true, nothing removed = false
bool list::remove(int item_to_remove)
{
    int before = number_of_items();
    if(!head->compare(item_to_remove))		    //Case 1: Removing from head
    {
	node * next = head->go_next();		    //Hold onto next node in list
	delete head;				    //Remove head
	head = next;				    //Shift the list back
    }
    remove(item_to_remove, head);		    //Case 2: Removing elsewhere
    if (before == number_of_items())		    //If nothing was removed
	return false;				    //Return failure
    return true;				    //Otherwise return success
}


//[Wrapper] Display display all data in a linear linked list
//INPUT: void
//OUTPUT: void
void list::display_list(void) const
{
    return display_list(head);			    //Display entire list recursively
}
void list::displayer_list(void) const
{
    return displayer_list(head);		    //Display entire list recursively
}

//[Wrapper] Determine number of items (list length)
//INPUT: void
//OUTPUT: number of items in list
int list::number_of_items(void) const
{
    return number_of_items(head);		    //Recursively determine number of items in list
}


///////////////////////// PROTECTED //////////////////////////
//Remove all list members recursively
//INPUT: head of list to remove
//OUTPUT: void
void list::remove_all(node *& head)
{
    if(!head)					    //If not head
	return;					    //Return
    remove_all(head->go_next());		    //Traverse to end of list
    if(head) delete head;			    //Delete a node
    head = NULL;				    //Set it to NULL
    return;					    //Return
}


//Recursively remove an instance of an item from the list
//INPUT: item to remove, head of list to traverse
//OUTPUT: node pointer for recursion
node * list::remove(const char * item_to_remove, node *& head)
{
    node * next = NULL;				    //Create a next node pointer
    if(!head)					    //If not head
	return NULL;				    //Return NULL
    if(!head->compare(item_to_remove))		    //If match is found
    {				    
	next = head->go_next();			    //Hold onto next node
	delete head;				    //Remove head
	head = NULL;				    //Set head to NULL
	return next;				    //Return next node
    }
    else					    //If a match is not found
    {						    //Traverse next pointer
	head->go_next() = remove(item_to_remove, head->go_next());
        return head;				    //Return head pointer
    }
}


//Recursively remove an instance of an item from the list
//INPUT: item to remove, head of list to traverse
//OUTPUT: node pointer for recursion
node * list::remove(int item_to_remove, node *& head)
{
    node * next = NULL;				    //Create a next node pointer
    if(!head)					    //If not head
	return NULL;				    //Return NULL
    if(!head->compare(item_to_remove))		    //If match is found
    {				    
	next = head->go_next();			    //Hold onto next node
	delete head;				    //Remove head
	head = NULL;				    //Set head to NULL
	return next;				    //Return next node
    }
    else					    //If a match is not found
    {						    //Traverse next pointer
	head->go_next() = remove(item_to_remove, head->go_next());
        return head;				    //Return head pointer
    }
}


//Recursive Display list
//INPUT: head of list to display
//OUTPUT: void
void list::display_list(node * head) const
{
    if (!head)					    //If not head
	return;					    //Return
    head->display();				    //Display node
    return display_list(head->go_next());	    //Traverse list
}
void list::displayer_list(node * head) const
{
    if (!head)					    //If not head
	return;					    //Return
    head->displayer();				    //Display node
    return displayer_list(head->go_next());	    //Traverse list
}


//Recursively determine length of a linear linked list
//INPUT: head of list to examine
//OUTPUT: number of items in the list
int list::number_of_items(node * head) const
{
    if(!head)					    //If not head
	return 0;				    //Return 0
    return number_of_items(head->go_next()) + 1;    //Count items
}



/********************************************************************
                            edge CLASS
********************************************************************/
//edge Default Constructor
//INPUT: void
edge::edge(void):next(NULL), previous(NULL), adjacent(NULL)
{

}


//edge Object Constructor
//INPUT: edge to copy
edge::edge(const edge & data_to_add):
    next(data_to_add.next),
    previous(data_to_add.previous),
    adjacent(data_to_add.adjacent)
{

}


//Traverse next DLL
//INPUT: void
//OUTPUT: next edge to traverse to
edge *& edge::go_next(void)
{
    return next;				    //Next edge		    
}


//Traverse previous DLL
//INPUT: void
//OUTPUT: previous edge to traverse to
edge *& edge::go_previous(void)
{
    return previous;				    //Previous edge
}


//Traverse to adjacent checkpoint
//INPUT: void
//OUTPUT: next edge to traverse to
checkpoint *& edge::go_adjacent(void)
{
    return adjacent;				    //Adjacent checkpoint
}


//Compare adjacent data
//INPUT: number to compare
//OUTPUT: result of comparison (analagous to strcmp)
int edge::compare(int to_compare)
{
    return adjacent->compare(to_compare);	    //Return comparison
}


//Connect next DLL node
//INPUT: next edge to connect
//OUTPUT: void
void edge::connect_next(edge * connection)
{
    next = connection;				    //Set next to connection
    return;					    //Return
}


//Connect previous DLL node
//INPUT: previous edge to connect
//OUTPUT: void
void edge::connect_previous(edge * connection)
{
    previous = connection;			    //Set previous to connection
    return;					    //Return
}


//Display adjacent data
//INPUT: void
//OUTPUT: void
void edge::display_adjacent(void)
{
    cout << "  " << adjacent->get_num();	    //Display adjacent checkpoints 
    return;					    //Return
}


//Assign a checkpoint to the edge
//INPUT: adjacent checkpoint to connect
//OUTPUT: void
void edge::assign_checkpoint(checkpoint * connection)
{
    adjacent = connection;			    //Set adjacent to connection
    return;					    //Return
}



/********************************************************************
			checkpoint CLASS
********************************************************************/
//Checkpoint Default Constructor
//INPUT: void
checkpoint::checkpoint(void):start(false), finish(false)
{
    head = NULL;
}


//Checkpoint Char Constructor
//INPUT: checkpoint name and start/finish information
checkpoint::checkpoint(const char * name, 
		       const bool start, 
		       const bool finish): 
    data(name), start(start), finish(finish)
{
    head = NULL;
}


//Checkpoint Object Constructor
//INPUT: Checkpoint to copy
checkpoint::checkpoint(const checkpoint & checkpoint_to_add): 
    data(checkpoint_to_add.info),
    start(checkpoint_to_add.start),
    finish(checkpoint_to_add.finish)
{
    head = NULL;
}


//Checkpoint destructor
checkpoint::~checkpoint(void)
{
    if (head) remove_all(head);			    //Recursively deallocate list
}


//For checking if at end of maze
//INPUT: void
//OUTPUT: finish = true, not finish = false
bool checkpoint::is_finish(void) const
{
    return finish;				    //Return result
}


//For setting the beginning of the maze
//INPUT: starting point set
//OUTPUT: void
void checkpoint::set_start(const bool set)
{
    start = set;				    //Assign starting checkpoint
    return;					    //Return
}


//For setting the end of the maze
//INPUT: finishing point set
//OUTPUT: void
void checkpoint::set_finish(const bool set)
{
    finish = set;				    //Assign finishing checkpoint
    return;					    //Return
}


//Insert edge into edge list
//INPUT: edge to insert
//OUTPUT:: failure = false, succss = true
bool checkpoint::insert_edge(edge *& to_insert)
{
    if (!to_insert)				    //If nothing to insert
	return false;				    //Return failure
    if (!head)					    //If no edges
	head = to_insert;			    //Set head to edge to insert
    else					    //If there are edges
    {
	to_insert->connect_next(head);		    //Insert new edge into DLL
	head->connect_previous(to_insert);
	head = to_insert;			    //Shift new edge into list
    }
    return true;				    //Return success
}


//[Wrapper] Count the number of items in the DLL (list length)
//INPUT: void
//OUTPUT: number of edges in a DLL list
int checkpoint::number_of_edges(void) const	
{
    return number_of_edges(head);		    //Return number of edges in list
}


//Traverse player to next checkpoint
//INPUT: void
//OUTPUT: user checkpoint selection
int checkpoint::traverse(void)
{
    cout << "\n Available Checkpoint(s): ";
    display_edges(head);			    //Display available options
    return get_selection();			    //Return user selection
}


///////////////////////// PROTECTED //////////////////////////
//Delete the entire edge list recursively
//INPUT: head of list to destroy
//OUTPUT: void
void checkpoint::remove_all(edge *& head)
{
    if (!head)					    //If not head
	return;					    //Return
    remove_all(head->go_next());		    //Traverse to end of list
    if (head) delete head;			    //Delete head of list
    head = NULL;				    //Set head to NULL
    return;					    //Return
}


//Count the number of items in the DLL recursively (list length)
//INPUT: head of list to count
//OUTPUT: number of edges counted
int checkpoint::number_of_edges(edge * head) const
{
    if (!head)					    //If not head
	return 0;				    //Return 0
    return number_of_edges(head->go_next()) + 1;    //Count edges
}


//Display connecting edges
//INPUT: edge list to display
//OUTPUT: void
void checkpoint::display_edges(edge * head)
{
    if (!head)					    //If nothig to display
	return;					    //Return
    head->display_adjacent();			    //Dipslay adjacent checkpoint
    display_edges(head->go_next());		    //Traverse to next edge
    return;					    //Return
}


//User direction choice
//INPUT: void
//OUTPUT: user selection
int checkpoint::get_selection(void)
{
    bool valid = false;				    //Loop control variable
    int selection = -1;				    //User selection
    do
    {
	cout << "\n Please enter the checkpoint to travel to: ";
	cin >> selection;			    //Get user selection
	cin.ignore(MAX, '\n');
	check_selection(selection, head, valid);    //Validate selection
    } while (!valid);				    //Repeat until valid
    return selection;				    //Return user selection
}


//Check user's selection
//INPUT: user selection, list to scan, and validity
//OUTPUT: void
void checkpoint::check_selection(int selection,
				 edge * head, 
				 bool & valid)
{
    if (!head)					    //If at end of list
	return;					    //Return
    check_selection(selection, head->go_next(), valid);//Travese
    if(!head->compare(selection))		    //If user picked a connecting edge
	valid = true;				    //Selection is valid
    return;					    //Return
}




