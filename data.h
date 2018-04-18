//Alex Olson		CS202		Assignment 1
//data.h		4/7/2017
/*
    This file is the header file for the maze game program. It delcares the libraries
    used in the rest of the program as well as a constant, the standard namespace, and
    function prototypes that are used throughout the program. This file also contains
    declarations for all classes used. The classes were split into two general 
    catagories: dataStructures and dataModules. The classes in the dataStructures group
    are generally smaller, building block classes that don't really do much on their
    own. The dataModule group uses the dataStructures to build layers of abstract data
    types that run the program. The highest level manager is the "maze_game" class and
    the "data" class was a fairly common base class.
 */


#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iostream>

using namespace std;
const int MAX = 1000;				//Primary usage in .ignore() functions

//Prototypes
void welcome(void);				//Clear screen and welcome user
bool again(void);				//Get user's decision to repeat a task
int number_of_players(void);			//Get number of players participating

/********************************************************************
                            dataStructures
********************************************************************/
//Dynamic character array class
class data
{
    public:
	data(void);				//Constructor
	data(const char * info);		//Char constructor
	data(const data & input);		//Copy constructor
	~data(void);				//Destructor
	void update();				//Add char data
	void update(int updated_data);		//Add integer data
	void display(void) const;		//Display info
	void displayer(void) const;		//Display player number offset
	int compare(const char * match) const;	//Compare info
	int compare(const int match) const;	//Compare numeric info
	char * retrieve(void) const;		//Retrieve info
	int get_num();				//Retrieve num
    
    protected:
	char * info;				//Dynamic char array
	int num;				//For number storage
};


//Node class for use in linear linked lists
class node: public data
{
    public:
	node(void);				//Constructor
	node(const char * data_to_add);		//Char constructor
	node(const node & data_to_add);		//copy constructor
	node *& go_next(void);			//Traverse next
	void connect_next(node * connection);	//Makes next connection
	
    protected:
	node * next;				//Next node pointer
};


//List class for managing linear linked lists of things
class list
{
    public:
	list(void);				//Constructor
	list(const char * item_to_add);		//Char constructor
	list(const list & list_to_add);		//Copy constructor
	~list(void);				//Destructor
	bool add(const char * item_to_add);	//Add an item	
	bool add(int number);			//Add a number
	bool remove(const char* item_to_remove);//Remove item [Wrapper]
	bool remove(int to_remove);		//Remove item [Wrapper]
	void display_list(void) const;		//Display list [Wrapper]
	void displayer_list(void) const;	//Display list [Wrapper]
	int number_of_items(void) const;	//List length [Wrapper]

    protected:
	node * head;				//Head of the LLL
	void remove_all(node *& head);		//Delete list
	node * remove(const char * item_to_remove, 
		      node *& head);            //Delete item
	node * remove(int to_remove, node *& head);//Delete numeric item
	void display_list(node * head) const;	//Display entire list
	void displayer_list(node * head) const;	//Display entire list
	int number_of_items(node * head) const;	//Number of list items
};


//Edge class for discribing connections between checkoints
class edge
{
    public:
	edge(void);				//Constructor
	edge(const edge & data_to_add);		//Copy constructor
	edge *& go_next(void);			//Traverse next
	edge *& go_previous(void);		//Traverse previous
	class checkpoint *& go_adjacent(void);	//Traverse adjacent
	int compare(int to_compare);		//Compare adjacent data
	void connect_next(edge * connection);	//Makes next connection
	void connect_previous(edge * connection);//Makes prev connection
	void display_adjacent(void);		//Display adjacent data
	void assign_checkpoint(checkpoint * connection);//Associate edge
						//with a primary checkpoint
    protected:
	edge * next;				//Next edge pointer
	edge * previous;			//Previous edge pointer	
	checkpoint * adjacent;			//Adjacent checkpoint
};


//Checkpoints are the "stops" in the maze connected together by edges
class checkpoint: public data
{
    public:
	checkpoint(void);			//Constructor
	checkpoint(const char * name,		//Char constructor
		   const bool start,
		   const bool finish);
	checkpoint(const checkpoint & checkpoint_to_add);//Copy constructor
	~checkpoint(void);			//Destructor
	bool is_finish(void) const;		//Check if finish
	void set_start(const bool set);		//Flag beginning
	void set_finish(const bool set);	//Flag finish
	bool insert_edge(edge *& to_insert);	//Insert into edge list	
	int number_of_edges(void) const;	//List length [Wrapper]
	int traverse(void);			//Traversal selection

    protected:
	edge * head;				//Edge list head pointer
	bool start;				//Beginning checkpoint
	bool finish;				//Ending checkpoint
	void remove_all(edge *& head);		//Delete edge list
	int number_of_edges(edge * head) const;	//List length
	void display_edges(edge * head);	//Output connecting edges
	int get_selection(void);		//User direction choice
	void check_selection(int selection,	//Check user's selection
			     edge * head,
			     bool & valid);
};



/********************************************************************
                            dataModules
********************************************************************/
//Table ADT that generates the maze
class maze
{
    public:
	maze(int size = 20);			//Maze constructor
	~maze(void);				//Destructor	
	bool is_finish(int number) const;	//Check if finished
	int current_checkpoint(int number);	//Get current checkpoint
	int traverse_player(int number);	//User movement decision

    protected:
	checkpoint * maze_checkpoint;		//Adjacency list
	int maze_size;				//Maze size
	int dead_ends;				//Number of dead ends
	int start;				//Starting checkpoint
	int finish;				//Finish checkpoint
	void generate(void);			//Randomly generate maze
	void find_dead_ends(void);		//Determine dead ends
	bool check_random(int & a, int & b);	//Filter random numbers
};


//Player participating in game
class player: public data
{
    public:
	player(void);				//Default constructor
	player(const char * name_to_add);	//Char constructor
	player(const player & player_to_add);	//Copy constructor
	void log_checkpoint(int current_checkpoint);//Log player's current checkpoint
	void checkpoint_history(void);		//Show visited checkpoint
	int player_location(void);		//Get player's current location
	void add_teammate(int player);		//Add a teammate to the player's list
	void add_reward(int reward);		//Add a reward to list
	bool remove_reward(int reward);		//Remove a reward
	void display_rewards(void);		//Display player rewards
	void display_team(void);		//Display teammates
	int total_rewards(void);		//Track rewards list

    protected:
	int location;				//Player location
	list checkpoints_visited;		//Checkpoints player has visited
	list teamlist;				//Player teammates
	list rewards;				//Player rewards
};


//Rewards List
class reward : public data
{
    public:
	reward(void);				//Constructor
	int reward_spin(void);			//Spin for a reward

    protected:
	list rewards;				//Available rewards
};


//Class the manages players playing through the maze
class maze_game: public maze
{
    public:
	maze_game(int number_of_players);	//Default constructor
	~maze_game(void);			//Destructor
	void player_turn(int player_to_move);	//Move player through the maze
	bool is_finish(int player) const;	//Check if at finish line
	void add_teammate(int player, int teammate);//Let player add teammates
	int use_reward(int player);		//Let player use reward
	void show_teams(void);			//Display teams
	void show_hints(void);			//Display hints about maze

    protected:
	int active_players;			//Number of players in game
	player * the_players;			//Dynamic list of players in the game
	reward game_rewards;			//Rewards system
};

