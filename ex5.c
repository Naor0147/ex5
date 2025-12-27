/***********
 ID:329218416
 NAME:Naor Biton
***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Episode
{
    char *name;
    char *length;
    struct Episode *next;
} Episode;

typedef struct Season
{
    char *name;
    Episode *episodes;
    struct Season *next;
} Season;

typedef struct TVShow
{
    char *name;
    Season *seasons;
} TVShow;

TVShow ***database = NULL;
int dbSize = 0;

char *getString();
int getInt();

int validLength(char *s);
int countShows();

void shrinkDB();
void expandDB();

void freeEpisode(Episode *e);
void freeSeason(Season *s);
void freeShow(TVShow *show);
void freeAll();

TVShow *findShow(char *name);
Season *findSeason(TVShow *show, char *name);
Episode *findEpisode(Season *season, char *name);

void addShow();
void addSeason();
void addEpisode();

void deleteShow();
void deleteSeason();
void deleteEpisode();

void printEpisode();
void printShow();
void printArray();

// custom
TVShow *findShowPostion(TVShow *show);
void insertShow(TVShow *show, TVShow *beforeShow);

void addMenu()
{
    int choice;
    printf("Choose an option:\n");
    printf("1. Add a TV show\n");
    printf("2. Add a season\n");
    printf("3. Add an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice)
    {
    case 1:
        addShow();
        break;
    case 2:
        addSeason();
        break;
    case 3:
        addEpisode();
        break;
    }
}

void deleteMenu()
{
    int choice;
    printf("Choose an option:\n");
    printf("1. Delete a TV show\n");
    printf("2. Delete a season\n");
    printf("3. Delete an episode\n");
    scanf("%d", &choice);
    getchar();
    switch (choice)
    {
    case 1:
        deleteShow();
        break;
    case 2:
        deleteSeason();
        break;
    case 3:
        deleteEpisode();
        break;
    }
}

void printMenuSub()
{
    int choice;
    printf("Choose an option:\n");
    printf("1. Print a TV show\n");
    printf("2. Print an episode\n");
    printf("3. Print the array\n");
    scanf("%d", &choice);
    getchar();
    switch (choice)
    {
    case 1:
        printShow();
        break;
    case 2:
        printEpisode();
        break;
    case 3:
        printArray();
        break;
    }
}

void mainMenu()
{
    printf("Choose an option:\n");
    printf("1. Add\n");
    printf("2. Delete\n");
    printf("3. Print\n");
    printf("4. Exit\n");
}

int main()
{
    int choice;
    do
    {
        mainMenu();
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            addMenu();
            break;
        case 2:
            deleteMenu();
            break;
        case 3:
            printMenuSub();
            break;
        case 4:
            // freeAll();
            break;
        }
    } while (choice != 4);
    return 0;
}

/// Add function
void addShow()
{
    char *showName = getString();

    // is there already a show with same name
    TVShow *show = findShow(showName);
    if (show != NULL)
    {
        free(showName);
        printf("Show already exists.\n");
        return;
    }

    expandDB(); // make sure there is enough space in the data base

    // create new show
    TVShow *newShow = (TVShow *)malloc((sizeof(TVShow)));

    newShow->name = showName;
    newShow->seasons = NULL;

    TVShow *posToInsert = findShowPostion(newShow);
    insertShow(newShow, posToInsert);
}

void addSeason()
{
}
void addEpisode()
{
}

void deleteShow()
{
}
void deleteSeason()
{
}
void deleteEpisode()
{
}
void printEpisode()
{
}

void printShow()
{
}

void printArray()
{
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] != NULL)
            {
                printf("[%s]", database[i][j]->name);
            }
            else
            {
                printf("[NULL]");
            }
        }
        printf("\n");
    }
}

// get string
char *getString()
{
    int stringSize = 1; // start from 1 to save \0
    char *string = (char *)malloc(stringSize * (sizeof(char)));
    char CurrentChar = getchar(); // gets the first char

    // if the char is not new line keep getting chars
    while (CurrentChar != '\n')
    {
        string[stringSize - 1] = CurrentChar;                  // save char
        stringSize++;                                          // increase size
        string = realloc(string, stringSize * (sizeof(char))); // make space for new char
        CurrentChar = getchar();                               // get the next char
    }
    string[stringSize - 1] = 0; // end of file
    return string;              // return the pointer to the string
}

TVShow *findShow(char *name)
{
    if (dbSize == 0 || database == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] != NULL)
            {
                if (strcmp(database[i][j]->name, name) == 0)
                {
                    return database[i][j];
                }
            }
        }
    }
    return NULL;
}

void expandDB()
{

    if (dbSize == 0)
    {
        dbSize = 1;
        database = (TVShow ***)calloc(dbSize, (sizeof(TVShow **)));
        database[0] = (TVShow **)calloc(dbSize, (sizeof(TVShow *)));
        return;
    }

    int currentAmountOfShows = 0;
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] != NULL)
            {
                currentAmountOfShows++;
            }
        }
    }
    // if increseing the size of array is needed
    if ((dbSize * dbSize) == currentAmountOfShows)
    {
        dbSize++;
        for (int i = 0; i < (dbSize - 1); i++)
        {
            database[i] = realloc(database[i], dbSize * sizeof(TVShow *)); // update size of every array
            database[i][dbSize - 1] = NULL;                                // intialize the new cell to null
        }

        database = realloc(database, dbSize * sizeof(TVShow **));             // update size of main array
        database[dbSize - 1] = (TVShow **)calloc(dbSize, (sizeof(TVShow *))); // intialize the new cell with a empty array
    }

    //need to realocate the old array 


    return;
}

// return the position of the show in the database where the new show should be inserted
TVShow *findShowPostion(TVShow *show)
{
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] == NULL)
            {
                return database[i][j];
            }
            if (strcmp(database[i][j]->name, show->name) > 0)
            {
                return database[i][j];
            }
        }
    }
    return NULL;
}

void insertShow(TVShow *show, TVShow *beforeShow)
{

    TVShow *temp = show;
    TVShow *temp2;
    int found = 0;
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            printArray();
            if (database[i][j] == NULL)
            {
                database[i][j] = temp;
                return;
            }

            if (found == 1)
            {
                temp2 = database[i][j];
                database[i][j] = temp;
                temp2 = temp;
            }
            if (database[i][j] == beforeShow && found == 0)
            {
                found = 1;
                temp = beforeShow;
                database[i][j] = show;
            }
        }
    }
    return;
}

void freeAll();
