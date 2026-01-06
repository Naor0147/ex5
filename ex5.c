/***********
 ID:329218416
 NAME:Naor Biton
 ASSIGNMENT:ex5
***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// custom
#define GO_BACK_ONE_CELL -1
#define GO_TO_NEXT_CELL 1
#define INVALID_POSTION -1
#define EPISODE_LENGTH_ARRAY_SIZE 8
#define NOTCHOOSENYET 0

// length
#define ASCII_ZERO '0'
#define ASCII_NINE '9'
#define ASCII_FIVE '5'
#define FREQUENCY_OF_COLONIC_APPEARANCE 3
#define POSITION_MINUTE_DIGIT_ARRAY 3
#define POSITION_MINUTE_SECONDS_ARRAY 6

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

// custom struct

typedef struct Pos
{
    int row;
    int column;
} Pos;

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

// locate
Pos findShowNewPostion(TVShow *show);
Season *doesItExsistSeason(TVShow *show, char *name);
Pos getShowCoordinates(TVShow *show);

// insert
void insertShow(TVShow *show, Pos posBefore);
void insertSeaon(TVShow *show, Season *season, int numberOfTheSeason);

Pos getCellNewPos(Pos currentPos, int move);

// shiftCells
void shiftCellsRightFrom(Pos *lastPos);
void swapTwoCells(Pos cell1, Pos cell2);
void shiftCellsLeftFrom(Pos *lastPos);

// episode Helpers
char *getEpisodeLength();
int getEpisodelengthHelper();
void insertEpisode(Season *season, Episode *newEpisode, int index);
int isDigit(char c);
int isDigit(char c);
int iscolon(char c);
int isValidTimeChar(char c);

// remove

// episode
void removeEpisode(Episode *theEpisode, Season *seasonOfTheEpisode);
void removeSeasonAllEpsiodes(Season *seasonOfTheEpisode);

// Season
void removeSeasonEpisodes(Episode *tempEpisode, Season *seasonOfTheEpisode);
void removeSeasonHelper(Season *seasonToRemove, TVShow *TVShowOfTheSeason);

// Tvshow
void removeTVShowSeason(TVShow *TVShowOfTheSeason);
void removeTVShowSeasonHelper(Season *tempSeason, TVShow *TVShowOfTheSeason);

// clean buffer
void clearBuffer();

void addMenu()
{
    int choice = NOTCHOOSENYET;
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
    int choice = NOTCHOOSENYET;
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
    int choice = NOTCHOOSENYET;
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
    int choice = NOTCHOOSENYET;

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
            freeAll();
            break;
        }
    } while (choice != 4);
    return 0;
}

/// Add function
void addShow()
{
    printf("Enter the name of the show:\n");
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

    Pos posToInsert = findShowNewPostion(newShow);
    insertShow(newShow, posToInsert);
}

void addSeason()
{
    printf("Enter the name of the show:\n");
    char *showName = getString();
    TVShow *show = findShow(showName);
    if (show != NULL)
    {
        printf("Enter the name of the season:\n");
        char *seasonName = getString();
        // need to check if the seaon already exsists
        if (doesItExsistSeason(show, seasonName) != NULL)
        {
            printf("Season already exists.\n");
            free(showName);
            free(seasonName);
            return;
        }

        // need to get number of seaon
        printf("Enter the position:\n");
        int numberOfTheSeason;
        scanf("%d", &numberOfTheSeason);

        // if left over char left in the buffer
        clearBuffer();

        Season *newSeason = (Season *)malloc(sizeof(Season));
        *newSeason = (Season){seasonName, NULL, NULL};

        insertSeaon(show, newSeason, numberOfTheSeason);
        free(showName);
        return;
    }
    else
    {
        printf("Show not found.\n");
        free(showName);
        return;
    }
}

void insertSeaon(TVShow *show, Season *newSeason, int index)
{
    Season *temp = show->seasons;
    // handels cases where there isnt season or the new season is the first season
    if (temp == NULL || index == 0)
    {
        newSeason->next = temp;
        show->seasons = newSeason;
        return;
    }

    int count = 0;

    // loop that itreates in the linked list
    while (temp->next != NULL && count < (index - 1))
    {
        temp = temp->next;
        count++;
    }

    newSeason->next = temp->next;
    temp->next = newSeason;

    return;
}
// return the season if exsistes
Season *doesItExsistSeason(TVShow *show, char *name)
{
    Season *temp = show->seasons;
    if (temp == NULL)
    {
        return NULL;
    }
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// return the episode if exsistes

Episode *doesItExsistEpisode(Season *season, char *name)
{
    Episode *temp = season->episodes;
    if (temp == NULL)
    {
        return NULL;
    }
    while (temp != NULL)
    {
        if (strcmp(temp->name, name) == 0)
        {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void addEpisode()
{
    printf("Enter the name of the show:\n");
    char *showName = getString();
    TVShow *show = findShow(showName);
    free(showName);

    if (show != NULL)
    {
        printf("Enter the name of the season:\n");
        char *seasonName = getString();
        // need to check if the seaon already exsists
        Season *seasonOfTheEpisode = doesItExsistSeason(show, seasonName);
        free(seasonName);

        if (seasonOfTheEpisode == NULL)
        {
            printf("Season not found.\n");
            return;
        }

        printf("Enter the name of the episode:\n");
        char *episodeName = getString();
        if (doesItExsistEpisode(seasonOfTheEpisode, episodeName)) // episode with the same names exsisest
        {
            printf("Episode already exists.\n");
            free(episodeName);
            return;
        }
        printf("Enter the length (xx:xx:xx):\n");
        char *episodeLength = getEpisodeLength();

        // need to get number of seaon
        printf("Enter the position:\n");
        int numberOfTheSeason;
        scanf("%d", &numberOfTheSeason);

        // if left over char left in the buffer
        clearBuffer();

        Episode *newEpisode = (Episode *)malloc(sizeof(Episode));
        *newEpisode = (Episode){episodeName, episodeLength, NULL};

        insertEpisode(seasonOfTheEpisode, newEpisode, numberOfTheSeason);

        return;
    }
    else
    {
        printf("Show not found.\n");
        return;
    }
}

void insertEpisode(Season *season, Episode *newEpisode, int index)
{
    Episode *temp = season->episodes;
    // handels cases where there isnt epsiode or the new epside is the first

    if (temp == NULL || index == 0)
    {
        newEpisode->next = season->episodes;
        season->episodes = newEpisode;
        return;
    }

    // loop that itreates in the linked list

    int count = 0;
    while (temp->next != NULL && count < (index - 1))
    {
        temp = temp->next;
        count++;
    }

    // 3. Link
    newEpisode->next = temp->next;
    temp->next = newEpisode;
}

void deleteShow()
{
    printf("Enter the name of the show:\n");
    char *showName = getString();
    TVShow *show = findShow(showName);
    free(showName);

    if (show == NULL)
    {
        printf("Show not found.\n");
        return;
    }

    freeShow(show);
    shrinkDB();
}
// deleteShow

void freeShow(TVShow *show)
{

    if (show == NULL)
    {
        return;
    }

    removeTVShowSeason(show);
    Pos pos = getShowCoordinates(show);

    database[pos.row][pos.column] = NULL;

    if (show->name != NULL)
    {
        free(show->name);
    }
    free(show);

    shiftCellsLeftFrom(&pos);
}

void deleteSeason()
{
    printf("Enter the name of the show:\n");
    char *showName = getString();
    TVShow *show = findShow(showName);
    free(showName);

    if (show != NULL)
    {
        printf("Enter the name of the season:\n");
        char *seasonName = getString();
        // need to check if the seaon already exsists
        Season *seasonToRemove = doesItExsistSeason(show, seasonName);
        free(seasonName); // frees the season Name

        if (seasonToRemove == NULL)
        {
            printf("Season not found.\n");
            return;
        }
        removeSeasonHelper(seasonToRemove, show);
        return;
    }
    else
    {
        printf("Show not found.\n");
        return;
    }
}
void deleteEpisode()
{

    printf("Enter the name of the show:\n");
    char *showName = getString();
    TVShow *show = findShow(showName);
    free(showName);

    if (show != NULL)
    {
        printf("Enter the name of the season:\n");
        char *seasonName = getString();
        // need to check if the seaon already exsists
        Season *seasonOfTheEpisode = doesItExsistSeason(show, seasonName);
        free(seasonName); // frees the season Name

        if (seasonOfTheEpisode == NULL)
        {
            printf("Season not found.\n");
            return;
        }

        printf("Enter the name of the episode:\n");
        char *episodeName = getString();
        Episode *theEpisdode = doesItExsistEpisode(seasonOfTheEpisode, episodeName);
        free(episodeName);       // frees the episode name
        if (theEpisdode == NULL) // episode with the same names exsisest
        {
            printf("Episode not found.\n");
            return;
        }
        removeEpisode(theEpisdode, seasonOfTheEpisode);
        return;
    }
    else
    {
        printf("Show not found.\n");
        return;
    }
}

void removeEpisode(Episode *theEpisode, Season *seasonOfTheEpisode)
{

    if (seasonOfTheEpisode == NULL || theEpisode == NULL || seasonOfTheEpisode->episodes == NULL)
    {
        return; //  safety check
    }
    Episode *temp = seasonOfTheEpisode->episodes;
    if (temp == theEpisode)
    {
        seasonOfTheEpisode->episodes = temp->next;
        freeEpisode(theEpisode);
        return;
    }

    while (temp->next != NULL && temp->next != theEpisode)
    {
        temp = temp->next;
    }

    // If we found it
    if (temp->next == theEpisode)
    {
        temp->next = theEpisode->next;
        freeEpisode(theEpisode);
    }
}

void freeEpisode(Episode *e)
{

    if (e == NULL)
    {
        return;
    }

    // Free the strings inside the struct
    if (e->name != NULL)
    {
        free(e->name);
    }
    if (e->length != NULL)
    {
        free(e->length);
    }

    // free the struct itself
    free(e);
}

void freeSeason(Season *s)
{
    if (s == NULL)
    {
        return;
    }

    // Free the strings inside the struct
    if (s->name != NULL)
    {
        free(s->name);
    }
    if (s->episodes != NULL)
    {
        removeSeasonAllEpsiodes(s);
    }

    // free the struct itself
    free(s);
}

void printEpisode()
{
    printf("Enter the name of the show:\n");
    char *showName = getString();
    TVShow *show = findShow(showName);
    free(showName); // free the memory

    if (show != NULL)
    {
        printf("Enter the name of the season:\n");
        char *seasonName = getString();
        // need to check if the seaon already exsists
        Season *seasonOfTheEpisode = doesItExsistSeason(show, seasonName);
        free(seasonName); // frees the season Name

        if (seasonOfTheEpisode == NULL)
        {
            printf("Season not found.\n");
            return;
        }

        printf("Enter the name of the episode:\n");
        char *episodeName = getString();
        Episode *theEpisdode = doesItExsistEpisode(seasonOfTheEpisode, episodeName);
        free(episodeName);       // frees the episode name
        if (theEpisdode == NULL) // episode with the same names exsisest
        {
            printf("Episode not found.\n");
            return;
        }
        printf("Name: %s\nLength: %s\n", theEpisdode->name, theEpisdode->length);

        return;
    }
    else
    {
        printf("Show not found.\n");
        return;
    }
}

void printShow()
{
    printf("Enter the name of the show:\n");
    char *showName = getString();
    // is there already a show with same name
    TVShow *show = findShow(showName);
    if (show != NULL)
    {
        printf("Name: %s\nSeasons:\n", showName);
        Season *tempSeason = show->seasons;
        int index = 0;
        while (tempSeason != NULL)
        {
            printf("\tSeason %d: %s\n", index, tempSeason->name);
            index++;
            Episode *tempEpisode = tempSeason->episodes;
            int indexEpsiode = 0;
            while (tempEpisode != NULL)
            {
                printf("\t\tEpisode %d: %s (%s)\n", indexEpsiode, tempEpisode->name, tempEpisode->length);
                indexEpsiode++;
                tempEpisode = tempEpisode->next;
            }

            tempSeason = tempSeason->next;
        }
    }
    else
    {
        printf("Show not found.\n");
    }

    free(showName);
    return;
}

void printArray()
{
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] != NULL)
            {
                printf("[%s] ", database[i][j]->name);
            }
            else
            {
                printf("[NULL] ");
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
    // if increseing the size of array is needed
    if (database[dbSize - 1][dbSize - 1] != NULL)
    {
        dbSize++;
        for (int i = 0; i < (dbSize - 1); i++)
        {
            database[i] = realloc(database[i], dbSize * sizeof(TVShow *)); // update size of every array
            database[i][dbSize - 1] = NULL;                                // intialize the new cell to null
        }

        database = realloc(database, dbSize * sizeof(TVShow **));             // update size of main array
        database[dbSize - 1] = (TVShow **)calloc(dbSize, (sizeof(TVShow *))); // intialize the new cell with a empty array

        for (int i = 0; i < dbSize; i++)
        {
            for (int j = 0; j < dbSize; j++)
            {
                if (database[i][j] == NULL)
                {
                    Pos curentPos = (Pos){i, j};
                    shiftCellsLeftFrom(&curentPos);
                }
            }
        }
    }

    return;
}

// return the position of the show in the database where the new show should be inserted
Pos findShowNewPostion(TVShow *show)
{
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] == NULL)
            {
                return (Pos){i, j};
            }
            if (strcmp(database[i][j]->name, show->name) > 0)
            {

                return (Pos){i, j};
            }
        }
    }
    return (Pos){INVALID_POSTION, INVALID_POSTION};
}

/// need to change
void insertShow(TVShow *show, Pos posBefore)
{
    // if the last cell isnt null it means the db is to small
    if (database[dbSize - 1][dbSize - 1] != NULL)
    {
        expandDB();
    }

    // printf("\nlet enter : %s  pos [row:%d , coulm:%d]  \n", show->name, posBefore.row, posBefore.column);
    shiftCellsRightFrom(&posBefore);
    database[posBefore.row][posBefore.column] = show;
    return;
}

void shiftCellsRightFrom(Pos *lastPos)
{
    Pos posOfOneCellBehind, currentPos;
    for (int i = dbSize - 1; 0 <= i; i--)
    {
        for (int j = dbSize - 1; 0 <= j; j--)
        {
            if (lastPos->row == i && lastPos->column == j) // if reached the last cell
            {
                return;
            }

            // cells we switch
            currentPos = (Pos){i, j};
            posOfOneCellBehind = getCellNewPos(currentPos, GO_BACK_ONE_CELL);
            swapTwoCells(currentPos, posOfOneCellBehind);
        }
    }
}
void shiftCellsLeftFrom(Pos *lastPos)
{
    Pos posOfOneCellBehind, currentPos;
    for (int i = lastPos->row; i <= dbSize - 1; i++)
    {
        for (int j = 0; j <= dbSize - 1; j++)
        {
            if (i == lastPos->row && lastPos->column > j)
            {
                continue;
            }
            currentPos = (Pos){i, j};
            posOfOneCellBehind = getCellNewPos(currentPos, GO_TO_NEXT_CELL);
            swapTwoCells(currentPos, posOfOneCellBehind);
        }
    }
}

void freeAll()
{

    if (database == NULL)
        return;

    for (int i = 0; i < dbSize; i++)
    {
        if (database[i] != NULL)
        {
            for (int j = 0; j < dbSize; j++)
            {
                // Free the show directly
                TVShow *currentShow = database[i][j];
                if (currentShow != NULL)
                {
                    // Free seasons and episodes
                    removeTVShowSeason(currentShow);

                    // Free name
                    if (currentShow->name != NULL)
                    {
                        free(currentShow->name);
                    }

                    // Free the struct
                    free(currentShow);
                }
            }
            // Free the row array
            free(database[i]);
        }
    }

    // Free the main array
    free(database);
}

Pos getCellNewPos(Pos currentPos, int move)
{
    // Prevent Division by Zero
    if (dbSize <= 0)
    {
        return (Pos){INVALID_POSTION, INVALID_POSTION};
    }

    // Validate Input Position
    if (currentPos.row < 0 || currentPos.row >= dbSize ||
        currentPos.column < 0 || currentPos.column >= dbSize)
    {
        return (Pos){INVALID_POSTION, INVALID_POSTION};
    }

    // Convert the 2d array to 1d array
    int posLineIndex = currentPos.row * dbSize + currentPos.column;

    // Apply Move
    int newLineIndex = posLineIndex + move;

    // Bounds Checking
    if (newLineIndex < 0)
    {
        return (Pos){0, 0}; // return first cell
    }

    if (newLineIndex >= (dbSize * dbSize))
    {
        return (Pos){INVALID_POSTION, INVALID_POSTION};
    }

    // 3. Convert back to 2D array
    Pos newPos;
    newPos.row = newLineIndex / dbSize;
    newPos.column = newLineIndex % dbSize;

    return newPos;
}

void swapTwoCells(Pos cell1, Pos cell2)
{
    // Validate Input Positions
    if (cell1.row == INVALID_POSTION || cell2.row == INVALID_POSTION)
    {
        return;
    }

    // Swap the elements
    TVShow *temp = database[cell1.row][cell1.column];
    database[cell1.row][cell1.column] = database[cell2.row][cell2.column];
    database[cell2.row][cell2.column] = temp;
    // printArray();

    return;
}

void clearBuffer()
{
    int leftOverChar;

    while (1)
    {
        leftOverChar = getchar();

        // If we hit a newline or the end of the input stops the program
        if (leftOverChar == '\n' || leftOverChar == EOF)
        {
            return;
        }
    }
}

char *getEpisodeLength()
{
    char *myString = getString();
    while (!getEpisodelengthHelper(myString))
    {
        printf("Invalid length, enter again:\n");
        myString = getString();
    }
    return myString;
}

int getEpisodelengthHelper(char *string)
{

    int len = strlen(string);
    if (len != EPISODE_LENGTH_ARRAY_SIZE)
    {
        free(string); // if the string is not i the right size
        return 0;
    }
    for (int i = 0; i < EPISODE_LENGTH_ARRAY_SIZE; i++)
    {
        if ((i + 1) % FREQUENCY_OF_COLONIC_APPEARANCE == 0)
        {
            if (!iscolon(string[i]))
            {
                free(string);
                return 0;
            }
        }
        else
        {
            if (!isDigit(string[i]))
            {
                free(string);
                return 0;
            }
        }
    }
    if (!isValidTimeChar(string[POSITION_MINUTE_DIGIT_ARRAY]) ||
        !isValidTimeChar(string[POSITION_MINUTE_SECONDS_ARRAY]))
    {
        free(string);
        return 0;
    }

    return 1;
}

int isValidTimeChar(char c)
{
    return (c >= ASCII_ZERO && c <= ASCII_FIVE);
}

int isDigit(char c)
{
    return (c >= ASCII_ZERO && c <= ASCII_NINE);
}

int iscolon(char c)
{
    return (c == ':');
}

// remove Recuseivly the last Episdode from the end to the start
void removeSeasonEpisodes(Episode *tempEpisode, Season *seasonOfTheEpisode)
{
    if (tempEpisode == NULL)
    {
        return;
    }
    removeSeasonEpisodes(tempEpisode->next, seasonOfTheEpisode);
    removeEpisode(tempEpisode, seasonOfTheEpisode);
}

void removeSeasonHelper(Season *seasonToRemove, TVShow *TVShowOfTheSeason)
{
    removeSeasonAllEpsiodes(seasonToRemove);
    if (seasonToRemove == NULL)
    {
        return; //  safety check
    }
    Season *temp = TVShowOfTheSeason->seasons;
    if (temp == seasonToRemove)
    {
        TVShowOfTheSeason->seasons = temp->next;
        freeSeason(seasonToRemove);
        return;
    }

    while (temp->next != NULL && temp->next != seasonToRemove)
    {
        temp = temp->next;
    }

    // If we found it
    if (temp->next == seasonToRemove)
    {
        temp->next = seasonToRemove->next;
    }
    freeSeason(seasonToRemove);
}

void removeSeasonAllEpsiodes(Season *seasonOfTheEpisode)
{
    if (seasonOfTheEpisode == NULL)
    {
        return; //  safety check
    }
    Episode *tempEpisode = seasonOfTheEpisode->episodes;

    removeSeasonEpisodes(tempEpisode, seasonOfTheEpisode);
}

void removeTVShowSeason(TVShow *TVShowOfTheSeason)
{
    Season *tempSeason = TVShowOfTheSeason->seasons;
    removeTVShowSeasonHelper(tempSeason, TVShowOfTheSeason);
}

void removeTVShowSeasonHelper(Season *tempSeason, TVShow *TVShowOfTheSeason)
{
    if (tempSeason == NULL)
    {
        return;
    }
    removeTVShowSeasonHelper(tempSeason->next, TVShowOfTheSeason);
    removeSeasonHelper(tempSeason, TVShowOfTheSeason);
}

// get the Show Pos
Pos getShowCoordinates(TVShow *show)
{
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            if (database[i][j] == show)
            {
                return (Pos){i, j};
            }
        }
    }
    return (Pos){INVALID_POSTION, INVALID_POSTION};
}

int countShows()
{
    int count = 0;
    for (int i = 0; i < dbSize; i++)
    {
        for (int j = 0; j < dbSize; j++)
        {
            // Only count if not null
            if (database[i][j] != NULL)
            {
                count++;
            }
        }
    }
    return count;
}
void shrinkDB()
{
    int count = countShows();

    // Check if we can shrink

    if (count <= (dbSize - 1) * (dbSize - 1))
    {

        // We only need to save items from the last column of the rows that will remain.
        for (int i = 0; i < dbSize - 1; i++)
        {
            // The item in the last column
            TVShow *showToMove = database[i][dbSize - 1];

            if (showToMove != NULL)
            {
                // remove it from the current spot so it doesnt get duplicated
                database[i][dbSize - 1] = NULL;

                // move to the next row
                Pos targetPos = {i + 1, 0};

                // Shift everything from that position onwards to the right
                shiftCellsRightFrom(&targetPos);

                // Place the show in the newly opened spot
                database[targetPos.row][targetPos.column] = showToMove;
            }
        }

        dbSize--;

        // Shrink every row
        for (int i = 0; i < dbSize; i++)
        {
            database[i] = realloc(database[i], dbSize * sizeof(TVShow *));
        }

        // Free the last row
        free(database[dbSize]);

        // Shrink the main array
        if (dbSize == 0)
        {
            free(database);
            database = NULL;
        }
        else
        {
            database = realloc(database, dbSize * sizeof(TVShow **));
        }
    }
}