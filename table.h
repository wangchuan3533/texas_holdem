#ifndef _TABLE_H
#define _TABLE_H

#include "list.h"
#include "card.h"
#include "player.h"

#define TABLE_MAX_PLAYERS 16
#define MIN_PLAYERS 3
#define MAX_TABLES 1024

typedef enum table_state_e {
    TABLE_STATE_WAITING,
    TABLE_STATE_PREFLOP,
    TABLE_STATE_FLOP,
    TABLE_STATE_TURN,
    TABLE_STATE_RIVER,
    TABLE_STATE_SHOWDOWN,
} table_state_t;

typedef struct table_s {
    table_state_t state;
    deck_t deck;
    card_t community_cards[5];

    player_t *players[TABLE_MAX_PLAYERS];
    int num_players;
    int dealer;
    int turn;
    int num_playing;

    int pot;
    int bid;
    int small_blind;
    int big_blind;
    int minimum_bet;
} table_t;

#define current_player(t) ((t)->players[(t)->turn])
int add_player(table_t *table, player_t *player);
int del_player(table_t *table, player_t *player);
int next_player(table_t *table, int index);
int player_fold(player_t *player);
int player_bet(player_t *player, int bid);

void table_init(table_t *table);
int handle_table(table_t *table);
void table_pre_flop(table_t *table);
void table_flop(table_t *table);
void table_turn(table_t *table);
void table_river(table_t *table);
void table_showdown(table_t *table);
int table_check_winner(table_t *table);
int table_to_json(table_t *table, char *buffer, int size);
table_t *available_table();

void broadcast(table_t *table, const char *fmt, ...);
void send_msg(player_t *player, const char *fmt, ...);

extern char g_table_report_buffer[4096];
#define report(table) do {\
    table_to_json((table), g_table_report_buffer, sizeof(g_table_report_buffer));\
    broadcast((table), "[\"update\",%s]\n", g_table_report_buffer);\
} while (0)

#endif
