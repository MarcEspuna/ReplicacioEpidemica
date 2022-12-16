#include <iostream>
#include "CoreNode.h"
#include "LayerOneNode.h"

static void Test()
{
    std::cout << "This is a thread test.\n";
}


int main(int argc, char** argv) {
    /**
     * @brief 
     * 
     * CoreNode A1({<ports to connect to>});
     * CoreNode A2({<...>});
     * CoreNode A3({<...>});
     * 
     * LayerOneNode B1({});
     * LayerOneNode B2({});
     * 
     * LayerTwoNode C1({});
     * LayerTwoNode C2({});
     */
    
    CoreNode A1(9000, {8888, 8889}, {39,59});
    A1.Start();

    std::cin.get();

    A1.Shutdown();
    A1.Wait();
    return 0;
}
