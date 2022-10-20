#include "gtest/gtest.h"

#include "core/Resource.hpp"

namespace ResourcePathTest
{
    TEST(ResourcePath, DefaultContructor)
    {
        ResourcePath resourcePath;
        EXPECT_FALSE(resourcePath.isValid());
    }

    TEST(ResourcePath, StringViewConstructor)
    {
        ResourcePath resourcePath("data://path1/path2");
        EXPECT_TRUE(resourcePath.isValid());
    }

    TEST(ResourcePath, InvalidRelativePath)
    {
        ResourcePath resourcePath("/relative/path");
        EXPECT_FALSE(resourcePath.isValid());
    }

    TEST(ResourcePath, InvalidPrefix)
    {
        ResourcePath resourcePath("notdata://relative/path");
        EXPECT_FALSE(resourcePath.isValid());
    }

    TEST(ResourcePath, Hash)
    {
        ResourcePath resourcePath("data://relative/path/to/file.jpeg");
        EXPECT_TRUE(resourcePath.isValid());
        EXPECT_EQ(resourcePath.hash(), 3872976306u);  // pre-computed hash
    }

    TEST(ResourcePath, Path)
    {
        const eastl::string path = "data://this/is/a/path";
        ResourcePath resourcePath(path);
        EXPECT_TRUE(resourcePath.isValid());
        EXPECT_EQ(resourcePath.path(), path);
    }

	TEST(ResourcePath, FindExtensionStart)
    {
        const eastl::string path1 = "data://this/is/a/file.jpg";
	    const auto it1 = ResourcePath::findExtensionStart(path1);
        EXPECT_EQ(it1, path1.cend() - 4);


        const eastl::string path2 = "data://this/is/a/file";
        auto it2 = ResourcePath::findExtensionStart(path2);
        EXPECT_EQ(it2, path2.cend());

		const eastl::string path3 = "";
        auto it3 = ResourcePath::findExtensionStart(path3);
        EXPECT_EQ(it3, path3.cend());

		const eastl::string path4 = "data://file.jpg";
        const auto it4 = ResourcePath::findExtensionStart(path4);
        EXPECT_EQ(it4, path4.cend() - 4);
    }

	TEST(ResourcePath, GetExtension) {
        const ResourcePath resourcePath1("data://this/is/a/file.jpg");
        EXPECT_EQ(resourcePath1.getExtension(), "jpg");

		const ResourcePath resourcePath2("data://this/is/a/file");
        EXPECT_EQ(resourcePath2.getExtension(), "");
        
		const ResourcePath resourcePath3("data://this/is/a/file.");
        EXPECT_EQ(resourcePath3.getExtension(), "");
        
		const ResourcePath resourcePath4("");
        EXPECT_EQ(resourcePath4.getExtension(), "");
    }

}  // namespace ResourcePathTest

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
            EXPECT_EQ(strcmp(type, KAllResources[i].Type), 0);
        }
    }
}  // namespace ResourceTypeTest