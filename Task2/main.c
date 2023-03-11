//
//  main.c
//  PA1_2021_2
//
//  Created by Igy on 28/10/21.
//  Copyright © 2021 Igy. All rights reserved.
//


// °equation representation:
//  num_len * ( s^exp - s^(exp-1) )




#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ld long double


// just to make thing a little sexier
int ERROR()
{
    printf("Nespravny vstup.\n");
    return 0;
}


// position as input  = definitive cursor position in the whole number system
// position as output = cursor position for the individual number
long findNumber( long * position, int * system )
{
    
    long number;  // given number in decimal system
    long tmp;     // ust a temporary variable
    ld exp = 1;   // exponent (also number's length)
    

    // cursor doesn't move, number is zero
    if ( *position == 0 )
        return 0;
    
    
    // if the position is somewhere among
    // the numbers in the one-digit segment
    if ( *position < *system )
    {
        number = *position;
        *position = 0; // first postion of the one-digit number (cursor doesn't move)
        return number;
    }
    

    *position -= *system; // skips all the single-digit numbers
    
    
    // ----- main number-finding loop -----
    while ( 1 )
    {
        
        exp++;
        
        //                  gotta cast, pow accepts and returns long double
        tmp = (long)( exp * (pow((ld)*system,exp) - pow((ld)*system,exp-1)) ); // a very nice equation°
        
        if ( *position - tmp <= 0 )
            break;
        
        *position -= tmp;
        
    }
    // ------------------------------------
    
    // skips shorter numbers + lower numbers of the same length
    number = pow(*system, exp-1) + (*position/exp);
    
    // definitive position of the digit
    *position = *position % (long)exp;
    
    
    return number;
    
}



char numToAscii( long number )
{
    if ( number < 10 )
        return (48 + number);
    
    return (87 + number);
}




// transfers decimal number system to any other given number system
char * normalize( long * number, int * system )
{
    
    char * string;
    int stringPos = 0;
    char tmp;
    
    string = (char*) malloc( 50 * sizeof(char) );
    
    if ( *number == 0 )
    {
        string[0] = '0';
        string[1] = '\0';
        return string;
    }
    
    // system transition magic
    while ( *number > 0 )
    {
        string[stringPos] = numToAscii(*number % (long)*system);
        *number /= *system;
        stringPos++;
        if (stringPos > 49) printf("Progtest jede bomby!\n"); // just so I know why I get segfaulted
    }
    
    string[stringPos] = '\0';
    stringPos--;
    
    // string flippin'
    for ( int i = 0; i < stringPos; ++i )
    {
        tmp = string[i];
        string[i] = string[stringPos];
        string[stringPos] = tmp;
        stringPos--;
    }
    
    return string;
    
}



int main()
{
   
    long position, number;
    int system;
    
    char * string;
    
    
    printf("Pozice a soustava:\n");
    
    while ( 1 )
    {
        int check;
        
        check = scanf("%ld %d", &position, &system);
        
        if (check != 2)
        {
            if (check == EOF)
                return 0;
            else
                return ERROR();
        }
        
    
        if ( system < 2 || system > 36 || position < 0 )
            return ERROR();
    
    
        number = findNumber(&position, &system);
    
    
        // if system is NOT decimal
        if ( system != 10 )
        {
            string = normalize(&number, &system);
            printf("%s\n", string);
            free(string);
        }
        else
            printf("%ld\n", number);

    
        // cursor shifter
        for( int i = 0; i < position; ++i )
            printf(" ");
    
    
        printf("^\n"); // cursor
    
    }
    
    return 0;

}
