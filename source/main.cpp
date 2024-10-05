#include "jsoncpp/json/json.h"
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>
#include <tgbot/net/HttpServer.h>
#include <tgbot/types/InputMediaPhoto.h>

#include "BotStorage.h"
#include "ThreadPool.h"

#include "TgCommandDtoSerializator.h"

#include <tgbot/tgbot.h>

#include "HtmlRequester.h"
#include "OnlinerHtmlParser.h"
#include "OnlinerUrlBuilder.h"
#include "TelegramCommandDto.h"
#include "UserConfig.h"

#include "DelayedCommandsDeque.h"
#include "OnlinerWebAdapter.h"
#include "TelegramBotAdapter.h"

#include <ctime>

using namespace TgBot;
using std::make_shared;
using std::string;
using std::vector;

void createOneColumnKeyboard(const vector<string>& buttonStrings, ReplyKeyboardMarkup::Ptr& kb)
{
    for (size_t i = 0; i < buttonStrings.size(); ++i)
    {
        vector<KeyboardButton::Ptr> row;
        KeyboardButton::Ptr button(new KeyboardButton);
        button->text = buttonStrings[i];
        row.push_back(button);
        kb->keyboard.push_back(row);
    }
}

void createOneColumnWebAppKeyboard(const vector<string>& buttonStrings, const string& url, ReplyKeyboardMarkup::Ptr& kb)
{
    for (size_t i = 0; i < buttonStrings.size(); ++i)
    {
        vector<KeyboardButton::Ptr> row;
        KeyboardButton::Ptr button(new KeyboardButton);
        WebAppInfo::Ptr webApp(new WebAppInfo);
        webApp->url = url;
        button->webApp = webApp;
        button->text = buttonStrings[i];
        row.push_back(button);
        kb->keyboard.push_back(row);
    }
}

void createKeyboard(const vector<vector<string>>& buttonLayout, ReplyKeyboardMarkup::Ptr& kb)
{
    for (size_t i = 0; i < buttonLayout.size(); ++i)
    {
        vector<KeyboardButton::Ptr> row;
        for (size_t j = 0; j < buttonLayout[i].size(); ++j)
        {
            KeyboardButton::Ptr button(new KeyboardButton);
            button->text = buttonLayout[i][j];
            row.push_back(button);
        }
        kb->keyboard.push_back(row);
    }
}

int main()
{
    string token(getenv("TOKEN"));

    BotStorage storage("bot_storage");
    Bot bot(token);
    ThreadPool<CommandsDeque> threadPool;
    ThreadPool<DelayedCommandsDeque> delayedThreadPool(1);

    Onliner::WebAdapter onlinerAdapter;
    threadPool.Initialize();
    delayedThreadPool.Initialize();
    storage.Init();

    Telegram::BotAdapter botAdapter(storage, bot, threadPool, delayedThreadPool);
    botAdapter.Initialize();

    printf("Token: %s\n", token.c_str());

    // Thanks Pietro Falessi for code
    // InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);

    ReplyKeyboardMarkup::Ptr keyboardOneCol(new ReplyKeyboardMarkup);
    vector<KeyboardButton::Ptr> row;
    KeyboardButton::Ptr button(new KeyboardButton);
    WebAppInfo::Ptr webApp(new WebAppInfo);
    webApp->url = "https://pi.temomik.keenetic.pro/settings.html";

    button->webApp = webApp;
    button->text = "Menu";
    button->requestContact = false;
    button->requestLocation = false;
    row.push_back(button);
    keyboardOneCol->keyboard.push_back(row);

    ReplyKeyboardRemove::Ptr removeKeyboard(new ReplyKeyboardRemove);

    bot.getEvents().onCommand("start", [&](Message::Ptr message)
                              {
                                  botAdapter.HandleCommand(message, Telegram::Command::Start);
                                  // bot.getApi().sendMessage(message->chat->id, "Hi!", false, 0, keyboard);
                              });

    bot.getEvents().onCommand("rm", [&](Message::Ptr message)
                              { bot.getApi().sendMessage(message->chat->id, "removed", false, 0, removeKeyboard); });

    bot.getEvents().onCommand("kb", [&](Message::Ptr message)
                              { bot.getApi().sendMessage(message->chat->id, "added", false, 0, keyboardOneCol); });

    // bot.getEvents().onCommand("thread", [&](Message::Ptr message)
    //                           { 
    //                             for (int i = 0; i < 20; i ++)
    //                             {
    //                                 threadPool.AddCommand([&bot, message]{bot.getApi().sendMessage(message->chat->id, "thread"); });
    //                             } });

    // bot.getEvents().onCommand("flats", [&](Message::Ptr message)
    //                           {
        
    //     auto mediaGroup = std::make_shared<InputMediaPhoto>();

    //     Telegram::User::Config config;

    //     config.City = "Minsk";
    //     config.Price.first = 100;
    //     config.Price.second = 320;
    //     config.FloorCount = {2, 3, 4};

    //     auto flats = onlinerAdapter.GetFlats(config);

    //     for (size_t i = 0; i < flats.size() && i < 20; i++)
    //     {
    //         auto& flatDto = flats[i];

    //         auto str = flatDto.Images[0];
    //         mediaGroup->media = str;
    //         mediaGroup->caption = flatDto.Price + "\n" + flatDto.Address + "\n" + flatDto.Info + "\n" + flatDto.Link;

    //         std::vector<InputMedia::Ptr> mediaGroups;
    //         mediaGroups.push_back(mediaGroup);

    //         try 
    //         {
    //             bot.getApi().sendMediaGroup(message->chat->id, mediaGroups);

    //         } catch(std::exception& e)
    //         {
    //             std::cout << e.what() << std::endl;
    //             std::cout << mediaGroup->media << std::endl;
    //             std::cout << mediaGroup->caption << std::endl;
    //         } 
    //     } });

    bot.getEvents().onAnyMessage([&botAdapter](Message::Ptr message)
                                 {
                                     printf("User wrote %s\n", message->text.c_str());
                                     if (StringTools::startsWith(message->text, "/start"))
                                     {
                                         return;
                                     }
                                     if (message->webAppData)
                                     {
                                            botAdapter.HandleWebAbbResponse(message);
                                     } });

    // signal(SIGINT, [](int s) {
    //     printf("SIGINT got\n");
    //     exit(0);
    // });

    try
    {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true)
        {
            // printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (std::exception& e)
    {
        // printf("error: %s\n", e.what());
    }

    return 0;
}