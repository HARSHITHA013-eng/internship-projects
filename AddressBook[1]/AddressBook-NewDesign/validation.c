#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"validation.h"
#include"contact.h"

//validate function
// Name
int i;
int validateName(const char *name,AddressBook *addressBook)
{
  if(i==0) //first letter should be capital
  {
    if(!(name[i]>='A' && name[i]<='Z'))
    {
      printf("First letter should be capital\n");
      return 0;
    }
    i++;
  }
  for(i=1; name[i]!='\0'; i++) //check from second letter
  {
   if( ! ((name[i] >='A' && name[i]<='Z') || (name[i] >='a' && name[i]<='z') || name[i]==' '||name[i]=='.') )
   {
     printf("Name should contain only characters\n");
     return 0; //invalid
   }
  }
   return 1; //valid
}
// phone
int validatePhone(const char *phone,AddressBook *addressBook)
{ 
  int len=strlen(phone);
  if(len!=10)
  {
    return 0;
  }
  for(int i=0; i<len; i++)
  {
   if( !  (phone[i] >='0' && phone[i]<='9')  )
   {
     printf("phone number should contain only numbers\n");
     return 0; //invalid
   }

   
  }
  
  return 1;
}
//email
int validateEmail(const char *email,AddressBook *addressBook)
{
    int len = strlen(email);

   // Find '@'
  char *atPos = strchr(email, '@');
  if (!atPos)
  { 
    printf("Invalid Input\n @ is not found\n");
    return 0; // '@' not found
  }
    // Find ".com"
  char *dotCom = strstr(email, ".com");
  if (!dotCom) 
  { 
    printf("Invalid input\n .com not found\n");
    return 0; // ".com" not found
  }
   // Ensure ".com" is at the very end
  if (dotCom != email + len - 4) 
  {
    printf("Invalid input\n .com should be at end\n");
    return 0; // not at the end
  }
   // Ensure '@' is before ".com"
  if (atPos >= dotCom) 
  {
    printf("invalid input\n @ should be before .com\n ");
    return 0; // invalid if '@' is after or at start of ".com"
  }
   // Ensure there’s at least one character between '@' and ".com"
  if (dotCom - atPos <= 1) 
  { 
    printf("Invalid Input\n no characters found between @ and .com\n");
    return 0; // nothing between '@' and ".com"
  }
    // check part before @
    for (int i = 0; email[i]!='@'; i++)
   {
    if (!( (islower(email[i])) || (isdigit(email[i])) ||  email[i]=='@' || email[i]==' ' ||  email[i]=='.')) 
      {
        printf("only letters and digits are allowed before @\n");
          return 0;     // only letters and digits allowed
      }
      
    }
    //first character cannot be digit or " " or '.'
    if(i==0 && (isdigit(email[0]) || email[0]==' ' || email[0]=='.'))
    {
      printf("First letter cannot be special character\n ");
      return 0;
    }  
   return 1; // valid
}

