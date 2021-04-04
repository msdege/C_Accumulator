#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "calc.h"
#include <math.h>


unsigned short get_binary_op (char *bin)   // convert bin str to short; return value
{
        unsigned short col, val=0;
        int i;

        col=1;

        for (i=strlen(bin)-1;i>=0;i--){  //iterate through binary string starting at rightmost bit
                if (bin[i]=='1'){        //adding appropriate power of 2 when bit is 1
                        val+=col;
                }
                col*=2;
        }
        return val;
}

void convert_to_binary (short acc, char *bin) // convert acc to binary str for output
{
        bin_str b="0000000000000000";   //b is used for holding binary bits
        int col;                      //col determines the current binary column value
        unsigned short uacc;          //used for converting acc from signed to unsigned
        int i, j=0;                  //i is iterator in for loops, j is index for b

        col=pow(2,15);      
        uacc=acc;

        for (i=0; i<=15; i++){      //loop through b adding ones where necessary
                if (uacc>=col){
                        b[i]='1';
                        uacc-=col;
                }
                col=col/2;
        }
        for (i=1; i<=20; i++){     //copy items from b to bin
            if (i%5==0){           //after every 4 bits, add an empty string in bin and update the counter i
                bin[i-1]=' ';
                i++;               //i goes up to 20, which is the last index of bin plus 1
            }
            bin[i-1]=b[j];        
            j++;                   //j only goes up to 15, the last index of b
        }

}

short get_operand(char mode) // read in numeric value in mode; return value
{ 
    short user_value;
    bin_str b;

    switch (mode)
    {
        case 'B': printf("Enter binary value: ");
                scanf("%s", b);
                printf("%s\n", b);              //echo print
                user_value=get_binary_op(b);
                break;

        case 'O': printf("Enter octal value: "); 
                scanf("%ho", &user_value); 
                printf("%ho\n", user_value);    //echo print
                break;

        case 'H': printf("Enter hex value: "); 
                scanf("%hx", &user_value); 
                printf("%hx\n", user_value);    //echo print
                break; 

        case 'D': printf("Enter decimal value: "); 
                scanf("%hd", &user_value); 
                printf("%hd\n", user_value);    //echo print
                break;
    }
    return user_value;
}

void add (short *acc, char mode) // call get_operand to get val in mode to add to accum; detect pos/neg overflow
{
    short int orig_acc_plus;
    short op_plus;
    
    orig_acc_plus=*acc;
    op_plus=get_operand(mode);
    *acc+=op_plus;

    if (orig_acc_plus>0 && op_plus>0 && *acc<0){   //pos overflow
        printf("Positive Overflow\n");
    }
    if (orig_acc_plus<0 && op_plus<0 && *acc>0){   //neg overflow
        printf("Negative Overflow\n");
    }
}

void subtract (short *acc, char mode) // similar to add, but subtract
{
    short int orig_acc_minus;
    short op_minus;
    
    orig_acc_minus=*acc;
    op_minus=get_operand(mode);
    *acc-=op_minus;

    if (orig_acc_minus>0 && (-op_minus)>0 && *acc<0){ //pos overflow
        printf("Positive Overflow\n");
    }
    if (orig_acc_minus<0 && (-op_minus)<0 && *acc>0){ //neg overflow
        printf("Negative Overflow\n");
    }
}

void print_acc(short acc, char mode) // print out accumulator values
{   
    char* acc_mode[]={"Bin","Oct","Hex","Dec"};
    int mode_value;   //integer to access appropriate index in acc_mode array
    bin_str z;      //create local bin_str for call to convert_to_binary

    convert_to_binary(acc, z);
    
    switch(mode)   //determines appropriate index
    {
        case 'B': mode_value=0;
                break;
        case 'O': mode_value=1;
                break;
        case 'H': mode_value=2;
                break;
        case 'D': mode_value=3;
                break;
    }
    
    printf("\n****************************************\n");
    printf("* Accumulator:         Input Mode: %s *\n", acc_mode[mode_value]);
    printf("*   Binary  :  %s    *\n", z);
    printf("*   Hex     :  %04hx                    *\n", acc);
    printf("*   Octal   :  %06ho                  *\n", acc);
    printf("*   Decimal :  %-6hd                  *\n", acc);
    printf("****************************************\n");
    
}

char print_menu(void) //print menu; get option until valid; return option
{
    char input[10];  //holds user input
    char option;     //holds first character in user input, i.e. the option
    char valid_options[]="BOHDCSQ&|^~<>+-N";
    
    printf("\nPlease select one of the following options:\n\n");
    printf("B  Binary Mode             &  AND with Accumulator           +  Add to Accumulator\n");
    printf("O  Octal Mode              |  OR  with Accumulator           -  Subtract from Accumulator\n");
    printf("H  Hexadecimal Mode        ^  XOR with Accumulator           N  Negate Accumulator\n");
    printf("D  Decimal Mode            ~  Complement Accumulator\n\n");
    printf("C  Clear Accumulator       <  Shift Accumulator Left\n");
    printf("S  Set Accumulator         >  Shift Accumulator Right\n\n");
    printf("Q  Quit\n\n");
    printf("Option: ");

    scanf("%s",input);
    printf("%s\n",input);        //echo print
    option=toupper(input[0]);
    
    if (strlen(input)>1 || !strchr(valid_options, option)){
        printf("\nInvalid option: %s\n", input);
        print_menu();
    }
    else {
        return option;
    }
}

int main(void)
{
    
    char mode='D';      //initial mode is decimal
    char selection='D'; //initiate selection to enter the while loop 
    short acc=0, second_acc=0;        //initial acc values are zero
    int bit_shift;    //initialize variable for left/right shift

    while (selection != 'Q'){
        print_acc(acc, mode);
        selection=print_menu();
        switch (selection){

                case 'B': mode='B';
                        printf("Mode is Binary\n");
                        break;

                case 'O': mode='O'; 
                        printf("Mode is Octal\n");
                        break;

                case 'H': mode='H'; 
                        printf("Mode is Hexidecimal\n");
                        break;

                case 'D': mode='D'; 
                        printf("Mode is Decimal\n");
                        break;

                case 'C': acc=0; 
                        mode='D';
                        break;

                case 'S': acc=get_operand(mode);
                        break;

                case '&': second_acc=get_operand(mode);
                        acc=second_acc&acc;
                        break;

                case '|': second_acc=get_operand(mode);
                        acc=second_acc|acc;
                        break;

                case '^': second_acc=get_operand(mode);
                        acc=second_acc^acc;
                        break;

                case '~': acc=~acc;
                        break;

                case 'N': acc=-acc;
                        break;

                case '<': printf("Enter number of positions to left shift accumulator: ");
                        scanf("%d", &bit_shift);
                        printf("%d\n", bit_shift);   //echo print
                        acc=acc<<bit_shift;
                        break;

                case '>': printf("Enter number of positions to right shift accumulator: ");
                        scanf("%d", &bit_shift);
                        printf("%d\n", bit_shift);   //echo print
                        acc=acc>>bit_shift;
                        break;

                case '+': add(&acc, mode);
                        break;

                case '-': subtract(&acc, mode);
                        break;
        }
        
    }
    return 0;
}
