#include "Application.h"
#include "utils/Debug.h"


int main()
{
    Application::Get().Run();
    Application::DeleteInstance();

    return 0;
}