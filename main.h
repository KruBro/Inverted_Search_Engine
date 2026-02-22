/**
 * @file   main.h
 * @brief  Central header for the Inverted Search Engine.
 *
 * Defines all shared types, data structures, status codes, and function
 * declarations used across the project's translation units.
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "color.h"

/* ─────────────────────────────────────────────
 *  Type Aliases
 * ───────────────────────────────────────────── */

typedef unsigned int u_int;

/* ─────────────────────────────────────────────
 *  Status Codes
 *  Returned by most functions to signal outcome.
 * ───────────────────────────────────────────── */
typedef enum
{
    SUCCESS,        /* Operation completed without error          */
    FAILURE,        /* Generic failure (malloc, fopen, etc.)      */
    LIST_EMPTY,     /* Flist is NULL when a traversal was needed  */
    DUPLICATE,      /* File already exists in the Flist           */
    DATA_NOT_FOUND  /* Word not present in the hash table         */
} Status;

/* ─────────────────────────────────────────────
 *  Flist — File List Node
 *  A singly-linked list of filenames that have
 *  been validated and loaded into the engine.
 * ───────────────────────────────────────────── */
typedef struct Node
{
    char        file_name[20]; /* Name of the .txt file          */
    struct Node *link;         /* Pointer to the next Flist node */
} Flist;

/* ─────────────────────────────────────────────
 *  sNode — Sub Node
 *  One sNode per file a word appears in.
 *  Tracks the filename and occurrence count.
 * ───────────────────────────────────────────── */
typedef struct subNode
{
    u_int           wordcount;     /* Times word appears in this file */
    char            file_name[20]; /* The filename                    */
    struct subNode *subLink;       /* Next file this word appears in  */
} sNode;

/* ─────────────────────────────────────────────
 *  mNode — Main Node
 *  One mNode per unique word in the index.
 *  Chains horizontally across hash bucket collisions,
 *  and vertically into sNode sub-nodes.
 * ───────────────────────────────────────────── */
typedef struct mainNode
{
    u_int           filecount;     /* Number of files this word appears in */
    char            word[20];      /* The word itself                      */
    sNode          *sLink;         /* Head of this word's sNode chain      */
    struct mainNode *mLink;        /* Next word in the same hash bucket    */
} mNode;

/* ─────────────────────────────────────────────
 *  hash_T — Hash Table Bucket
 *  The hash table is a fixed array of 27 of these:
 *    - Index 0–25  → words starting with a–z (case-insensitive)
 *    - Index 26    → words starting with non-alphabetic characters
 * ───────────────────────────────────────────── */
typedef struct hashT
{
    u_int  index; /* Bucket index (0–26)              */
    mNode *link;  /* Head of this bucket's mNode chain */
} hash_T;

/* ─────────────────────────────────────────────
 *  Function Declarations
 * ───────────────────────────────────────────── */

/* validation.c */
Status read_and_validation(char *argv[], int i, Flist **head);
Status compare(Flist *head, char *fname);

/* flist_utils.c */
Status insert_at_last(Flist **head, char *fname);
void   print_list(Flist *head);
void   free_list(Flist **head);

/* hash_t_utils.c */
void   initialize_hashTable(hash_T *arr);
void   free_hash_table(hash_T *arr);

/* create_database.c */
Status create_database(hash_T *arr, Flist *head);

/* display_database.c */
void   display_database(hash_T *arr);

/* search_database.c */
Status search_database(hash_T *arr, char *word);

/* update_database.c */
Status update_database(hash_T *arr, Flist **head, char **fileName, u_int fileCount);

/* save_database.c */
Status save_database(hash_T *arr);

/*Remove Punctuations*/
void strip_punctuation(char *word); 

#endif /* MAIN_H */