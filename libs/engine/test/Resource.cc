#include "gtest/gtest.h"

#include "core/Resource.hpp"

namespace ResourceTypeTest
{
    struct EncodedResource {
        char Type[ResourceType::KMaxSize + 1];
        uint32_t Hash;
    };

    constexpr EncodedResource K4LetterType = {"jpeg", 1785750887};
    constexpr EncodedResource K3LetterType = {"png", 7368295};
    constexpr EncodedResource K2LetterType = {"so", 29551};
    constexpr EncodedResource K1LetterType = {"a", 97};
    
    constexpr EncodedResource KAllResources[4] = {K1LetterType, K2LetterType, K3LetterType, K4LetterType};
    constexpr int KAllResourcesLength = (sizeof(KAllResources) / sizeof(KAllResources[0]));

    TEST(ResourceType, DefaultConstructor)
    {
        ResourcePath resourcePath;
        EXPECT_FALSE(resourcePath.isValid());
    }

    TEST(ResourceType, StringViewContructor)
    {
        ResourceType resourceType1(K4LetterType.Type);
        EXPECT_TRUE(resourceType1.isValid());
    }

    TEST(ResourceType, Hash)
    {
        for (int i = 0; i < KAllResourcesLength; ++i) {
            ResourceType resourceType(KAllResources[i].Type);
            EXPECT_EQ(resourceType.hash(), KAllResources[i].Hash);
        }
    }

    TEST(ResourceType, Decode)
    {
        for (int i = 0; i < KAllResourcesLength; ++i) {
            ResourceType resourceType(KAllResources[i].Type);
            char type[5];
            resourceType.decode(type);
            EXPECT_EQ(strcmp( type, KAllResources[i].Type), 0);
        }
    }
}  // namespace ResourceTypeTest