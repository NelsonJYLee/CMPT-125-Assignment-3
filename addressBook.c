/*
Author: Nelson Lee
Date: March 22nd, 2025
Description: This C program implements an address book application in C that uses dynamic memory allocation 
to manage a list of contacts.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

enum MenuOption 
{
    APPEND_CONTACT_OPTION = 1,
    INSERT_ALPHA_OPTION,
    REMOVE_CONTACT_OPTION,
    REMOVE_CONTACT_NAME,
    FIND_EDIT_CONTACT_OPTION,
    LIST_CONTACT_OPTION,
    PRNT_FILE_OPTION,
    PRNT_FILE_HR_OPTION,
    LOAD_CONTACTS_OPTION,
    APPEND_FILE_OPTION,
    MERGE_FILE_OPTION,
    EXIT_OPTION
};

enum EditOption 
{
    EDIT_FIRST = 1,
    EDIT_LAST,
    EDIT_ADDR,
    EDIT_PHN,
    EDIT_AGE,
    CANCEL
};

typedef struct Contact {
    char* firstName;
    char* familyName;
    long long phonNum; /* 10-digit phone number stored as a 64-bit integer */
    char* address;
    int age;
} Contact;

void printMenuOptions();

int countContacts(Contact **contacts);

bool validPhoneNumber(char buffer[]);

bool validAge(char buffer[]);

Contact *readNewContact();

Contact **appendContact(Contact **contacts, Contact *newContact);

Contact** insertContactAlphabetical(Contact** contacts, Contact* newContact);

void freeContact(Contact *c);

void freeAddressBook(Contact** addressBook);

Contact** removeContactByIndex(Contact** contacts);

int removeContactByFullName(Contact** contacts);

void listContacts(Contact** contacts);

void saveContactsToFile(Contact** contacts, char* filename);

void printContactsToFile(Contact** contacts, char* filename);

Contact** loadContactsFromFile(Contact** addressBook, char* filename);

Contact** appendContactsFromFile(Contact** contacts, char* filename);

void InsertionSort(Contact** contacts);

Contact **mergeContactsFromFile(Contact** contacts, char* filename);

Contact** editContact(Contact** contacts);

int main()
{

    int option = 0;
    Contact** addressBook = NULL;
    Contact** newAddressBook = NULL;
    char filename[100] = {"\0"};

    /*
    start by adding the NULL ending to the array
    */

    addressBook = (Contact**)malloc(sizeof(Contact*));
    if (addressBook == NULL)
    {
        fprintf(stderr, "Could not allocate address book");
        return 1;
    }
    addressBook[0] = NULL;
    
    while (option != EXIT_OPTION)
    {
        printMenuOptions();
        scanf("%d", &option);

        /* change to use case statements*/
        switch(option)
        {
            case APPEND_CONTACT_OPTION:
                printf("Adding a constact interactively: \n");
                addressBook = appendContact(addressBook, readNewContact());
                break;
            case INSERT_ALPHA_OPTION:
                printf("Inserting a contact in alphabetical order interactively: \n");
                addressBook = insertContactAlphabetical(addressBook, readNewContact());
                break;
            case REMOVE_CONTACT_OPTION:
                printf("Removing a Contact by index: \n");
                addressBook = removeContactByIndex(addressBook);
                break;
            case REMOVE_CONTACT_NAME:
                printf("Removing a Contact with a particular Name: \n");
                removeContactByFullName(addressBook);
                break;
            case FIND_EDIT_CONTACT_OPTION:
                addressBook = editContact(addressBook);
                break;
            case LIST_CONTACT_OPTION:
                printf("\n");
                listContacts(addressBook);
                break;
            case PRNT_FILE_OPTION:
                printf("Enter filename to save: ");
                scanf("%s", filename);
                saveContactsToFile(addressBook, filename);
                break;
            case PRNT_FILE_HR_OPTION:
                printf("Enter filename to print: ");
                scanf("%s", filename);
                printContactsToFile(addressBook, filename);
                break;
            case LOAD_CONTACTS_OPTION:
                printf("Enter filename to load (replaces current contacts): ");
                scanf("%s", filename);
                addressBook = loadContactsFromFile(addressBook, filename);
                break;
            case APPEND_FILE_OPTION:
                printf("Enter filename to append: ");
                scanf("%s", filename);
                addressBook = appendContactsFromFile(addressBook, filename);
                break;
            case MERGE_FILE_OPTION:
                printf("Enter filename to merge: ");
                scanf("%s", filename);
                addressBook = mergeContactsFromFile(addressBook, filename);
                break;
            case EXIT_OPTION:
                printf("Exiting program. Goodbye!");
                freeAddressBook(addressBook);
                break;
        }
        printf("\n");
    }
    return 0;
}

void printMenuOptions()
{
    printf("Address Book Menu:\n1.  Append Contact\n2.  Insert Contact in Alphabetical Order\n3.  Remove Contact by Index\n");
    printf("4.  Remove Contact by Full Name\n5.  Find and Edit Contact\n6.  List Contacts\n7.  Print Contacts to File with the format of an input file\n");
    printf("8.  Print Contacts to File (Human Readable)\n9.  Load Contacts from File Replacing Existing Contacts\n10. Append Contacts from File\n");
    printf("11. Merge Contacts from File\n12. Exit\n");
    printf("Choose an option: ");
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

bool validAge(char buffer[])
{
    int myAge = 0;
    const int MAX_AGE = 150;
    const int MIN_AGE = 1;
    for (int i = 0; i < strlen(buffer); i++)
    {
        if (!isdigit(buffer[i]))
        {
            return false;
        }
    }
    myAge = atoi(buffer);
    if (!(myAge >= MIN_AGE && myAge <= MAX_AGE))
    {
        return false;
    }
    return true;
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
        fprintf(stderr, "Error: Could not read a valid phone number\n");
        myPhoneNumber = 0;
    }
    else
    {
        myPhoneNumber = atoll(buffer);
    }

    attempts = 1;

    printf("Enter the age: ");
    fscanf(stdin, " %99[^\n]", buffer);
    while (!(validAge(buffer)) && attempts < MAX_ATTEMPTS)
    {
        printf("Error: Invalid age. Try again: ");
        fscanf(stdin, " %99[^\n]", buffer);
        attempts += 1;
    }
    if (!(validAge(buffer)))
    {
        myAge = 0;
        fprintf(stderr, "Error: Could not read a valid age\n");
    }
    else
    {
        myAge = atoi(buffer);
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

Contact** insertContactAlphabetical(Contact** contacts, Contact* newContact)
{
    /*requires an non-NULL contacts*/
	int numContacts = countContacts(contacts);
	Contact** newContacts = NULL;
	int index = 0;
	if (newContact == NULL)
	{
		return contacts;
	}
	
	newContacts = (Contact**)realloc(contacts, (numContacts + 2) * sizeof(Contact*));
	if (newContacts == NULL)
	{
		fprintf(stderr, "Error: Memory reallocation error in insertContactAlphabetical");
		return contacts;
	}
	newContacts[numContacts + 1] = NULL;


	/*find the correct index to place newContact*/
    if (numContacts != 0)
    {
        while (index < numContacts && (strcmp(newContact->familyName, newContacts[index]->familyName) > 0 || (strcmp(newContact->familyName, newContacts[index]->familyName) == 0 && strcmp(newContact->firstName, newContacts[index]->firstName) > 0)))
        {
            index += 1;
        }
        
        /*move all contacts down by one index*/
        for (int i = numContacts; i > index; i--)
        {
            newContacts[i] = newContacts[i-1];
        }
    }


	/*place contact at the index*/
	newContacts[index] = newContact;

	contacts = newContacts;

	printf("Contact added in alphabetical order successfully.\n");

	return contacts;
}

void freeContact(Contact *c)
{
    free(c->firstName);
    free(c->familyName);
    free(c->address);
    free(c);
};

void freeAddressBook(Contact** addressBook)
{
    int numContacts = countContacts(addressBook);

    for (int i = 0; i < numContacts; i++)
    {
        freeContact(addressBook[i]);
    }
    free(addressBook);
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
    printf("Enter index to remove: ");
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

    printf("Contact removed successfully.\n");

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
    while (getchar() != '\n'); 
    fscanf(stdin, "%99[^\n]", firstName);
    printf("Enter family name: ");
    while (getchar() != '\n'); 
    fscanf(stdin, "%99[^\n]", familyName);

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
        printf("Contact '%s %s' not found.\n", firstName, familyName);
        printf("No Contact with name %s %s found\n", firstName, familyName);
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

    printf("Contact removed successfully.\n");
    printf("Contact '%s %s' removed successfully.\n", firstName, familyName);
    printf("removed a Contact with name %s  %s\n", firstName, familyName);
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
        printf("Contacts List:\n");
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

    printf("Contacts saved to %s\n", filename);

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
        fprintf(outputStream, "   Age: %d\n\n", contacts[i]->age);
    }
    fprintf(outputStream, "-------------------\n");
    fprintf(outputStream, "Total Contacts: %d\n", numContacts);

    printf("Contacts printed to %s (human-readable format).\n", filename);

    fclose(outputStream);

    return;
}

Contact** loadContactsFromFile(Contact** addressBook, char* filename)
{
    FILE* inputStream = NULL;
    int numContacts = 0;
    Contact* newContact = NULL;
    char getBuffer[100] = {"\0"};
    char scanBuffer[100] = {"\0"};
    char* myFirstName = NULL;
    char* myFamilyName = NULL;
    char* myAddress = NULL;
    long long myPhoneNumber = 0;
    int myAge = 0;
    const int MAX_AGE = 150;
    const int MIN_AGE = 1;


    inputStream = fopen(filename, "r");
    if (inputStream == NULL)
    {
        fprintf(stderr, "Error: File to load not found");
        return NULL;
    }

    fgets(getBuffer, sizeof(getBuffer), inputStream);
    if (sscanf(getBuffer, "%d", &numContacts) != 1)
    {
        fprintf(stderr, "Error: Memory allocation error, addressBook in loadContactsFromFile");
        fclose(inputStream);
        return NULL;
    }

    if (addressBook != NULL)
    {
        freeAddressBook(addressBook);
    }

    addressBook = (Contact**)calloc(numContacts + 1, sizeof(Contact*));
    if (addressBook == NULL)
    {
        fprintf(stderr, "Error: Memory allocation error, addressBook in loadContactsFromFile");
        fclose(inputStream);
        return NULL;
    }
    addressBook[numContacts] = NULL;

    for (int i = 0; i < numContacts; i ++)
    {
        newContact = (Contact*)malloc(sizeof(Contact));
        if (newContact == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, Contact %d in loadContactsFromFile", i);
            freeAddressBook(addressBook);
            fclose(inputStream);
            return NULL;
        }

        /*firstName*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        myFirstName = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myFirstName == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
            free(newContact);
            freeAddressBook(addressBook);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myFirstName, scanBuffer);
        newContact->firstName = myFirstName;

        /*familyName*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        myFamilyName = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myFamilyName == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
            free(myFirstName);
            free(newContact);
            freeAddressBook(addressBook);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myFamilyName, scanBuffer);
        newContact->familyName = myFamilyName;

        /*address*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        myAddress = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myAddress == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
            free(myFirstName);
            free(myFamilyName);
            free(newContact);
            freeAddressBook(addressBook);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myAddress, scanBuffer);
        newContact->address = myAddress;

        /*phoneNumber*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        if (!(validPhoneNumber(scanBuffer)))
        {
            fprintf(stderr, "Error: Invalid phone number.");
            myPhoneNumber = 0;
        }
        else
        {
            myPhoneNumber = atoll(scanBuffer);
        }
        newContact->phonNum = myPhoneNumber;

        /*age*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%d", &myAge);
        if (!(myAge >= MIN_AGE && myAge <= MAX_AGE))
        {
            fprintf(stderr, "Error: Invalid age.");
            myAge = 0;
        }
        newContact->age = myAge;
        
        addressBook[i] = newContact;
    }
    printf("Contacts loaded from file: %s\n", filename);
    fclose(inputStream);
    return addressBook;
}

bool nameInBook(char* firstName, char* familyName, Contact** addressBook)
{
    int numContacts = countContacts(addressBook);

    for (int i = 0; i < numContacts; i++)
    {
        if (strcmp(addressBook[i]->firstName, firstName) == 0 && strcmp(addressBook[i]->familyName, familyName) == 0)
        {
            return true;
        }
    }
    return false;
}

Contact** appendContactsFromFile(Contact** contacts, char* filename)
{
    FILE* inputStream = NULL;
    Contact* newContact = NULL;
    Contact** newAddressBook = NULL;
    char getBuffer[100] = {"\0"};
    char scanBuffer[100] = {"\0"};
    int numContacts = 0;
    char* myFirstName = NULL;
    char* myFamilyName = NULL;
    char* myAddress = NULL;
    long long myPhoneNumber = 0;
    int myAge = 0;
    const int MAX_AGE = 150;
    const int MIN_AGE = 1;
    
    inputStream = fopen(filename, "r");
    if (inputStream == NULL)
    {
        fprintf(stderr, "Error: File to load not found");
        return NULL;
    }

    fgets(getBuffer, sizeof(getBuffer), inputStream);
    if (sscanf(getBuffer, "%d", &numContacts) != 1)
    {
        fprintf(stderr, "Error: failed to get number of contacts in file");
        fclose(inputStream);
        return NULL;
    }

    printf("Contacts loaded from file: %s\n", filename);

    for (int i = 0; i < numContacts; i++)
    {
        newContact = (Contact*)malloc(sizeof(Contact));
        if (newContact == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, Contact %d in loadContactsFromFile", i);
            fclose(inputStream);
            return NULL;
        }

        /*firstName*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        myFirstName = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myFirstName == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
            free(newContact);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myFirstName, scanBuffer);
        newContact->firstName = myFirstName;

        /*familyName*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        myFamilyName = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myFamilyName == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
            free(myFirstName);
            free(newContact);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myFamilyName, scanBuffer);
        newContact->familyName = myFamilyName;

        /*address*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        myAddress = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myAddress == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
            free(myFirstName);
            free(myFamilyName);
            free(newContact);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myAddress, scanBuffer);
        newContact->address = myAddress;

        /*phoneNumber*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        if (!(validPhoneNumber(scanBuffer)))
        {
            fprintf(stderr, "Error: Invalid phone number.");
            myPhoneNumber = 0;
        }
        else
        {
            myPhoneNumber = atoll(scanBuffer);
        }
        newContact->phonNum = myPhoneNumber;

        /*age*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%d", &myAge);
        if (!(myAge >= MIN_AGE && myAge <= MAX_AGE))
        {
            fprintf(stderr, "Error: Invalid age.");
            myAge = 0;
        }
        newContact->age = myAge;

        /*check to see if this name is already in the book*/
        if (nameInBook(myFirstName, myFamilyName, contacts))
        {
            printf("Duplicate Contact detected\n");
            free(myFirstName);
            free(myFamilyName);
            free(myAddress);
            free(newContact);
            continue;
        }
        
        newAddressBook = appendContact(contacts, newContact);
        contacts = newAddressBook;
    }

    printf("Appended contacts from %s\n", filename);
    return contacts;
}

Contact **mergeContactsFromFile(Contact** contacts, char* filename)
{
    FILE* inputStream = NULL;
    Contact* newContact = NULL;
    Contact** newAddressBook = NULL;
    char getBuffer[100] = {"\0"};
    char scanBuffer[100] = {"\0"};
    int numContacts = 0;
    char* myFirstName = NULL;
    char* myFamilyName = NULL;
    char* myAddress = NULL;
    long long myPhoneNumber = 0;
    int myAge = 0;
    const int MAX_AGE = 150;
    const int MIN_AGE = 1;

    inputStream = fopen(filename, "r");
    if (inputStream == NULL)
    {
        fprintf(stderr, "Error: File to load not found");
        return NULL;
    }

    fgets(getBuffer, sizeof(getBuffer), inputStream);
    if (sscanf(getBuffer, "%d", &numContacts) != 1)
    {
        fprintf(stderr, "Error: failed to get number of contacts in file");
        fclose(inputStream);
        return NULL;
    }

    for (int i = 0; i < numContacts; i++)
    {
        newContact = (Contact*)malloc(sizeof(Contact));
        if (newContact == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, Contact %d in loadContactsFromFile", i);
            fclose(inputStream);
            return NULL;
        }

        /*firstName*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        myFirstName = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myFirstName == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
            free(newContact);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myFirstName, scanBuffer);
        newContact->firstName = myFirstName;

        /*familyName*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        myFamilyName = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myFamilyName == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
            free(myFirstName);
            free(newContact);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myFamilyName, scanBuffer);
        newContact->familyName = myFamilyName;

        /*address*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        myAddress = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myAddress == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
            free(myFirstName);
            free(myFamilyName);
            free(newContact);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myAddress, scanBuffer);
        newContact->address = myAddress;

        /*phoneNumber*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%99[^\n]", scanBuffer);
        if (!(validPhoneNumber(scanBuffer)))
        {
            fprintf(stderr, "Error: Invalid phone number.");
            myPhoneNumber = 0;
        }
        else
        {
            myPhoneNumber = atoll(scanBuffer);
        }
        newContact->phonNum = myPhoneNumber;

        /*age*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%d", &myAge);
        if (!(myAge >= MIN_AGE && myAge <= MAX_AGE))
        {
            fprintf(stderr, "Error: Invalid age.");
            myAge = 0;
        }
        newContact->age = myAge;

        /*check to see if this name is already in the book*/
        if (nameInBook(myFirstName, myFamilyName, contacts))
        {
            printf("Duplicate Contact detected\n");
            free(myFirstName);
            free(myFamilyName);
            free(myAddress);
            free(newContact);
            continue;
        }

        newAddressBook = insertContactAlphabetical(contacts, newContact);
        contacts = newAddressBook;
    }
    printf("Appended contacts from %s\n", filename);
    return contacts;
}

void printEditMenu()
{
    printf("1. Edit First Name\n");
    printf("2. Edit Last Name\n");
    printf("3. Edit Address\n");
    printf("4. Edit Phone Number\n");
    printf("5. Edit Age\n");
    printf("6. Cancel\n");
    printf("Choose an option: ");
}

Contact** editContact(Contact** contacts)
{
    int numContacts = countContacts(contacts);
    int index = 0;
    Contact* selectedContact = NULL;
    int option = 0;
    char scanBuffer[100] = {"\0"};
    char* myFirstName = NULL;
    char* myFamilyName = NULL;
    char* myAddress = NULL;
    long long myPhoneNumber = 0;
    int myAge = 0;
    const int MAX_AGE = 150;
    const int MIN_AGE = 1;

    if (numContacts == 0)
    {
        printf("No contacts available to edit.\n");
        return contacts;
    }

    printf("Enter index of contact to edit (0-%d): ", numContacts-1);
    if (scanf("%d", &index) != 1 || !(0 <= index && index <= numContacts-1))
    {
        fprintf(stderr, "Error: Invalid Index");
        return contacts;
    }

    selectedContact = contacts[index];
    
    printf("Editing contact: %s %s\n", selectedContact->firstName, selectedContact->familyName);

    printEditMenu();
    scanf("%d", &option);

    switch(option)
    {
        case EDIT_FIRST:
            printf("Enter new first name: ");
            scanf("%s", scanBuffer);
            selectedContact->firstName = (char*)realloc(selectedContact->firstName, (strlen(scanBuffer) + 1) * sizeof(char));
            if (selectedContact == NULL)
            {
                fprintf(stderr, "Error: Memory allocation error for string in editContact");
                return NULL;
            }
            strcpy(selectedContact->firstName, scanBuffer);
            break;
        case EDIT_LAST:
            printf("Enter new family name: ");
            scanf("%s", scanBuffer);
            selectedContact->familyName = (char*)realloc(selectedContact->familyName, (strlen(scanBuffer) + 1) * sizeof(char));
            if (selectedContact == NULL)
            {
                fprintf(stderr, "Error: Memory allocation error for string in editContact");
                return NULL;
            }
            strcpy(selectedContact->familyName, scanBuffer);
            break;
        case EDIT_ADDR:
            printf("Enter new address: ");
            fscanf(stdin, " %99[^\n]", scanBuffer);
            selectedContact->address = (char*)realloc(selectedContact->address, (strlen(scanBuffer) + 1) * sizeof(char));
            if (selectedContact == NULL)
            {
                fprintf(stderr, "Error: Memory allocation error for string in editContact");
                return NULL;
            }
            strcpy(selectedContact->address, scanBuffer);
            break;
        case EDIT_PHN:
            printf("Enter new phone number: Enter 10-digit phone number that must not start with 0: ");
            fscanf(stdin, " %99[^\n]", scanBuffer);
            if (!(validPhoneNumber(scanBuffer)))
            {
                fprintf(stderr, "Error: Invalid phone number.\n");
                return contacts;
            }
            else
            {
                myPhoneNumber = atoll(scanBuffer);
            }
            selectedContact->phonNum = myPhoneNumber;
            break;
        case EDIT_AGE:
            printf("Enter new age: ");
            fscanf(stdin, "%d", &myAge);
            if (!(myAge >= MIN_AGE && myAge <= MAX_AGE))
            {
                fprintf(stderr, "Error: Invalid age.");
                return contacts;
            }
            selectedContact->age = myAge;
            break;
        case CANCEL:
            printf("Edit cancelled.\n");
            return contacts;
    }
    printf("Contact updated successfully.\n");
    return contacts;
}









