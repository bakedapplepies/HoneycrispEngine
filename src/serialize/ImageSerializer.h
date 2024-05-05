#pragma once

#include "src/pch/pch.h"
#include "src/serialize/Serializer.h"
#include "src/serialize/schemas/image_generated.h"


HNCRSP_NAMESPACE_START

class ImageSerializer : public Serializer<Serialized::Image>
{
private:
    flatbuffers::FlatBufferBuilder m_builder;
    int m_components_per_pixel;

    std::unique_ptr<char[]> m_serialized_data;

public:
    ImageSerializer(int components_per_pixel);
    ~ImageSerializer() = default;

    void AddImage(
        const uint8_t* image_data,
        int width,
        int height,
        const FileSystem::Path& path_to_image
    );
    Serialized::Image* GetDeserializedObject(const FileSystem::Path& path_to_image) override;
};

HNCRSP_NAMESPACE_END