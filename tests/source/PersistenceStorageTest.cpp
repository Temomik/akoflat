#include <filesystem>
#include <gtest/gtest.h>
#include <string>

#include "PersistenceStorage.h"

using std::string;

TEST(PersistenceStorage, SaveToFile_WhenEmpty)
{
    const std::string saveData = "this is \n simple string";
    std::string folder(test_info_->name());
    std::string fileName(test_info_->test_suite_name());

    PersistenceStorage storage("tmp_tests");
    storage.RemoveAll("");

    storage.Save(folder, fileName, saveData);
    const string loadData = storage.Load(folder, fileName);

    EXPECT_EQ(saveData, loadData);
}

TEST(PersistenceStorage, SaveToFile_WhenNotEmpty_ShouldOverrite)
{
    const std::string initialData = "12312312312";
    const std::string saveData = "this is \n simple string";
    std::string folder(test_info_->name());
    std::string fileName(test_info_->test_suite_name());

    PersistenceStorage storage("tmp_tests");
    storage.RemoveAll("");

    storage.Save(folder, fileName, initialData);
    storage.Save(folder, fileName, saveData);
    const string loadData = storage.Load(folder, fileName);

    EXPECT_EQ(saveData, loadData);
}

TEST(PersistenceStorage, AppendToFile_WhenEmpty)
{
    const std::string saveData = "this is \n simple string";
    std::string folder(test_info_->name());
    std::string fileName(test_info_->test_suite_name());

    PersistenceStorage storage("tmp_tests");
    storage.RemoveAll("");

    storage.Append(folder, fileName, saveData);
    const string loadData = storage.Load(folder, fileName);

    EXPECT_EQ(saveData, loadData);
}

TEST(PersistenceStorage, AppendToFile_WhenNotEmpty_ShouldNotBeOverrited)
{
    const std::string initialData = "12312312312";
    const std::string saveData = "this is \n simple string";
    std::string folder(test_info_->name());
    std::string fileName(test_info_->test_suite_name());

    PersistenceStorage storage("tmp_tests");
    storage.RemoveAll("");

    storage.Append(folder, fileName, initialData);
    storage.Append(folder, fileName, saveData);
    const string loadData = storage.Load(folder, fileName);

    EXPECT_EQ(initialData + saveData, loadData);
}

TEST(PersistenceStorage, Load_ShouldLoadAllData)
{
    const std::string initialData = "data for load";
    std::string folder(test_info_->name());
    std::string fileName(test_info_->test_suite_name());

    PersistenceStorage storage("tmp_tests");
    storage.RemoveAll("");

    storage.Save(folder, fileName, initialData);
    const string loadData = storage.Load(folder, fileName);

    EXPECT_EQ(initialData, loadData);
}

TEST(PersistenceStorage, GetLines_ShouldLoadAllLines)
{
    const std::string initialData = "1\n2\n3\n4\n5\n";
    std::string folder(test_info_->name());
    std::string fileName(test_info_->test_suite_name());

    PersistenceStorage storage("tmp_tests");
    storage.RemoveAll("");

    storage.Save(folder, fileName, initialData);
    auto lines = storage.GetLines(folder, fileName);

    EXPECT_EQ(5, lines.size());

    for (size_t i = 1; i < 5; i++)
    {
        EXPECT_EQ(std::stoi(lines[i - 1]), i);
    }
}
