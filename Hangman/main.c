#include <curl/curl.h>
#include <string.h>
#include <ctype.h>

#include "../Utils/print-ascii.h"


typedef struct Memory {
    char *response;
    size_t size;
} memory_t;

// Callback function to get the size of the response from the api
static size_t callback(char *data, size_t size, size_t nmemb, void *clientp) {
    size_t realSize = size * nmemb;
    memory_t *mem = (memory_t *)clientp;
    char *ptr = realloc(mem->response, mem->size + realSize + 1);
    if(!ptr) {
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
    if(handle) {
        curl_easy_setopt(handle, CURLOPT_URL, "https://random-word-api.herokuapp.com/word?length=7");
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callback);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(handle);
        curl_easy_cleanup(handle);
        return chunk.response;
    }
}

// Removes non letter charachters from the response
char *parse_response(char *word) {
    int length = strlen(word);
    if(length < 7) {
        exit(0);
    }
    char *parsedWord = (char *)malloc(sizeof(char) * (length - 4) + 1);
    char *start = parsedWord;
    while(*word != '\0') {
        if(isalpha(*word)) {
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
    // printf("Pre parsed response: %s\n", response);
    if(response == NULL) {
        exit(0);
    }
    char *parsed_word = parse_response(response);
    printf("Parsed word: %s\n", parsed_word);
    printf("Word generated, you have 7 tries to save your life \n");
    return 1;
}
