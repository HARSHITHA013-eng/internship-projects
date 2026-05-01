#ifndef USER_H
#define USER_H

#include <string>
using std::string;
//class for users
class User
{
    protected:
    string userid;  //user id
    string user_password; //password

    public:
    //constructor to initialise user id and password
    user(string id,string password)
    {
        userid=id;   //user id
        user_password=password;  //password
    }
    //function for login
    virtual bool login(string id,string password)=0;

    //function to display
    virtual void showmenu()=0;

    //deconstructor
    virtual ~user()
    {

    }   

};
#endif

               