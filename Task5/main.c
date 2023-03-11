


#include <stdio.h>
#include <stdlib.h> // (re)alloc
#include <string.h>




// BOOL DEFINITION
#define bool int8_t
#define true  1
#define false 0
// ---------------






// ARTICLE STRUCTURE -----------------------------------------------------
typedef struct S_ARTICLE
{
    
    char     name[100];  // up to 99 characters + '\0'
    unsigned count;      // number of given articles sold
    
} article;



// Linked List node structure
typedef struct S_NODE
{
    
    article item;
    int next;   // position in array of the next aphabetically ordered element
    
    int prev_R; // position of the previous ranked article
    int next_R; // position of the next ranked article
    
} node;




unsigned BASE_ARRAY_SIZE = 400; // variable array size


node *   array      = NULL; // array of itemz
unsigned HEAD       = 0;    // alphabetically first node

int R_HEAD     = -1;    // rank head
int R_TAIL     = -1;    // rank tail

unsigned node_num   = 0;





//                Finite State Machine
typedef enum
{
    
    INIT,           // initial state, print init message and continue
    GET_ACTION,     // determine next state based on user's input
    ADD_ARTICLE,    // add new article to the list
    GET_SUMM,       // prints out the count of all the top selling articles
    GET_LIST,       // prints out the list of top selling articles
    EXIT,           // no further input expected - exit the program
    FAIL,           // unwanted defined behaviour occured
    UNDEFINED       // unwanted undefined behaviour occured
    
}STATE;
// ----------------------------------------------------




// saves space
int fail(void)
{
    printf("Nespravny vstup.\n");
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
            return ADD_ARTICLE;
            
        case '?':
            return GET_SUMM;
            
        case '#':
            return GET_LIST;
            
            // ... space for potential expansion ...
            
        default:
            return FAIL;
    }
    // --------------------------------
    
    return FAIL;
}











//
STATE addArticle(void)
{
    
    article new_article;
    
    
    // skip whitespaces and read first char
    if ( scanf(" %c", &new_article.name[0]) != 1 )
        return FAIL;
    
    for ( int i = 1; i < 101; ++i )
    {
        if ( i == 100 )
            return FAIL;
        
        if ( scanf("%c", &new_article.name[i]) != 1 )
            return FAIL;
        
        if ( new_article.name[i] == '\n' )
        {
            new_article.name[i] = '\0';
            break;
        }
    }
    
    new_article.name[99] = '\0'; // just to be sure
    
    new_article.count    = 1;    // first.. maybe
    
    
    // - - - - - - - - - -  array insertion region  - - - - - - - - - - - - -
    
    
    // first - check if sufficient ammount of space is allocated
    if ( !(node_num % BASE_ARRAY_SIZE) )
    {
        BASE_ARRAY_SIZE *= 2;
        array = (node*)realloc( array, sizeof(node) * BASE_ARRAY_SIZE );
        
    }
    
    
    // if no node present yet
    if ( !node_num )
    {
        array[0].item   = new_article;  // insert into array for nodes
        array[0].next   = -1;           // no other node to point at at this moment
        array[0].prev_R = -1;           // no lower rank present yet
        array[0].next_R = -1;           // no higher rank present yet
        
        HEAD   = 0;
        R_HEAD = 0;
        R_TAIL = 0;
        
        node_num++;
        return GET_ACTION;              // everything OK now, get another user's input
    }
    
    // else
    int tmp  = HEAD;      // create temporary node
    int prev = -1;        // pointer to previous node
    int res;              // result for strcmp function
    
    while ( 1 ) // potential infinite loop, i know, shitty
    {
        res = strcmp(new_article.name, array[tmp].item.name ); // compare the two names
        
        if ( res < 0 ) // if new name is alphabetically superior
        {
            array[node_num].item = new_article; // add the item to the array
            array[node_num].next = tmp;         // next node will be actual tmp node
            
            if ( prev != -1 )
                array[prev].next = node_num;    // previous' next node will be the newly added one
            else
                HEAD = node_num;                // HEAD override - new node comes sooner than actual HEAD
            
            // append article to the tail - it's its first appearance
            array[R_TAIL].prev_R = node_num;
            array[node_num].next_R = R_TAIL;
            array[node_num].prev_R = -1;
            R_TAIL = node_num;
            // -------------------------------
            node_num++;                         // new node added
            
            return GET_ACTION;  // DONE
        }
        
        // same name found
        if ( res == 0 ) // if we found already existing article
        {
            array[tmp].item.count++; // increase the count
            
            // highest rank doesn't have to make changes
            if ( R_HEAD == tmp )
                return GET_ACTION;
            
            // if TAIL is what is being updated
            if ( R_TAIL == tmp )
            {
                if ( array[tmp].next_R != -1 ) // check if it's not the only node
                {
                    if (array[array[R_TAIL].next_R].item.count > array[tmp].item.count)
                        return GET_ACTION; // no need to sort
                    else
                        R_TAIL = array[tmp].next_R;
                }
                else
                    return GET_ACTION; // if it's the only node present, just skip
            }
            
            /*
             // if somewhere in the middle but rank doesn't change
             if ( array[array[tmp].next_R].item.count > array[tmp].item.count )
             return GET_ACTION;
             */
            
            // update the "hole" after node update
            if (array[tmp].next_R != -1)
                array[array[tmp].next_R].prev_R = array[tmp].prev_R;
            if (array[tmp].prev_R != -1)
                array[array[tmp].prev_R].next_R = array[tmp].next_R;
            
            // find the desired position for the updated node
            int r_tmp = R_HEAD;
            while ( array[r_tmp].item.count > array[tmp].item.count )
                r_tmp = array[r_tmp].prev_R;
            
            
            if ( array[r_tmp].next_R != -1 )
                array[array[r_tmp].next_R].prev_R = tmp;
            
            array[tmp].next_R = array[r_tmp].next_R;
            
            if ( r_tmp != tmp )
                array[tmp].prev_R = r_tmp;
            
            if ( r_tmp != tmp )
                array[r_tmp].next_R = tmp;
            
            if (array[tmp].next_R == -1) // update HEAD if it changed
                R_HEAD = tmp;
            
            return GET_ACTION;       // DONE
        }
        
        // new name is aphabetically after already present one
        if ( res > 0 ) // if new node is alphabeticaly inferior to the node compared to
        {
            if ( array[tmp].next != -1 ) // if "inferior" node exists
            {
                prev = tmp;
                tmp = array[tmp].next;
                continue;           // unneccessary, but for clarity..
            }
            else
            {
                array[node_num].item = new_article; // add the item to the array
                array[node_num].next = -1;          // next node doesn't exist yet
                array[tmp].next = node_num;
                
                // append article to the tail - it's its first appearance
                array[R_TAIL].prev_R = node_num;
                array[node_num].next_R = R_TAIL;
                array[node_num].prev_R = -1;
                R_TAIL = node_num;
                // -------------------------------
                node_num++;                         // new node added
                
                return GET_ACTION; // DONE
            }
        }
    }
    
    return UNDEFINED;
    
}




STATE getSumm( int count, bool print )
{
    
    unsigned summ = 0;
    int prev_count = -1;
    unsigned same_count = 0;
    
    int arr_pos = R_HEAD;
    int arr_pos_bkp; // array position backup
    
    int act_rank = 1; // rank placeholder
    
    
    
    while ( count > 0 )
    {
        
        if ( arr_pos == -1 )
            break;
        
        same_count = 0;
        
        arr_pos_bkp = arr_pos;
        
        // loop for succesive same count counting (lulz)
        while ( arr_pos != -1 )
        {
            prev_count = array[arr_pos].item.count;
            arr_pos = array[arr_pos].prev_R;
            if ( arr_pos == -1 ) break;
            if ( prev_count == (int)array[arr_pos].item.count )
                same_count++;
            else
                break;
        }
        
        arr_pos = arr_pos_bkp;
        
        // if there's only 1 unique article count
        if ( !same_count )
        {
            if (print)
                printf("%d. %s, %dx\n", act_rank, array[arr_pos].item.name, array[arr_pos].item.count);
            summ += array[arr_pos].item.count;
            arr_pos = array[arr_pos].prev_R;
            count--;
        }
        
        else
            for ( int i = same_count + 1; i > 0; i-- )
            {
                if (print)
                    printf("%d.-%d. %s, %dx\n", act_rank, act_rank + same_count, array[arr_pos].item.name, array[arr_pos].item.count);
                summ += array[arr_pos].item.count;
                arr_pos = array[arr_pos].prev_R;
                count--;
            }
        
        act_rank = act_rank + same_count + 1;
        
    }
    
    
    
    printf("Nejprodavanejsi zbozi: prodano %d kusu\n", summ);
    
    return GET_ACTION;
}





// load number of top selling articles to watch
STATE getWatchCount( int * count )
{
    if ( scanf("%d", count) != 1 )
        return FAIL;
    
    if ( *count < 1 )
        return FAIL;
    
    return GET_ACTION;
}





int main(void)
{
    
    STATE current_state   = INIT;
    STATE next_state      = INIT;
    
    
    int watch_count = 0; // number of top selling articles in watchlist
    
    while (true)
    {
        
        next_state = UNDEFINED; // next state undefined by default
        
        switch(current_state)
        {
            case INIT:
                printf("Pocet sledovanych:\n");
                next_state = getWatchCount(&watch_count);
                if ( next_state != FAIL )
                    printf("Pozadavky:\n");
                break;
                // -------------------------
            case GET_ACTION:
                next_state = getAction();
                break;
                // -------------------------
            case ADD_ARTICLE:
                next_state = addArticle();
                break;
                // -------------------------
            case GET_SUMM:
                getSumm(watch_count, false);
                next_state = GET_ACTION;
                break;
                // -------------------------
            case GET_LIST:
                getSumm(watch_count, true);
                next_state = GET_ACTION;
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
                // free(logs);
                return 69;
                // -------------------------
            default:
                printf("This is super undefined state!\n");
                return 420;
                // -------------------------
        }
        
        current_state = next_state;
        
    }
    
    return 0;
    
}
