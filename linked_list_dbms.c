// This is simple Database Management System based on linked list.
// It can add, remove and show list.
// db consist just of 2 fields: #order and it's sum.

#include <stdio.h>
#include <stdlib.h> // malloc(), system()
#include <ctype.h>  // toupper()
#include <conio.h>  // getch()

#define CLS system("cls")
#define FFLUSH while(getchar()!='\n') // fflush(stdin)
#define NEXT current=current->next

// create prototype of order
struct order
{
    int number;
    int sum;
    struct order * next;
};

struct order * first;
struct order * newNode;
struct order * current; //always work with current node

char * filename = "list.dat"; // database name

// functions prototype
int showMenu(void); // command available: add, remove and show all
void addNode(void); // addNode ask for memAlloc and getInput
    struct order * memAlloc(void); //allocate memory in heap
    void getInput(int * result, int min, int max, char * welcome);
void removeNode(void);
void showList(void);
void loadList(void); // load list from db
void saveList(void);

int main()
{
    int choice = '\0';
    first = NULL; // there is no list when program begin to work
    loadList();   // load db, if it already exist
    while(choice != 'Q') // main control menu
    {
        choice = showMenu();
        switch (choice)
        {
        case 'S':
            CLS;
            showList();
            break;
        case 'A':
            CLS;
            addNode();
            break;
        case 'R':
            CLS;
            removeNode();
            break;
        case 'Q':
            puts("Exit...");
            saveList(); // save list in db, before exit
            system("pause");
            break;

        default:
            CLS;
            puts("Incorrect input.");
            break;
        }
    }
}

int showMenu(void)
{
    int ch = '\0';
    printf("A - Add node, R - Remove node, S - Show list, Q - Exit\n");
    ch = getch();
    return(toupper(ch));
}

void addNode(void)
{
    if(first == NULL)
    {
        first = memAlloc();
        current = first;
    }
    else
    {
        current = first;
        // move to the end of the list
        while(current->next != NULL)
            current = current->next;
        newNode = memAlloc(); //allocate memory for new order
        current->next = newNode; // last node refer to new
        current = newNode; // new node become current
    }
    current->next = NULL; //close the list

    getInput(&current->number, 0, 1000000, "Enter order number: ");
    getInput(&current->sum, 0, 4000000, "Enter sum of order: ");
}

struct order * memAlloc(void)
{
    struct order * temp;
    temp = (struct order *)malloc(sizeof(struct order));
    if(temp == NULL)
    {
        puts("Error of allocation!!! Exit...");
        system("pause");
        exit(1);
    }
    return temp;
}

void getInput(int * result, int min, int max, char * welcome)
{
    *result = -1; // -1 allow enter the while loop
    while(*result < min || *result > max)
    {
        printf("%s", welcome);
        scanf("%d", result);
        FFLUSH;
    }
}

void removeNode(void)
{
    struct order * prev;
    int rem = 0;
    int num;

    if(first == NULL)
    {
        puts("Nothing to remove! List is empty.");
        return;
    }
    showList();

    getInput(&rem, 1, 1000000, "Choose number of node for remove: ");

    num = 1;
    current = first; // begin from first node
    prev = NULL;     // first node doesn't have previous node
    while(num != rem) // move to node, that should be deleted
    {
        prev = current;  // save previuos node
        current = current->next;
        num++;
        if(current == NULL)
        {
            puts("Node not found.");
            return;
        }
    }

    if(prev == NULL) // if first node should be deleted
        first = current->next;
    else
        prev->next=current->next; // previous node should refer to new node
    printf("Node #%d is removed\n", rem);
    free(current);
}

void showList(void)
{
    int count = 1; //auto increment record numbers
    if(first == NULL)
    {
        puts("List is empty");
        return;
    }
    puts("List:");

    current = first; // begin from first node
    while(current != NULL) //until pointer to next node will be NULL
    {
        printf("Node #%d: order #%d, sum is %d\n",
               count, current->number, current->sum);
        current = current->next; // next node will be current
        count++;
    }
}

void loadList(void)
{
	FILE * fptr;
	struct order loaded; // struct that loaded from db
	int records = 0; // how many struct order will load

	fptr = fopen(filename, "rb");
	if(fptr == NULL) // exist if db is not exist
		return;

	// read binary block from fptr to struct order one by one
	while( fread(&loaded, sizeof(struct order), 1, fptr) )
	{
	    // allocate memory for linked list, same as addNode()
		if(first == NULL)
		{
			first = memAlloc();
			current = first;
		}
		else
		{
			current = first;
			while(current->next)
				NEXT;
			newNode = memAlloc();
			current->next = newNode;
			current = newNode;
		}
		// fill linked list with data from db
		current->number = loaded.number;
		current->sum = loaded.sum;
		current->next = NULL;
		records++; // count how many elements are loaded
	}
	fclose(fptr);
	printf("%d records read from \'%s\'\n", records, filename);
}

void saveList(void)
{
	int records = 0; // how many struct order will save
	FILE * fptr;

    fptr = fopen(filename, "wb");
    //similar to showList(), just printf change to fwrite
	if(fptr == NULL)
	{
		puts("It is not possible to create file for save!");
		return;
	}
    current = first;
    while(current)
    {
        fwrite(current, sizeof(struct order), 1, fptr);
        NEXT; //current = current->next;
        records++;
    }

    fclose(fptr);
    printf("%d records are written to the \'%s\'\n", records, filename);
}
