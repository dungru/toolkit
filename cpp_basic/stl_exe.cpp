#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;


class Vehicle {
    private:
        string name;
        int door;
    public:
        Vehicle(): name(""), door(0) {};
        Vehicle(string name, int door) : name(name), door(door) {};
        /* Copy Constructer */
        Vehicle(const Vehicle &other) {
            cout << "Copy Constructor Running "<< other.name << endl;
            name = other.name;
            door = other.door;
        }
/**
 *      bool operator<(const Vehicle &other) const {
 *          return name < other.name;
 *      }
 */
        void print() {
            cout << name << ": " << door << endl;
        }
        friend bool compare(Vehicle &a, Vehicle &b);
};

bool compare(Vehicle &a, Vehicle &b) {
    return a.door < b.door;
}

class Vehicle_key {
    private:
        string name;
        int door;
    public:
        Vehicle_key(): name(""), door(0) {};
        Vehicle_key(string name, int door) : name(name), door(door) {};
        /* Copy Constructer */
        Vehicle_key(const Vehicle_key &other) {
            cout << "Copy Constructor Running "<< other.name << endl;
            name = other.name;
            door = other.door;
        }

        void print() const {
            cout << name << ": " << door << endl;
        }

        bool operator<(const Vehicle_key &other) const {
            if (name == other.name)
            {
                return door < other.door; //Make different door become different object
            }
            return name < other.name;
        }
};

class Company {
    private:
        string name;
        int people;
    public:
        Company() : name(""), people(0) {}
        Company(string name, int people) : name(name), people(people) {}

        bool operator<(const Company &other) const {
            return name < other.name;
        }
        void print() const {
            cout << name <<": "<< people << endl;
        }
};

int main(int argc, char const *argv[])
{
    /* Vector */
    cout << "===== STL Vector =====" << endl;

    std::vector<string> name;
    std::vector<double> numbers(40);

    name.push_back("Dung-Ru");
    name.push_back("Dung-Ru1");
    name.push_back("Dung-Ru2");
    name.push_back("Dung-Ru3");

    for (std::vector<string>::iterator it = name.begin(); it != name.end(); ++it) {
        cout << *it << endl; 
    }
    unsigned int capacity = numbers.capacity();
    for (int i = 0; i < 2048; ++i)
    {
        numbers.push_back(i);
        if (capacity != numbers.capacity())
        {
            capacity = numbers.capacity();
            cout<< i << " Capacity: " << capacity << endl;
        }
    }
    numbers.resize(80);
    numbers.reserve(160);
    cout << "Size: " <<  numbers.size() <<endl;
    cout << "Capacity: " <<  numbers.capacity() <<endl;
    /* Two Dimensional vector*/
    std::vector< vector<int> > grid(3, vector<int>(6,7));
    for (int row = 0; row < grid.size(); ++row)
    {
        for (int col = 0; col < grid[row].size(); ++col)
        {
            cout << grid[row][col];

        }
        cout << endl;
    }
    /* List */
    cout << "===== STL List =====" << endl;
    std::list<int> mylist;
    mylist.push_back(1);
    mylist.push_back(2);
    mylist.push_back(3);
    mylist.push_back(4);
    mylist.push_front(0);

    std::list<int>::iterator it = mylist.begin();
    it++; //point to 1
    mylist.insert(it, 5566); // insert after 1
    //it++;
    mylist.erase(it); //erase 1

    for (std::list<int>::iterator it = mylist.begin(); it != mylist.end(); ++it)
    {
        cout<< *it << endl;
    }
    /* Map */
    cout << "===== STL MAP =====" << endl;
    std::map<string, int> prices;
    prices["BMW"] = 100;
    prices["Tesla"] = 120;
    prices["Toyota"] = 30;
    pair<string, int> newcar("Benz", 1);
    prices.insert(newcar);
    prices.insert(pair<string, int>("Porsche", 100));
    prices.insert(make_pair("Mazada", 500));


    if (prices.find("Nissan") != prices.end())
    {
        cout << "Nissan Found" << endl;
        //Do not just use prices[Nissan], It wiil add the key to the map.
    }

    for (std::map<string, int>::iterator it = prices.begin(); it != prices.end(); ++it)
    {
        pair<string, int> price_pair = *it;
        cout << price_pair.first << ": " << price_pair.second << endl;
    }
    cout << endl;
    for (std::map<string, int>::iterator it = prices.begin(); it != prices.end(); ++it)
    {
        cout << it->first << ": " << it->second << endl;
    }
    /* Object as MAP Value*/
    cout << "===== Object as MAP Value =====" << endl;
    std::map<int, Vehicle> parking; //This need the no parameter constructure
    parking[0] = Vehicle("Nissan", 4);
    parking[444] = Vehicle("Porsche", 2);
    parking[547] = Vehicle("Tesla", 6);

    /* It will use the copy constructor */
    parking.insert(make_pair(2454, Vehicle("Mazada", 4)));
    parking.insert(make_pair(2330, Vehicle("Ford", 4)));

    for (std::map<int, Vehicle>::iterator it = parking.begin(); it != parking.end(); ++it)
    {
        cout << it->first << ": " << flush;
        it->second.print();
    }
    /* Object as MAP Key*/
    cout << "===== Object as MAP Key =====" << endl;
    std::map<Vehicle_key, int> parking_key; //This need the no parameter constructure
    parking_key[Vehicle_key("Nissan", 4)] = 123;
    parking_key[Vehicle_key("Porsche", 2)] = 456;
    parking_key[Vehicle_key("Porsche", 4)] = 456;  // Make this work
    parking_key[Vehicle_key("Tesla", 6)] = 789;

    /* It will use the copy constructor */
    parking_key.insert(make_pair(Vehicle_key("Mazada", 4), 456));
    parking_key.insert(make_pair(Vehicle_key("Ford", 4), 7788));

    for (std::map<Vehicle_key, int>::iterator it = parking_key.begin(); it != parking_key.end(); ++it)
    {
        cout << it->second << ": " << flush;
        it->first.print(); //Need operator overloading make it work.
    }

    /* Set */
    cout << "===== Set =====" << endl;
    std::set<int> digit;
    digit.insert(1);
    digit.insert(1);
    digit.insert(2);
    digit.insert(30);

    for (std::set<int>::iterator it = digit.begin(); it != digit.end(); it++) {
        cout << *it << endl;
    }
    
    std::set<int>::iterator itfind = digit.find(30);
    if(itfind != digit.end()) {
        cout << "Found: " << *itfind << endl;
    }

    if (digit.count(30))
    {
        cout << "Digit Count!! " << endl;
    }
    cout << "===== Object as Set Member=====" << endl;
    std::set<Company> company;
    company.insert(Company("google",5000));
    company.insert(Company("cisco",50000));
    company.insert(Company("apple",7000));
    company.insert(Company("apple",9000)); // in set it will not insert.

    for (std::set<Company>::iterator it = company.begin(); it != company.end(); it++) {
        it->print();
    }

    cout << "===== STACK =====" << endl;
    std::stack<Vehicle> car_stack;
    car_stack.push(Vehicle("BMW",5));
    car_stack.push(Vehicle("Austin",5));
    car_stack.push(Vehicle("Benz",5));
    car_stack.push(Vehicle("Honda",5));

    while (car_stack.size() > 0) {
        Vehicle &tmp = car_stack.top();
        tmp.print();
        car_stack.pop();/* Do not use the item after the pop*/
    }
    
    cout << "===== QUEUE =====" << endl;
    std::queue<Vehicle> car_queue;
    car_queue.push(Vehicle("BMW",5));
    car_queue.push(Vehicle("Austin",5));
    car_queue.push(Vehicle("Benz",5));
    car_queue.push(Vehicle("Honda",5));

    cout << "Queue Last: " << flush;
    car_queue.back().print();

    while (car_queue.size() > 0) {
        Vehicle &tmp = car_queue.front();
        tmp.print();
        car_queue.pop();/* Do not use the item after the pop*/
    }

    /*Sort Vector*/
    std::vector<Vehicle> garage;
    garage.push_back(Vehicle("SUSUKI",78));
    garage.push_back(Vehicle("GM",3));
    garage.push_back(Vehicle("SAAB",8));
    garage.push_back(Vehicle("Volkswagen",2));
    garage.push_back(Vehicle("Volvo",8));
    
    sort(garage.begin(), garage.end(), compare);
    for (std::vector<Vehicle>::iterator it = garage.begin(); it != garage.end(); ++it)
    {
        it->print();
    }
    


    return 0;
}