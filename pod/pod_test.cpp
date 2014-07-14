#include <cstring>

struct pod_type 
{
   int a;
   int b;

   virtual void reset()
   {
       memset(this, 0, sizeof(pod_type) );
   }
};



int main()
{
    struct pod_type aPod;

    aPod.a = 0xdead;
    aPod.b = 0xbeef;
    int pod_size = sizeof(pod_type);
    struct pod_type *pPod  = &aPod;
    pPod->reset();
    return 0;
}
