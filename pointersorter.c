#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 *the Nodes that will make up the linked list
 */
typedef struct Nodes
{
	char * string; //designed to hold the string
	struct Nodes * next; //connects the node to another
}Node;

/*
*   Function Declarations in Order as they appear
*/
int stringSplitter(char *input);
int minimum(char*nodeString, char*newString);
int stringCompare(char * nodeString, char * newString);
int addNode(Node**head, char *input);
void printList(Node*cursor);
void clearList(Node**head);
int errorMessage(int errorCode, int line);
/*
 * Function: main
 *-----------------------
 *   Checks to make sure the number of arguments match the expected, then
 *   calls stringSplitter() with the inputted string(argv[1])
 *
 *   argc: the number of arguments (always >= 1)
 *   argv: the array of arguments (argv[0] is always the name of the program)
 *
 *   returns: 0 (if its successful), -1 if something fails
 */
int main(int argc, char *argv[])
{
    if(argc == 1) //if only 1 argument is passed (which is the name of the executable itself)
    {
        printf("Warning: Not enough arguments!"); //tells the user we don't have a string to split
    }
    else //there was a string in the 2nd argument (ignoring the 3rd or more arguments if any)
    {
        return stringSplitter(argv[1]);//returns any error code we got farther down the file
    }
    return 0; //return 0 if everything went well
}

/*
 * Function: stringSplitter
 *-----------------------
 *   Takes in a string (Currently takes in 1st argument) and
 *   splits the string into substrings that only contain alphabetical
 *   characters
 *
 *   Called by: main() (line 41)
 *
 *   input: the string we will be splitting
 *
 *   returns: 0 (if its successful), -1 if something fails
 */
int stringSplitter(char *input)
{
    char *tempString = NULL; //design to hold the new string temporarily until it can be passed to a node
    int position = 0; //we use to know how far down the string we are
    int start = 0; //we use to keep track of the beginning of the string
    int stop = -1; //we use to keep track the end of the string
    int length = 0; //length of the string
    int errorCode = 0; //the value we return to main to tell if program ran successfully or not
    Node * head = NULL; //initialize the head node
    while (*input != '\0' && errorCode == 0) //keep going until either we hit the end of the string or code down the line fails
    {
        if (isalpha(*input)) //is c an alphabetic character
        {
            start = position; //point to where the new string starts
            position++; //move the position along
            input++; //move input along the string
            while (isalpha(*input)) //keeps adding to the string if there alphabetical characters
            {
                input++; //move input along the string
                position++; //move the position along
            }
            stop = position; //tells us where the string ends
            length = stop - start; //get the length of the string
            tempString = malloc(sizeof(char)*(length+1)); //allocate memory and add a byte for '\0'
            if(tempString == NULL) //if malloc() failed return errorCode and message
            {
                errorCode = errorMessage(-1, 88); //tell user malloc() failed to allocate memory
            }
            else //if it worked, go ahead and add new string to the list
            {
                strncpy(tempString,input - length, length); //copy the new string to the string variable
                tempString[length] = '\0'; //null terminate the new string
                errorCode = addNode(&head, tempString); //add the node and return an error code if any
                free(tempString); //free string since we don't need it anymore
            }
        }
        else //else move onto the next
        {
            position++; //move the position by one
            input++; //move input cursor down the string
        }
    }
    if(stop == -1) //tell user there were no strings to find
    {
        printf("Warning: No strings found!");
    }
    printList(head); //print the string inside every node
    clearList(&head); //clear the entire list including head
    return errorCode; //tell main if program is successful or not
}


/*
* Function: minimum
*-----------------------
*   Finds which string is the shortest in length
*
*   Called by: strCompare() (line 154)
*
*   nodeString: the string from the linked list being compared
*   newString: the new string to be added to the linked list
*
*   returns: the length of smallest string
*/
int minimum(char * nodeString, char * newString)
{
	int lengthOfnodeString = strlen(nodeString); //length of the node string
	int lengthOfnewString = strlen(newString); //length of the new string
	if (lengthOfnodeString < lengthOfnewString) //if the node string has less characters
		return lengthOfnodeString;
	else //if the new String is shorter than the node string
		return lengthOfnewString; //
}

/*
* Function: stringCompare
*-----------------------
*  compares two strings by first
*  getting the length of the smaller
*  of the two strings, then compares
*  the strings to min bytes. If the 2 strings have the same min bytes
*  then the function chooses which string is the shortest
*
*   Called by: addNode() (line 209)
*
*   nodeString: the string from the linked list being compared
*   newString: the new string to be added to the linked list
*
*   returns: -1 (string1 is after string2), 1 (string1 is before string2
*/
int stringCompare(char * nodeString, char * newString)
{
	int min = minimum(nodeString, newString);//get least string length for comparison
	int compareNum = strncmp(nodeString, newString, min);//compare the strings up to min characters

	if (compareNum == 0) //if the strings share the same n bytes
		if (strlen(nodeString) < strlen(newString))
			return -1;//nodeString goes before newString
		else
			return 1; //newString goes before nodeString
	else
		return compareNum; //return the comparison value
}

/*
* Function: addNode
*-----------------------
*    Adds a new string to the linked list
*    it checks if the list is empty first
*    otherwise, it searches through the list to
*    to find which node to put it after
*
*    Called by: stringSplitter()
*
*    head: the beginning of the linked list
*
*    returns: 0 if everything went smoothly, -1 if something went wrong
*/
int addNode(Node**head, char *input)
{
	int length = strlen(input)+1; //length of the input string plus one for null terminator
	if (!head) //if head is null
	{
		//allocates memory space for the head Node
		*head = (Node*)malloc(sizeof(Node)); //allocate memory for head
		if(*head == NULL) //check if malloc() allocated memory for head
        {
            return errorMessage(-2,189); //tell user malloc() failed to allocate memory for node
        }
		//makes the head Node contain the new string
		(*head)->string = malloc(sizeof(char)*length);
		if((*head)->string == NULL)
        {
            return errorMessage(-1,195); //tell user
        }
		strncpy((*head)->string,input,length); //copy the string from input into the node
		(*head)->string[length-1] = '\0'; //null terminate the string
		//the head currently doesn't connect to another Node right now
		(*head)->next = NULL;
	}
	else//if head Node doesn't exist, make the new string the head
	{
		short isItPlaced = 0; //used to detected if it found a spot for the new string
		Node * cursor = *head; //starts the cursor at the head of the linked list
		Node * previousCursor = NULL; //will follow behind the cursor if we need to put the new in between 2 Nodes in the linked list
		while (cursor && !isItPlaced) //while the cursor is still traversing and we haven't found a spot for the new Node
		{
			if (stringCompare(cursor->string, input) > 0) //if the string should be before the cursor
			{
				isItPlaced = 1; //found the spot to place the new Node
			}
			else //needs to traverse further in the linked list
			{
				previousCursor = cursor; //previous cursor now points to where cursor did
				cursor = cursor->next; //cursor traverses the list
			}
		}
		Node * newNode; //initialize the new Node to insert in the linked list
		newNode = (Node*)malloc(sizeof(Node)); //allocate's memory space for the new Node
		if(newNode == NULL) //check if malloc() failed
        {
            return errorMessage(-2,223); //tell user malloc() failed to allocate memory for a new node
        }
		newNode->string = malloc(sizeof(char) * length); //allocate memory to hold the string
		if(newNode->string == NULL) //checks if malloc() failed
        {
            return errorMessage(-1,228); //tell the user malloc() failed
        }
		strncpy(newNode->string,input,length); //copies the string from input into the new node
		newNode->string[length-1] = '\0'; //null terminates the string
		if (cursor == *head) //the new Node will become the head Node
		{
			Node ** newHead = head; //take a pointer of the head (pointer of a pointer)
			newNode->next = *newHead; //the new Node now points to the previous head
			*newHead = newNode; //head now points to the new Node making it first Node
		}
		else //puts the new Node
		{
			newNode->next = cursor; //the new Node now points to where the cursor is pointing to
			previousCursor->next = newNode; //the previous cursor now points to the new Node
		}
	}
	return 0; //everything worked
}

/*
* Function: printList
*-----------------------
*    Prints each string inside each node until it hits the end
*
*    Called by: stringSplitter() (line 108)
*
*    cursor: traverses through the linked list starting at the head
*
*    returns: Nothing
*/
void printList(Node * cursor)
{
	while (cursor) //while cursor doesn't equal to NULL
	{
		printf("%s\n", cursor->string); //prints current Node's string
		cursor = cursor->next; //moves to next Node
	}
}

/*
* Function: clear
*-----------------------
*  Clears all allocated memory given the the linked list
*
*  Called by: stringSplitter() (line 109)
*
*  head: the beginning of the linked list
*
*  returns: Nothing
*/
void clearList(Node ** head)
{
	Node * cursor = *head; //points to where head is actually pointing at
	Node * nodeToDelete = NULL; //design to hold the node to be deleted
	while (cursor != NULL) //loops through linked list until it hits the end
	{
		nodeToDelete = cursor; //moves the cursor node to nodeTODelete so it can be cleared
		cursor = cursor->next; //moves the cursor to next node in the list
		nodeToDelete -> next = NULL; //clears pointer
		free(nodeToDelete -> string); //clears string since memory is allocated
		free(nodeToDelete); //clears the struct itself and the memory allocated
	}
	*head = NULL; //clears the head node itself
}

/*
* Function: errorMessage
*-----------------------
*  Sends the user a message if something goes wrong
*  like if malloc() failed to allocate memory
*
*  Called by: stringSPlitter() (line 88) and addNode() (line 189, 195, 223, and 228)
*
*  errorCode: determines what went wrong with the program
*  line: the line that caused an error
*
*  returns: The error code so the main can return it
*/
int errorMessage(int errorCode, int line)
{
    switch(errorCode) //determines what message to display based on error code
    {
        case -1: //malloc() failed to allocate memory for a new string
            printf("Error: Memory was not able to be allocated for new string %d", line);
            break;
         case -2://malloc() failed to allocate memory for a new Node
            printf("Error: Memory was not able to be allocated for new node on line %d", line);
            break;
    }
    return errorCode; //returns the error code
}
