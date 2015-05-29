#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <vector>

#include"TaskQueue.h"
#include"LoopScheduler.h"
#include <timer.h>

using namespace std;

const int SLOWDOWN = 2;

const int outer_s = 50;
const int inner_s = 200;

const double PI = acos(-1.0);

//double Load(int outer, int inner) {    
//     return outer*inner_s*2 + inner;
//};

double Load(int outer, int inner) {    
    return (exp(1.0/(outer_s-outer)) * inner_s + exp(1.0/(inner_s-inner))) * 10;
};
// double Load(int outer, int inner) {    
//     return (100*sin(2 * PI*outer/outer_s) + sin(2*PI*inner/inner_s)) * 10;
// };


SimpleLock dumpLock;

vector<vector<double> > data;

class InnerLoopTest:public TaskQueue
{
    const int outer;

    LoopScheduler* sched;

public:
  
    InnerLoopTest(int oi,  LoopScheduler* asched):
	outer(oi),
	sched(asched)
    {
    };

    virtual ~InnerLoopTest() {
    }



    virtual void run_task(const int threadID, const int threadNum)
        {
            int chunkInd = -1;
            while((chunkInd = sched->get_task(chunkInd)) >= 0) {
		int from = sched->get_chunkStart(chunkInd);
		int to= sched->get_chunkEnd(chunkInd);
#ifdef DEBUGFLG
		dumpLock.lock();
		assert(outer >= 0);
		printf("Thread %d, outer = %d, inner chunk %d, iter: %d, %d\n", threadID, outer, chunkInd, from, to);
		dumpLock.unlock();
#endif
                Integrate(from, to);
            }
        }

    void run() {
        sched->reset_tasks();
        ThreadPool::get_instance()->run(this);
    }

    void Integrate(const int from, const int to) {
        for(int i = from; i <= to; i++) {
	    for(int j = 0; j < Load(outer, i); j++) {
		for(int k = 0; k < SLOWDOWN; k++) {
		    data[outer][i] += sin(Load(outer, i));
		}
	    }
        }
    };

};

class OuterLoopTest:public TaskQueue
{

    LoopScheduler* o_sched;
    LoopScheduler** i_sched1;
    LoopScheduler** i_sched2;

    vector<InnerLoopTest*> iloop1;
    vector<InnerLoopTest*> iloop2;

public:
  
    OuterLoopTest( LoopScheduler* outer_sched, LoopScheduler** inner_sched1, LoopScheduler** inner_sched2):
 	o_sched(outer_sched),
	i_sched1(inner_sched1),
	i_sched2(inner_sched2)
   {
        data.resize(outer_s);
	iloop1.resize(outer_s);
	iloop2.resize(outer_s);

	for(int i = 0; i < outer_s; i++) {
	    data[i].resize(inner_s);
	    data[i].assign(inner_s, 0);
	    
	    iloop1[i] = new InnerLoopTest(i, inner_sched1[i]);
	    iloop2[i] = new InnerLoopTest(i, inner_sched2[i]);
	}
    };



    virtual void run_task(const int threadID, const int threadNum)
        {
            int chunkInd = -1;
            while((chunkInd = o_sched->get_task(chunkInd)) >= 0) {
		int from = o_sched->get_chunkStart(chunkInd);
		int to = o_sched->get_chunkEnd(chunkInd);
#ifdef DEBUGFLG
		dumpLock.lock();
                printf("Thread %d, chunk %d, outer iter %d, %d\n", threadID, chunkInd, from, to);
		dumpLock.unlock();
#endif
                Integrate(from, to);
            }
        }

    void run() {
#ifdef DEBUGFLG
	dumpLock.lock();
	printf("Outer loop will run in parallel\n");
	dumpLock.unlock();
#endif
        o_sched->reset_tasks();
        ThreadPool::get_instance()->run(this);
    }

    void Integrate(const int from, const int to) {

	double d - 0;
	for(int j = 0; j < inner_s; j++) {
	    for(int k = 0; k < Load(from, 0); k++) {
		d += sin(Load(from, 0));
	    }
	}

        for(int i = from; i <= to; i++) {

	    iloop1[i]->run();
	    
	    for(int j = 0; j < inner_s; j++) {
		data[i][j] += 1;
	    }

	    iloop2[i]->run();
        }
    };

};




int main(const int argc, const char* argv[]) {

    int npe = 0;
    int nrun = 0;
    if(argc == 3) {
	npe =  atoi(argv[1]);
	nrun = atoi(argv[2]);
    }
    else {
	printf("Usage: test <npe> <niter>\n");
	npe = 2;
	nrun = 20;
    }

    printf("Initializing the thread pool\n");
    ThreadPool::start(npe);

    LoopScheduler** sched1 = new LoopScheduler*[outer_s];
    LoopScheduler** sched2 = new LoopScheduler*[outer_s];

    #define ISCH INFGLoopScheduler
    #define OSCH INFGLoopScheduler
    //#define OSCH DynamicLoopScheduler
    //#define ISCH DynamicLoopScheduler
	//#define OSCH DynamicLoopScheduler

    LoopScheduler* o_sched = new OSCH(outer_s);
    for(int i = 0; i < outer_s; i++) {
	 sched1[i] =  new ISCH(inner_s);
	 sched2[i] = new ISCH(inner_s);
    }

    OuterLoopTest test(o_sched, sched1, sched2);

    Timer total_time;
      
    for(int i = 0; i < nrun; i++) {
	printf("Running test iteration %d\n", i);
	total_time.start();
	test.run();
	total_time.stop();
	
	printf("Run took %d ms, iter %d\n", total_time.get_milli_last(), i);
    }
  
    printf("Destroying thread pool\n");

    ThreadPool::stop();

    printf("Done\n");
}
