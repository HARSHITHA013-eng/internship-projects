#ifndef TRANSACTION_H
#define TRANSACTION_H

#include<string>
#include<iostream>
#include<ctime>

using namespace std;
class Transaction
{
    private:

    string type;  //deposit\withdraw
    double amount; 
    string date;

    public:

    Transaction(string type,double amount)
    {
        this->type=type;
        this->amount=amount;
       
        char buffer[11];
        strftime(buffer,sizeof(buffer),"%y-%m-%d",ltm);
        date=buffer;
    }
    //transmittion type
    string getType()
    {
        return type;
    }
    //transaction amount
    double getAmount()
    {
        return amount;
    }
    //transaction date
    string getdate()
    {
        return date;
    }
    void display()
    {
        cout<<"type\n:"<<type;
        cout<<"Amount\n:"<<amount;
        cout<<"Date:"<<date;
    }
};
#endif