#include <iostream>
using namespace std;

class Target{
    private:
        static Target* currInstance;
        // make constructor private 
        //so that it can't be called in other places to initialise objects of this type.
        Target(){
            cout<<"Constructor called!"<<endl;
        }
    public:
        // expose getInstance function to give access to object of this type
        static Target* getInstance(){
            if(Target::currInstance==NULL){
                Target::currInstance = new Target;
            }
            return Target::currInstance;
        }
};

Target* Target::currInstance = NULL;

int main(){
    Target* t = Target::getInstance();
    Target* t1 = Target::getInstance();
    if(t==t1) cout<<"Singleton implemented correctly"<<endl;
    else cout<<"Incorrect Implementation"<<endl;
    return 0;
}