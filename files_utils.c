#include "main.h"

void strip_punctuation(char *word)
{
    int len   = strlen(word);
    int write = 0;

    for(int read = 0; read < len; read++)
    {
        if(isalpha(word[read]))
        {
            // Always keep alphabetic characters
            word[write++] = word[read];
        }
        else if(word[read] == '\''
                && read > 0 && read < len - 1
                && isalpha(word[read - 1])
                && isalpha(word[read + 1]))
        {
            // Keep apostrophes ONLY if surrounded by letters on both sides
            // e.g. it's → it's, but 'hello' → hello, o'' → o
            word[write++] = word[read];
        }
        // Everything else is silently dropped
    }
    word[write] = '\0';
}