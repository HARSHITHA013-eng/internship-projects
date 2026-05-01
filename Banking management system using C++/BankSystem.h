#ifndef BANKSYSTEM_H
#define BANKSYSTEM_H

#include "Customer.h"
#include "Bank.h"
#include <vector>
#include<iostream>
using namespace std;

class BankSystem
{
    private:
    vector<customer> customers;
    Banker banker;
    public:
    BankSystem(): banker("B001","admin123")
    {  }
    void run();
    void bankerLogic();
    void customerLogic();
    void bankerMenu();
    void createCustomer();
    void customerMenu(customer &c);
    void viewCustomers();
};
#endif