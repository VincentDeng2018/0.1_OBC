#include <stdio.h>
#include "CanOpenBasic.h"

extern  void TestOdReadWrite(void);

int main(void)
{
    TestOdReadWrite();
    while(1);
}