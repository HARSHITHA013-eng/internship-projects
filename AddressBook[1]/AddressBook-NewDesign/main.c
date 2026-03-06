/*Name: Harshitha G H
Date: 24-09-2025
DESCRIPTION : This project is an Address Book in C. It uses structures to keep details like name, phone number, email. 
              With the help of file handling, the contacts are saved in a file so that they are not lost when the 
              program is closed. 
              The user can easily add, search, edit, delete, and view contacts
 */
#include <stdio.h>
#include "contact.h"


int main() {
    int choice;
    int sortChoice = 0;  
    AddressBook addressBook;
    initialize(&addressBook);
    // addressBook. contactCount = 0;


    do {
        printf("\nAddress Book Menu:\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
    	printf("6. Save contacts\n");		
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                createContact(&addressBook);
                break;
            case 2:
                searchContact(&addressBook);
                break;
            case 3:
                editContact(&addressBook);
                break;
            case 4:
                deleteContact(&addressBook);
                break;
            case 5:          
                listContacts(&addressBook, sortChoice);
                break;
            case 6:
                printf("Saving and Exiting...\n");
                saveContactsToFile(&addressBook);
                break;
            case 7:
                 printf("Exiting file");
                 break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);
    
       return 0;
}
