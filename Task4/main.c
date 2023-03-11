//
//  main.c
//  PA1_4
//
//  Created by já, the second smartest programmer that's ever lived, on 69/420/2021.
//  Copyright © 2021 also já. All right.
//


#include <stdio.h>
#include <stdlib.h> // realloc




// BOOL DEFINITION
#define bool int
#define true 1
#define false 0
// ---------------


// allocated memory size for logs is based on the BASE_LOG_SIZE macro
#define BASE_LOG_SIZE 1000

// determine the granularity of cache blocks
#define CACHE_BLOCK_SIZE 1000 // 32767 at max! >> [REF01]





// GLOBAL VARS AREA

unsigned connections = 0;     // number of connections established

struct S_USER * logs = NULL;  // logs of users' connections

// -----------------






// USER STRUCTURE -----------------------------------------------------
// instead of using 32b integer for userID alone, utilise a 24b integer
// with an 8b successive connections counter for performance enhancement

typedef struct S_USER
{
    
    int     userID : 24;    // 24bit integer
    u_int8_t count;          // up to 255 successive connections
    
} user;



/* TODO
// CACHE-BLOCK STRUCTURE -----------------------------------------
// preprocessed chunks of connections during certain intervals
typedef struct S_CACHE
{

    short client_count;                     // [REF01] <<
    // dynamic array
    
} cache_block;
*/






//                Finite State Machine
typedef enum
{
    
    INIT,           // initial state, print init message
    GET_ACTION,     // determine
    CLIENT_CONNECT, // new client connection established
    ANALYZE,        // analyze given position interval
    
    EXIT,           // no further input expected - exit the program
    FAIL,           // unwanted defined behaviour occured
    UNDEFINED       // unwanted undefined behaviour occured
    
}STATE;
// ----------------------------------------------------



// saves space
/* well, not in this particular HW */
int fail(void)
{
    printf("Nespravny vstup.\n");
    free(logs); // free NULL = k
    return 0;
}




// user action specification expected - parse and decide what happens next
STATE getAction(void)
{
    
    char action;
 
    int check = scanf(" %c", &action);
    
    if ( check == 0 )
        return FAIL;
    
    if ( check == EOF )
        return EXIT;
    
    // --------------------------------
    if ( check == 1 )
        switch(action)
        {
            case '+':
                return CLIENT_CONNECT;
            
            case '?':
                return ANALYZE;

            // ... space for potential expansion ...
                
            default:
                return FAIL;
        }
    // --------------------------------
    
    return FAIL;
}



// new incoming user conection - validate and save log
STATE clientConnect(void)
{
    
    int ID;
    static int prevID = -1;  // previous user's ID
    static int succ;         // number of successive connections, very nice name
    
    // array of users pre-filled with zeros
    static unsigned USERS[100000] = {0}; // array position = user's ID

    if ( connections > 1000000 )
        return FAIL;
    
    if ( !scanf("%d", &ID) )
        return FAIL;
    
    if ( ID < 0 || ID > 99999 )
        return FAIL;
    
    
    if ( !(connections % BASE_LOG_SIZE) ) // if reallocation is needed
        logs = (user*)realloc( logs, ((connections/BASE_LOG_SIZE + 1) * BASE_LOG_SIZE * sizeof(user)) );
    
    if ( ID == prevID )
    {
        for ( int i = 1; i <= succ; ++i ) // for all previous logs with the same ID
        {
            if ( logs[connections-i].count == 255 ) // no need to continue, if we reached 255. position
                break;

            logs[connections-i].count++; // increase the number of logs with the same ID ahead of them
        }
        succ++; // increment the successive counter
    }
    else
        succ = 1;
    
    logs[connections].userID = ID; // actual ID
    logs[connections].count  = 1;
    
    prevID = ID;
    
    /* cannot overflow in this HW - further checks neglected */
    USERS[ID]++;
    connections++;
    // ---------------------------------------------------------
    
    
    
    /*
        TODO - CACHE ROUTINE IMPLEMENTATION
    */

    
    
    if ( USERS[ID] == 1 )
        printf("> prvni navsteva\n");
    else
        printf("> navsteva #%d\n", USERS[ID]);
    
    return GET_ACTION;
    
}



// very crude function to deal with the shit before midnite
STATE analyze(void)
{
    
    u_int8_t present[100000] = {false}; // 8bit int - very small, cute, space efficient
    int from, to, diff;

    if ( scanf("%d", &from) != 1 || scanf("%d", &to) != 1 )
        return FAIL;

    diff = to - from;
    
    if ( from < 0 || diff < 0 || to >= (int)connections ) // >= according to progtest, lol ok
        return FAIL;
    
    
    while ( from <= to )
    {
        present[logs[from].userID] = true;
        from += logs[from].count;
    }
    
    from = 0; // will be used as unique ID placeholder
    
    for ( int i = 0; i < 100000; ++i )
        if (present[i])
            from++;
    
    printf("> %d / %d\n", from, diff+1);
    
    return GET_ACTION;
}



int main(void)
{
    
    STATE current_state   = INIT;
    STATE next_state      = INIT;
    
    while (true)
    {
        
        next_state = UNDEFINED; // next state undefined by default
        
        switch(current_state)
        {
            case INIT:
                printf("Pozadavky:\n");
                next_state = GET_ACTION;
                break;
            // -------------------------
            case GET_ACTION:
                next_state = getAction();
                break;
            // -------------------------
            case CLIENT_CONNECT:
                next_state = clientConnect();
                break;
            // -------------------------
            case ANALYZE:
                /*
                for ( int i = 0; i < connections; ++i )
                {
                printf("%d\n",logs[i].userID);
                printf("%d\n",logs[i].count);
                }*/
                next_state = analyze();          // to be deleted
                break;
            // -------------------------
            case FAIL:
                return fail();
            // -------------------------
            case UNDEFINED:
                printf("Undefined behaviour! Terminating..\n");
                return 69;
            // -------------------------
            case EXIT:
                free(logs);
                return 69;
            // -------------------------
            default:
                printf("This is super undefined state!\n");
                return 420;
            // -------------------------
        }
        
        current_state = next_state;
        
    }
    
}
