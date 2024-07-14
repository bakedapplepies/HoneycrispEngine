#include "ImageSerializer.h"


HNCRSP_NAMESPACE_START

ImageSerializer::ImageSerializer(int components_per_pixel) :
    m_componentsPerPixel(components_per_pixel)
{}

void ImageSerializer::AddImage(
    const uint8_t* image_data,
    int width,
    int height,
    const FileSystem::Path& path_to_image
) {
    auto serialized_image_data = m_builder.CreateVector(image_data, width * height * m_componentsPerPixel);
    auto serialized_image = Serialized::CreateImage(
        m_builder,
        serialized_image_data,
        width,
        height
    );
    m_builder.Finish(serialized_image);

    // Get hash from path
    FileSystem::Path serialized_dir(fmt::format("build/{}/src/serialized/images", HNCRSP_BUILD_TYPE));
    int64_t path_hash = std::hash<std::string>{}(path_to_image.string());
    std::filesystem::create_directories(serialized_dir.string());

    // Get path to store serialized data
    std::string serialized_image_path = fmt::format("{}/{}.hncimg", serialized_dir.string(), path_hash);

    // Store buffer
    void* buffer = m_builder.GetBufferPointer();
    size_t buffer_size = m_builder.GetSize();

    std::ofstream outFile(serialized_image_path, std::ios::out | std::ios::binary | std::ios::trunc);
    outFile.write(reinterpret_cast<char*>(buffer), buffer_size);
    outFile.close();
}

Serialized::Image* ImageSerializer::GetDeserializedObject(const FileSystem::Path& path_to_image)
{
    // Get hash from path
    int64_t path_hash = std::hash<std::string>{}(path_to_image.string());
    FileSystem::Path serialized_image_path(fmt::format("build/{}/src/serialized/images/{}.hncimg", HNCRSP_BUILD_TYPE, path_hash));
    if(!std::filesystem::exists(serialized_image_path.string()))
    {
        return nullptr;
    }
    
    // Read data
    std::ifstream inFile(serialized_image_path.string(), std::ios::in | std::ios::binary);
    inFile.seekg(0, std::ios::end);
    unsigned int length = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    m_serializedData = std::make_unique<char[]>(length);
    inFile.read(m_serializedData.get(), length);
    inFile.close();

    return Serialized::GetMutableImage(m_serializedData.get());
}

HNCRSP_NAMESPACE_END