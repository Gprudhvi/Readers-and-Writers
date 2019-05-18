#include <iostream>
#include <thread>
#include<ctime>
#include<atomic>
#include<bits/stdc++.h>
#include<semaphore.h>
 
 
  using namespace std;
  ostringstream *arr1,*arr2;
  sem_t m;
  sem_t rwm;
  double *avg1,*avg2;
  int kw,kr;
  int nw,nr,read_count ;
  int csSeed=1,remSeed=1;
  int *p;
  int l =0;
  ifstream ip;
  ofstream fp;
  ofstream f;
  
	const std::string currentDateTime() { //function to print the system time
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);

    return buf;
}

	
      void writer(int wid)							// test CS function into which all threads are sent
      {
          time_t reqtime,enttime,extime,cstime,remtime;
          
          for(int i=0;i<kw;i++)
          {		time(&reqtime);
          		//cout<< l++<<endl;
          		arr1[wid]<<i<<"th cs request by ::writer:: thread "<<wid<<" at"<<currentDateTime()<<endl;
          		sem_wait(&rwm);
          		//entry section 
          		time(&enttime);
          		avg1[wid] = avg1[wid]+(double)(enttime-reqtime);
          		arr1[wid]<<i<<"th cs entry by ::writer:: thread"<<wid<<" at"<<currentDateTime()<<endl;
          			srand(csSeed);
          			cstime=rand();
          			this_thread::sleep_for(chrono::milliseconds(cstime%10));		//execution of critical section.
          			
          					//exit section
                sem_post(&rwm);
          			time(&extime);
          			
          			arr1[wid]<<i<<"th cs exit by ::writer:: thread "<<wid<<" at"<<currentDateTime()<<endl;
          			srand(remSeed);
          			remtime= rand();
          			this_thread::sleep_for(chrono::milliseconds(remtime%10));		//remainder section
          		
          }
       }
          	
      

      void reader(int rid)              // test CS function into which all threads are sent
      {
          time_t reqtime,enttime,extime,cstime,remtime;
          cout<<l++<<endl;
          for(int i=0;i<kr;i++)
          {   time(&reqtime);
              
              arr2[rid]<<i<<"th cs request by ;;reader;; thread "<<rid<<" at"<<currentDateTime()<<endl;
              
               //entry section 
              sem_wait(&m);
              read_count++;
              if (read_count ==1) sem_wait(&rwm);
              sem_post(&m);

              time(&enttime);
              avg2[rid] = avg2[rid]+(double)(enttime-reqtime);
             arr2[rid]<<i<<"th cs entry by ;;reader;; thread "<<rid<<" at"<<currentDateTime()<<endl;
                srand(csSeed);
                cstime=rand();
                this_thread::sleep_for(chrono::milliseconds(cstime%10));    //execution of critical section.
                
                       //exit section
                sem_wait(&m);
                read_count--;
                if (read_count ==0) sem_post(&rwm);
                sem_post(&m);
                time(&extime);
                
                arr2[rid]<<i<<"th cs exit by ;;reader;; thread "<<rid<<" at"<<currentDateTime()<<endl;
                srand(remSeed);
                remtime= rand();
                this_thread::sleep_for(chrono::milliseconds(remtime%10));   //remainder section
              
          }
       }    	
 
 
 
     int main() 
     {
     		
     		fp.open("RW-log.txt");
     		f.open("avgerage_time.txt");
     		ip.open("inp-params.txt");
     		ip>>nw;
        ip>>nr;
     		arr1 =new ostringstream [nw];
        	std::thread t1[nw];
        arr2 =new ostringstream [nr];
          std::thread t2[nr];
 			ip>> kw;
      ip>> kr;
 			ip>>csSeed;
 			ip>>remSeed;
 			avg1 = new double[nw];
      avg2 = new double[nr];
      sem_init(&m,1,1);           //initialising semaphores
      sem_init(&rwm,1,1);
        												 //Launch a group of threads and sending them to a function
         	for (int i = 0; i < nw; ++i)
          	{
             	t1[i] = thread(writer, i);

          	}
 
         //	cout << "writers Launched from the main\n";
            for (int i = 0; i < nr; ++i)
            {
              t2[i] = thread(reader, i);
            }

            //  cout << "readers Launched from the main\n";
 
         													 //Join the threads with the main thread
        	 for (int i = 0; i < nw; ++i)
        	 {
           	 	 t1[i].join();
         	 }


           for (int i = 0; i < nr; ++i)
           {
               t2[i].join();
           }


         	 for(int i=0;i<nw; i++)
        		{
          		fp<<arr1[i].str();
          	}
            for(int i=0;i<nr; i++)
            {
              fp<<arr2[i].str();
            }
           
          	 double sum1 =0;										//finding the average waiting time for all threads and each thread.
          	 for(int i=0;i<nw;i++)
         	 {
         	 	sum1 = sum1+avg1[i];
          		f<<"average waiting time for::writer::"<< i << "is" << avg1[i]/(kw) <<endl;
         	 }
 				 f<<"avereage waiting time for :::writers::: is"<<(sum1/(kw*nw))<<endl;
          double sum2 = 0;
            for(int i=0;i<nr;i++)
           {
            sum2 = sum2+avg2[i];
              f<<"average waiting time for ;;reader;;"<< i << "is" <<(double) (avg2[i]/(kr)) <<endl;
             }
       f<<"avereage waiting time for ;;;readers;;; is"<<(double)(sum2/(kr*nr))<<endl;
       sem_destroy(&m);
       sem_destroy(&rwm); //destroying semaphores
        	 return 0;
    }

