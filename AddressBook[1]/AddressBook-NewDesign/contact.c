#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"
#include "validation.h"

void listContacts(AddressBook *addressBook, int sortCriteria) 
{
    // Sort contacts based on the chosen criteria
     if (addressBook->contactCount == 0) {
        printf("\nNo contacts available.\n");
        return;
    }

    printf("\n---- Contact List ----\n");
    for (int i = 0; i < addressBook->contactCount; i++) {
        printf("  Name : %s", addressBook->contacts[i].name);
        printf("  Phone: %s", addressBook->contacts[i].phone);
        printf("  Email: %s\n", addressBook->contacts[i].email);
        
    }
    
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}

void saveAndExit(AddressBook *addressBook) 
{
    //save and exit
    saveContactsToFile(addressBook); // Save contacts to file
    printf("Contacts saved and program exiting\n");
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)     
{
    Contact newcontact;
	/* Define the logic to create a Contacts */
    //validation of name
        
        int valid;
        printf("Enter the student details:\n 1.name\n 2.phnum\n3.email \n:");
        do
        {
            
            printf("Enter the name :");
            scanf(" %[^\n]",newcontact.name);
            valid = validateName(newcontact.name);
            if(!valid)
            {
                printf("Enter valid name\n");
            }
       
        }while(!valid) ;  
         //validation of phone number
        do
        {   
            printf("Enter the phone :");
            scanf("%s",newcontact.phone);
            valid = validatePhone(newcontact.phone,addressBook);
            if(!valid)
            {
                printf("Enter valid phone number\n");
            }
            //checking duplicate
               int i;
               for(i=0; i <addressBook->contactCount; i++)
                {
                     if(strcmp(addressBook->contacts[i].phone,newcontact.phone)==0)
                     {
                           printf("Duplicate phone number\n");
                           valid=0;
                               break ;
                      }
   
                }
            
        } while (!valid);
        //validation of email
        do
        {
            printf("Enter the mail.id :");
            scanf("%s",newcontact.email);
            valid = validateEmail(newcontact.email,addressBook);
           if(!valid)
            {
                printf("Enter valid email\n");
            }
            //checking duplicate
               int i;
               for(i=0; i <addressBook->contactCount; i++)
                {
                     if(strcmp(addressBook->contacts[i].email,newcontact.email)==0)
                     {
                           printf("Duplicate email \n");
                              valid=0;
                               break ;;
                      }
   
                }
        } while (!valid);
        

    addressBook->contacts[addressBook->contactCount++] = newcontact;
    printf("Contact added successfully");
}

void searchContact(AddressBook *addressBook) 
{
    /* Define the logic for search */
    Contact newcontact;
    if(addressBook->contactCount==0)
    {
        printf("No contact found. Please create new contact");
        createContact(addressBook);
        return;
    }
         int valid;
        printf("Enter the student details:\n 1.name\n 2.phnum\n3.email \n:");
        int choice;
         char search[50];
         int found = 0;
        do {
        printf("\n--- Search Menu ---\n");
        printf("1. Search by Name\n");
        printf("2. Search by Phone Number\n");
        printf("3. Search by Email ID\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
            { // Search by Name
                do
                {
                     printf("Enter the name to search: ");
                    scanf(" %[^\n]", search);
                    valid = validateName(search);
                      if(!valid)
                    {
                    printf("Enter a valid name");
                    }
                } while (!valid);
                
                int matchedIndices[50]; // store all matches
                int matchCount = 0;
                for (int i = 0; i < addressBook->contactCount; i++) 
                {
                    if (strcmp(addressBook->contacts[i].name, search) == 0)
                    {
                      matchedIndices[matchCount++] = i;
                    }
                }
                if (matchCount == 0) //if incorrect input no matches found
                {
                  printf("Name not found!\n");
                  return;
                } 
                else if (matchCount == 1)
                {
                  int idx = matchedIndices[0];
                  printf("\nContact Found:\n");
                  printf("Name : %s\n", addressBook->contacts[idx].name);
                  printf("Phone : %s\n", addressBook->contacts[idx].phone);
                  printf("Email : %s\n", addressBook->contacts[idx].email);
                }
                printf("\nMultiple contacts found with the same name:\n");
                  for (int j = 0; j < matchCount; j++)
                  {
                    int idx = matchedIndices[j];
                    printf("Name : %s\n", addressBook->contacts[idx].name);
    
                   
                  }
          
                  printf("\nEnter phone number or email ID to refine search: ");
                  char refine[50];
                  scanf(" %[^\n]", refine);
                  int refinedFound = 0;
                  for (int j = 0; j < matchCount; j++)
                  {
                    int idx = matchedIndices[j];
                    if (strcmp(addressBook->contacts[idx].phone, refine) == 0 ||
                            strcmp(addressBook->contacts[idx].email, refine) == 0) 
                        {
                            printf("\nExact Contact Found:\n");
                            printf("Name : %s\n", addressBook->contacts[idx].name);
                            printf("Phone: %s\n", addressBook->contacts[idx].phone);
                            printf("Email: %s\n", addressBook->contacts[idx].email);
                            refinedFound = 1;
                            break;
                        }
                  }
                  if (!refinedFound)
                  {
                    printf("No contact matched the refinement!\n");
                  }
            }
              break;
        

            case 2:
            { // Search by Phone
                do
                {
                     printf("Enter the phone number to search: ");
                    scanf(" %[^\n]", search);
                    valid = validatePhone(search,addressBook);
                      if(!valid)
                    {
                    printf("Enter a valid phone number");
                    }
                } while (!valid);
                for (int i = 0; i < addressBook->contactCount; i++)
                {
                    if (strcmp(addressBook->contacts[i].phone, search) == 0)
                    {
                        printf("\nContact Found:\n");
                        printf("Name : %s\n", addressBook->contacts[i].name);
                        printf("Phone: %s\n", addressBook->contacts[i].phone);
                        printf("Email: %s\n", addressBook->contacts[i].email);
                        found = 1;
                        break;
                    }
                }
                 if (!found)
                {
                  printf("Phone number not found!\n");
                }
                found = 0;
                break;
            }
            case 3:
            { // Search by Email
                do
                {
                     printf("Enter the email ID to search: ");
                     scanf(" %[^\n]", search);
                    valid=validateEmail(search,addressBook);
                    if(!valid)
                    {
                        printf("Enter a valid email id");
                    }
                } while (!valid);
                for (int i = 0; i < addressBook->contactCount; i++)
                {
                    if (strcmp(addressBook->contacts[i].email, search) == 0)
                    {
                        printf("\nContact Found:\n");
                        printf("Name : %s\n", addressBook->contacts[i].name);
                        printf("Phone: %s\n", addressBook->contacts[i].phone);
                        printf("Email: %s\n", addressBook->contacts[i].email);
                        found = 1;
                        break;
                    }
                }
                 if (!found)
                {
                  printf("Email id  not found!\n");
                }
                found = 0;
                break;
            }
            case 4:
                printf("Exiting search menu...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice !=4);

  
}

void editContact(AddressBook *addressBook)
{
	/* Define the logic for Editcontact */
    char edit[100];
    int valid;
    char newname[100];
    int choice;
    char newnumber[100];
    char newemail[100];
    if(addressBook->contactCount==0)
    {
        printf("NO contact found. PLease create new contact");
        createContact(addressBook);
        return;
    }
    do
    {
        printf("Search contact to edit");
        printf("1.Name\n2.Phone\n3.email\n4.Exit\n");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
            {
               do{
                   printf("Enter the name to edit:\n");
                   scanf(" %[^\n]",edit);
                   valid = validateName(edit);
                   if(!valid)
                   {
                    printf("Enter a valid name:");
                    }
                }
                while(!valid);
                int matchesIndices[50];
                int matchCount=0;
                for(int i=0; i<addressBook->contactCount; i++)
                {
                    if(strcmp(addressBook->contacts[i].name,edit)==0)
                    {
                        matchesIndices[matchCount++]=i;
                    }
                }
                if(matchCount==0)
                {
                    printf("Name not found");
                    return;
                }
                else if(matchCount==1)
                {
                    do{
                       printf("Enter the new name");
                        scanf(" %[^\n]",newname);
                        valid=validateName(newname);
                        if(!valid)
                        {
                        printf("Enter the valide name"); 
                        }
                    } while(!valid);
                   
                    int idx=matchesIndices[0];
                      strcpy(addressBook->contacts[idx].name, newname);
                      printf("Updated Contact: %s | %s | %s\n",
                                  addressBook->contacts[idx].name,
                                     addressBook->contacts[idx].phone,
                                   addressBook->contacts[idx].email);
                                       
                }
                printf("Multiple matches found with same name");
                for(int j=0; j<matchCount; j++)
                {
                    int index=matchesIndices[j];
                    printf("Name|%s\n",addressBook->contacts[index].name);
                }
                printf("\nEnter phone number or email ID to refine search: ");
                  char refine[50];
                  scanf(" %[^\n]", refine);
                  int refinedFound = 1;
                  for (int j = 0; j < matchCount; j++)
                  {
                    int idx = matchesIndices[j];
                    if (strcmp(addressBook->contacts[idx].phone, refine) == 0 ||
                            strcmp(addressBook->contacts[idx].email, refine) == 0) 
                        {
                             do{
                                 printf("Enter the new name");
                                 scanf(" %[^\n]",newname);
                                 valid=validateName(newname);
                                 if(!valid)
                                  {
                                        printf("Enter the valide name"); 
                                    }
                                }  while(!valid);
                                   
                                    strcpy(addressBook->contacts[idx].name, newname);
                                    printf("Updated Contact: %s | %s | %s\n",
                                  addressBook->contacts[idx].name,
                                     addressBook->contacts[idx].phone,
                                   addressBook->contacts[idx].email);   
                        }
                    }
                     if (!refinedFound)
                     {
                        printf("No contact matched the refinement!\n");
                     }
                    
            
            }
            break;
            case 2: //phone number validation
            {
               do{
                   printf("Enter the phone number to edit:\n");
                   scanf(" %[^\n]",edit);
                   valid = validatePhone(edit,addressBook);
                   if(!valid)
                   {
                    printf("Enter a valid number:\n");
                    }
                }
                while(!valid);
                int refinedFound=1;
                int matchesIndices[50];
                int matchcount=0;
                for(int i=0; i<addressBook->contactCount; i++)
                {
                    if(strcmp(addressBook->contacts[i].phone,edit)==0)
                    {
                        matchesIndices[matchcount++]=i;
                    }
                }
                if(matchcount==0)
                {
                    printf("Number not found");
                    return;
                }
                else if(matchcount==1)
                {
                    do{
                       printf("Enter the new number");
                        scanf(" %[^\n]",newnumber);
                        valid=validatePhone(newnumber,addressBook);
                        if(!valid)
                        {
                        printf("Enter the valide number\n"); 
                        }
                        //checking duplicate
                          int i;
                         for(i=0; i <addressBook->contactCount; i++)
                           {
                               if(strcmp(addressBook->contacts[i].phone,newnumber)==0)
                                {
                                    printf("Duplicate phone number\n");
                                    valid=0;
                                    break ;
                                }
                           }
                          
                    } while(!valid);
                     int idx=matchesIndices[0];
                      strcpy(addressBook->contacts[idx].phone, newnumber);
                      printf("Updated Contact: %s | %s | %s\n",
                                  addressBook->contacts[idx].name,
                                     addressBook->contacts[idx].phone,
                                   addressBook->contacts[idx].email);                   
                }
                if (!refinedFound)
                     {
                        printf("No contact matched the refinement!\n");
                     }

            
            }
            break;
            case 3:
            {
               do{
                   printf("Enter the email to edit:\n");
                   scanf(" %[^\n]",edit);
                   valid = validateEmail(edit,addressBook);
                   if(!valid)
                   {
                    printf("Enter a valid email:\n");
                    }
                }
                while(!valid);
                int refinedFound=1;
                int matchedIndices[50];
                int matchcount=0;
                for(int i=0; i<addressBook->contactCount; i++)
                {
                    if(strcmp(addressBook->contacts[i].email,edit)==0)
                    {
                        matchedIndices[matchcount++]=i;
                    }
                }
                if(matchcount==0)
                {
                    printf("Email not found");
                    return;
                }
                else if(matchcount==1)
                {
                    do{
                       printf("Enter the new Email");
                        scanf(" %[^\n]",newemail);
                        valid=validateEmail(newemail,addressBook);
                        if(!valid)
                        {
                        printf("Enter the valide email\n"); 
                        }
                        //checking duplicate
                          int i;
                         for(i=0; i <addressBook->contactCount; i++)
                           {
                               if(strcmp(addressBook->contacts[i].email,newemail)==0)
                                {
                                    printf("Duplicate phone number");
                                    valid=0;
                                    break ;
                                }
                           }
                    } while(!valid);
                      int idx=matchedIndices[0];
                      strcpy(addressBook->contacts[idx].email, newemail);
                      printf("Updated Contact: %s | %s | %s\n",
                                  addressBook->contacts[idx].name,
                                     addressBook->contacts[idx].phone,
                                   addressBook->contacts[idx].email);           
                            
                      if (!refinedFound)
                     {
                        printf("No contact matched the refinement!\n");
                     }
                }
            
            }
            break;
            case 4:
                printf("Exiting search menu...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");

        }
    }while(choice !=4);    
   
}


void deleteContact(AddressBook *addressBook)
{
	/* Define the logic for deletecontact */
   
    char edit[100];
    int valid;
    int choice;

    if (addressBook->contactCount == 0)
    {
        printf("No contacts found. Please create a contact first.\n");
        return;
    }

    do
    {
        printf("Search contact to delete\n");
        printf("1. Name\n2. Phone\n3. Email\n4. Exit\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
        {
            do
            {
                printf("Enter the name to delete:\n");
                 scanf(" %[^\n]", edit);
                valid = validateName(edit);
                 if (!valid)
                {
                      printf("Invalid name.\n");
                      break;
                }
            } while (!valid);
            int matchesIndices[50];
            int matchCount = 0;
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].name, edit) == 0)
                {
                    matchesIndices[matchCount++] = i;
                }
            }

            if (matchCount == 0)
            {
                printf("Name not found.\n");
                break;
            }
            else if (matchCount == 1)
            {
                int idx = matchesIndices[0];  //which index is matched
                for (int k = idx; k < addressBook->contactCount - 1; k++)
                {
                    addressBook->contacts[k] = addressBook->contacts[k + 1]; //from the matched index the value should keep on shifting to eleminate the space
                }
                addressBook->contactCount--; //after eleminating decrease the size 

                printf("Contact deleted successfully.\n");
                if (addressBook->contactCount == 0) 
                {
                     printf("No remaining contacts.\n");

                } 
                else 
                {
                    printf("Remaining contacts:\n");
                        for (int i = 0; i < addressBook->contactCount; i++) 
                        {
                            printf("%s | %s | %s\n",
                              addressBook->contacts[i].name,
                                  addressBook->contacts[i].phone,
                                     addressBook->contacts[i].email);
                        }
                }
            }
            else
            {
                printf("Multiple matches found with same name:\n");
                for (int j = 0; j < matchCount; j++)
                {
                    int index = matchesIndices[j];
                    printf("%s | %s | %s\n",
                           addressBook->contacts[index].name,
                           addressBook->contacts[index].phone,
                           addressBook->contacts[index].email);
                }

                printf("Enter phone number or email ID to refine search: ");
                char refine[50];
                scanf(" %[^\n]", refine);
                int refinedFound = 0;

                for (int j = 0; j < matchCount; j++)
                {
                    int idx = matchesIndices[j];
                    if (strcmp(addressBook->contacts[idx].phone, refine) == 0 ||
                        strcmp(addressBook->contacts[idx].email, refine) == 0)
                    {
                        for (int k = idx; k < addressBook->contactCount - 1; k++)
                        {
                            addressBook->contacts[k] = addressBook->contacts[k + 1];
                        }
                        addressBook->contactCount--;

                        printf("Contact deleted successfully.\n");
                        if (addressBook->contactCount == 0) 
                        {
                              printf("No remaining contacts.\n");
                        } 
                        else
                         {
                                 printf("Remaining contacts:\n");
                              for (int i = 0; i < addressBook->contactCount; i++) 
                                 {
                                printf("%s | %s | %s\n",
                                     addressBook->contacts[i].name,
                                         addressBook->contacts[i].phone,
                                               addressBook->contacts[i].email);
                               }
                        }
                        refinedFound = 1;
                        break; // stop after one deletion
                    }
                }

                if (!refinedFound)
                {
                    printf("No contact matched the refinement!\n");
                }
            }
        }
        break;

        case 2:
        { 
            do
            {
                 printf("Enter the phone number to delete:\n");
                 scanf(" %[^\n]", edit);
                  valid = validatePhone(edit,addressBook);
                 if (!valid)
                 {
                      printf("Invalid phone number.\n");
                      break;
                 }
            }while(!valid);

            int matchesIndices[50];
            int matchCount = 0;
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].phone, edit) == 0)
                {
                    matchesIndices[matchCount++] = i;
                }
            }

            if (matchCount == 0)
            {
                printf("Phone number not found.\n");
                break;
            }

            int idx = matchesIndices[0];
            for (int k = idx; k < addressBook->contactCount - 1; k++)
            {
                addressBook->contacts[k] = addressBook->contacts[k + 1];
            }
            addressBook->contactCount--;

            printf("Contact deleted successfully.\n");
            if (addressBook->contactCount == 0)
            {
                printf("No remaining contacts.\n");
            } 
            else 
            {
                printf("Remaining contacts:\n");
                for (int i = 0; i < addressBook->contactCount; i++)
                    {
                         printf("%s | %s | %s\n",
                         addressBook->contacts[i].name,
                          addressBook->contacts[i].phone,
                            addressBook->contacts[i].email);
                     }
            }
        }
        break;

        case 3:
        {
            do
            {
                printf("Enter the email to delete:\n");
                scanf(" %[^\n]", edit);
                valid = validateEmail(edit,addressBook);
                if (!valid)
               {
                   printf("Invalid email.\n");
                   break;
                }

            } while (!valid);
                            
            int matchesIndices[50];
            int matchCount = 0;
            for (int i = 0; i < addressBook->contactCount; i++)
            {
                if (strcmp(addressBook->contacts[i].email, edit) == 0)
                {
                    matchesIndices[matchCount++] = i;
                }
            }

            if (matchCount == 0)
            {
                printf("Email not found.\n");
                break;
            }

            int idx = matchesIndices[0];
            for (int k = idx; k < addressBook->contactCount - 1; k++)
            {
                addressBook->contacts[k] = addressBook->contacts[k + 1];
            }
            addressBook->contactCount--;

            printf("Contact deleted successfully.\n");
            if (addressBook->contactCount == 0) 
            {
                       printf("No remaining contacts.\n");
            } 
            else 
            {
                printf("Remaining contacts:\n");
                 for (int i = 0; i < addressBook->contactCount; i++)
                  {
                          printf("%s | %s | %s\n",
                             addressBook->contacts[i].name,
                              addressBook->contacts[i].phone,
                          addressBook->contacts[i].email);
                   }
            }
        }
        break;

        case 4:
            printf("Exiting delete menu...\n");
            break;

        default:
            printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 4);
}

   

