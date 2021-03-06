#ifndef RR_HIGH_SCORES_H
#define RR_HIGH_SCORES_H

#include <stddef.h>

typedef struct rrHighScores rrHighScores;

typedef void (*rrHighScoresChangedCallbackFunc)(void* data);

rrHighScores* rr_high_scores_create(const char* data_dir);
void rr_high_scores_destroy(rrHighScores* high_scores);

/**
 * Loads serialized high score data into memory.
 * @param high_scores
 * @return nonzero value if score data was successfully loaded, otherwise zero.
 */
int rr_high_scores_load(rrHighScores* high_scores);

/**
 * Serializes high score data to storage
 * @return nonzero value if data was successfully serialized, otherwise zero
 */
int rr_high_scores_write(rrHighScores* high_scores);

/**
 * Gets the number of high scores currently loaded.
 */
size_t rr_high_scores_count(rrHighScores* high_scores);

/**
 * Clears all high scores from memory.
 * @see rr_high_scores_write to persist changes.
 */
void rr_high_scores_clear(rrHighScores* high_scores);

/**
 * Gets the high score at given scoreboard index.
 * @param index scoreboard position.
 * @param name out parameter which will receive the name associated with the score.
 * @param score out parameter which will receive the score.
 * @return nonzero value of the scoreboard position was valid, otherwise zero.
 */
int rr_high_scores_get_score(rrHighScores* high_scores, size_t index, char** name, int* score);

/**
 * Indicates if a score is high enough to be added to the board.
 * @param score value to test.
 * @return nonzero value if the score is high enough to be added to the board, otherwise zero.
 */
int rr_high_scores_score_is_high(rrHighScores* high_scores, int score);

/**
 * Attempts to add a new high score to the scoreboard.
 * @param score score to be added.
 * @param name player name associated with the score.
 * @return nonzero value if the score was high enough to be added to the board, otherwise zero.
 */
int rr_high_scores_add(rrHighScores* high_scores, int score, const char* name);

/**
 * Sets a function to be called when a new high score is added.
 * @param func callback function to bind.
 * @param data user supplied data to be passed to bound function.
 */
void rr_high_scores_set_updated_callback(rrHighScores* high_scores, rrHighScoresChangedCallbackFunc func, void* data);

#endif
