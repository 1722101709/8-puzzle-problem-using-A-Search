#include <stdio.h>
#include <string.h>
#include <iostream.h>
#include <math.h>
#include <stdlib.h>

#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3
#define H2

struct elementstruct// represent block
{
  int block[9];//stores numbers on the block
  char* str;//to stores moves
  int pathcost;//cost 
  int valid;//valid or invalid block
  int totalcost;//total cost
  elementstruct* next;//link to next move or block
};

int heur(int block[]);//to find heuristic value
void prepend(elementstruct* newnode, elementstruct* oldnode, int operator1);//to create new move
int goal(int* block);//check goal
int notonqueue(int block[]);
elementstruct* bestnodefromqueue();//to store the optimum node out of 4 (up,down,left,right)
void print_block(int* block);//to print the block
int apply (int* newstate, int* oldstate, int op);//to move the blank or apply the operations like left,right,....
elementstruct* newelement();//new move
int op(char);//to know the operation(like left,top,....)
char to_char(int i);//int to char

char rep[] = "dulr";//repository of directions or operations
int notvalid1[4] = { 6, 0, 0, 2 };//invalid moves 
int notvalid2[4] = { 7, 1, 3, 5 };//invalid moves
int notvalid3[4] = { 8, 2, 6, 8 };//invalid moves
int applyparam[4] = { +3, -3, -1, +1 };
int goal_block[9] = { 0, 1, 2, 3, 4, 5, 6, 7, 8}; //8 indicates no tile
int maxdepth;//max search depth
elementstruct* top;//initial state

int main()
{
  int block[9];//to store the given input

  printf("\nThe Eight Puzzle problem using A * Search\n");

  printf("\nPlease Enter the initial state of the game \n"
" [Represent tiles with numbers 1 to 8, and the blank space as 'x'.\n"
" Your final string will look similar to this '1 4 2 3 x 6 7 8 5'.\n");

  int i = 0;
  while(i<9)
	{
	char chr;
	chr = fgetc(stdin);//to get each character from the std input
	if (chr==32) continue;//ignore spaces
	if (chr=='x') 
	block[i] = 8;//replace x(blank) with 8
	else if (chr >= '1' && chr <= '9') 
	block[i] = chr - '1';// store i as i-1
	else 
	{ 
	printf("Invalid Input. Example of valid input...2 1 3 4 7 5 6 8 x.", chr); //invalid inputs
	return 1; 
	}
	i++;
	}

  fgetc(stdin); //flush out the end of line character

  printf("\n Now Enter the Goal State in a similar way. (Typical. 1 2 3 4 5 6 7 8 x): ");

  i = 0;
  while(i<9)//similar to initial state input , we are taking goal state as input from the user
	{
	char chr;
	chr = fgetc(stdin);
	if (chr==32) continue;
	if (chr=='x') goal_block[i] = 8;
	else if (chr >= '1' && chr <= '9') goal_block[i] = chr - '1';
	else {
		 printf("chr=%d. Invalid Input. Example of valid input...2 1 3 4 7 5 6 8 x.",(int) chr); 
		 return 1; 
		 }
	i++;
	}

  printf("Enter the maximum depth you want to search (<25 is solved quickly): ");
  scanf("%d", &maxdepth);//asking max search depth to save the time

  printf("\nWorking...");

  top = newelement();//new element
  for(i=0; i<9; i++)
    top->block[i] = block[i];//initialize with user given values i.e initial state
  top->totalcost = heur(block);//heuristic of initial state 
  
  elementstruct* newnode = newelement();//to create another newnode to store the next move

  while (1)
    {
      elementstruct* node = bestnodefromqueue();//best of 4 nodes
      if (node == NULL) 
      {
	printf("done!\n");//no solution
	printf("There is no solution to this of less than %d depth.\n", maxdepth);
	printf("Try increasing the depth.\n");
	break;
      }
      else if (goal(node->block)) //reached goal state
      {
	char chr[15];
	printf("done. \nFound the solution of least number of steps (%d).", node->pathcost);//to show pathcost
	printf("\nWant a graphical display of each step? (Y/N)?");//to ask for displaying each move
	scanf("%s", chr);
	if(chr[0] =='n' || chr[0]=='N') {
	  printf("\n (Move Blank u=up, d=down, l=left, r=right)\n");
	  printf(node->str);//if no just prints the directions like "ludrrlu" where l,d,r,u are directions
	  printf("\n");
	  break;
	}
	//if yes then
	int block2[9];
	for (i=0; i<node->pathcost; i++)//printing pathcost number of blocks
	  {
	    print_block(block);//print block
	    apply(block2, block, op(node->str[i]));//to apply left,right ,... operation on current block and store in newblock block2
	    for(int j=0; j<=8; j++)
	      block[j] = block2[j];//copy block2 values into block
	  }
	print_block(block);//to print the final state after moving to any direction
	
	printf("\nGraphical Display Complete.\nThe steps taken were: (Move blank u=up, d=down, l-left, r=right)\n");
	printf(node->str);//like "rudllddur"where l,r,d,u are directions
	printf("\n");
	break;
	
      }

      if (node->totalcost > maxdepth)
      continue;//as user given max depth , depth wont cross the specified depth

      for(i=0; i<=3; i++) {
	if (apply(newnode->block, node->block, i) == -1)//invalid move
	  continue;

	if (notonqueue(newnode->block)) {
	  prepend(newnode, node, i);//create next block or next staye after moving blank
	  newnode = newelement();
	  if (newnode==NULL)
	   { 
	   //if newnode is not created then that means no extra memory for the process in the memory
	  printf ("ERROR!! insufficient memory!! Try decreasing depth!"); 
	  return 1; 
	  }
	}
      }

	}
	return 0;
}

int heur(int* block)
{
	//to calculate heuristic value of the current state or block
#ifdef H2
  int to_return = 0;

  for(int i=0; i<9; i++)
    {
    to_return += abs((i/3) - (block[i]/3));
    to_return += abs((i%3) - (block[i]%3));
    }
  return to_return;
  
#else
  int to_return = 0;

  for(int i=0; i<9; i++)
    {
      if (block[i] != i) to_return++;
    }
  return to_return;
  

#endif

}


void prepend(elementstruct* newnode, elementstruct* oldnode, int op)//to initialize next state with dirextions,pathcost ,totalpathcost ,block values
{
  newnode->next = top;
  top = newnode;

  strcpy(newnode->str, oldnode->str);
  newnode->str[oldnode->pathcost] = rep[op];
  newnode->str[oldnode->pathcost+1] = 0;

  newnode->pathcost = oldnode->pathcost+1;
  newnode->totalcost = newnode->pathcost + heur(newnode->block);
  if (newnode->totalcost < oldnode->totalcost) 
  newnode->totalcost = oldnode->totalcost;
}


int goal(int* block)
{
  int* g_block = goal_block;
//checks each value of reached state block with the goal state values
  for(int i=0; i<9; i++)
    if ((*(block++))!=(*(g_block++)))
      return 0;//if any value is not same then return 0

  return 1;// means reached goal state
}

int notonqueue(int* block)
{
  int i,j;
  elementstruct* t = top;//initial state
  
  while (t!=NULL)
    {
      for(i=0; i<9; i++)
	if (t->block[i] != block[i]) 
	break;
      if (i==9) //if all 9 values are same then returns 0
      return 0;
      
      t = t->next;// to go the next state
    }
  return 1;
}

elementstruct* bestnodefromqueue()
{
  elementstruct* t = top;
  int min_totalpathcost = 1000;//initialize to large integer
  int totalpathcost;
  elementstruct* to_return = NULL;

  while (t != NULL)
    {
    	//finding min total path cost move
	  if (t->valid==1 && t->totalcost < min_totalpathcost)
	{
	min_totalpathcost = t->totalcost;
	to_return = t;
	}
      t = t->next;
    }
  
  if (to_return != NULL) 
  to_return->valid = 0;

  return to_return;
}


int apply (int* newstate, int* oldstate, int op)// to move the blank
{
  int j;
  int blank;// to store the blank index

  for (j=0; j<9; j++)//to find the blank index
	if (oldstate[j]==8)
	 { 
	blank=j; 
	break; 
	}

  if (blank==notvalid1[op] || blank==notvalid2[op] || blank==notvalid3[op])//invalid oprations
    return -1;

  for (j=0; j<9; j++)  
    newstate[j] = oldstate[j];
//below 3 lines are to move the blank by swaping the array values
  newstate[blank] = newstate[blank+applyparam[op]];
  newstate[blank+applyparam[op]] = 8;

  return 1;
}

elementstruct* newelement()// to create , initialize and return new block
{
  elementstruct* t = new elementstruct;
  if (t==NULL)
  return NULL;
  t->valid = 1;
  t->str = new char[maxdepth+1];
  if (t->str ==NULL) 
  return NULL;
  t->str[0] = 0;
  t->pathcost = t->totalcost = 0;
  t->next = NULL;
  return t;
}

void print_block(int* block)//to print the block 
{
  printf("\n");
  printf ("\n-------");
  printf ("\n|%c|%c|%c|", to_char(block[0]), to_char(block[1]), to_char(block[2]));//1st row
  printf ("\n-------");
  printf ("\n|%c|%c|%c|", to_char(block[3]), to_char(block[4]), to_char(block[5]));//2nd row
  printf ("\n-------");
  printf ("\n|%c|%c|%c|", to_char(block[6]), to_char(block[7]), to_char(block[8]));//3rd row
  printf ("\n-------");
}

char to_char(int i)// to convert int to char
{
  if (i>=0 &&i<=7) return i+'1';
  else if (i==8) return 'x';
  else { printf("ERROR in Program!"); return -1; }

}

int op(char i)//to map char representing direction to the character
{
switch (i)
  {
  case 'd': return 0;
  case 'u': return 1;
  case 'l': return 2;
  case 'r': return 3;
  default: printf("ERROR!"); return -1;
  }
}




