 #ifndef CUSTOMER_H
#define CUSTOMER_H

#include "User.h"
#include "Transaction.h"
#include <vector>
#include <iostream>
#include <stdexcept>
using namespace std;

  //customers class
class Customer: public User
{
    private:
    string name;  //customer name
    double balance; //account balance

    public:
    Customer(string id,string name,string password):User(id,password)
    {
        this->name=name;
        balance=0.0;
    }
    //login for customers
    bool login(string id,string password) override
    {
        return (userid==id && user_password==password);

    }

    //customer menu
    void showMenu() override
    {
        cout <<"\n1. View Account Details";
        cout <<"\n2. Deposit";
        cout <<"\n3. Withdraw";
        cout <<"\n4. View Transaction History";
        cout <<"\n5. logout\n";

    }
    //deposit money
    void deposit(double amount)
    {
        balance=balance+amount;
    }
    //withdraw
    void withdraw(double amount)
    {
        if(amount>balance)
        {
            throw runtime_error("Insufficient funds for transaction");
            balance=balance-amount;
        }

    }
    //current balance
    double getbalance()
    {
        return balance;
    }
    string getName()
    {
     return name;
    }

    string getPassword()
    {
       return user_password;
    }

};
#endif
              