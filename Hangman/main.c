#include <curl/curl.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../Utils/print-ascii.h"

#define STARTING_LIVES 7

typedef struct Memory
{
    char *response;
    size_t size;
} memory_t;

// Callback function to get the size of the response from the api
static size_t callback(char *data, size_t size, size_t nmemb, void *clientp)
{
    size_t realSize = size * nmemb;
    memory_t *mem = (memory_t *)clientp;
    char *ptr = realloc(mem->response, mem->size + realSize + 1);
    if (!ptr)
    {
        return 0;
    }
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realSize);
    mem->size += realSize;
    mem->response[mem->size] = 0;
    return realSize;
}

// Gets a random word of length 7 for use in the game
char *generateWord()
{
    memory_t chunk = {0};
    CURLcode res = 0;
    CURL *handle = curl_easy_init();
    if (handle)
    {
        curl_easy_setopt(handle, CURLOPT_URL, "https://random-word-api.herokuapp.com/word?length=7");
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(handle);
        curl_easy_cleanup(handle);
        return chunk.response;
    }
}

// Removes non letter charachters from the response
char *parse_response(char *word)
{
    int length = strlen(word);
    if (length < 7)
    {
        exit(0);
    }
    char *parsedWord = (char *)malloc(sizeof(char) * (length - 4) + 1);
    char *start = parsedWord;
    while (*word != '\0')
    {
        if (isalpha(*word))
        {
            *parsedWord = *word;
            parsedWord++;
        }
        word++;
    }
    *parsedWord = '\0';
    return start;
}

int main(int argc, char const *argv[])
{
    curl_global_init(CURL_GLOBAL_WIN32);
    printAscii("ascii.txt");
    printAscii("HangmanModels/0.txt");
    printf("Thinking of a word . . .\n");
    char *response = generateWord();
    if (response == NULL)
        exit(0);
    char *parsed_word = parse_response(response);
    printf("Parsed word: %s\n", parsed_word);
    char word_visualized[7] = {
        '_',
        '_',
        '_',
        '_',
        '_',
        '_',
        '_',
    };
    printf("%s\n", word_visualized);
    printf("Word generated, you have 7 tries to save your life \n");
    // initialize the guess number the player is on, starting at 0
    int guessNo = 0;
    // initialize space for 7 incorrect and correct guesses (and the '\0')
    char *correctGuesses = (char *)malloc(sizeof(char) * 7 + 1);
    char *incorrectGuesses = (char *)malloc(sizeof(char) * 7 + 1);
    // ensure memory was allocated correctly
    if (incorrectGuesses == NULL || correctGuesses == NULL)
    {
        exit(0);
    }

    /*
    TODO :
    take a guess in from stdin

    add checks to ensure guess is a letter

    make a dictionary like structure for the parsed word
    eg {'w': 1, 'o': 2, 'l': 1} for the word wool

    check the guess against the dictionary

    if the guess is in the dictionary and the number of occurances is > 0
    decrease the number and add the letter to the correctGuesses buffer

    otherwise add the guess to the incorrectGuesses buffer, increase the guessCount and continue

    */

    printAscii("HangmanModels/GameOver.txt");
    curl_global_cleanup();
    free(correctGuesses);
    free(incorrectGuesses);
    return 1;
}
