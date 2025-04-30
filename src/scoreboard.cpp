#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scoreboard.hpp"
#include "cJSON.hpp"

/*
 * parse:
 *  Parse through the JSON returned by the score database
 */
TopScore* parse(char *str) 
{
    /* Parse through a JSON of the format: 
    {
        "placement": 1,
        "top_scores": {
        "1": {
            "name": "pierce",
            "score": 1000,
        },
        "2": {
            "name": "billy",
            "score": 900,
        }
        }
    }
    Returns a list of the top ten TopScore structs in order
    */

    /* Allocate memory for the top scores */
    TopScore *top_scores = (TopScore*)malloc(10 * sizeof(TopScore));
    if (!top_scores) {
        printf("JSON parsing error: Memory allocation failed\n");
        return NULL;
    }
    memset(top_scores, 0, 10 * sizeof(TopScore));

    /* Parse the JSON */
    cJSON *root = cJSON_Parse(str);
    if (!root) {
        printf("JSON parsing error: %s\n", cJSON_GetErrorPtr());
        free(top_scores);
        return NULL;
    }

    /* Get the hero's placement */
    cJSON *placement_dict = cJSON_GetObjectItem(root, "placement");
    if (!placement_dict || !cJSON_IsNumber(placement_dict)) {
        printf("JSON parsing error: 'placement' missing or not a number\n");
        cJSON_Delete(root);
        free(placement_dict);
        return NULL;
    }

    if (cJSON_IsNumber(placement_dict)) {
        placement = placement_dict->valueint;
    } else {
        printf("JSON parsing error: 'placement' value is not a number\n");
        cJSON_Delete(root);
        free(top_scores);
        return NULL;
    }

    /* Get the top scores object */
    cJSON *score_dict = cJSON_GetObjectItem(root, "top_scores");
    if (!score_dict || !cJSON_IsObject(score_dict)) {
        printf("JSON parsing error: 'top_scores' missing or not an object\n");
        cJSON_Delete(root);
        free(top_scores);
        return NULL;
    }

    cJSON *entry = NULL;

    /* Iterate through each top score and add it to the list as a TopScore struct */
    cJSON_ArrayForEach(entry, score_dict) {
        if (num_topscores >= 10)
        {
            break;
        }

        cJSON *name = cJSON_GetObjectItem(entry, "name");
        cJSON *score = cJSON_GetObjectItem(entry, "score");

        if (name && cJSON_IsString(name) &&
            score && cJSON_IsNumber(score)) {

            strncpy(top_scores[num_topscores].name, name->valuestring, sizeof(top_scores[num_topscores].name) - 1);
            top_scores[num_topscores].score = score->valueint;
            num_topscores++;
        }
    }

    cJSON_Delete(root);
    return top_scores;
}

/* 
 * write_callback:
 *  used as a callback for CURL to write to a buffer instead of stdout
 */
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = size * nmemb;
    struct ResponseBuffer *buf = (struct ResponseBuffer *)userdata;

    char *new_data = (char*)realloc(buf->data, buf->size + total_size + 1); // +1 for null terminator
    if (new_data == NULL) {
        return 0; // out of memory
    }

    buf->data = new_data;
    memcpy(&(buf->data[buf->size]), ptr, total_size);
    buf->size += total_size;
    buf->data[buf->size] = '\0'; // null terminate

    return total_size;
}