#include "high_scores.h"

#include "cutil/strbuf.h"
#include "cutil/vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int score;
    char* name;
} rrHighScore;

void rr_high_score_destroy(rrHighScore* high_score) {
    free(high_score->name);
    free(high_score);
}

struct rrHighScores {
    cutil_trait score_trait;
    cutil_vector* scores;
};

rrHighScores* rr_high_scores_create() {
    rrHighScores* high_scores = malloc(sizeof(rrHighScores));

    high_scores->scores = cutil_vector_create(cutil_trait_ptr());

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

int rr_high_scores_load_file(rrHighScores* high_scores, const char* path) {
    int i, count;
    cutil_strbuf* strbuf;
    FILE* file = fopen(path, "r");

    if (!file) return 0;
    strbuf = cutil_strbuf_create();
    rr_highscore_readline(file, strbuf);
    sscanf(cutil_strbuf_cstring(strbuf), "%d", &count);

    for (i = 0; i < count; i++) {
        rrHighScore* high_score = malloc(sizeof(rrHighScore));

        cutil_strbuf_clear(strbuf);
        rr_highscore_readline(file, strbuf);
        high_score->name = malloc(cutil_strbuf_length(strbuf) + 1);
        strcpy(high_score->name, cutil_strbuf_cstring(strbuf));

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

int rr_high_scores_score_is_high(rrHighScores* high_scores, int score);
int rr_high_scores_add(rrHighScores* high_scores, int score, const char* name);
