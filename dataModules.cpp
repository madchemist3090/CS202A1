// Alex Olson		CS202		Assignment 1
// dataModules.cpp	4/10/2017
/*
    This file contain the more compound data types used to build the maze 
    and allow players to play the maze game. The maze class constructs the
    maze out of edge and checkpoint building blocks. The maze_game class
    IS a maze that HAS a list of players and rewards. The maze_game class
    also tracks the movement of players throughout the maze and randomly
    allocates rewards as well. The rewards class is very simple and if I
    had more time, I could probably have combined it with the maze_game 
    class or something like that. Most of the functionality of the program
    will come from the application program calling functions from the
    maze_game class which then delegates the work to most of the other 
    classes.
 */

#include "data.h"

#ifndef MAX_DEAD_ENDS
    #define MAX_DEAD_ENDS 4
#endif

/********************************************************************
			    maze CLASS
********************************************************************/
//Maze Default Constructor
//INPUT: size of maze
maze::maze(int size): maze_size(size), dead_ends(0)
{
    int i;
    maze_checkpoint = new checkpoint[maze_size];    //Create and array of checkpoints
    for (i = 0; i < maze_size; ++i)		    //Iterate through checkpoints
	maze_checkpoint[i].update(i);		    //Update checkpoint data
    generate();					    //Randomly generate maze
}


//Maze Destructor
maze::~maze(void)
{
    if(maze_checkpoint) delete [] maze_checkpoint;
    maze_checkpoint = NULL;
}


//Check if at finish
//INPUT: player's current checkpoint number
//OUTPUT: true if at finish line, false otherwise
bool maze::is_finish(int number) const
{
    if (number == finish)			    //If at finish
	return true;				    //Return true
    return false;				    //Else return false
}

//Find current checkpoint
//INPUT: void
//OUTPUT: maze checkpoint number
int maze::current_checkpoint(int number)
{
    return maze_checkpoint[number].get_num();	    //Return checkpoint number
}


//Moves players in maze
//INPUT: player location
//OUTPUT: location to traverse to
int maze::traverse_player(int number)
{
    return maze_checkpoint[number].traverse();	    //Traverse player
}



///////////////////////// PROTECTED //////////////////////////
//Generates edges connecting the checkpoints in the maze
//INPUT: void
//OUTPUT: void
void maze::generate(void)
{ 
    bool stop = false;				    //Loop stopping condition
    int a, b;					    //For holding random numbers
    srand(time(NULL));				    //Random seed
        
    do						    //Attempt to insert until 
    {						    //dead end requirement met
	a = rand()%maze_size;			    //Random number ranging from 0 to 19
	b = rand()%maze_size;			    //Random number ranging from 0 to 19
	if (check_random(a, b))			    //If the connection checks out
	{
	    edge * temp1 = new edge;		    //Allocate new connection
	    edge * temp2 = new edge;		    //Allocate connection back
	    temp1->assign_checkpoint(&maze_checkpoint[b]);  //Assign connection to
	    temp2->assign_checkpoint(&maze_checkpoint[a]);  //Assign connection from
	    maze_checkpoint[a].insert_edge(temp1);  //Assign edge to checkpoint
	    maze_checkpoint[b].insert_edge(temp2);  //Assign edge to checkpoint
	    find_dead_ends();			    //Recheck dead ends in maze
	}
    } while (dead_ends > MAX_DEAD_ENDS);	    //While over dead end requirement
    do						    //Randomly select a start at a dead end
    {
	a = rand()%maze_size;			    //Random checkpoint index
	if (maze_checkpoint[a].number_of_edges() == 1)//If dead ends meet requirement
	{
	    maze_checkpoint[a].set_start(true);	    //Assign start
	    start = a;				    //Record starting location
	    stop = true;			    //Signal for stopping condition
	}
    } while (!stop);
    stop = false;				    //Reset stopping condition
    do						    //Randomly select a finish at a dead end
    {
	a = rand()%maze_size;			    //Random checkpoint index
	if (maze_checkpoint[a].number_of_edges() == 1 && a != start) //If dead end
	{
	    maze_checkpoint[a].set_finish(true);    //Assign finish
	    finish = a;				    //Record finishing location
	    stop = true;			    //Signal for stopping condition
	}
    } while (!stop);

    
    return;					    //Return
}


//Find the number of dead ends in a maze and update data member
//INPUT: void
//OUTPUT: void
void maze::find_dead_ends(void)
{
    int i;					    //Loop counter
    dead_ends = 0;				    //Reset dead ends to zero before updating
    
    for(i = 0; i < maze_size; ++i)		    //Increment dead ends if <= 1 edges
    {						    
	if (maze_checkpoint[i].number_of_edges() <= 1)//If dead end
	    ++dead_ends;			    //Increment dead ends
    }
    return;					    //Return
}


//Filter random numbers for graph generation
//INPUT: numbers to check
//OUTPUT: false if negative input, true otherwise
bool maze::check_random(int & a, int & b)
{
    if ((a < 0) || (b < 0))			    //If random numbers are negative
	return false;				    //Return failure
    bool stop = false;				    //Initialize stopping condition
    while(!stop)
    {
	a = rand()%maze_size;			    //Randomize a
	b = rand()%maze_size;			    //Randomize b
	if ((a != b) &&				    //If a and b have the right properties
	    (maze_checkpoint[a].number_of_edges() < 4) && 
	    (maze_checkpoint[b].number_of_edges() < 4))
	    stop = true;			    //Set stopping condition
    }
    return true;				    //Return true
}


/********************************************************************
			    player CLASS
********************************************************************/
//Player Default Constructor
player::player(void):
    location(0), checkpoints_visited(NULL), 
    teamlist(NULL), rewards(NULL)
{

}


//Player Char Constructor
player::player(const char * name_to_add):
    data(name_to_add), location(0), checkpoints_visited(NULL), 
    teamlist(NULL), rewards(NULL)
{

}


//Player Object Constructor
player::player(const player & player_to_add): 
    data(player_to_add.info), location(0), checkpoints_visited(NULL), 
    teamlist(NULL), rewards(NULL)
{

}


//Keep track of visited checkpoint
//INPUT: checkpoint to add
//OUTPUT: void
void player::log_checkpoint(int current_checkpoint)
{
    location = current_checkpoint;		    //Update location
    
    if(!checkpoints_visited.add(current_checkpoint))//If not adding
	cerr << "\n Nothing was added";
    return;					    //Return
}


//Show checkpoints visited
//INPUT: void
//OUTPUT: void
void player::checkpoint_history(void)		    //Show maze history
{
    cout << "\n Checkpoints visited: ";
    return checkpoints_visited.display_list();	    //Display list
}


//Return checkpoint that a player is occupying
//INPUT: void
//OUTPUT: player location (array index)
int player::player_location(void)
{
    return location;				    //Return index
}


//Add a player to your team
//INPUT: player to add to you team
//OUTPUT: void
void player::add_teammate(int player)
{
    teamlist.add(player);			    //Add teammate to player list
    return;					    //Return
}


//Add a reward to list
//INPUT: reward number to add
//OUTPUT: void
void player::add_reward(int reward)
{
    rewards.add(reward);			    //Add rewards
    return;					    //Return
}

//Remove a reward from the list
//INPUT: reward to remove
//OUTPUT: something removed = true, nothing removed = false
bool player::remove_reward(int reward)
{
    if (rewards.remove(reward))			    //If item removed
	return true;				    //Return success
    return false;				    //Or return failure
}


//Display player rewards
//INPUT: void
//OUTPUT: void
void player::display_rewards(void)
{
    cout << "\n ***Choices:\n 0 = Gain a turn\t1 = Lose a turn\n 2 = Store reward";
    cout << "\n\n Player Rewards: ";
    return rewards.display_list();		    //Display rewards
}


//Display player teammates
//INPUT: void
//OUTPUT:void
void player::display_team(void)
{
    cout << "\n TEAM PLAYERS: ";
    return teamlist.displayer_list();		    //Display team
}


//Track rewards list
//INPUT: void
//OUTPUT: number of rewards
int player::total_rewards(void)
{
    return rewards.number_of_items();		    //Return item total
}



/********************************************************************
			    reward CLASS
********************************************************************/
//Reward Default Constructor
//INPUT: void
reward::reward(void)
{
    rewards.add("Gain a turn");			    //Reward 0
    rewards.add("Lose a turn");			    //Reward 1
}


//Spin to get a reward
//INPUT: void
//OUTPUT: reward number
int reward::reward_spin(void)
{
    int ret = -1;				    //Return value
    srand(time(NULL));				    //Seed random
    
    if(rand()%9 > 6)				    //33% chance to get reward
    {
	if(rand()%2)				    //If reward 1
	    ret = 1;				    //Return 1
	else					    //If reward 0
	    ret = 0;				    //Return 0
    }
    return ret;					    //Return reward number
}




/********************************************************************
			    maze_game CLASS
********************************************************************/
//Maze Game Default Constructor
maze_game::maze_game(int number_of_players):active_players(number_of_players)
{
    the_players = new player[number_of_players];    //Create dynamic array of players
    for (int i = 0; i < number_of_players; ++i)
    {
	cout << "\n ===> PLAYER " << i + 1; 
	the_players[i].update();		    //Add player info
	the_players[i].log_checkpoint(start);	    //Set player starting point
    }
}


//Maze Game Destructor
maze_game::~maze_game(void)
{
    if(the_players)
	delete [] the_players;
    the_players = NULL;
}


//Helper function for moving players about the maze
//INPUT: player number to move
//OUTPUT: void
void maze_game::player_turn(int player_to_move)
{
    int reward;
    int from = the_players[player_to_move].player_location();//Record where player is moving from
    int to = traverse_player(from);		    //Location player is moving to
    the_players[player_to_move].log_checkpoint(to); //Log new checkpoint as player's current checkpoint
    the_players[player_to_move].checkpoint_history();//Show history   ************adjust******************************
    reward = game_rewards.reward_spin();
    if (reward >= 0)
    {
	cout << "\n A reward was added to your list";
	the_players[player_to_move].add_reward(reward);//Add reward
    }
    return;					    //Return
}


//Checks if a player has reached the finish
//INPUT: player to check
//OUTPUT: true if at finish, false otherwise
bool maze_game::is_finish(int player) const
{						    //Check if player is at finish
    return maze::is_finish(the_players[player].player_location());
}


//Add a team mate for a particular player
//INPUT: primary player, teammate to add
//OUTPUT: void
void maze_game::add_teammate(int player, int teammate)
{
    the_players[player].add_teammate(teammate);	    //Add teammate to player team
    return;					    //Return
}


//Let player use a stored reward
//INPUT: player number
//OUTPUT: reward to use
int maze_game::use_reward(int player)
{
    if (the_players[player].total_rewards() < 1)    //If the player has no rewards
	return -1;				    //Return failure
    int selection;				    //User selection
    the_players[player].display_rewards();	    //Display rewards
    cout << "\n Pick a reward from your list: ";
    cin >> selection;				    //Get reward choice
    cin.ignore(MAX, '\n');
    if(the_players[player].remove_reward(selection))//If the player has the reward
	return selection;			    //Return reward number
    return -1;					    //Else return fail
}
	

//Display current teams
//INPUT: void
//OUTPUT: void
void maze_game::show_teams(void)
{
    int i;					    //Loop counter
    cout << "\n\n*********************************************";


    for (i = 0; i < active_players; ++i)	    //Loop through players
    {
	cout << "\n\n ===> PLAYER " << i + 1 << " TEAMMATES";
	the_players[i].display_team();		    //Display player team
    }
    return;					    //Return
}


//Display hints about maze
//INPUT: void
//OUTPUT: void
void maze_game::show_hints(void)
{
    cout << "\n\n\n ************** Location Hints **************"
	 << "\n   Starting Location: " << start 
	 << "\n  Finishing Location: " << finish << "\n";
    return;					    //Return
}


