#include<iostream>
#include<string>
#include<stdexcept>
using namespace std;

class passwordException :public invalid_argument
{
public:

    passwordException(string msg):invalid_argument(msg){}

};

class overloadException
{
    string errmsg;
public:
    overloadException(string msg):errmsg(msg){}
    void showError()
    {
        cout<<errmsg<<endl;
    }
};


class ChatHistory
{
    string sender[30];
    int n1;
    string receiver[30];
    int n2;
    string message[30];
    int n3;
public:
    ChatHistory(){
    n1=0;
    n2=0;
    n3=0;
    }
    //ChatHistory(string n1,string n2,string m):sender(n1),receiver(n2),message(m){}
    void setHistory(string u,string s,string m)
    {
        sender[n1++]=u;
        receiver[n2++]=s;
        message[n3++]=m;
    }

    void printChathistory()
    {
        cout<<"Chat history : \n";
        for(int i=0;i<n1;i++)
        {
            cout<<sender[i]<<" Sent a message : "<<message[i]<<" to "<<receiver[i]<<endl;
        }
    }
};


class User
{
protected:

    string password;
    int status;
    string email_id;
    int user_id;
    string name;
public:
    string group[15];
    int c;
    User *friends[15];
    int fc;
    ChatHistory *history;
    User(){}
    User(string n,string p,string e,int uid,string a[],int c):name(n),password(p),email_id(e),user_id(uid),status(0){
    int i;

    for(i=0;i<c;i++)
    {
        group[i]=a[i];
    }
    this->c=c;
    fc=0;
    history = new ChatHistory();
    }

    void UaddFriend(User *a)
    {
        friends[fc++]=a;
    }


    void ULogin(string password)
    {
        if(this->password==password)
        {
            cout<<"Login successful\n";
            status=1;
        }
        else
        {
            throw passwordException("Cannot Login\n");
        }
    }
    void ULogout()
    {
        cout<<"Logged out\n";
        status=0;
    }
    void UprintUser()
    {
        int j;
        cout<<"User details:\n";
        cout<<"Name    :"<<name<<endl;
        cout<<"User id :"<<user_id<<endl;
        cout<<"Email id:"<<email_id<<endl;
        cout<<"Groups : ";
        for(j=0;j<c;j++)
        {
            cout<<"("<<j+1<<")"<<group[j]<<"\t";
        }
        cout<<endl;
        cout << "Friends : ";
        for (int i = 0; i < fc; i++) {
            cout << friends[i]->name<<"  ";
        }
        cout<<endl;
    }
    string UgetName()
    {
        return this->name;
    }

    void Uaddgroup(string sg)
    {

        group[c++]=sg;
    }

    void Usubgroup(string lg)
    {
        int y=c;
        for(int k=0;k<y;k++)
        {
            if(lg==group[k])
            {
                for(int j=k;j<y-1;j++)
                {
                    group[j]=group[j+1];
                }
                y--;
                cout<<" group : "<<lg<<" is deleted"<<endl;
            }
        }
        c--;

    }
        void UremoveFriend(User *rf)
    {
        int y=fc;
        for(int k=0;k<y;k++)
        {
            if(rf->name==friends[k]->name)
            {
                for(int j=k;j<y-1;j++)
                {
                    friends[j]=friends[j+1];
                }
                y--;
                cout<<" Friend : "<<rf->name<<" is removed"<<endl;
            }
        }
        fc--;

    }
    void UprintHistory()
    {
        history->printChathistory();
    }
    int getstatus()
    {
        return status;
    }

    friend class ManageProfile;
};


class Chat
{
protected:
    string chatType;
    //string content;
public:

    Chat(string type):chatType(type){}
    virtual void sendMessage(string msg,string x,User *u)
    {
        cout<<u->UgetName()<<" Sent a message : "<<msg<<endl;
    }
    virtual void receiveMessage(string msg,string x,User *u)
    {
        cout<<u->UgetName()<<" Received message : "<<msg<<endl;
    }
    virtual void display(User *u)
    {
        cout<<chatType<<endl;
    }
    string getType()
    {
        return chatType;
    }
};
class IndividualChat:public Chat
{
public:
    IndividualChat(string type):Chat(type){}

    void sendMessage(string msg,string n,User *u)
    {
        User *x;
        int y=0;
        for(int i=0;i<u->fc;i++)
        {
            if(u->friends[i]->UgetName()==n)
            {
                x=u->friends[i];
                cout<<u->UgetName()<<" Sent a message : "<<msg<<" to "<<x->UgetName()<<endl;
                u->history->setHistory(u->UgetName(),x->UgetName(),msg);
                y=1;
                break;
            }

        }
            if(y==0)
            {
                throw overloadException("User is not a friend with the receipient.Cannot send message\n");
            }
    }
    void receiveMessage(string msg,string n,User *u)
    {
        User *x;
        int y=0;
        for(int i=0;i<u->fc;i++)
        {
            if(u->friends[i]->UgetName()==n)
            {
                x=u->friends[i];
                cout<<u->UgetName()<<" Received a message : "<<msg<<" from "<<x->UgetName()<<endl;
                u->history->setHistory(x->UgetName(),u->UgetName(),msg);
                y=1;
                break;
            }

        }
            if(y==0)
            {
                throw overloadException("User is not a friend with the sender.Cannot receive message\n");
            }
    }


};

class GroupChat:public Chat
{

public:
    GroupChat(string type):Chat(type){}

    void joingroup(string sg,User *u)
    {
        u->Uaddgroup(sg);
    }
    void leavegroup(string lg,User *u)
    {
        u->Usubgroup(lg);
    }
    void sendMessage(string msg,string n,User *u)
    {
        int y=0;
        for(int i=0;i<u->c;i++)
        {
            if(u->group[i]==n)
            {

                cout<<u->UgetName()<<" Sent a message : "<<msg<<" to "<<n<<endl;
                y=1;
                u->history->setHistory(u->UgetName(),n,msg);
                break;
            }
        }
        if(y==0)
        {
            throw overloadException("User is not a part of the group.Cannot receive message\n");
        }
    }
    void receiveMessage(string msg,string n,User *u)
    {
        int y=0;
        for(int i=0;i<u->fc;i++)
        {
            if(u->group[i]==n)
            {
                cout<<u->UgetName()<<"  Received a message : "<<msg<<" to "<<n<<endl;
                u->history->setHistory(n,u->UgetName(),msg);
                y=1;
                break;
            }
        }
        if(y==0)
        {
            throw overloadException("User is not a part of the group.Cannot receive message\n");
        }
    }



};

class ManageProfile
{
public:
    void changePassword(User *a)
    {
        string n,m;
        cout<<"Enter current password\n";
        cin>>n;
        if(n==a->password)
        {
            cout<<"Enter new password\n";
            cin>>m;
            a->password=m;
            cout<<"New password set successfully\n";
        }
        else
        {
            throw passwordException("Cannot set new password\n");
        }


    }
    void changeEmail(User *a)
    {
        string m,n;
        cout<<"Enter current password to set new Email\n";
        cin>>n;

        if(n==a->password)
        {
            cout<<"Enter new Email Id\n";
            cin>>m;
            a->email_id=m;
            cout<<"New Email Id set successfully\n";
        }
        else
        {
             throw passwordException("Cannot set new Email\n");
        }

    }
    void changeName(User *a)
    {
        string m,n;
        cout<<"Enter current password to change name\n";
        cin>>n;

        if(n==a->password)
        {
            cout<<"Enter new Name\n";
            cin>>m;
            a->name=m;
            cout<<"New Name set successfully\n";
        }
        else
        {
             throw passwordException("Cannot set new Name\n");
        }
    }
};

class ChatApplication
{
    string name;
public:
    User *u;
    Chat *c1;
    Chat *c2;
    ChatApplication(User *a):u(a),c1(nullptr),c2(nullptr){}
    void setGroupChat(GroupChat *g)
    {
        c1=g;
    }
    void setIndividualChat(IndividualChat *i)
    {
        c2=i;
    }
    void addFriend(User *a)
    {
        if(u->getstatus()==1)
        u->UaddFriend(a);
        else
            cout<<"Please Login\n";
    }
    void Login(string password)
    {
        u->ULogin(password);
    }
    void Logout()
    {
        u->ULogout();
    }
    void printUser()
    {
        if(u->getstatus()==1)
        u->UprintUser();
        else
            cout<<"Please Login\n";

    }
    string getName()
    {
        return u->UgetName();
    }

    void addgroup(string sg)
    {
        if(u->getstatus()==1)
        u->Uaddgroup(sg);
        else
            cout<<"Please Login\n";

    }

    void removegroup(string lg)
    {
        if(u->getstatus()==1)
        u->Usubgroup(lg);
        else
            cout<<"Please Login\n";

    }

    void sendM(string msg,string x,string t)
    {
        if(u->getstatus()==1)
        {
            if(t=="Individual")
            c2->sendMessage(msg,x,u);
            else
            c1->sendMessage(msg,x,u);
        }
        else
            cout<<"Please Login\n";

    }
    void receiveM(string msg,string x,string t)
    {
        if(u->getstatus()==1)
        {
            if(t=="Individual")
            c2->receiveMessage(msg,x,u);
            else
            c1->receiveMessage(msg,x,u);
        }
        else
            cout<<"Please Login\n";

    }
    void removeFriend(User *fu)
    {
        if(u->getstatus()==1)
        {
            u->UremoveFriend(fu);
        }
        else
            cout<<"Please Login\n";
    }
    void DisplayChatHistory()
    {
        if(u->getstatus()==1)
        u->UprintHistory();
        else
            cout<<"Please Login\n";

    }
};

int main()
{
    string a[2]={"Oops_D_div","MicroC"};
    User *u1=new User("Rahul","12345","@gmail",14587,a,2);
    User *u2=new User("Vandana","2003","@yahoo",789,a,2);
    User *u3=new User("Siri","hello123","Siri@gmail.com",158,a,2);
    GroupChat *g1=new GroupChat("Group");
    IndividualChat *i1=new IndividualChat("Individual");
    ChatApplication wt(u1);
    wt.setGroupChat(g1);
    try{
    wt.receiveM("Hello","Vandana","Individual");
    wt.Login("12345");
    wt.setIndividualChat(i1);
    wt.addFriend(u2);
    wt.addFriend(u3);
    wt.addgroup("EDA");
    wt.printUser();
    cout<<endl;
    wt.sendM("hi","Oops_D_div","Group");
    wt.receiveM("Hello","Vandana","Individual");
    wt.sendM("Good evening","Siri","Individual");
    wt.receiveM("GM","MicroC","Group");
    cout<<endl;
    wt.DisplayChatHistory();
    wt.removeFriend(u3);
    wt.removegroup("MicroC");
    wt.printUser();
    cout<<endl;
    ManageProfile p1;
    p1.changePassword(u1);
    wt.printUser();
    p1.changeEmail(u1);

    }
    catch(passwordException &pe)
    {
        cout<<pe.what();
    }
    catch(overloadException & oe)
    {
        oe.showError();
    }
    try{
        cout<<endl;
        wt.sendM("GM","Friends","Group");
    }
     catch(passwordException &pe)
    {
        cout<<pe.what();
    }
    catch(overloadException & oe)
    {
        oe.showError();
    }




    //p1.changeEmail(u1);
    //p2.changeName(u1);
}