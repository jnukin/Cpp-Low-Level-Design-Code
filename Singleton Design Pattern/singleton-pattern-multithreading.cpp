#include <iostream>
#include <thread>
using namespace std;

class Target{
    private:
        static Target* currInstance;
        static mutex lock;
        // make constructor private 
        //so that it can't be called in other places to initialise objects of this type.
        Target(){
            cout<<"Constructor called!"<<endl;
        }
    public:
        // expose getInstance function to give access to object of this type
        static Target* getInstance(){
            if(Target::currInstance==NULL){
                // get lock to initialise object
                lock_guard<mutex> lg(Target::lock);
                // after get lock recheck if object not created while this thread was waiting for lock
                if(Target::currInstance==NULL)
                    Target::currInstance = new Target;
            }
            return Target::currInstance;
        }
};

Target* Target::currInstance = NULL;
mutex Target::lock;

int main(){
    Target *tout, *t1out;

    thread t([&tout](){tout = Target::getInstance();});
    thread t1([&t1out](){t1out = Target::getInstance();});

    t.join();
    t1.join();

    if(tout==t1out) cout<<"Singleton implemented correctly"<<endl;
    else cout<<"Incorrect Implementation"<<endl;

    return 0;
}