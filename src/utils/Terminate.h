#pragma once


#define HNCRSP_TERMINATE(msg) Honeycrisp::terminate(msg, __FILE__, __LINE__);

namespace Honeycrisp
{
    void terminate(const char* msg, const char* file, unsigned int line);
}