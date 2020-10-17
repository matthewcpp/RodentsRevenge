#ifndef RR_SCOREBOARD_H
#define RR_SCOREBOARD_H

#define SCOREBOARD_SIZE 10

#include "basic_sprite.h"
#include "../high_scores.h"

typedef struct {
    rrHighScores* _high_scores;
    rrRenderer* _renderer;
    int _needs_refresh;

    rrUiBasicSprite names[SCOREBOARD_SIZE];
    rrUiBasicSprite scores[SCOREBOARD_SIZE];
} rrUiScoreboard;

void rr_ui_scoreboard_init(rrUiScoreboard* scoreboard, rrHighScores* high_scores, rrRenderer* renderer);
void rr_ui_scoreboard_layout(rrUiScoreboard* scoreboard, rrPoint* position);
void rr_ui_scoreboard_update(rrUiScoreboard* scoreboard);
void rr_ui_scoreboard_draw(rrUiScoreboard* scoreboard);

#endif
