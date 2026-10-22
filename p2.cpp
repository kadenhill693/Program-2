#include "request.h"
#include "webserver.cpp"
#include "loadbalancer.h"

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>

using namespace std;

const int NUM_SERVERS = 5;
static int ps = 0;			// the process ID incrementer, static so one copy persists

request createRandomRequest()
{
   stringstream ips, ipd;
   request r;
   // For example, 12.243.87.112
   ips << (rand() % 256) << "." << (rand() % 256) << "." << (rand() % 256) << "." << (rand() % 256);
   ipd << (rand() % 256) << "." << (rand() % 256) << "." << (rand() % 256) << "." << (rand() % 256);
   r.source = ips.str();
   r.destination = ipd.str();
   r.processTime = (rand() % 500);
   r.pid = ++ps;
   return r;
}

int main()
{
   // random number generator (fixed seed so runs are deterministic)
   srand(7);

   // create a loadbalancer
   loadbalancer lb;
   
   // start off with a "full" queue
   for (int i = 0; i < 10; i++)
   {
      lb.addRequest(createRandomRequest());
   }
   
   // an array of webservers, each given an initial request to process
   webserver webarray[NUM_SERVERS];
   for (int i = 0; i < NUM_SERVERS; i++)
   {
      webarray[i] = webserver('A' + i);
      int startTime = lb.getTime();   // read the clock first, then take the request
      webarray[i].addRequest(lb.getRequest(), startTime);
   }
   // loop
   while (lb.getTime() < 10000)
   {
      int currTime = lb.getTime();
      int size = lb.getSize();
      // check each webserver if it's done
      if(webarray[currTime % NUM_SERVERS].isRequestDone(currTime))
      {
         // get request from the server
         request r = webarray[currTime % NUM_SERVERS].getRequest();
		 
		 // print some stats
         cout << "Finished at " << currTime << " " << webarray[currTime % NUM_SERVERS].getName();
         cout << " pid " << r.pid <<"/" << size << " process time was " << r.processTime ;
         cout << endl;
		 
         // then give it a new request
         if (!lb.isRequestQueueEmpty())
         {
            webarray[currTime % NUM_SERVERS].addRequest(lb.getRequest(), currTime);
         }
      }
      // every random set of time, get a new request
      if (rand() % 10 == 0)
      {
         lb.addRequest(createRandomRequest());
      }
      lb.incTime();
   }

}

