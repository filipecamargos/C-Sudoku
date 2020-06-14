/***********************************************************************
* Author:
*    Filipe Ferreira
* Summary: 
*    This program writes the code necessary to make a Sudoku game
*    appear on the screen and to allow user interaction
************************************************************************/

#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

void getFile(char fileName[]);
void displayOptions();
void readFile(char fileName[], int board[][9]);
void displayBoard(int board[][9]);
void interact(int board[][9]);
bool getCoordinates(int board[][9], char coordinates[], int &row, int &col);
bool editSquare(int board[][9], char coordinates[], int row,
                int col, bool possibleValue[]);
void writeBoard(int board[][9]);
void computeValue(bool possibleValue[], int board[][9], int row, int col);
void showValue(bool possibleValue[], int board[][9], char coordinates[],
               int row, int col);
void solveBoard(int board[][9], bool possiblesValue[]);

/************************************
* Main will be our driven fuction
*************************************/
int main()
{
   //Get the file for the board
   char fileName[256];
   getFile(fileName);
      
   //Call our fuction to display option
   displayOptions();

   //Deal with the file and the board
   int board[9][9];
   readFile(fileName, board);

   //Display the board
   displayBoard(board);

   //Interac with the user
   interact(board); 
   
   return 0;
}

/************************************
* This fuction will get our file name
*************************************/
void getFile(char fileName[])
{
   cout << "Where is your board located? ";
   cin  >> fileName;
}

/************************************
* It will display our options
*************************************/
void displayOptions()
{
   cout << "Options:\n"
        << "   ?  Show these instructions\n"
        << "   D  Display the board\n"
        << "   E  Edit one square\n"
        << "   S  Show the possible values for a square\n"
        << "   Q  Save and Quit\n"
        << "   Z  Solve the Board\n" << endl;
}

/************************************
* It will display our options
*************************************/
void readFile(char fileName[], int board[][9])
{
   // Open out the file
   ifstream fin(fileName);

   if (fin.fail())
   {
      cout << "Error reading the file" << endl;
   }

   for (int row  = 0; row < 9; row++)
   {
      for (int col = 0; col < 9; col++)
      {
         fin >> board[row][col];
      }
   }
      
   fin.close();
}

/****************************************************
* This fuction will be responsible for displaying our
* board so the use can* see what is in the file and
* start the game
*****************************************************/
void displayBoard(int board[][9])
{
   // Afabetic headers
   cout << "   A B C D E F G H I" << endl;

   // Will display our board through a loop
   for (int i = 0; i < 9; i++)
   {
      //Display my row headers
      cout << i + 1 << "  ";

      // Check for condition to crear the box
      for (int n = 0; n < 9; n++)
      {
         // Check for the vertical line and spaces conditions
         if ( n + 1 == 3 || n + 1 == 6)
         {
            if (board[i][n] == 0)
            {
               cout << " " << "|";
            }
            else
            {
               cout << board[i][n] << "|";
            }
         }
         // Check the codions space lines. 
         else
         {
            if (n == 8 && board[i][n] == 0)
            {
               cout << " ";
            }
            else if (n == 8 && board[i][n] < 10)
            {
               cout << board[i][n];
            }
            else if (board[i][n] == 0)
            {
               cout << " " << " ";
            }
            else
            {
               cout << board[i][n] << " ";
            }
         }
      }

      //check for the horizontal line
      if (i + 1 == 3 || i + 1 == 6)
      {
         cout << endl << "   -----+-----+-----" << endl;
      }
      else
      {
         cout << endl;
      }
      
   }
   cout << endl;
}

/*****************************************************
* This fuction will receive the user imput for the
* option and call the right fuction for interaction
*****************************************************/
void interact(int board[][9])
{
   // All the variables that will be used through my interaction
   char coordinates[3];
   bool possibleValue[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
   int row;
   int col;
   char option;

   //We want to make sure it run at least once to give the opptions
   do
   {
      cout << "> ";
      cin  >> option;
      option = toupper(option);
      
      // Check the input of the user
      switch (option)
      {
         case '?':
            displayOptions();
            cout << endl;
            break;
         case 'D':
            displayBoard(board);
            break;
         case 'E':
            if (getCoordinates(board, coordinates, row, col))
            {
               editSquare(board, coordinates, row, col,
                        possibleValue);
            }
            break;
         case 'S':
            if (getCoordinates(board, coordinates, row, col))
            {
               computeValue(possibleValue, board, row, col);
               showValue(possibleValue, board, coordinates, row, col);
            }
            break;
         case 'Q':
            writeBoard(board);
            break;
         case 'Z':
            solveBoard(board, possibleValue);
            break;
      }
   }
   while (option != 'Q');
}

/*****************************************************
* This fuction give the option to the user to interact
* with the game by change the value inside the array
* (board)
*****************************************************/
bool getCoordinates(int board[][9], char coordinates[], int &row, int &col)
{
   // prompt the coodenate
   cout << "What are the coordinates of the square: ";
   cin  >> coordinates;
   cin.ignore();

   coordinates[0] = toupper(coordinates[0]);
         
   row = (int)coordinates[1] - '1';
   col = (int)coordinates[0] - 'A';

   // Check if it is square is avalaible
   if (board[row][col] != 0)
   {
      //Display a message of error for the invalid square
      cout << "ERROR: Square '" <<  coordinates[0]
           << coordinates[1] << "\' is filled\n" << endl;
      
      return false;
   }
   
   return true;    
}
   
/*****************************************************
* This fuction will receive the users coordinate and
* and will add the value to the right Square
*****************************************************/
bool editSquare(int board[][9], char coordinates[], int row,
                int col, bool possibleValue[])
{
   // Prompt for the value to be added
   int value;
   cout << "What is the value at " << "\'"
        << coordinates[0] << coordinates[1]
        << "\': ";
   cin  >> value;
   cin.ignore();

   // Call our fuction to gives the right values
   computeValue(possibleValue, board, row, col);

   // Set the conditions for update the board
   if (possibleValue[value] == true)
   {    
      board[row][col] = value;
      cout << endl;
      return true;        
   }

   // Display a message if the value is no valide
   else
   {
      cout << "ERROR: Value '" << value
           << "\' in square '" << coordinates[0] << coordinates[1]
           << "\' is invalid" << endl << endl;
   }
   return true;
}

/*****************************************************
* This fuction will receive the new information that
* was add writh the board into a file so it can save
*****************************************************/
void writeBoard(int board[][9])
{
   // Prompt the user for the new file
   char newFile[256];
   cout << "What file would you like to write your board to: ";
   cin  >> newFile;
   cin.ignore();
   
   // Write the information to the new file
   ofstream fout(newFile);
   for (int row = 0; row < 9; row++)
   {
      for (int col = 0; col < 9; col++)
      {
         fout << board[row][col] << " ";
      }
      fout << "\n";
   }
   
   cout << "Board written successfully" << endl;
}

/*****************************************************
* This fuction will be responsible for computating
* the possible values inside the square  
*****************************************************/
void computeValue(bool possibleValue[], int board[][9], int row, int col)
{
   //This fuction set the condition by assing the number in
   // the array as true for the 1 that will be the spot
   // that represent the availble number and false for the rest
   for (int i = 0; i < 10; i++)
   {
      possibleValue[i] = true;
   }
   
   for (int i = 0; i < 9; i++)
   {
      possibleValue[board[i][col]] = false;
   }

   for (int j = 0; j < 9; j++)
   {
      possibleValue[board[row][j]] = false;
   }

   int rowBox = (row / 3) * 3;
   int colBox = (col / 3) * 3;

   for (int i = rowBox; i < rowBox + 3; i++)
   {
      for (int j =  colBox; j < colBox + 3; j++)
      {
         possibleValue[board[i][j]] = false;
      }
   }
}
 
/*****************************************************
* This fuction will get teh infor with the values 
* avalaible inside the array and display it nicelly
*****************************************************/
void  showValue(bool possibleValue[], int board[][9],
                char coordinates[], int row, int col)
{
   //Display our message before run the loop;
   cout << "The possible values for " << "\'"
        << coordinates[0] << coordinates[1]
        << "\' are: ";

   //set our variable to true
   int first = true;
   
   //Set our conditions of displying
   for (int k = 0; k < 10; k++)
   {
      if (possibleValue[k])
      {
         if (!first)
         {
            cout << ", ";
         }
         cout << k;
         first = false;
      }
   }

   cout << endl << endl;   
}

/***********************************************
* This fuction will be responsible for solving
* the board
************************************************/
void solveBoard(int board[][9], bool possiblesValue[])
{
                   
   int numEmpty = 0;
   for(int r = 0; r < 9; r++)
   {           
      for(int c = 0; c < 9; c++)
      {
         if (board[r][c] == 0)
            numEmpty++;
      }
   }
   
   while (numEmpty > 0)
   {           
      for(int row = 0; row < 9; row++)
      {
         for(int col = 0; col < 9; col++)
         {
         
            if (board[row][col] == 0)
            {            
               computeValue(possiblesValue, board, row, col);
               
               int z = 0;
              
               for(int i  = 1; i < 10; i++)
               {
                  if(possiblesValue[i])
                     z++;                     
               }
               
               if (z == 1)
               {                                 
                  for (int n = 1; n < 10; n++)
                     if(possiblesValue[n])
                     {
                        board[row][col] = n;
                        numEmpty-- ;                                             
                     }
               }

               z = 0;
            }
         }
      }
   }
   displayBoard(board);
}
                  
                  
                  

   
