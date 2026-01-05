/***********
 ID:329218416
 NAME:Naor Biton
***********/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// custom
#define GOBACKONECELL -1
#define GOTONEXTCELL 1
#define INVALIDPOSTION -1
#define EPISODELENGTHARRAYSIZE 9//

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
Pos findShowPostion(TVShow *show);
Season *doesItExsistSeason(TVShow *show, char *name);

// insert
void insertShow(TVShow *show, Pos posBefore);
void insertSeaon(TVShow *show, Season *season, int numberOfTheSeason);

Pos getCellNewPos(Pos currentPos, int move);

// shiftCells
void shiftCellsRightFrom(Pos *lastPos);
void swapTwoCells(Pos cell1, Pos cell2);
void shiftCellsLeftFrom(Pos *lastPos);


//episode Helpers
char *getEpisodelength();
void insertEpisode(Season *season, Episode *newEpisode, int index);


// clean buffer
void clearBuffer();

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

    Pos posToInsert = findShowPostion(newShow);
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

Episode *doesItExsistEpisode(Season *season ,char *name)
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
        Season *seasonOfTheEpisode=doesItExsistSeason(show, seasonName);
        free(seasonName);

        if (seasonOfTheEpisode == NULL)
        {
            printf("Season not found.\n");
            return;
        }

        printf("Enter the name of the episode:\n");
        char *episodeName = getString();
        if (doesItExsistEpisode(seasonOfTheEpisode,episodeName))//episode with the same names exsisest
        {
            printf("Episode already exists.\n");
            free(episodeName);
            return;
        }
        printf("Enter the length (xx:xx:xx):\n");
        //char *episodeLength=getEpisodelength();

        // need to get number of seaon
        printf("Enter the position:\n");
        int numberOfTheSeason;
        scanf("%d", &numberOfTheSeason);

        // if left over char left in the buffer
        clearBuffer();

        Episode *newEpisode = (Episode *)malloc(sizeof(Episode));
        *newEpisode = (Episode){episodeName, NULL, NULL};

        insertEpisode(seasonOfTheEpisode, newEpisode, numberOfTheSeason);

        return;
    }
    else
    {
        printf("Show not found.\n");
        free(showName);
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
    printf("Enter the name of the show:\n");
    char *showName = getString();
    // is there already a show with same name
    TVShow *show = findShow(showName);
    if (show != NULL)
    {
        printf("Name: %s\nSeasons:\n",showName);
        Season *tempSeason = show->seasons;
        int index = 0;
        while (tempSeason != NULL)
        {
            printf("\tSeason %d: %s\n", index, tempSeason->name);
            index++;
            tempSeason = tempSeason->next;
        }
    }
    else{
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
                    shiftCellsLeftFrom(&curentPos); // have problem
                }
            }
        }
    }

    // need to realocate the old array

    return;
}

// return the position of the show in the database where the new show should be inserted
Pos findShowPostion(TVShow *show)
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
    return (Pos){INVALIDPOSTION, INVALIDPOSTION};
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
            posOfOneCellBehind = getCellNewPos(currentPos, GOBACKONECELL);
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
            posOfOneCellBehind = getCellNewPos(currentPos, GOTONEXTCELL);
            swapTwoCells(currentPos, posOfOneCellBehind);
        }
    }
}

void freeAll();

Pos getCellNewPos(Pos currentPos, int move)
{
    // Prevent Division by Zero
    if (dbSize <= 0)
    {
        return (Pos){INVALIDPOSTION, INVALIDPOSTION};
    }

    // Validate Input Position
    if (currentPos.row < 0 || currentPos.row >= dbSize ||
        currentPos.column < 0 || currentPos.column >= dbSize)
    {
        return (Pos){INVALIDPOSTION, INVALIDPOSTION};
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
        return (Pos){INVALIDPOSTION, INVALIDPOSTION};
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
    if (cell1.row == INVALIDPOSTION || cell2.row == INVALIDPOSTION)
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

char *getEpisodelength(){

    char* string=getString();

   
    return;
}

int isValidInput(char theChar,int index,int maxValue)
{

    
    if ((index+1)%3==0)
    {
        if (theChar==':')
        {
            return 1;
        }
        return 0;
    }



    

}