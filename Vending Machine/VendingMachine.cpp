#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;
class Product{
    private:
        string name;
        int price;
        int id;
    public:
        Product(int price, int id){
            this->name = to_string(id);
            this->price = price;
            this->id = id;
        }
        int getId(){
            return id;
        }
        int getPrice(){
            return price;
        }
};
class Inventory{
    private:
        unordered_map<int, Product*> aisleToProductMap;
        unordered_map<int, int> productIdToCountMap;
        unordered_set<int> availableAisles;
    public:
        Inventory(int aisles){
            for(int i=1;i<=aisles;i++) availableAisles.insert(i);
        }
        void addProduct(Product &p, int count=1){
            int pid = p.getId();
            if(productIdToCountMap.count(pid)==0){
                if(availableAisles.size()>0){
                    int aisle = *availableAisles.begin();
                    aisleToProductMap[aisle] = &p;
                    availableAisles.erase(aisle);
                }
            }
            productIdToCountMap[pid]+=count;
        }
        void deductProductAt(int aisle){
            int pid = aisleToProductMap[aisle]->getId();
            productIdToCountMap[pid]--;
            if(productIdToCountMap[pid]==0){
                productIdToCountMap.erase(pid);
                aisleToProductMap.erase(aisle);
                availableAisles.insert(aisle);
            }
        }
        int getProductPriceById(int aisle){
            return aisleToProductMap[aisle]->getPrice();
        }
        bool checkProductInStock(int aisle){
            int pid = aisleToProductMap[aisle]->getId();
            return productIdToCountMap[pid] > 0;
        }
};
class VendingMachine;
class VendingMachineState{
    public:
        virtual void insertCoin(int coin)=0;
        virtual void pressButton(int num)=0;
        virtual void dispense(int num)=0;
};
class NoCoinInsertedState: public VendingMachineState{
    private:
        VendingMachine* vm;
    public:
        NoCoinInsertedState(VendingMachine* vm);
        void insertCoin(int coin);
        void pressButton(int num);
        void dispense(int num);
};
class CoinInsertedState: public VendingMachineState{
    private:
        VendingMachine* vm;
    public:
        CoinInsertedState(VendingMachine* vm);
        void insertCoin(int coin);
        void pressButton(int num);
        void dispense(int num);
};
class DispenseState: public VendingMachineState{
    private:
        VendingMachine* vm;
    public:
        DispenseState(VendingMachine* vm);
        void insertCoin(int coin);
        void pressButton(int num);
        void dispense(int num);
};

class VendingMachine{
    private:
        int amount;
        VendingMachineState* curState;
        NoCoinInsertedState* noCoinInsertedState;
        CoinInsertedState* coinInsertedState;
        DispenseState* dispenseState;
        Inventory* inventory;
        const static int aisle_count =2;
    public:
        VendingMachine(){
            noCoinInsertedState = new NoCoinInsertedState(this);
            coinInsertedState = new CoinInsertedState(this);
            dispenseState = new DispenseState(this);
            inventory = new Inventory(aisle_count);
            curState = noCoinInsertedState;
            amount = 0;
        }
        ~VendingMachine(){
            delete noCoinInsertedState;
            delete coinInsertedState;
            delete dispenseState;
            delete inventory;
            curState = NULL;
        }
        int getAmount(){
            return amount;
        }
        NoCoinInsertedState* getNoCoinInsertedState(){
            return noCoinInsertedState;
        }
        CoinInsertedState* getCoinInsertedState(){
            return coinInsertedState;
        }
        DispenseState* getDispenseState(){
            return dispenseState;
        }
        Inventory* getInventory(){
            return inventory;
        }
        void setVMState(VendingMachineState* newState){
            curState = newState;
        }
        void setAmount(int x){
            amount = x;
        }
        void insertCoin(int coin){
            curState->insertCoin(coin);
        }
        void pressButton(int aisle){
            curState->pressButton(aisle);
            curState->dispense(aisle);
        }
};

NoCoinInsertedState::NoCoinInsertedState(VendingMachine* vm){
    this->vm = vm;
}
void NoCoinInsertedState::insertCoin(int coin){
    vm->setAmount(coin);
    vm->setVMState(vm->getCoinInsertedState());
    cout<<"coin added "<<coin<<". Amount is "<<vm->getAmount()<<endl;
}
void NoCoinInsertedState::pressButton(int num){
    cout<<"No Coins Inserted"<<endl;
}
void NoCoinInsertedState::dispense(int num){
    cout<<"Can't Dispense. No Coins Inserted"<<endl;
}

CoinInsertedState::CoinInsertedState(VendingMachine* vm){
    this->vm = vm;
}
void CoinInsertedState::insertCoin(int coin){
    vm->setAmount(coin+vm->getAmount());
    cout<<"coin added "<<coin<<". Amount is "<<vm->getAmount()<<endl;
    // vm->setVMState(vm->getNoCoinInsertedState());
}
void CoinInsertedState::pressButton(int num){
    if(!vm->getInventory()->checkProductInStock(num)){
        cout<<"Product with Id: "<<num<<" not in stock."<<endl;
        return;
    }
    if(vm->getInventory()->getProductPriceById(num)>vm->getAmount()){
        cout<<"Insufficient Balance for product with Id: "<<num<<". Add "<< vm->getInventory()->getProductPriceById(num)-vm->getAmount()<<" more coins."<<endl;
        return;
    }
    vm->setVMState(vm->getDispenseState());
    cout<<"Aisle "<<num<<" selected."<<endl;
}
void CoinInsertedState::dispense(int num){
        cout<<"Can't Dispense. Add Product Num"<<endl;
}

DispenseState::DispenseState(VendingMachine* vm){
    this->vm = vm;
}
void DispenseState::insertCoin(int coin){
    cout<<"Can't add coins. VM is Dispensing."<<endl;
}
void DispenseState::pressButton(int num){
    cout<<"Can't add new product. VM is Dispensing."<<endl;
}
void DispenseState::dispense(int num){
    vm->getInventory()->deductProductAt(num);
    int change = vm->getAmount()-vm->getInventory()->getProductPriceById(num);
    vm->setAmount(0);
    vm->setVMState(vm->getNoCoinInsertedState());
    cout<<"Product dispensed. Collect Change of "<<change<<" coins"<<endl;
}


int main(){
    Product choco(5, 1);
    Product chips(10, 2);
    VendingMachine vm;
    vm.getInventory()->addProduct(choco, 2);
    vm.getInventory()->addProduct(chips, 1);
    vm.pressButton(1);
    vm.insertCoin(5);
    vm.insertCoin(10);
    vm.pressButton(2);
    vm.pressButton(1);
    return 0;
}