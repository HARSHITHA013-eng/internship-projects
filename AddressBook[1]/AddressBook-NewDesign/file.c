#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook)
{
    FILE *fp = fopen("contact.csv","w"); // overwrite mode
    if (fp == NULL) {
        printf("Error opening file for writing\n");
        return;
    }

    // save contact count 
    fprintf(fp,"%d\n", addressBook->contactCount);

    // save contacts
    for (int i = 0; i < addressBook->contactCount; i++) {
        fprintf(fp,"%d,%s,%s,%s\n", i+1,
                addressBook->contacts[i].name,
                addressBook->contacts[i].phone,
                addressBook->contacts[i].email);
    }

    fclose(fp);
    printf("Contacts successfully saved to the CSV file\n");
}

void loadContactsFromFile(AddressBook *addressBook) 
{  
    FILE *fp = fopen("contact.csv","r");
    if (fp == NULL) {
        printf("No contacts found\n");
        addressBook->contactCount = 0; // reset
        return;
    }

    // read contact count
    if (fscanf(fp,"%d\n",&addressBook->contactCount) != 1) {
        printf("Failed to read contact count\n");
        addressBook->contactCount = 0;
        fclose(fp);
        return;
    }

    for (int i = 0; i < addressBook->contactCount; i++) {
        int j; // index from file
        fscanf(fp,"%d,%[^,],%[^,],%[^\n]\n",
               &j,
               addressBook->contacts[i].name,
               addressBook->contacts[i].phone,
               addressBook->contacts[i].email);
    }
    fclose(fp); 
}
