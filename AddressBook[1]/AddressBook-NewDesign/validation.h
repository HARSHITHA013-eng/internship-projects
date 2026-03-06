#ifndef VALIDATION_H
#define VALIDATION_H
#include "contact.h"

int validateName(const char *name);
int validatePhone(const char *phone, AddressBook *addressBook);
int validateEmail(const char *email,AddressBook *addressBook);

#endif
