// TODO: move
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define MAX_DISPLAY 32 
#define MAX_LINE 128 // TODO: pretty sure your steps should not be longer than this!
#define UNIT 100
#define NUTRITION_DB "pantry.txt"

bool DEBUG = true;

typedef struct item {
    float calories; 
    char name[MAX_DISPLAY];
    // TODO: maybe add the rest (don't care right now)
} item; 

typedef struct pantry {
    int size; 
    int capacity;
    item* items;
} pantry; 

typedef struct step {
    char line[MAX_LINE];
} step;

typedef struct ingredient {
    float g; 
    char name[MAX_DISPLAY]; 
} ingredient;

typedef struct recipe {
    int n_ingredients; 
    int n_steps; 
    ingredient* ingredients; 
    step* steps;
    char name[MAX_DISPLAY]; 
} recipe;

void myerror(const char* name, const char* fmt, ...) {
    fprintf(stderr, "[ERROR] %s: ", name);

    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args); 
    va_end(args);

    fprintf(stderr, "\n");
}

void debug(const char* name, const char* fmt, ...) {
    if (DEBUG) {
        printf("[DEBUG] %s: ", name);

        va_list args; 
        va_start(args, fmt);
        vfprintf(stdout, fmt, args); 
        va_end(args);

        printf("\n");
    }
}

FILE* read_base(char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        file = fopen(filename, "w"); 
        if (!file) {
            myerror(__func__, "file cannot be created");
            return NULL;
        }
        fprintf(file, "0\n");
        fclose(file); 
        file = fopen(filename, "r");
        if (!file) {
            myerror(__func__, "file cannot be opened");
            return NULL;
        }
    }
    return file;
}

void line_base(FILE* file, char line[][MAX_DISPLAY], int len) {
    char buf[MAX_LINE];
    debug(__func__, "tokens %d", len);
    // i dneed to be consistent with fgets and fscanf lol
    if (!fgets(buf, MAX_LINE, file)) {
        myerror(__func__, "cannot get line");
    }
    fgets(buf, MAX_LINE, file); // TODO: fix
    buf[strlen(buf) - 1] = '\0';
    debug(__func__, "buf %s", buf);

    const char* delim = ",";
    char* tok = strtok(buf, delim);
    for (int i = 0; i < len; i++) {
        strncpy(line[i], tok, MAX_DISPLAY - 1);
        line[i][MAX_DISPLAY - 1] = '\0';

        tok = strtok(NULL, delim);

        debug(__func__, "line i %d, %s", i, line[i]);
    }
}

bool load_pantry(pantry* pantry) {
    FILE* file = read_base(NUTRITION_DB); 
    if (!file) {
        myerror(__func__, "cannot load database");
        return false;
    }

    if (fscanf(file, "%d", &pantry->size) != 1) {
        myerror(__func__, "malformed file");
        return false;
    }

    pantry->capacity = pantry->size == 0 ? 10 : pantry->size * 2; 
    debug(__func__, "capacity: %d, size: %d", pantry->capacity, pantry->size);

    pantry->items = malloc(sizeof(item) * pantry->capacity);
    char line[2][MAX_DISPLAY];
    for (int i = 0; i < pantry->size; i++) {
        line_base(file, line, 2);

        item it = pantry->items[i];
        strncpy(it.name, line[0], MAX_DISPLAY - 1);
        it.name[MAX_DISPLAY - 1] = '\0';

        it.calories = strtof(line[1], NULL);

        debug(__func__, "item %s, cal %f", it.name, it.calories);
    }


    fclose(file);
    return true;
}

bool save_pantry(pantry* pantry) {

    free(pantry->items);
    return true;
}

int main(int argc, char* argv[]) {
    pantry pantry = {0};
    load_pantry(&pantry);

    save_pantry(&pantry); 
    return 0;
}