#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_SLOTS 10

// Linked list for parked vehicles
struct vehicle
{
    char number[20];
    char owner[30];
    char type[15];
    int slot;
    struct vehicle *next;
};

// Queue for vehicles waiting when parking is full
struct waiting
{
    char number[20];
    char owner[30];
    char type[15];
    struct waiting *next;
};

struct vehicle *head = NULL;

struct waiting *front = NULL;
struct waiting *rear = NULL;

int totalParked = 0;


/* Find a vehicle using its number */
struct vehicle* searchVehicle(char number[])
{
    struct vehicle *p = head;

    while (p != NULL)
    {
        if (strcmp(p->number, number) == 0)
        {
            return p;
        }

        p = p->next;
    }

    return NULL;
}


/* Find the first available parking slot */
int findSlot()
{
    int i;
    struct vehicle *p;

    for (i = 1; i <= TOTAL_SLOTS; i++)
    {
        p = head;

        while (p != NULL)
        {
            if (p->slot == i)
            {
                break;
            }

            p = p->next;
        }

        if (p == NULL)
        {
            return i;
        }
    }

    return -1;
}


/* Add vehicle to waiting queue */
void addToQueue(char number[], char owner[], char type[])
{
    struct waiting *newnode;

    newnode = (struct waiting*)malloc(sizeof(struct waiting));

    if (newnode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    strcpy(newnode->number, number);
    strcpy(newnode->owner, owner);
    strcpy(newnode->type, type);

    newnode->next = NULL;

    if (rear == NULL)
    {
        front = rear = newnode;
    }
    else
    {
        rear->next = newnode;
        rear = newnode;
    }

    printf("\nParking is full.\n");
    printf("Vehicle has been added to waiting queue.\n");
}


/* Remove first vehicle from waiting queue */
struct waiting* removeFromQueue()
{
    struct waiting *temp;

    if (front == NULL)
    {
        return NULL;
    }

    temp = front;
    front = front->next;

    if (front == NULL)
    {
        rear = NULL;
    }

    temp->next = NULL;

    return temp;
}


/* Park a vehicle */
void parkVehicle()
{
    char number[20];
    char owner[30];
    char type[15];

    int slot;

    struct vehicle *newnode;

    printf("\nEnter vehicle number: ");
    scanf("%19s", number);

    if (searchVehicle(number) != NULL)
    {
        printf("This vehicle is already parked.\n");
        return;
    }

    printf("Enter owner name: ");
    scanf(" %29[^\n]", owner);

    printf("Enter vehicle type (Car/Bike/Scooty): ");
    scanf("%14s", type);

    slot = findSlot();

    if (slot == -1)
    {
        addToQueue(number, owner, type);
        return;
    }

    newnode = (struct vehicle*)malloc(sizeof(struct vehicle));

    if (newnode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    strcpy(newnode->number, number);
    strcpy(newnode->owner, owner);
    strcpy(newnode->type, type);

    newnode->slot = slot;

    newnode->next = head;
    head = newnode;

    totalParked++;

    printf("\nVehicle parked successfully.\n");
    printf("Parking slot: %d\n", slot);
}


/* Remove a vehicle */
void removeVehicle()
{
    char number[20];

    struct vehicle *p;
    struct vehicle *previous;

    int slot;

    printf("\nEnter vehicle number: ");
    scanf("%19s", number);

    p = head;
    previous = NULL;

    while (p != NULL)
    {
        if (strcmp(p->number, number) == 0)
        {
            break;
        }

        previous = p;
        p = p->next;
    }

    if (p == NULL)
    {
        printf("Vehicle not found.\n");
        return;
    }

    slot = p->slot;

    if (previous == NULL)
    {
        head = p->next;
    }
    else
    {
        previous->next = p->next;
    }

    free(p);

    totalParked--;

    printf("\nVehicle removed successfully.\n");
    printf("Slot %d is now available.\n", slot);


    /* Give the free slot to the first waiting vehicle */

    if (front != NULL)
    {
        struct waiting *w;
        struct vehicle *newnode;

        w = removeFromQueue();

        newnode = (struct vehicle*)malloc(sizeof(struct vehicle));

        if (newnode == NULL)
        {
            printf("Memory allocation failed.\n");
            return;
        }

        strcpy(newnode->number, w->number);
        strcpy(newnode->owner, w->owner);
        strcpy(newnode->type, w->type);

        newnode->slot = slot;

        newnode->next = head;
        head = newnode;

        totalParked++;

        printf("\nVehicle from waiting queue got the free slot.\n");
        printf("Vehicle number: %s\n", newnode->number);
        printf("Assigned slot: %d\n", slot);

        free(w);
    }
}


/* Search vehicle */
void search()
{
    char number[20];

    struct vehicle *p;

    printf("\nEnter vehicle number: ");
    scanf("%19s", number);

    p = searchVehicle(number);

    if (p == NULL)
    {
        printf("\nVehicle not found.\n");
    }
    else
    {
        printf("\nVehicle found.\n");
        printf("Vehicle Number : %s\n", p->number);
        printf("Owner Name     : %s\n", p->owner);
        printf("Vehicle Type   : %s\n", p->type);
        printf("Parking Slot   : %d\n", p->slot);
    }
}


/* Display all parked vehicles */
void displayVehicles()
{
    struct vehicle *p = head;

    if (p == NULL)
    {
        printf("\nNo vehicles are parked.\n");
        return;
    }

    printf("\n---------------------------------------------\n");
    printf("Slot\tVehicle Number\tOwner\t\tType\n");
    printf("---------------------------------------------\n");

    while (p != NULL)
    {
        printf("%d\t%s\t\t%s\t\t%s\n",
               p->slot,
               p->number,
               p->owner,
               p->type);

        p = p->next;
    }

    printf("---------------------------------------------\n");
}


/* Display parking slots */
void displaySlots()
{
    int i;
    struct vehicle *p;

    printf("\n----------- Parking Slots -----------\n");

    for (i = 1; i <= TOTAL_SLOTS; i++)
    {
        p = head;

        while (p != NULL)
        {
            if (p->slot == i)
            {
                break;
            }

            p = p->next;
        }

        if (p == NULL)
        {
            printf("Slot %d : Available\n", i);
        }
        else
        {
            printf("Slot %d : Occupied (%s)\n",
                   i, p->number);
        }
    }

    printf("------------------------------------\n");

    printf("Total slots     : %d\n", TOTAL_SLOTS);
    printf("Occupied slots  : %d\n", totalParked);
    printf("Available slots : %d\n",
           TOTAL_SLOTS - totalParked);
}


/* Display waiting queue */
void displayQueue()
{
    struct waiting *p = front;

    if (p == NULL)
    {
        printf("\nWaiting queue is empty.\n");
        return;
    }

    printf("\n--------- Waiting Queue ---------\n");

    while (p != NULL)
    {
        printf("Vehicle: %s\tOwner: %s\tType: %s\n",
               p->number,
               p->owner,
               p->type);

        p = p->next;
    }

    printf("---------------------------------\n");
}


/* Save parked vehicle data */
void saveData()
{
    FILE *file;

    struct vehicle *p = head;

    file = fopen("parking.txt", "w");

    if (file == NULL)
    {
        printf("File could not be opened.\n");
        return;
    }

    while (p != NULL)
    {
        fprintf(file, "%s|%s|%s|%d\n",
                p->number,
                p->owner,
                p->type,
                p->slot);

        p = p->next;
    }

    fclose(file);
}


/* Load previous parking data */
void loadData()
{
    FILE *file;

    char number[20];
    char owner[30];
    char type[15];

    int slot;

    struct vehicle *newnode;

    file = fopen("parking.txt", "r");

    if (file == NULL)
    {
        return;
    }

    while (fscanf(file,
                  " %19[^|]|%29[^|]|%14[^|]|%d",
                  number,
                  owner,
                  type,
                  &slot) == 4)
    {
        newnode = (struct vehicle*)malloc(sizeof(struct vehicle));

        if (newnode == NULL)
        {
            break;
        }

        strcpy(newnode->number, number);
        strcpy(newnode->owner, owner);
        strcpy(newnode->type, type);

        newnode->slot = slot;

        newnode->next = head;
        head = newnode;

        totalParked++;
    }

    fclose(file);
}


/* Free memory before program ends */
void freeMemory()
{
    struct vehicle *p;

    while (head != NULL)
    {
        p = head;
        head = head->next;
        free(p);
    }

    struct waiting *q;

    while (front != NULL)
    {
        q = front;
        front = front->next;
        free(q);
    }

    rear = NULL;
}


/* Main function */
int main()
{
    int choice;

    loadData();

    while (1)
    {
        printf("\n\n====================================\n");
        printf("       SMART PARKING SYSTEM\n");
        printf("====================================\n");

        printf("1. Park Vehicle\n");
        printf("2. Remove Vehicle\n");
        printf("3. Search Vehicle\n");
        printf("4. Display Parked Vehicles\n");
        printf("5. Display Parking Slots\n");
        printf("6. Display Waiting Queue\n");
        printf("7. Exit\n");

        printf("====================================\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                parkVehicle();
                saveData();
                break;

            case 2:
                removeVehicle();
                saveData();
                break;

            case 3:
                search();
                break;

            case 4:
                displayVehicles();
                break;

            case 5:
                displaySlots();
                break;

            case 6:
                displayQueue();
                break;

            case 7:
                saveData();
                freeMemory();

                printf("\nThank you for using the system.\n");

                return 0;

            default:
                printf("\nInvalid choice.\n");
        }
    }

    return 0;
}