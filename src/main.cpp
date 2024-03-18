#include "Application.h"


using namespace Honeycrisp;

int main()
{
    HNCRSP_LOG_INFO(std::this_thread::get_id());
    Application::Run();

    return 0;
}