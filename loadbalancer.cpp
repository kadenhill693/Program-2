/* loadbalancer.cpp
 *
 * Manage the CPU load by assigning next job to CPU with the least load
 */

#include "loadbalancer.h"

int loadbalancer::getTime()
{
   return systemTime;
}

void loadbalancer::incTime()
{
   systemTime++;
}
  
void loadbalancer::addRequest(request r)
{
   requestQueue.push(r);
   incTime();   // each request added or removed advances the clock
}
  
request loadbalancer::getRequest()
{
   request r = requestQueue.front(); // queue
   //request r = requestQueue.top(); // priority_queue and stack
   requestQueue.pop();
   incTime();
   return r;
}

bool loadbalancer::isRequestQueueEmpty()
{
   return requestQueue.empty();
} 

int loadbalancer::getSize()
{
   return requestQueue.size();
} 
