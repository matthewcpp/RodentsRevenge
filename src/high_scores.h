#ifndef RR_HIGH_SCORES_H
#define RR_HIGH_SCORES_H

#include <stddef.h>

typedef struct rrHighScores rrHighScores;

rrHighScores* rr_high_scores_create();
void rr_high_scores_destroy(rrHighScores* high_scores);

int rr_high_scores_load_file(rrHighScores* high_scores, const char* path);
int rr_high_scores_write_file(rrHighScores* high_scores, const char* path);
size_t rr_high_scores_count(rrHighScores* high_scores);
void rr_high_scores_clear(rrHighScores* high_scores);
int rr_high_scores_score_is_high(rrHighScores* high_scores, int score);
int rr_high_scores_add(rrHighScores* high_scores, int score, const char* name);

#endif
