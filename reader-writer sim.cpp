/*
 * File:     inputsim.cpp
 * Author:   Adam.Lewis@athens.edu
 *
 * This code serves multiple purposes: (1) provide a more extensive example of
 * using threads, (2) demo how to use to C++ STL random classes for generating
 * random numbers, (3) provide more practice with the STL container and
 * iterator classes, and (4) provide code for use in a future programming
 * project.
 *
 * NOTES:
 * (a) Be aware that this code has some potential race conditions in how it's
 * reading from the deque.  You will address these conditions in the
 * aforementioned homework assignment.
 * (b) Most people are aware of the srand()/rand() pseudo-random number
 * generator that's in the cstdlib library.  It's preferred with Modern C++ to
 * use the C++ STL's random classes.
 * (c) We need to make this app sleep at multiple points in the
 * application. There are multiple ways of doing this but the preferred method
 * in Modern C++ is a combination of methods from the chrono and thread
 * libraries
 *
 */

#include <iostream>
#include <random>
#include <thread>
#include <string>
#include <sstream>
#include <deque>
#include <chrono>
#include <fstream>
#include <mutex>

 // I am being lazy here... the common preference is to not do a using
 // statement. 
using namespace std;

// We will be using this deque as a shared global between two threads: a thread
// that simulates the reading of a set of devices and the main thread that is
// extracting values from the queue and printing them.
deque<string> inputQueue;
mutex myMutex;
//
// void simulateInput(int rate)
//
// This function simulates a set of devices generating input to the
// program. Here's where you get to use some of those things that we learn from
// our Prob. and Stats. class.   The devices generate new input at a predefined
// rate.  For example, we expect input at a rate of 3 samples/second.
//
// Data from the devices is simulated using a normal distribution with a mean
// of 5.0 and standard deviation of 3.0.   We simulate device id by randomly
// selecting a uniformly distributed integer (this is what we
// commonly think of when we do random numbers in a program) between 0 and 10.
//
// We build the data for the queue using a string stream.  The randomly
// generated data is written to the stream and we then push the backing string
// onto the queue.
//

void simulateInput(int rate)
{
    random_device rd{};
    mt19937 gen{ rd() };
    int count = 0;

    poisson_distribution<> pD(rate);
    normal_distribution<> nD(5.0, 3.0);
    uniform_int_distribution<int> uD(0, 10);

    while (true)
    {   
        
        int numberEvents = pD(gen);
        if (numberEvents > 0)
        {
            
            for (int i = 0; i < numberEvents; ++i)
            {
                std::lock_guard<std::mutex> guard(myMutex);
                cout << "creator locked\n";
                stringstream sampleStream;
                int deviceNumber = uD(gen);
                float sample = nD(gen);
                count++;
                sampleStream << count << " " << deviceNumber << " " << sample;
                inputQueue.push_front(sampleStream.str());
                cout << "creator unlocked\n";
            }
        }
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}

void writer(ofstream& out,int id) {
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(200));
        std::lock_guard<std::mutex> guard(myMutex);
        if (inputQueue.size() != 0) {
            
            //std::cout << id << "locked" << std::endl;
            out << inputQueue.back() << std::endl;
            inputQueue.pop_back();
            //std::cout << id << "unlocked" << std::endl;
            
        }
    }
}
//
// int main(int argc, char *argv[])
//
// We test our input simulator by creating two threads: our main application
// thread (created when we start our program) and a thread that executes the
// simulateInput() program.   Then, we go into an infinite loop reading and
// printing the data values from the input queue.
//
// NOTES:
// You must manually terminate this program as both threads intentionally
// has infinite loops.
//
// TODO:
// For the moment, we use a constant to define the average sample rate passed
// to the simulateInput() function.  This needs to be changed to get that value
// from the command-line arguments.
//

const int SAMPLE_ARRIVAL_RATE = 3;
int main(int argc, char* argv[])
{
    inputQueue.push_front("0 START");
    // Build and lanuch the input simulator thread
    thread inputThread(simulateInput, SAMPLE_ARRIVAL_RATE);
    inputThread.detach();
    // Pause three seconds
    cout << "Pausing three seconds for station identification" << endl;
    this_thread::sleep_for(chrono::milliseconds(3000));
    // Now start printing stuff from the queue
   
    ofstream out1;
    out1.open("output1.txt");
    thread writerThread1(writer, std::ref(out1),1);
    writerThread1.detach();

    ofstream out2;
    out2.open("output2.txt");
    thread writerThread2(writer, std::ref(out2),2);
    writerThread2.detach();

    ofstream out3;
    out3.open("output3.txt");
    thread writerThread3(writer, std::ref(out3),3);
    writerThread3.detach();

    ofstream out4;
    out4.open("output4.txt");
    thread writerThread4(writer, std::ref(out4),4);
    writerThread4.detach();

    ofstream out5;
    out5.open("output5.txt");
    thread writerThread5(writer, std::ref(out5),5);
    writerThread5.detach();

    ofstream out6;
    out6.open("output6.txt");
    thread writerThread6(writer, std::ref(out6),6);
    writerThread6.detach();

    ofstream out7;
    out7.open("output7.txt");
    thread writerThread7(writer, std::ref(out7),7);
    writerThread7.detach();

    ofstream out8;
    out8.open("output8.txt");
    thread writerThread8(writer, std::ref(out8),8);
    writerThread8.detach();

    ofstream out9;
    out9.open("output9.txt");
    thread writerThread9(writer, std::ref(out9),9);
    writerThread9.detach();

    ofstream out10;
    out10.open("output10.txt");
    thread writerThread10(writer, std::ref(out10),10);
    writerThread10.detach();

    while (true) {

    }
}