#include "high_scores.h"

#include "util.h"

#include "cutil/strbuf.h"
#include "cutil/vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HIGH_SCORE_COUNT 10

typedef struct {
    int score;
    char* name;
} rrHighScore;

rrHighScore* rr_high_score_create() {
    return calloc(1, sizeof(rrHighScore));
}

void rr_high_score_destroy(rrHighScore* high_score) {
    free(high_score->name);
    free(high_score);
}

void rr_high_score_set_name(rrHighScore* high_score, const char* name) {
    size_t len = strlen(name);

    if (high_score->name)
        free(high_score->name);

    high_score->name = malloc(len + 1);
    strcpy(high_score->name, name);
}

struct rrHighScores {
    cutil_vector* scores;
    char* data_file_path;
    rrHighScoresChangedCallbackFunc update_callback;
    void* callback_user_data;
};

#define HIGH_SCORE_DATA_FILE "high_scores.txt"

void set_data_file_str(rrHighScores* high_scores, const char* data_dir) {
    size_t len = strlen(data_dir);
    high_scores->data_file_path = malloc(len + (strlen(HIGH_SCORE_DATA_FILE) + 2));
    sprintf(high_scores->data_file_path, "%s%s%s", data_dir, rr_path_sep(), HIGH_SCORE_DATA_FILE);
}

rrHighScores* rr_high_scores_create(const char* data_dir) {
    rrHighScores* high_scores = malloc(sizeof(rrHighScores));

    high_scores->scores = cutil_vector_create(cutil_trait_ptr());
    high_scores->update_callback = NULL;
    high_scores->callback_user_data = NULL;

    set_data_file_str(high_scores, data_dir);

    return high_scores;
}

void rr_high_scores_destroy(rrHighScores* high_scores) {
    size_t i, count = cutil_vector_size(high_scores->scores);

    for (i = 0; i < count; i++) {
        rrHighScore* high_score;
        cutil_vector_get(high_scores->scores, i, &high_score);
        rr_high_score_destroy(high_score);
    }

    cutil_vector_destroy(high_scores->scores);
    free(high_scores->data_file_path);
    free(high_scores);
}

void rr_highscore_readline(FILE* file, cutil_strbuf* strbuf) {
    int ch = fgetc(file);

    while (ch != '\n' && ch != EOF) {
        if (ch == '\r') continue;
        cutil_strbuf_append_char(strbuf, (char)ch);
        ch = fgetc(file);
    }
}

int rr_high_scores_load(rrHighScores* high_scores) {
    int i, count;
    cutil_strbuf* strbuf;
    FILE* file = fopen(high_scores->data_file_path, "r");

    if (!file) return 0;

    strbuf = cutil_strbuf_create();
    rr_highscore_readline(file, strbuf);
    sscanf(cutil_strbuf_cstring(strbuf), "%d", &count);

    for (i = 0; i < count; i++) {
        rrHighScore* high_score = rr_high_score_create();

        cutil_strbuf_clear(strbuf);
        rr_highscore_readline(file, strbuf);
        rr_high_score_set_name(high_score, cutil_strbuf_cstring(strbuf));

        cutil_strbuf_clear(strbuf);
        rr_highscore_readline(file, strbuf);
        sscanf(cutil_strbuf_cstring(strbuf), "%d", &high_score->score);

        cutil_vector_push_back(high_scores->scores, &high_score);
    }

    cutil_strbuf_destroy(strbuf);
    fclose(file);
    return 1;
}

int rr_high_scores_write_file(rrHighScores* high_scores, const char* path);

size_t rr_high_scores_count(rrHighScores* high_scores) {
    return cutil_vector_size(high_scores->scores);
}

void rr_high_scores_clear(rrHighScores* high_scores) {
    cutil_vector_clear(high_scores->scores);
}

int rr_high_scores_get_score(rrHighScores* high_scores, size_t index, char** name, int* score) {
    rrHighScore* high_score;

    if (index >= cutil_vector_size(high_scores->scores))
        return 0;

    cutil_vector_get(high_scores->scores, index, &high_score);
    *name = high_score->name;
    *score = high_score->score;

    return 1;
}

int rr_high_scores_score_is_high(rrHighScores* high_scores, int score) {
    rrHighScore* high_score;

    if (score <= 0)
        return 0;

    if (cutil_vector_size(high_scores->scores) < HIGH_SCORE_COUNT)
        return 1;

    cutil_vector_get(high_scores->scores, cutil_vector_size(high_scores->scores) - 1, &high_score);

    return score > high_score->score;
}

int rr_high_scores_add(rrHighScores* high_scores, int score, const char* name) {
    size_t i, count = cutil_vector_size(high_scores->scores);
    rrHighScore* target;

    if (!rr_high_scores_score_is_high(high_scores, score))
        return 0;

    /* if the list is full re-use the last high score that will be popped off */
    if (count == HIGH_SCORE_COUNT){
        cutil_vector_get(high_scores->scores, count -1, &target);
        cutil_vector_pop_back(high_scores->scores);
        count -= 1;
    }
    else {
        target = rr_high_score_create();
    }

    rr_high_score_set_name(target, name);

    /* find index to insert new score */
    for (i = 0; i < count; i++) {
        rrHighScore* high_score;
        cutil_vector_get(high_scores->scores, i, &high_score);

        if (score > high_score->score)
            break;
    }

    if (i == count)
        cutil_vector_push_back(high_scores->scores, &target);
    else
        cutil_vector_insert(high_scores->scores, i, &target);

    if (high_scores->update_callback)
        high_scores->update_callback(high_scores->callback_user_data);

    return 1;
}

void rr_high_scores_set_updated_callback(rrHighScores* high_scores, rrHighScoresChangedCallbackFunc func, void* data) {
    high_scores->update_callback = func;
    high_scores->callback_user_data = data;
}
