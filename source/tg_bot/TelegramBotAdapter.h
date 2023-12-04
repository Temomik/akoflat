#pragma once

#include <memory>
#include <mutex>
#include <optional>
#include <unordered_map>
#include <utility>

#include "tgbot/types/InputMedia.h"

#include "IThreadPool.h"
#include "IWebFlatAdapter.h"
#include "TelegramCommandType.h"

class BotStorage;

namespace TgBot
{
    class Message;
    class Bot;
    class ReplyKeyboardMarkup;
}

namespace Telegram
{
    class FlatDto;

    class BotAdapter
    {
    public:
        class UserInfo
        {
        public:
            std::mutex mutex;
            bool forceStop = false;
        };

        typedef std::unordered_map<size_t, std::shared_ptr<UserInfo>>::iterator KnownIdsIterator;

        BotAdapter(BotStorage& storage, TgBot::Bot& tgBot, IThreadPool& threadPool, IThreadPool& delayedThreadPool);
        virtual ~BotAdapter();

        bool Initialize();

        void HandleWebAbbResponse(const std::shared_ptr<TgBot::Message> message);
        void HandleResponse(const std::shared_ptr<TgBot::Message> webAppData);
        void HandleCommand(const std::shared_ptr<TgBot::Message> message, Command type);

    private:
        void Process(const size_t id, std::shared_ptr<UserInfo> info);
        void DelayedProcess(const size_t id, std::shared_ptr<UserInfo> info);

        void SendFlat(const Telegram::FlatDto& flat, const size_t id);

        void RequestForceStopUserProcess(const size_t id);

        bool StopProcessing(const size_t id, const std::string& msg = "");
        std::optional<KnownIdsIterator> GetKnownIdsInfo(const size_t id);

        bool SendMessage(const size_t id, const std::string& message);
        bool SendMediaGroup(const size_t id, const std::vector<TgBot::InputMedia::Ptr>& media);

        bool HandleErrors(std::function<void()> func, const size_t id);

        std::vector<std::shared_ptr<IWebFlatAdapter>> GetWebAdapters() const;

        TgBot::Bot& mTgBot;
        BotStorage& mStorage;
        std::shared_ptr<TgBot::ReplyKeyboardMarkup> mKeyboard;
        IThreadPool& mThreadPool;
        IThreadPool& mDelayedThreadPool;

        std::unordered_map<size_t, std::shared_ptr<UserInfo>> mKnownIdsInfo;

        std::mutex mMutex;
    };
}
