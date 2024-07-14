#pragma once

#include "src/pch/hncrsp_pch.h"
#include "vendor/flatbuffers/flatbuffers.h"


HNCRSP_NAMESPACE_START

template <typename T>
class Serializer
{
public:
    Serializer() = default;
    Serializer(const Serializer&) = delete;
    Serializer& operator=(const Serializer&) = delete;
    Serializer(Serializer&&) = delete;
    Serializer& operator=(Serializer&&) = delete;
    virtual ~Serializer() = default;

    virtual const T* GetDeserializedObject(const FileSystem::Path& path_to_serialized_obj) = 0;
};

HNCRSP_NAMESPACE_END