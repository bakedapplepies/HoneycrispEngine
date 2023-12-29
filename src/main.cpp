#include "Application.h"


int main()
{
    Application::Get().Run();
    Application::DeleteInstance();

    return 0;
}