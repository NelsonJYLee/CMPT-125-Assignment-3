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

	printf("Contact was successfully added in alphabetical order\n");

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

void freeAddressBook(Contact** addressBook)
{
    int numContacts = countContacts(addressBook);

    for (int i = 0; i < numContacts; i++)
    {
        freeContact(addressBook[i]);
    }
    free(addressBook);
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


    inputStream = fopen(filename, "r");
    if (inputStream == NULL)
    {
        fprintf(stderr, "Error: File to load not found");
        return NULL;
    }
    /*make sure this WORKS!
    then scan every attribute for every contact input, dynamically allocate memory for the attributers, and append it to the contract list*/
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
        if (!(myAge >= 1 && myAge <= 150))
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
        if (!(myAge >= 1 && myAge <= 150))
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

    printf("Appeneded contacts from %s\n", filename);
    return contacts;
}

void InsertionSort(Contact** contacts)
{
    int n  = countContacts(contacts);
    for (int i = 1; i < n; i++)
    {
        char* key1 = contacts[i]->familyName;
        char* key2 = contacts[i]->firstName;
        Contact* key = contacts[i];

        int j = i - 1;

        while (j>= 0 && (strcmp(contacts[j]->familyName, key1) > 0 || (strcmp(contacts[j]->familyName, key1) == 0 && strcmp(contacts[j]->firstName, key2) > 0)))
        {
            contacts[j + 1] = contacts[j];
            j--;
        }
        contacts[j + 1] = key;
    }
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

    /*sort the current contacts*/
    InsertionSort(contacts);

    /*read and place contacts from file in a sorted manner*/
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
        if (!(myAge >= 1 && myAge <= 150))
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

    if (numContacts == 0)
    {
        printf("No contacts available to edit\n");
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
        case 1:
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
        case 2:
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
        case 3:
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
        case 4:
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
        case 5:
            printf("Enter new age: ");
            fscanf(stdin, "%d", &myAge);
            if (!(myAge >= 1 && myAge <= 150))
            {
                fprintf(stderr, "Error: Invalid age.");
                return contacts;
            }
            selectedContact->age = myAge;
            break;
        case 6:
            printf("Edit cancelled\n");
    }
    return contacts;
}


// Main function to test the program
int main() {
    Contact** addressBook = NULL;
    char* inputFileName = "output.txt";

    addressBook = loadContactsFromFile(addressBook, inputFileName);

    listContacts(addressBook);

    addressBook = editContact(addressBook);

    listContacts(addressBook);

    for (int i = 0; i < countContacts(addressBook); i++)
    {
        freeContact(addressBook[i]);
    }

    free(addressBook);

    return 0;
}