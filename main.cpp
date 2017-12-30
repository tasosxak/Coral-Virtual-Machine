/*

Author : Tasos Temperekidis
Program : Coral VM
Version: 1 (2017)

*/

#include "CVM.h"
using namespace std;


int main()
{

    CVM* cvm = new CVM("arrayint.txt");

    cvm->start();

    return 0;
}
