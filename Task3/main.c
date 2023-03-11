#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif /* __PROGTEST__ */




// OVERALL CONSUMPTION VALUES OF GIVEN TIMELINE IN JOULES

#define MIN_CONS    200      // 200J for one minute transition + proper digitConsumption value
#define HOUR_CONS   12200    // 12200J for one complete hour 60 x 200 + 200
#define DAY_CONS    292886   // 24 x 12200 + 86 [00->23->00]

// -----------------------------------------------------

#define T128_CONS   13692715729088  // 128000 years consumption
#define T64_CONS    6846357864544   // 64000  years consumption
#define T32_CONS    3423178932272   // 32000  years consumption
#define T16_CONS    1711589466136   // 16000  years consumption
#define T8_CONS     855794733068    // 8000   years consumption
#define T4_CONS     427897366534    // 4000   years consumption


// how many Joules are needed for a given digit transition
//                                0->1->2->3->4->5->6->7->8->9->0
const int digitConsumption[10] = { 4, 5, 2, 3, 3, 1, 5, 4, 1, 2 };
//                                 0  1  2  3  4  5  6  7  8  9

const int daysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };



// ---------------------- MINUTES ----------------------
long minutesDifferenceConsumption( int from, int to )
{
    
    long summ = 0;
    
    
    while ( from != to )
    {
        
        if ( from < to )
        {
            summ += MIN_CONS;
            summ += digitConsumption[from%10]; // days' first digit changed
            
            if ( from%10 == 9 )
                summ += digitConsumption[from/10];
            
            from++;
            continue;
        }
        
        summ -= MIN_CONS;
        summ -= digitConsumption[(from-1)%10]; // reverse direction
        
        if ( from%10 == 0 ) // if second number was ending with 0
            summ -= digitConsumption[(from/10)-1]; // ((from-1)/10) would also work
        
        from--;
        
    }

    return summ;
    
}
// ------------------------------------------------------


// ----------------------- HOURS ------------------------
long hoursDifferenceConsumption( int from, int to )
{
    
    long summ = 0;
    
    while ( from != to )
    {
        
        if ( from < to )
        {
            summ += HOUR_CONS;
            summ += digitConsumption[from%10]; // days' first digit changed
            
            if ( from%10 == 9 )
                summ += digitConsumption[from/10];
            
            from++;
            continue;
        }
        
        summ -= HOUR_CONS;
        summ -= digitConsumption[(from-1)%10]; // reverse direction
        
        if ( from%10 == 0 ) // if second number was ending with 0
            summ -= digitConsumption[(from/10)-1]; // ((from-1)/10) would also work
        
        from--;

    }

    return summ;
    
}
// ------------------------------------------------------


// ------------------------ DAYS ------------------------
long daysDifferenceConsumption( int from, int to )
{
    long summ = 0;

    while ( from != to )
    {
        
        if ( from < to )
        {
            summ += DAY_CONS;
            from++;
            continue;
        }
        
        summ -= DAY_CONS;
        
        from--;
        
    }
    
    return summ;
}
// ------------------------------------------------------


int isLeapYear(int y)
{
    if (y%4000==0)
        return 0;
    
    if (y%400==0)
        return 1;
    
    if (y%100==0)
        return 0;
    
    if (y%4==0)
        return 1;
    
    else
        return 0;
}



// validates the date provided
int valiDate( int y, int m, int d, int h, int i )
{
    
    if ( y < 1600 )
        return 0;
    
    if ( m < 1 || m > 12 || d < 1 || h > 23 || h < 0 || i > 59 || i < 0 )
        return 0;
    
    if ( m==1 || m==3 || m==5 || m==7 || m==8 || m==10 || m==12 )
        if ( d > 31 )
            return 0;
    
    if ( m==4 || m==6 || m==9 || m==11 )
        if ( d > 30 )
            return 0;
    
    if ( isLeapYear(y) && m==2 )
        if ( d > 29 )
            return 0;
    
    if ( !isLeapYear(y) && m==2 )
        if ( d > 28 )
            return 0;
    
    return 1;
}




// ------ PROGTEST DEFINED FUNCTION ------
int energyConsumption
(
 int y1, int m1, int d1, int h1, int i1,    // FROM
 int y2, int m2, int d2, int h2, int i2,    // TO
 long long int * consumption
)
{
 
    *consumption = 0;
    
    // valiDate the dates provided first
    if ( !valiDate(y1, m1, d1, h1, i1) || !valiDate(y2, m2, d2, h2, i2) )
        return 0;
    
    // no time travel allowed in my code
    if ( y1 > y2 )
        return 0;
    
    // MINUTES
    *consumption += minutesDifferenceConsumption(i1, i2);

    // HOURS
    *consumption += hoursDifferenceConsumption(h1, h2);
    
    // DAYS
    *consumption += daysDifferenceConsumption(d1, d2);
    d1 = d2;

    
    // MONTHS -------------------------------------------
    while ( m1 != m2 )
    {
        if ( m1 < m2 )
        {
            *consumption += daysInMonth[m1-1] * DAY_CONS;
            
            if ( m1==2 )
                if ( isLeapYear(y1) )
                    *consumption += DAY_CONS;
            
            m1++;
            continue;
        }
        
        *consumption -= daysInMonth[m1-2] * DAY_CONS;

        if ( m1==3 )
            if ( isLeapYear(y1) )
                *consumption -= DAY_CONS;
        
        m1--;
    }
    // --------------------------------------------------


    // speed boost
    if ( y2 > 4000 ) // long interval progTEST
    {
        while( (y2 - y1) > 3999 )
        {
            if ( (y2 - y1) > 127999 )
            {
                y1 += 128000;
                *consumption += T128_CONS;
                continue;
            }
            if ( (y2 - y1) > 63999 )
            {
                y1 += 64000;
                *consumption += T64_CONS;
                continue;
            }
            if ( (y2 - y1) > 31999 )
            {
                y1 += 32000;
                *consumption += T32_CONS;
                continue;
            }
            if ( (y2 - y1) > 15999 )
            {
                y1 += 16000;
                *consumption += T16_CONS;
                continue;
            }
            if ( (y2 - y1) > 7999 )
            {
                y1 += 8000;
                *consumption += T8_CONS;
                continue;
            }
            if ( (y2 - y1) > 3999 )
            {
                y1 += 4000;
                *consumption += T4_CONS;
                continue;
            }
        }
    }
    
    
    
    // YEARS --------------------------------------------
    while ( y1 < y2 )
    {
        *consumption += (365 * DAY_CONS);
        
        if ( m2 < 3 )
            if ( isLeapYear(y1) )
                *consumption += DAY_CONS; // https://youtu.be/fwK7ggA3-bU
            
        
        if ( isLeapYear(y1+1) && (m2 > 2) )
            *consumption += DAY_CONS;
        
        y1++;
    }
    // --------------------------------------------------
    
    
    // negative consumption means date2 < date1
    if ( *consumption < 0 )
        return 0;
    
    
    return 1; // success
    
}
// ---------------------------------------




#ifndef __PROGTEST__
int main ( int argc, char * argv [] )
{
    long long int consumption;
    
    assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                                2021, 10,  1, 18, 45, &consumption ) == 1
            && consumption == 67116LL );
    
    assert ( energyConsumption ( 2021, 10,  1, 13, 15,
                                2021, 10,  2, 11, 20, &consumption ) == 1
            && consumption == 269497LL );
    assert ( energyConsumption ( 2021,  1,  1, 13, 15,
                                2021, 10,  5, 11, 20, &consumption ) == 1
            && consumption == 81106033LL );
    assert ( energyConsumption ( 2024,  1,  1, 13, 15,
                                2024, 10,  5, 11, 20, &consumption ) == 1
            && consumption == 81398919LL );
    assert ( energyConsumption ( 1900,  1,  1, 13, 15,
                                1900, 10,  5, 11, 20, &consumption ) == 1
            && consumption == 81106033LL );
    assert ( energyConsumption ( 2021, 10,  1,  0,  0,
                                2021, 10,  1, 12,  0, &consumption ) == 1
            && consumption == 146443LL );
    assert ( energyConsumption ( 2021, 10,  1,  0, 15,
                                2021, 10,  1,  0, 25, &consumption ) == 1
            && consumption == 2035LL );
    assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                                2021, 10,  1, 12,  0, &consumption ) == 1
            && consumption == 0LL );
    assert ( energyConsumption ( 2021, 10,  1, 12,  0,
                                2021, 10,  1, 12,  1, &consumption ) == 1
            && consumption == 204LL );
    assert ( energyConsumption ( 2021, 11,  1, 12,  0,
                                2021, 10,  1, 12,  0, &consumption ) == 0 );
    assert ( energyConsumption ( 2021, 10, 32, 12,  0,
                                2021, 11, 10, 12,  0, &consumption ) == 0 );
    assert ( energyConsumption ( 2100,  2, 29, 12,  0,
                                2100,  2, 29, 12,  0, &consumption ) == 0 );
    assert ( energyConsumption ( 2400,  2, 29, 12,  0,
                                2400,  2, 29, 12,  0, &consumption ) == 1
            && consumption == 0LL );
    
    
    return 0;
}
#endif /* __PROGTEST__ */
