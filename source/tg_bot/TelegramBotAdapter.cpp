#include "TelegramBotAdapter.h"

#include <tgbot/tgbot.h>
#include <tgbot/types/InputMediaPhoto.h>

#include "BotStorage.h"

#include "Logger.h"
#include "OnlinerWebAdapter.h"
#include "SaveUserConfigWebCommand.h"
#include "TgCommandDtoSerializator.h"

typedef TgBot::TgException::ErrorCode TgErrorCode;

using Telegram::Web::CommandType;
using Telegram::Web::SaveUserConfigWebCommand;
using TgBot::InputMedia;
using TgBot::InputMediaPhoto;
using TgBot::KeyboardButton;
using TgBot::ReplyKeyboardMarkup;
using TgBot::WebAppInfo;

using namespace std;

Telegram::BotAdapter::BotAdapter(BotStorage& storage, TgBot::Bot& tgBot, IThreadPool& threadPool, IThreadPool& delayedThreadPool)
    : mTgBot(tgBot), mStorage(storage)
    , mKeyboard(new ReplyKeyboardMarkup())
    , mThreadPool(threadPool)
    , mDelayedThreadPool(delayedThreadPool)
{
    vector<KeyboardButton::Ptr> row;
    KeyboardButton::Ptr button(new KeyboardButton);
    WebAppInfo::Ptr webApp(new WebAppInfo);
    webApp->url = "https://pi.temomik.keenetic.pro/settings.html";
    button->webApp = webApp;
    button->text = "Menu";
    row.push_back(button);
    mKeyboard->keyboard.push_back(row);
}

Telegram::BotAdapter::~BotAdapter()
{
}

bool Telegram::BotAdapter::Initialize()
{
    auto ids = mStorage.GetIds();

    for (auto& id : ids)
    {
        auto digitId = static_cast<size_t>(stoul(id));
        auto info = make_shared<UserInfo>();
        mKnownIdsInfo.insert({digitId, info});

        DelayedProcess(digitId, info);
    }

    return true;
}

void Telegram::BotAdapter::HandleWebAbbResponse(const shared_ptr<TgBot::Message> message)
{
    scoped_lock lock(mMutex);

    Telegram::CommandDto commandDto;

    Serializator<Telegram::CommandDto>::Deserialize(commandDto, message->webAppData->data);
    size_t id = message->chat->id;

    switch (commandDto.Command)
    {
    case CommandType::SetUserConfig:
    {
        RequestForceStopUserProcess(id);

        SaveUserConfigWebCommand command(mStorage);
        auto id = to_string(message->chat->id);
        command.Process(id, commandDto.Value);

        break;
    }
    case CommandType::Stop:
    {
        RequestForceStopUserProcess(id);
        break;
    }
    case CommandType::Start:
    {
        auto idsInfo = GetKnownIdsInfo(id);

        if (!idsInfo.has_value())
        {
            auto info = make_shared<UserInfo>();
            mKnownIdsInfo.insert({id, info});

            DelayedProcess(id, info);
        }
        break;
    }
    case CommandType::None:
    {
        SendMessage(message->chat->id, "error");
        break;
    }
    }
}

void Telegram::BotAdapter::HandleResponse(const shared_ptr<TgBot::Message> webAppData)
{
    scoped_lock lock(mMutex);
}

void Telegram::BotAdapter::HandleCommand(const shared_ptr<TgBot::Message> message, Command type)
{
    scoped_lock lock(mMutex);
    // auto id = message->chat->id;

    switch (type)
    {
    default:
        break;
    case Command::Start:
        break;
    }
}

void Telegram::BotAdapter::Process(const size_t id, shared_ptr<UserInfo> info)
{
    scoped_lock lock(info->mutex);

    auto stringId = to_string(id);
    auto config = mStorage.GetUserConfig(stringId);

    if (!config.has_value())
    {
        SendMessage(id, "Please update your config !!!");
        StopProcessing(id, "Invalid config");
        return;
    }

    auto webAdapters = GetWebAdapters();
    bool isFlatsFound = false;

    for (auto adapter : webAdapters)
    {
        auto flats = adapter->GetFlats(config.value());
        isFlatsFound |= !flats.empty();

        for (auto& flat : flats)
        {
            if (info->forceStop)
            {
                StopProcessing(id, "Force stop");

                return;
            }

            if (mStorage.IsNewFlatId(to_string(id), flat.id))
            {
                SendFlat(flat, id);
            }
        }
    }

    if (!isFlatsFound)
    {
        SendMessage(id, "Can't find any flats for you. Please update config !!!");
        StopProcessing(id, "Invalid config");
        return;
    }

    if (info.use_count() > 1)
    {
        mDelayedThreadPool.AddCommand([this, id, info]
                                      { DelayedProcess(id, info); });
    }
}

void Telegram::BotAdapter::DelayedProcess(const size_t id, shared_ptr<UserInfo> info)
{
    if (info.use_count() > 1)
    {
        mThreadPool.AddCommand([this, id, info]
                               { Process(id, info); });
    }
}

void Telegram::BotAdapter::SendFlat(const Telegram::FlatDto& flat, const size_t id)
{
    auto mediaGroup = make_shared<InputMediaPhoto>();
    auto str = flat.Images[0];
    mediaGroup->media = str;
    mediaGroup->caption = flat.Price + "\n" + flat.Address + "\n" + flat.Info + "\n" + flat.Link;
    mediaGroup->hasSpoiler = false;

    vector<InputMedia::Ptr> mediaGroups;
    mediaGroups.push_back(mediaGroup);

    if (SendMediaGroup(id, mediaGroups))
    {
        mStorage.SaveShownFlatId(to_string(id), flat.id);
    }
}

void Telegram::BotAdapter::RequestForceStopUserProcess(const size_t id)
{
    auto idsInfo = GetKnownIdsInfo(id);

    if (idsInfo.has_value())
    {
        idsInfo.value()->second->forceStop = true;
    }
}

bool Telegram::BotAdapter::StopProcessing(const size_t id, const string& msg)
{
    auto idsInfo = GetKnownIdsInfo(id);

    if (idsInfo.has_value())
    {
        idsInfo.value()->second->forceStop = true;

        mKnownIdsInfo.erase(idsInfo.value());
        LOG_INFO("processing is stopped for - " + to_string(id) + "; Reason " + msg);

        return true;
    }

    return false;
}

optional<Telegram::BotAdapter::KnownIdsIterator> Telegram::BotAdapter::GetKnownIdsInfo(const size_t id)
{
    auto it = find_if(mKnownIdsInfo.begin(), mKnownIdsInfo.end(),
                      [id](pair<size_t, shared_ptr<UserInfo>> pair)
                      { return pair.first == id; });

    if (it != mKnownIdsInfo.end())
    {
        return it;
    }

    return {};
}

bool Telegram::BotAdapter::SendMessage(const size_t id, const std::string& message)
{
    auto func = [id, &message, this]
    {
        mTgBot.getApi().sendMessage(id, message);
    };

    return HandleErrors(func, id);
}

bool Telegram::BotAdapter::SendMediaGroup(const size_t id, const std::vector<InputMedia::Ptr>& media)
{
    auto func = [id, &media, this]
    {
        mTgBot.getApi().sendMediaGroup(id, media, true);
    };

    return HandleErrors(func, id);
}

bool Telegram::BotAdapter::HandleErrors(std::function<void()> func, const size_t id)
{
    auto idStr = to_string(id);

    try
    {
        func();
    }
    catch (TgBot::TgException& exception)
    {
        switch (exception.errorCode)
        {
        case TgErrorCode::Forbidden:
        case TgErrorCode::NotFound:
        case TgErrorCode::Unauthorized:
            mStorage.RemoveId(idStr);
            StopProcessing(id, "Forbidden");
            LOG_INFO(idStr + " removed from process");
            return false;

        case TgErrorCode::Flood:
            LOG_INFO(idStr + " received flood");
            // TODO add wait with timeout
            return false;

        case TgErrorCode::Internal:
        case TgErrorCode::HtmlResponse:
        case TgErrorCode::BadRequest:
        case TgErrorCode::InvalidJson:
            // TODO handle these errors
            LOG_INFO(idStr + " known errors scope");
            return false;

        case TgErrorCode::Undefined:
        default:
            auto codeStr = to_string(static_cast<size_t>(exception.errorCode));
            LOG_ERROR("Undefined error: " + codeStr + "; message: " + exception.what());
            return false;
        }
    }

    return true;
}

std::vector<std::shared_ptr<IWebFlatAdapter>> Telegram::BotAdapter::GetWebAdapters() const
{
    auto adapters = std::vector<std::shared_ptr<IWebFlatAdapter>>();

    adapters.push_back(std::make_shared<Onliner::WebAdapter>());
    // adapters.push_back(std::make_shared<Kufar::WebAdapter>());
    // adapters.push_back(std::make_shared<Realt::WebAdapter>());

    return adapters;
}
