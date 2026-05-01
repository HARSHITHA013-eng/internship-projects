#include "BankSystem.h"
void BankSystem:: run()
{
    int choice;
    do
    {
        /* code */
        cout <<"Welcome to global bank\n";
        cout <<"1. banker login\n";
        cout <<"2. Customer Login\n";
        Cout <<"3. Exit\n";
        cout <,"choose an option:";
        cin >>choice;
        switch(choice)
        {
            case 1:
            bankerLogin();
            break;

            case 2:
            customerlogin();
            break;

            case 3:
             saveCustomers(); 
            cout<<"Exiting system\n";
            default:
            cout<<"Invalid choice\n";
        }
    } while (choice!=3);
    
}
void BankSystem::createCustomer()
{
    string id,name,password;
    cout <<"Enter Customer id: ";
    cin>>id;

    cout<<"Enter Name: ";
    cin>>name;

    cout<<"Enter password: ";
    cin>>password;

    customers.push_back(customer(id,name,password));

    cout <<"Customer created successfully\n";
}
view BankSystem:: viewCustomers()
{
    for(auto &c: customers)
    {
        cout<<"customer id:"<<c.getid() <<endl;
    }
}


void BankSystem:: bankerLogic()
{
    string id,password;
    cout <<"Enter banker id: ";
    cin >>id;

    cout<<"Enter password: ";
    cin>>password;

    if(banker.login(id,password))
    {
        cout<<"Banker login successful\n";
        bankerMenu();

    }
    else{
        cout <<"Authentication failed\n";
    }
}

void BankSystem::bankerMenu()
{
    int choice;
    do
    {
        /* code */
        banker.showMenu();
        cout<<"choose an option: ";
        cin>>choice;
        switch(choice)
        {
            case 1:
            createCustomer();
            break;

            case 2:
            viewCustomers()
            break;

            case 3:
            cout<<"Logged out successfully\n";
            break;

            default:
            cout<<"Invalid choice\n";

        }
    } while (choice!=3);
    
}

void BankSystem::customerLogin()
{
    string id,password;
    cout <<"Enter customer id: ";
    cin>>id;

    cout<<"Enter password: ";
    cin>>password;

    for(auto &c:customers)
    {
        if(c.login(id,password))
        {
            cout<<"customer login successful\n";
            customerMenu(c);
            return;
        }
    }
    cout<<"Authentication failed\n";
}

void BankSystem::customerMenu(customer &c)
{
    int choice;
    double amount;
    do
    {
        /* code */
        c.showMenu();
        cout <<"choose an option: ";
        cin>>choice;
        try
        {
            switch(choice)
            {
               case 1:
               cout<<"Balance: "<<c.getbalance()<<endl;
               break;
               
               case 2:
               cout<<"Enter amount to deposit: ";
               cin>>amount;
               c.deposit(amount);
               cout<<"Amount deposited successfully\n";
               break;

               case 3:
               cout<<"Enter amount to withdraw: ";
               cin>>amount;
               c.withdraw(amount);
               cout<<"withdraw successfully\n";
               break;

               case 4:
               c.showTransactions();
               break;

               case 5:
               cout<<"Logged out successfullt\n";
               break;

               default:
               cout<<"Invalid choice\n";


            }
        }

        catch(const std::exception& e)
        {
           cout<<"Error: "<<e.what()<<endl;
        }
        
    } while (choice!=5);
    
}