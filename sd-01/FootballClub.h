#ifndef FOOTBALL_CLUB_H_D
#define FOOTBALL_CLUB_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// macro for memory allocation 
#define CheckAllocation(ptr) if(ptr == NULL){fprintf(stderr, "Memory allocation\
 failed at line %d\nProgram will exit...\n", __LINE__ - 1); exit(-1);}

// List node which contains information about one football player.
typedef struct Player {
	char *name;					// player's name
	char *position;				// player's game position
	int score;					// player's score
	int injured;				// indicator for injury (1 true, 0 false)
	struct Player *next;		// next list node
	struct Player *prev;		// previous list node
} Player;

// Structure which contains information about one football club.
typedef struct FootballClub {
	char *name;					// club's name
	Player *players;			// list with players in good shape
	Player *injured_players;	// list with injured players
	struct FootballClub *next;	// next list node
} FootballClub;

/******************************************************************************/
/****************************Helper functions**********************************/
/******************************************************************************/

FootballClub* init_club(char* name)
{
	FootballClub* newClub = (FootballClub*)malloc(sizeof(FootballClub));
	CheckAllocation(newClub);

	// allocate name
	newClub->name = (char*)malloc(sizeof(char) * strlen(name) + 1);
	CheckAllocation(newClub->name);
	strcpy(newClub->name, name);

	newClub->players = NULL;
	newClub->injured_players = NULL;

	return newClub;
}

Player* init_player(char *player_name, char *position, int score)
{
	Player* newPlayer = (Player*)malloc(sizeof(Player));
	CheckAllocation(newPlayer);

	// allocate name
	newPlayer->name = (char*)malloc(sizeof(char) * strlen(player_name) + 1);
	CheckAllocation(newPlayer->name);
	strcpy(newPlayer->name, player_name);

	// allocate position
	newPlayer->position = (char*)malloc(sizeof(char)*strlen(position) + 1);
	CheckAllocation(newPlayer->position);
	strcpy(newPlayer->position, position);

	newPlayer->score = score;
	newPlayer->injured = 0;
	newPlayer->next = NULL;
	newPlayer->prev = NULL;

	return newPlayer;
}

/*
 *	If player1 is "smaller" than player2 we return 1
 * 	else return 0 
 */ 
int is_smaller_by_pos(Player* player1, Player* player2)
{
	int posCmp = strcmp(player1->position, player2->position);
	int scoreDiff = player1->score - player2->score;
	int nameCmp = strcmp(player1->name, player2->name);

	// compare in the specified order
	if(posCmp < 0)
		return 1;
	else if(posCmp == 0 && scoreDiff > 0)
			return 1;
	else if(posCmp == 0 && scoreDiff == 0 && nameCmp < 0)
			return 1;

	return 0;	
}

int is_smaller_by_name(Player* player1, Player* player2)
{
	return strcmp(player1->name, player2->name);	
}

Player* get_player(Player* list, char* player_name)
{
	Player* currentPlayer = list;

	// iterate over all players
	while(currentPlayer && strcmp(player_name, currentPlayer->name) != 0)
		currentPlayer = currentPlayer->next;

	return currentPlayer;
}


/*
 * Inserts player into list using the given function
 * If function is NULL the default function used is is_smaller_by_pos
 */ 
Player* insert_player(Player* playerList, Player* player, 
					  int (*cmp)(Player*, Player*))
{
	player->next = NULL;
	player->prev = NULL;
	
	if(playerList == NULL)
	{
		playerList = player;
		return playerList;
	}

	if(cmp == NULL) cmp = is_smaller_by_pos;

	Player* currentPlayer = playerList;
	Player* newPlayer = player;
	while(currentPlayer != NULL)
	{
		if(cmp(newPlayer, currentPlayer) == 1)
		{
			newPlayer->next = currentPlayer;
			newPlayer->prev = currentPlayer->prev;
			// not the first item
			if(currentPlayer->prev != NULL)
			{
				currentPlayer->prev->next = newPlayer;
				currentPlayer->prev = newPlayer;
				return playerList;
			}

			currentPlayer->prev = newPlayer;	
					
			// first item 
			return newPlayer;
		}
		else 
		{
			// we got to the last element and we haven't inserted
			// break and insert at the end
			if(currentPlayer->next == NULL) break;
			currentPlayer = currentPlayer->next;		
		}
	}
	newPlayer->prev = currentPlayer;
	newPlayer->next = NULL;
	currentPlayer->next = newPlayer;

	return playerList;
}

/*
 * It only destroyes the links
 * The memory is not freed
 */ 
void remove_player_from_list(Player** listHead, Player* currentPlayer)
{
	if(currentPlayer == NULL) return;

	// if it is not the first element
	if(currentPlayer->prev != NULL)
		currentPlayer->prev->next = currentPlayer->next;
	else
		*listHead = currentPlayer->next; // delete first elem, change head

	// if it is not the last element
	if(currentPlayer->next != NULL)
		currentPlayer->next->prev = currentPlayer->prev;

	currentPlayer->next = NULL;
	currentPlayer->prev = NULL;

	return;
}

FootballClub* get_club(FootballClub* clubs, char* name)
{
	// iterate over all the clubs
	while(clubs && strcmp(name, clubs->name) != 0)
		clubs = clubs->next;

	return clubs;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

FootballClub *initialize_clubs(int clubs_no, char **names) 
{
	if(clubs_no <= 0) return NULL;

	// create inital club, aka list head 
	FootballClub* clubs = init_club(names[0]);

	clubs->next = NULL;

	// add the rest of the clubs
	FootballClub* current = clubs;
	for(int i = 1; i < clubs_no; i++)
	{
		FootballClub* newClub = init_club(names[i]);

		newClub->next = NULL;
		current->next = newClub;
		current = newClub;
	}

	return clubs;
}

FootballClub *add_club(FootballClub *clubs, char *name) 
{
	// if no clubs
	if(clubs == NULL)
	{
		clubs = init_club(name);

		clubs->next = NULL;
		
		return clubs;
	}

	FootballClub* lastClub = clubs;

	// get the last club
	while(lastClub->next != NULL)
		lastClub = lastClub->next;
	
	// create a new club and insert it at the end
	FootballClub* newClub = init_club(name);

	newClub->next = NULL;
	lastClub->next = newClub;

	return clubs;
}

void add_player(FootballClub *clubs, char *club_name, 
				char *player_name, char *position, int score) 
{
	if(clubs == NULL) return;
	
	// get the player's club
	clubs = get_club(clubs, club_name);
	if(clubs == NULL) return;

	// create the player and add it to the list
	Player* newPlayer = init_player(player_name, position, score);
	clubs->players = insert_player(clubs->players, newPlayer, NULL);

	return;
}

void remove_player(FootballClub *clubs, char *club_name, char *player_name) 
{
	clubs = get_club(clubs, club_name);
	if(clubs == NULL) return;

	// get the player and set the list head accordingly
	Player* currentPlayer = get_player(clubs->players, player_name);
	Player** listHead = &clubs->players;

	// check if the players is in the injured list
	if(currentPlayer == NULL)
	{
		currentPlayer = get_player(clubs->injured_players, player_name);
		if(currentPlayer == NULL) return;
		listHead = &clubs->injured_players;
	}
	
	remove_player_from_list(listHead, currentPlayer);

	// free the memeory
	free(currentPlayer->name);
	free(currentPlayer->position);
	free(currentPlayer);

	return;
}

void transfer_player(FootballClub *clubs, char *player_name, 
					char *old_club, char *new_club) 
{
	FootballClub* newClub = get_club(clubs, new_club);
	if(newClub == NULL) return;

	clubs = get_club(clubs, old_club);
	if(clubs == NULL) return;

	// check if player is in the uninjured list
	Player* currentPlayer = get_player(clubs->players, player_name);
	Player** oldClubPlayers = &clubs->players;
	Player** newClubPlayers = &newClub->players;

	// check if the players is in the injured list
	if(currentPlayer == NULL)
	{
		currentPlayer = get_player(clubs->injured_players, player_name);
		if(currentPlayer == NULL) return;
		oldClubPlayers = &clubs->injured_players;
		newClubPlayers = &newClub->injured_players;
	}
	
	// remove him from the right list and insert it in the correct one
	remove_player_from_list(oldClubPlayers, currentPlayer);
	if(*newClubPlayers != newClub->injured_players)
		*newClubPlayers = insert_player(*newClubPlayers, currentPlayer, NULL);
	else
		*newClubPlayers = insert_player(*newClubPlayers, currentPlayer, 
										is_smaller_by_name);

	return;
}

void update_score(FootballClub *clubs, char *club_name, 
					char *player_name, int score) 
{
	clubs = get_club(clubs, club_name);
	if(clubs == NULL) return;

	// check if the players is in the uninjured list
	Player* currentPlayer = get_player(clubs->players, player_name);
	Player** listHead = &clubs->players;

	// check if the players is in the injured list
	if(currentPlayer == NULL)
	{
		currentPlayer = get_player(clubs->injured_players, player_name);
		if(currentPlayer == NULL) return;
		listHead = &clubs->injured_players;
	}

	// update score
	currentPlayer->score = score;

	// reinsert him in the list so that we keep the sorted property
	remove_player_from_list(listHead, currentPlayer);
	if(*listHead != clubs->injured_players)
		*listHead = insert_player(*listHead, currentPlayer, NULL);
	else
		*listHead = insert_player(*listHead, currentPlayer, is_smaller_by_name);
	
	return;
}

void update_game_position(FootballClub *clubs, char *club_name, 
							char *player_name, char *position, int score) 
{
	clubs = get_club(clubs, club_name);
	if(clubs == NULL) return;

	// check if the players is in the uninjured list
	Player* currentPlayer = get_player(clubs->players, player_name);
	Player** listHead = &clubs->players;

	// check if the players is in the injured list
	if(currentPlayer == NULL)
	{
		currentPlayer = get_player(clubs->injured_players, player_name);
		if(currentPlayer == NULL) return;
		listHead = &clubs->injured_players;
	}

	// allocate new memory for the new possition
	currentPlayer->score = score;
	free(currentPlayer->position);
	currentPlayer->position = (char*)malloc(sizeof(char)*strlen(position) + 1);
	CheckAllocation(currentPlayer->position);

	strcpy(currentPlayer->position, position);

	// reinsert him in the list so that we keep the sorted property
	remove_player_from_list(listHead, currentPlayer);
	if(*listHead != clubs->injured_players)
		*listHead = insert_player(*listHead, currentPlayer, NULL);
	else
		*listHead = insert_player(*listHead, currentPlayer, is_smaller_by_name);

	return;
}

void add_injury(FootballClub *clubs, char *club_name,
				char *player_name, int days_no) 
{
	clubs = get_club(clubs, club_name);
	if(clubs == NULL) return;

	Player* currentPlayer = get_player(clubs->players, player_name);

	if(currentPlayer == NULL) return;

	currentPlayer->injured = 1;
	currentPlayer->score -= 0.1 * days_no;
	if(currentPlayer->score < -100) currentPlayer->score = -100;

	remove_player_from_list(&clubs->players, currentPlayer);
	clubs->injured_players = insert_player(clubs->injured_players, 
							 			   currentPlayer, is_smaller_by_name);

	return;
}

void recover_from_injury(FootballClub *clubs, char *club_name, 
							char *player_name) 
{
	clubs = get_club(clubs, club_name);
	if(clubs == NULL) return;

	Player* currentPlayer = get_player(clubs->injured_players, player_name);

	if(currentPlayer == NULL) return;

	currentPlayer->injured = 0;

	remove_player_from_list(&clubs->injured_players, currentPlayer);
	clubs->players = insert_player(clubs->players, currentPlayer, NULL);

	return;
}

// Frees memory for a list of Player.
void destroy_player_list(Player *player) 
{
	while(player != NULL)
	{
		Player* currentPlayer = player;
		player = player->next; 
		// free dynamic allocated elements
		free(currentPlayer->name);
		free(currentPlayer->position);
		currentPlayer->next = NULL;
		currentPlayer->prev = NULL;
		// free the player itself
		free(currentPlayer);
	}

	return;
}

// Frees memory for a list of FootballClub.
void destroy_club_list(FootballClub *clubs) 
{
	while(clubs != NULL)
	{
		FootballClub* currentClub = clubs;
		clubs = clubs->next;
		// free the name		
		free(currentClub->name);
		// free both lists
		destroy_player_list(currentClub->players);
		destroy_player_list(currentClub->injured_players);	
		free(currentClub);
	}

	return;
}

// Displays a list of players.
void show_list(FILE *f, Player *players, int free_memory) 
{
	fprintf(f, "P: ");
	Player *player = players;
	while (player) {
		fprintf(f, "(%s, %s, %d, %c) ", 
			player->name,
			player->position,
			player->score,
			player->injured ? 'Y' : '_');
		player = player->next;
	}
	if (free_memory) {
		destroy_player_list(players);
	}
	fprintf(f, "\n");
}

// Displays a list of players in reverse.
void show_list_reverse(FILE *f, Player *players, int free_memory) 
{
	fprintf(f, "P: ");
	Player *player = players;
	if (player) {
		while (player->next) {
			player = player->next;
		}
		while (player) {
			fprintf(f, "(%s, %s, %d, %c) ", 
				player->name,
				player->position,
				player->score,
				player->injured ? 'Y' : '_');
			player = player->prev;
		}
	}
	if (free_memory) {
		destroy_player_list(players);
	}
	fprintf(f, "\n");
}


// Displays information about a football club.
void show_clubs_info(FILE *f, FootballClub *clubs) 
{
	fprintf(f, "FCs:\n");
	while (clubs) {
		fprintf(f, "%s\n", clubs->name);
		fprintf(f, "\t");
		show_list(f, clubs->players, 0);
		fprintf(f, "\t");
		show_list(f, clubs->injured_players, 0);
		clubs = clubs->next;
	}
}

#endif // FOOTBALL_CLUB_H_INCLUDED