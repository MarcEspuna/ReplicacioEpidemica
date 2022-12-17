#include "Commons.h"
#include "CoreNode.h"
#include "LayerOneNode.h"
#include "efsw.h"

int main(int argc, char** argv) {


    Log::StartLogging();
    Log::CreateLogger("CORE");
    Socket::Init();

    CoreNode A1(8888);
    CoreNode A2(8889);
    CoreNode A3(8890);
    
    A1.Connect({8889, 8890});
    A2.Connect({8888, 8890});
    A3.Connect({8888, 8889});

    A1.Start();
    A2.Start();
    A3.Start();

    std::cin.get();

    A1.Shutdown();
    A2.Shutdown();
    A3.Shutdown();
    A1.Wait();
    A2.Wait();
    A3.Wait();
    Socket::Finit();
    Log::EndLogging();
    return 0;
}
