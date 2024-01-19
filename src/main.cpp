#include "Application.h"


using namespace Honeycrisp;

int main()
{
    Application::Get().Run();
    Application::DeleteInstance();

    return 0;
}