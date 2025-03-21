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

Contact **appendContact(Contact **contacts, Contact *newContact);

void freeContact(Contact *c);

void freeAddressBook(Contact** addressBook)

Contact** removeContactByIndex(Contact** contacts);

int removeContactByFullName(Contact** contacts);

void listContacts(Contact** contacts);

void saveContactsToFile(Contact** contacts, char* filename);

void printContactsToFile(Contact** contacts, char* filename);

Contact** loadContactsFromFile(Contact** addressBook, char* filename);

int main()
{

    int option = 0;
    Contact** addressBook = NULL;
    Contact** newAddressBook = NULL;
    const int EXIT_OPTION = 12;
    const int APPEND_CONTACT_OPTION = 1;

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
        printf("Address Book Menu\n1. Append Contact\n2. Insert Contact in Alphabetical Order\n3. Remove Contact by Index\n");
        printf("4. Remove Contact by Full Name\n5. Find and Edit Contact\n6. List Contacts\n7. Print Contacts to File with the format of an input file\n");
        printf("8. Print Contacts to File (Human Readable)\n9. Load Contacts from File Replacing Existing Contacts\n10. Append Contacts from File\n");
        printf("11. Merge Contacts from File\n12. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &option);

        /* change to use case statements*/
        switch(option)
        {
            case APPEND_CONTACT_OPTION:
                if (addressBook == NULL)
                {
                    addressBook = (Contact**)malloc(sizeof(Contact*));
                    if (addressBook == NULL)
                    {
                        fprintf(stderr, "ERROR allocating memory");
                        return 1;
                    }
                }
                newAddressBook = (Contact**)realloc(addressBook, sizeof(Contact*) * countContacts(addressBook) + 2);
                
        }
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
    fscanf(stdin, " 99[^\n]", buffer);
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


/*check and test this big function*/
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
    if (sscanf(getBuffer, "%d", numContacts) != 1)
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
        sscanf(getBuffer, "%s", scanBuffer);
        myFirstName = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myFirstName == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
            freeAddressBook(addressBook);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myFirstName, scanBuffer);
        newContact->firstName = myFirstName;

        /*familyName*/
        fgets(getBuffer, sizeof(getBuffer), inputStream);
        sscanf(getBuffer, "%s", scanBuffer);
        myFamilyName = (char*)calloc((strlen(scanBuffer) + 1), sizeof(char));
        if (myFamilyName == NULL)
        {
            fprintf(stderr, "Error: Memory allocation error, memory for string in Contact %d not allocated", i);
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
            freeAddressBook(addressBook);
            fclose(inputStream);
            return NULL;
        }
        strcpy(myAddress, scanBuffer);
        newContact->familyName = myAddress;

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

    printf("Contacts load from file: %s", filename);
}








