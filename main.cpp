// Alex Olson		CS202		Assignment 1
// main.cpp		4/7/2017
/*
    This file is an example of how the application could run. It is
    separated into three sections. The first section is the "PREGAME"
    section which initializes data members, finds out how many players
    are playing, constructs the maze, and organizes the teams. The 
    second section is the "GAME" section which is where the game is
    actually played through a series of nested loops. The final "POSTGAME"
    section lets the user know the results of the game.
 */


#include "data.h"

int main()
{
    //PREGAME
    welcome();					    //Welcome user
    bool finished = false;			    //Control flag
    int i = 0;					    //Loop counter
    int j = 0;					    //Loop counter
    int first_place = -1;			    //Player in first place
    int players_finished = 0;			    //Players that have finished
    int player_number = number_of_players();	    //Number of players playing
    int reward_to_use = -1;			    //Reward use flag
    int user_team = -1;				    //Team number
    
    maze_game the_game(player_number);		    //Construct game with player_number players
    for (i= 0; i < player_number; ++i)		    //Controls player turn sequence
    {
	j = 0;					    //Initialize j
	cout << "\n Player " << i + 1 << ", Add Teamates";
	do					    //Player turn
	{
	    cout << "\n Enter player number: ";
	    cin >> user_team;			    //Get player team mate
	    cin.ignore(MAX, '\n');
	    --user_team;			    //Account for offset
	    if (user_team < 0 || user_team >= player_number)//If invalid
		cerr << "\n *** Please enter a valid player ***\n";
	    else				    //If valid
	    {
		the_game.add_teammate(i, user_team);//Add player to team
		++j;				    //Increment j
		finished = again();
	    }
	} while (finished);			    //Repeat until finished
    }

    //GAME
    the_game.show_teams();			    //Show player teams
    the_game.show_hints();			    //Show start/finish hints
    do
    {
	i = 0;					    //Initialize loop counter
	while (i < player_number)		    //Loop through active players
	{
	    if (i == first_place)		    //Skip over player that won
	    {
		if (i == player_number)		    //If at the end of the array
		    i = 0;			    //Jump back to begging
		else				    //If not at the end
		    ++i;			    //Increment index
	    }
	    cout << "\n\n ********************************************"
		 << "\n	         PLAYER " << i + 1 << " TURN"
		 << "\n ********************************************";
	    reward_to_use = the_game.use_reward(i); //Let player use reward
	    if (reward_to_use >= 0)		    //If using a reward
	    {
		if (reward_to_use == 1)		    //If player wants to skip a turn
		    cout << "\n .... skipping turn";
		else				    //If player wants bonus turn
		{
		    the_game.player_turn(i);	    //Player bonus turn
		    the_game.player_turn(i);	    //Player turn
		}
	    }
	    else				    //If not using a reward
		the_game.player_turn(i);	    //Player standard turn
	    if(the_game.is_finish(i))		    //If the player found the finish
	    {
		++players_finished;		    //Increment players finished
		--player_number;		    //Decrement player count
		if (first_place < 0)		    //If first place has not been set yet
		    first_place = i;		    //Establish first place
	    }
	    ++i;				    //Increment index
	}
    } while (players_finished < 2);		    //While the game is active
    ++first_place;				    //Adjust first place offset

    //POST GAME
    cout << "\n\n ********************************************"
	   << "\n              1ST PLACE: PLAYER " << first_place
	   << "\n              2ND PLACE: PLAYER " << i
	   << "\n ********************************************\n\n";
    
    return 0;
}


