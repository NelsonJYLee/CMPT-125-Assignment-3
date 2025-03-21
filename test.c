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
};

int countContacts(Contact **contacts)
{
    int count = 0;
    while (contacts[count] != NULL)
    {
        count += 1;
    }
    return count;
}

Contact *readNewContact()
{
    char buffer[100] = {'\0'};
    Contact* newContact = NULL;
    char* myFirstName = NULL;
    char* myFamilyName = NULL;
    char* myAddress = NULL;
    long long myPhoneNumber = 0;
    int myAge = 0;
    int attempts = 1;
    const int MAX_ATTEMPTS = 5;

    printf("Enter the first name: ");
    fscanf(stdin, " %99[^\n]", buffer);
    myFirstName = (char*)calloc(strlen(buffer) + 1, sizeof(char));
    if (myFirstName == NULL)
    {
        fprintf(stderr, "Error: unable to allocate memory for the first name string");
        return NULL;
    }
    strcpy(myFirstName, buffer);

    printf("Enter the family name: ");
    fscanf(stdin, " %99[^\n]", buffer);
    myFamilyName = (char*)calloc(strlen(buffer) + 1, sizeof(char));
    if (myFamilyName == NULL)
    {
        fprintf(stderr, "Error: unable to allocate memory for the family name string");
        free(myFirstName);
        return NULL;
    }
    strcpy(myFamilyName, buffer);

    printf("Enter the address: ");
    fscanf(stdin, " %99[^\n]", buffer);
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
    fscanf(stdin, " %99[^\n]", buffer);
    while (!validPhoneNumber(buffer) && attempts < MAX_ATTEMPTS)
    {
        printf("Error: Invalid phone number. Try again: ");
        fscanf(stdin, " %99[^\n]", buffer);
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

    myPhoneNumber = atoll(buffer);

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

    newContact = (Contact*)malloc(sizeof(Contact));
    if (newContact == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for Contact in readNewContact");
        free(myFirstName);
        free(myFamilyName);
        free(myAddress);
        return NULL;
    }

    newContact->firstName = myFirstName;
    newContact->familyName = myFamilyName;
    newContact->address = myAddress;
    newContact->phonNum = myPhoneNumber;
    newContact->age = myAge;

    return newContact;
}

// Function to print a Contact
void printContact(const Contact *c) {
    if (!c) {
        printf("Invalid contact.\n");
        return;
    }
    printf("\n--- Contact Information ---\n");
    printf("First Name: %s\n", c->firstName);
    printf("Family Name: %s\n", c->familyName);
    printf("Address: %s\n", c->address);
    printf("Phone Number: %lld\n", c->phonNum);
    printf("Age: %d\n", c->age);
}

// Function to free memory allocated for a Contact
void freeContact(Contact *c) {
    if (!c) return;
    free(c->firstName);
    free(c->familyName);
    free(c->address);
    free(c);
};

Contact** appendContact(Contact **contacts, Contact *newContact)
{
    Contact** newContacts = NULL;
    int numContacts = 0;

    /*
    creates a new contacts if it doesn't already exist
    */
    if (contacts == NULL || newContact == NULL)
    {
        newContacts = (Contact**)malloc(sizeof(Contact*)*2);
        if (newContacts == NULL)
        {
        fprintf(stderr, "Memory reallocation error in appendContact");
        return contacts;
        }
    }
    /*
    adds an extra space to contacts
    */
    else
    {    
        numContacts = countContacts(contacts);

        newContacts = (Contact**)realloc(contacts, (numContacts + 2) * sizeof(Contact*));
        if (newContacts == NULL)
        {
            fprintf(stderr, "Memory reallocation error in appendContact");
            return contacts;
        }
    }
    newContacts[numContacts] = newContact;
    newContacts[numContacts + 1] = NULL;
    contacts = newContacts;
    printf("Contact appended successfully by appendContact\n");
    return contacts;
}

Contact** removeContactByIndex(Contact** contacts)
{
    int index = 0;
    int originalSizeContacts = countContacts(contacts);
    Contact** newContacts = NULL;
    if (contacts == NULL)
    {
        fprintf(stderr, "Error: value of addressBook received in removeContactByIndex was NULL");
        return NULL;
    }

    printf("Removing a Contact by index\n");
    printf("Enter index to remove (0 based): ");
    if (scanf("%d", &index) != 1)
    {
        fprintf(stderr, "Error: Value of index supplied could not be read.");
        return contacts;
    }

    if (!(0 <= index && index < originalSizeContacts))
    {
        fprintf(stderr, "Error: Index out of range in removeContactByIndex");
        return contacts;
    }

    freeContact(contacts[index]);

    for (int i = index; i < originalSizeContacts - 1; i ++)
    {
        contacts[i] = contacts[i+1];
    }

    contacts[originalSizeContacts - 1] = NULL;

    newContacts = (Contact**)realloc(contacts, originalSizeContacts * sizeof(Contact*));

    if (newContacts == NULL)
    {
        fprintf(stderr, "Error: Memory reallocation failed in removeContactByIndex");
        return contacts;
    }

    printf("Contact removed successfully by removeContactByIndex");

    contacts = newContacts;

    return contacts;
}

int removeContactByFullName(Contact** contacts)
{
    char firstName[100] = {"\0"};
    char familyName[100] = {"\0"};
    int index = 0;
    int contactsSize = countContacts(contacts);
    Contact** newContacts = NULL;
    
    if (contacts == NULL)
    {
        fprintf(stderr, "Error: value of contacts received in removeContactByFullName was NULL");
        return 0;
    }

    printf("Enter first name: ");
    scanf("%s", firstName);
    printf("Enter family name: ");
    scanf("%s", familyName);

    /*
    find matching first and family names
    */
    while (index < contactsSize)
    {
        if (strcmp(contacts[index]->firstName, firstName) == 0 && strcmp(contacts[index]->familyName, familyName) == 0)
        {
            break;
        }
        index += 1;
    }
    
    if  (index == contactsSize)
    {
        printf("Contact '%s %s' not found", firstName, familyName);
        return 2;
    }
    
    freeContact(contacts[index]);
    for (int i = index; i < contactsSize - 1; i++)
    {
        contacts[i] = contacts[i + 1];
    }

    contacts[contactsSize - 1] = NULL;

    newContacts = (Contact**)realloc(contacts, contactsSize * sizeof(Contact*));

    if (newContacts == NULL)
    {
        fprintf(stderr, "Error: Memory reallocation failed in removeContactByFullName");
        return 2;
    }

    contacts = newContacts;

    printf("Contact '%s %s' removed successfully\n", firstName, familyName);
    return 1;
}

void listContacts(Contact** contacts)
{
    int numContacts = countContacts(contacts);

    if (numContacts == 0)
    {
        printf("No contacts available.\n");
    }
    else
    {
        for (int i = 0; i < numContacts; i++)
        {
            printf("%d. %s %s\n", i + 1, contacts[i]->firstName, contacts[i]->familyName);
            printf("   Phone: %lld\n", contacts[i]->phonNum);
            printf("   Address: %s\n", contacts[i]->address);
            printf("   Age: %d\n", contacts[i]->age);
        }
    }
}

void saveContactsToFile(Contact** contacts, char* filename)
{
    FILE* outputStream = NULL;
    int numContacts = countContacts(contacts);

    if (filename == NULL)
    {
        fprintf(stderr, "Error: filename formal parameter passed value NULL in saveContactsToFile");
        return;
    }

    if (contacts == NULL)
    {
        fprintf(stderr, "Error: addressBook formal parameter passed value NULL in saveContactsToFile");
        return;
    }

    outputStream = fopen(filename, "w");
    if (outputStream == NULL)
    {
        fprintf(stderr, "Error file not opended in saveContactsTofile");
        return;
    }
    fprintf(outputStream, "%d\n", numContacts);
    for (int i = 0; i < numContacts; i++)
    {
        fprintf(outputStream, "%s\n%s\n%s\n%lld\n%d\n", contacts[i]->firstName, contacts[i]->familyName, contacts[i]->address, contacts[i]->phonNum, contacts[i]->age);
    }

    fclose(outputStream);
    return;
}

void printContactsToFile(Contact** contacts, char* filename)
{
    FILE* outputStream = NULL;
    int numContacts = countContacts(contacts);

    if (filename == NULL)
    {
        fprintf(stderr, "Error: filename formal parameter passed value NULL in printContactsToFile");
        return;
    }

    if (contacts == NULL)
    {
        fprintf(stderr, "Error: addressBook formal parameter passed value NULL in printContactsToFile");
        return;
    }

    outputStream = fopen(filename, "w");
    if (outputStream == NULL)
    {
        fprintf(stderr, "Error: file not opened in printContactsToFile");
        return;
    }

    fprintf(outputStream, "Address Book Report\n-------------------\n");
    for (int i = 0; i < numContacts; i++)
    {
        fprintf(outputStream, "%d. %s %s\n", i + 1, contacts[i]->firstName, contacts[i]->familyName);
        fprintf(outputStream, "   Phone: %lld\n", contacts[i]->phonNum);
        fprintf(outputStream, "   Address: %s\n", contacts[i]->address);
        fprintf(outputStream, "   Age: %d\n", contacts[i]->age);
    }
    fprintf(outputStream, "-------------------\n");
    fprintf(outputStream, "Total Contacts: %d", numContacts);

    return;
}


// Main function to test the program
int main() {
    char outputName[100] = "output.txt";
    Contact** addressBook = NULL;
    addressBook = appendContact(addressBook, readNewContact());
    addressBook = appendContact(addressBook, readNewContact());
    listContacts(addressBook);

    saveContactsToFile(addressBook, outputName);

    for (int i = 0; i < countContacts(addressBook); i++)
    {
        freeContact(addressBook[i]);
    }

    free(addressBook);

    return 0;
}