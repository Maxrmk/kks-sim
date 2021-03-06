#include <fstream>
#include <algorithm>
#include <iostream>

#include "Loader.hxx"

Loader::Loader(std::list<Task*>* taskList, std::string fileName) :
                                    loadedTasks(taskList) {
    std::ifstream inFile;
    inFile.open(fileName);
    inFile.ignore(512,'\n');

    //start time, length, deadline, priority, block period, blcok length
    unsigned long start, length, deadline, priority, blockPeriod, blockLength;

    while(inFile>>start) {
        inFile>>length;
        inFile>>deadline;
        inFile>>priority;
        inFile>>blockPeriod;
        inFile>>blockLength;
        unloadedTasks.push_back(new Task(length, start,deadline,
                                         priority,blockPeriod,blockLength));
    }
    unloadedTasks.sort(
        [](Task* a,Task* b){ return a->getSpawnTime() < b->getSpawnTime();});
}

Loader::~Loader() {
    std::for_each(unloadedTasks.begin(),unloadedTasks.end(),
        [](Task* a){delete a;});
}

void Loader::update(int time) {//We should change this interface, I'm just not sure how
    //std::cout<<unloadedTasks.size()<<std::endl;
    while(unloadedTasks.size() && (*unloadedTasks.begin())->getSpawnTime() <= time){        //non empty and first should spawn
        (*unloadedTasks.begin())->setTimeStarted(time);                                     //set task time started
        loadedTasks->push_back(*unloadedTasks.begin());                                     //push task onto scheduler's queue

        unloadedTasks.pop_front();                                                          //pop from unloaded
    }
}
