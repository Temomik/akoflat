#include <gtest/gtest.h>

#include "Thread.h"
#include "CommandsDeque.h"


TEST(ThreadTest, isThreadWorking)
{
    size_t i = 0;
    std::mutex mutex;

    CommandsDeque commands;
    Thread thread(commands);
    Thread thread1(commands);
    Thread thread2(commands);
    thread.Start();
    thread1.Start();
    thread2.Start();

    commands.Push([&i]{
         i = 10;
         });

    commands.Push([&i]{
         i = 11;
         });

    commands.Push([&i]{
         i = 1;
         });


    thread.Join();
    thread1.Join();
    thread2.Join();

    EXPECT_EQ(i, 1);
}
