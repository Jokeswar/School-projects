
#ifndef TEAM_EXTRACTOR_H_D
#define TEAM_EXTRACTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FootballClub.h"

/******************************************************************************/
/****************************Helper functions**********************************/
/******************************************************************************/

int is_smaller_by_score(Player* player1, Player* player2)
{
	int scoreDiff = player1->score - player2->score;
	int nameCmp = strcmp(player1->name, player2->name);

	if(scoreDiff > 0) return 1;
	else if(scoreDiff == 0 && nameCmp < 0) return 1;

	return 0;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

Player *union_teams(FootballClub *clubs, char *club_A, char *club_B) 
{
	Player* playerList = NULL;

	FootballClub* clubA = get_club(clubs, club_A);
	FootballClub* clubB = get_club(clubs, club_B);

	if(clubA == NULL || clubB == NULL) return NULL;

	Player* listHead = clubA->players;
	while(listHead != NULL)
	{
		Player* newPlayer = init_player(listHead->name, listHead->position, 
										listHead->score); // clone it
		playerList = insert_player(playerList, newPlayer, NULL); // insert it
		listHead = listHead->next; // go to next
	}

	listHead = clubB->players;
	while(listHead != NULL)
	{
		Player* newPlayer = init_player(listHead->name, listHead->position, 
										listHead->score);
		playerList = insert_player(playerList, newPlayer, NULL);
		listHead = listHead->next;
	}

	return playerList;
}

Player *get_best_player(FootballClub *clubs, char *position) {
	Player* player = NULL;

	while(clubs != NULL)
	{
		Player* listHead = clubs->players;

		// find from where players respect the condition
		while(listHead && strcmp(position, listHead->position) != 0)
			listHead = listHead->next;

		if(listHead)
		{
			if(player)
			{
				if(is_smaller_by_score(listHead, player))
					player = listHead;
			}
			else player = listHead;
		}

		clubs = clubs->next;
	}

	if(player) 
		player = init_player(player->name, player->position, player->score);

	return player;
}

Player *get_top_players(FootballClub *clubs, int N) 
{
	Player* playerList = NULL;

	while(clubs != NULL)
	{
		Player* listHead = clubs->players;
		Player* tmpList = NULL;

		// insert them until they don't
		while(listHead)
		{
			Player* newPlayer = init_player(listHead->name, 
											listHead->position, 
											listHead->score);
			tmpList = insert_player(tmpList, newPlayer, 
										is_smaller_by_score);
			listHead = listHead->next;	
		}

		for(int i = 0; i < N && tmpList; i++)
		{
			Player* newPlayer = tmpList;
			remove_player_from_list(&tmpList, newPlayer);
			playerList = insert_player(playerList, newPlayer, 
										is_smaller_by_score);	
		}

		destroy_player_list(tmpList);
		clubs = clubs->next;
	}

	return playerList;
}

Player *get_players_by_score(FootballClub *clubs, int score) 
{
	Player* playerList = NULL;

	while(clubs != NULL)
	{
		Player* listHead = clubs->players;

		// insert them until they don't
		while(listHead)
		{
			if(listHead->score >= score)
			{
				Player* newPlayer = init_player(listHead->name, 
												listHead->position, 
												listHead->score);
				playerList = insert_player(playerList, newPlayer, 
										   is_smaller_by_score);
			}	
			listHead = listHead->next;
		}

		listHead = clubs->injured_players;

		// insert them until they don't
		while(listHead)
		{
			if(listHead->score >= score)
			{
				Player* newPlayer = init_player(listHead->name, 
												listHead->position, 
												listHead->score);
				playerList = insert_player(playerList, newPlayer, 
										   is_smaller_by_score);
				newPlayer->injured = 1;
			}	
			listHead = listHead->next;
		}

		clubs = clubs->next;
	}

	return playerList;
}

Player* get_players_by_position(FootballClub *clubs, char *position) 
{
	Player* playerList = NULL;

	while(clubs != NULL)
	{
		Player* listHead = clubs->players;

		// find from where players respect the condition
		while(listHead && strcmp(position, listHead->position) != 0)
			listHead = listHead->next;

		// insert them until they don't
		while(listHead && strcmp(position, listHead->position) == 0)
		{
			Player* newPlayer = init_player(listHead->name, 
											listHead->position, 
											listHead->score); // clone it
			// all have the same position, no need for is_smaller_by_score
			// as param
			playerList = insert_player(playerList, newPlayer, NULL);
			listHead = listHead->next; // go to next	
		}

		listHead = clubs->injured_players;

		// find from where players respect the condition
		while(listHead && strcmp(position, listHead->position) != 0)
			listHead = listHead->next;

		// insert them until they don't
		while(listHead && strcmp(position, listHead->position) == 0)
		{
			Player* newPlayer = init_player(listHead->name, 
											listHead->position, 
											listHead->score); // clone it
			// all have the same position, no need for is_smaller_by_score
			// as param
			newPlayer->injured = 1;
			playerList = insert_player(playerList, newPlayer, NULL);
			listHead = listHead->next; // go to next	
		}

		clubs = clubs->next;
	}

	return playerList;
}

void  get_best_N_by_pos(FootballClub* clubs, char* position, int N, 
						  Player** playerList)
{
	Player* players = get_players_by_position(clubs, position);
	for(int i = 0 ; i < N && players; i++)
	{
		Player* currentPlayer = players;
		remove_player_from_list(&players, currentPlayer);
		*playerList = insert_player(*playerList, currentPlayer, 
								   is_smaller_by_score);
	}
	destroy_player_list(players);
}

Player *get_best_team(FootballClub *clubs) 
{
	Player* playerList = NULL;

	get_best_N_by_pos(clubs, "portar", 1, &playerList);
	get_best_N_by_pos(clubs, "fundas", 4, &playerList);
	get_best_N_by_pos(clubs, "mijlocas", 3, &playerList);
	get_best_N_by_pos(clubs, "atacant", 3, &playerList);
	
	return playerList;
}

#endif // TEAM_EXTRACTOR_H_INCLUDED