#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

struct CricketDataPacket{
    int runs=0;
    int overs=0;
    int wickets=0;
};
class Observer{
    public:
        virtual void notify()=0;
};
class Observable{
    public:
        virtual void registerObserver(Observer*)=0;
        virtual void removeObserver(Observer*)=0;
        virtual void sendUpdate()=0;
};

class CricketData: public Observable{
    private:
        CricketDataPacket cricketDataPacket;
        vector<Observer*> observers;
    public:
        void registerObserver(Observer* currObserver){
            observers.push_back(currObserver);
        }
        void removeObserver(Observer* currObserver){
            auto it = find(observers.begin(), observers.end(), currObserver);
            if(it!=observers.end()){
                observers.erase(it);
            }
        }
        void sendUpdate(){
            for(auto observer: observers){
                observer->notify();
            }
        }
        void getUpdates(int run, int overs, int wicket){
            cricketDataPacket.runs = run;
            cricketDataPacket.overs = overs;
            cricketDataPacket.wickets = wicket;
            sendUpdate();
        }
        int getRuns(){
            return cricketDataPacket.runs;
        }
        int getWickets(){
            return cricketDataPacket.wickets;
        }
        int getOvers(){
            return cricketDataPacket.overs;
        }
};

class Scoreboard: public Observer{
    private:
        CricketDataPacket cricketDataPacket;
        CricketData* observable;
        void display(){
            cout<<"ScoreBoard:"<<endl;
            cout<<"\tRuns: "<<cricketDataPacket.runs<<endl;
            cout<<"\tOvers: "<<cricketDataPacket.overs<<endl;
            cout<<"\tWickets: "<<cricketDataPacket.wickets<<endl;
        }
    public:
        Scoreboard(CricketData* o){
            o->registerObserver(this);
            observable = o;
        }
        void notify(){
            cricketDataPacket.runs = observable->getRuns();
            cricketDataPacket.overs = observable->getOvers();
            cricketDataPacket.wickets = observable->getWickets();
            display();
        }
};

int main(){
    cout<<"Starting Program...................."<<endl;
    CricketData subject;
    Scoreboard board(&subject);
    Scoreboard board2(&subject);
    subject.getUpdates(6, 1, 1);
    subject.removeObserver(&board2);
    subject.getUpdates(10, 2, 1);
    return 0;
}