/**
 * @file game_serializer.h
 * @brief Functions to serialize (save) and deserialize (load) the game state.
 *
 * This module allows saving the current game state to a file and loading
 * it back.
 */

#include<stdlib.h>
#include<stdio.h>

#include"game_config.h"

/**
 * @brief Saves the score list to a binary file.
 *
 * @param[in] file_name Path to the file.
 * @param[in] score_list Pointer to the array of scores.
 * @param[in] n Number of scores to write.
 * @return int Returns 1 on success, 0 on failure.
 *
 * @details
 * Opens the specified file and writes the scores as binary data.
 */
int serialize_game_score(const char *file_name, unsigned int *score_list, int n);

/**
 * @brief Loads the score list from a binary file.
 *
 * @param[in] file_name Path to the file.
 * @param[out] score_list Pointer to the array to store scores.
 * @param[in] n Number of scores to read.
 * @return int Returns 1 on success, 0 on failure.
 *
 * @details
 * Opens the specified file and reads the scores as binary data.
 */
int deserialize_game_score(const char *file_name, unsigned int *score_list, int n);

/**
 * @brief Saves the current game configuration to a file.
 *
 * @param[in] file_name Name/path of the file to save to.
 * @param[in] config Pointer to the GameConfig structure to serialize.
 * @return int Returns 1 if successful, 0 if there was an error opening/writing the file.
 */
int serialize_game(const char *file_name, GameConfig *config);

/**
 * @brief Loads the game configuration from a file.
 *
 * @param[in] file_name Name/path of the file to read from.
 * @param[out] config Pointer to the GameConfig structure where the game state will be loaded.
 * @return int Returns 1 if successful, 0 if there was an error opening/reading the file.
 */
int deserialize_game(const char *file_name, GameConfig *config);