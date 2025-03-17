#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Contact {
    char* firstName;
    char* familyName;
    long long phonNum; /* 10-digit phone number stored as a 64-bit integer */
    char* address;
    int age;
} Contact;

int countContacts(Contact **contacts);

bool validPhoneNumber(char buffer[]);

Contact *readNewContact();


int main()
{

    int option = 0;
    Contact** addressBook = NULL;
    const int EXIT_OPTION = 12;
    const int APPEND_CONTACT_OPTION = 1;
    
    while (option != EXIT_OPTION)
    {
        printf("Address Book Menu\n1. Append Contact\n2. Insert Contact in Alphabetical Order\n3. Remove Contact by Index\n");
        printf("4. Remove Contact by Full Name\n5. Find and Edit Contact\n6. List Contacts\n7. Print Contacts to File with the format of an input file\n");
        printf("8. Print Contacts to File (Human Readable)\n9. Load Contacts from File Replacing Existing Contacts\n10. Append Contacts from File\n");
        printf("11. Merge Contacts from File\n12. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        /* change to use case statements*/
        if (option == APPEND_CONTACT_OPTION)
        {
            if (addressBook == NULL)
            {
                addressBook = (Contact**)malloc(sizeof(Contact*));
                if (addressBook == NULL)
                {
                    fprintf(stderr, "ERROR allocating memory");
                    return 1;
                }
            }

        }
        return 0;
    }


}

int countContacts(Contact **contacts)
{
    int count = 0;
    while (contacts[count] != NULL)
    {
        count += 1;
    }
    return count;
}

bool validPhoneNumber(char buffer[])
{
    if (strlen(buffer) != 10 || buffer[0] == '0')
    {
        return false;
    }
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (!isdigit(buffer[i]))
        {
            return false;
        }
    }
    return true;
}

Contact *readNewContact()
{
    char buffer[100] = {'\0'};
    char* myFirstName = NULL;
    char* myFamilyName = NULL;
    char* myAddress = NULL;
    long long myPhoneNumber = 0;
    int myAge = 0;
    int attempts = 1;
    const int MAX_ATTEMPTS = 5;

    printf("Enter the first name: ");
    fscanf(stdin, " 99[^\n]", buffer);
    myFirstName = (char*)calloc(strlen(buffer) + 1, sizeof(char));
    if (myFirstName == NULL)
    {
        fprintf(stderr, "Error: unable to allocate memory for the first name string");
        return NULL;
    }
    strcpy(myFirstName, buffer);

    printf("Enter the family name: ");
    fscanf(stdin, " 99[^\n]", buffer);
    myFamilyName = (char*)calloc(strlen(buffer) + 1, sizeof(char));
    if (myFamilyName == NULL)
    {
        fprintf(stderr, "Error: unable to allocate memory for the family name string");
        free(myFirstName);
        return NULL;
    }
    strcpy(myFamilyName, buffer);

    printf("Enter the address: ");
    fscanf(stdin, " 99[^\n]", buffer);
    myAddress = (char*)calloc(strlen(buffer) + 1, sizeof(char));
    if (myAddress == NULL)
    {
        fprintf(stderr, "Error: unable to allocate memory for the address string");
        free(myFirstName);
        free(myFamilyName);
        return NULL;
    }
    strcpy(myAddress, buffer);

    printf("Enter 10-digit phone number that must not start with 0: ");
    fscanf(stdin, " 99[^\n]", buffer);
    while (!validPhoneNumber(buffer) && attempts < MAX_ATTEMPTS)
    {
        printf("Error: Invalid phone number. Try again: ");
        fscanf(stdin, " 99[^\n]", buffer);
        attempts += 1;
    }
    if (!validPhoneNumber(buffer))
    {
        fprintf(stderr, "Error: Could not read a valid phone number");
        free(myFirstName);
        free(myFamilyName);
        free(myAddress);
        return NULL;
    }

    myPhoneNumber = atoi(buffer);

    attempts = 1;

    printf("Enter the age of the contact: ");
    fscanf(stdin, "%d", &myAge);
    while (!(myAge >= 1 && myAge <= 150) && attempts < MAX_ATTEMPTS)
    {
        printf("Error: Invalid age. Try again: ");
        fscanf(stdin, "%d", &myAge);
        attempts += 1;
    }
    if (!(myAge >= 1 && myAge <= 150))
    {
        myAge = 0;
        fprintf(stderr, "Error: Could not read a valid age");
        free(myFirstName);
        free(myFamilyName);
        free(myAddress);
        return NULL;
    }









}