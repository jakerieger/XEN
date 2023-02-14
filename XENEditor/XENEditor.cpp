#include "CoreDefines.h"
#include "CoreMinimal.h"
#include <cstdio>

XEN_USE_CORE_NAMESPACE

int main(int argc, char* argv[]) {
    GLogger::Log(ELoggerClass::LOG_ENGINE, "Test log");
    GLogger::CollectLogs(R"(C:\Users\Jake\Desktop)");
    
    return 0;
}
