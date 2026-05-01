 #ifndef BANKER_H
#define BANKER_H

#include "User.h"
#include <iostream>

using namespace std;

//banker class
class Banker: public User
{
    public:
    Banker(string id,string password): User(id,password)
    {

    }
    //login
    bool logic(string id,string password)override
    {
         return (userid==id && user_password==password);
    }
    void showMenu()override
    {
       cout<<"/n1. Create Customer";
       cout<<"/n2. View all Customer";
       cout<<"/n3. Logout\n";

    }
};
#endif

