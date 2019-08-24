#include <iostream>
#include <chrono>
#include <array>
#include <vector>

#include <math.h>

#include <cassert>
#include <pthread.h>
#include <unistd.h>


using namespace std;
using namespace chrono;

enum ProfType {k_init, k_alloc, k_sched, k_run, k_other};
class TimingReference {
 public:
  using D = duration<int,nano>;
  vector<string> names {"Init", "Alloc", "Sched", "Run", "Other"};
  ProfType cur;
  array<long long unsigned, 5> times;
  array<long long unsigned, 5> times_sq;
  time_point<high_resolution_clock> last;
  int recs {0};

  TimingReference() {
    for (int i=0; i<times.size(); i++) {
      times[i] = 0;
      times_sq[i] = 0;
    }
    ReInit(k_other);
  }

  void ReInit(ProfType t) {
    cur = t;
    last = high_resolution_clock::now();
  }

  void StartNew(ProfType t) {
    _fin();
    cur = t;
  }

  void Finish() {
    _fin();
    for (int i=0; i<times.size(); i++) {
      times_sq[i] = times[i]*times[i];
    }
  }

  void Add(TimingReference &t) {
    for (int i=0; i<times.size(); i++) {
      times[i] += t.times[i];
      t.times[i] = 0;
      times_sq[i] += t.times_sq[i];
      t.times_sq[i] = 0;
    }
    recs += t.recs;
  }

  void Print() {
    cout << "Captured profiling data from " << recs << " traces" << endl;
    printf("%16s  %16s        %16s\n", "", "mean", "Pop. SD");
    for (int i=0; i<times.size(); i++) {
      int sd = sqrt((times_sq[i]-times[i]*times[i]/recs)/recs);
      printf("%16s: %16d ns +/- %16d ns\n", names[i].c_str(), times[i]/recs, sd);
    }
  }

 private:
  void _fin() {
    auto next = high_resolution_clock::now();
    //D elapsed = duration_cast(next - last);
    D elapsed = next - last;
    times[cur] += elapsed.count();
    last = next;
    assert(recs == 0 || recs == 1);
    recs = 1;
  }
};

void *threadfunc(void *p) {
  cout << "Hello from thread: " << *(int*)p << endl;
  delete (int*)p;
  return NULL;
}

int main(int argc, char **argv) {
  cout << "Clock duration " << endl;

  TimingReference a;
  for (int i=0; i<1000; i++) {
    TimingReference r;
    r.ReInit(k_alloc);
    vector<pthread_t> threads;
    for (int i=0; i<10; i++) {
      int *p = new int;
      *p = i;
      threads.emplace_back();
      int tmp = pthread_create(&threads.back(), NULL, threadfunc, (void*)p);
    }
    r.StartNew(k_run);
    for (auto &t : threads) {
      pthread_join(t, NULL);
    }
    r.StartNew(k_other);
    cout << "All threads joined" << endl;
    r.Finish();
    a.Add(r);
  }
  a.Print();
}

