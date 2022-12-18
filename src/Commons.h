#pragma once


#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <WinSock2.h>
#include <Windows.h>
#include <functional>
#include <array>
#include <cassert>
#include <future>
#include <queue>
#include <sstream>
#include <chrono>

#define assertm(exp, msg) assert(((void)msg, exp))

#define TRANSACTION_DIRECTORY           "../../ClientTransactions"