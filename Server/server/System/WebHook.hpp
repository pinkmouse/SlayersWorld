#pragma once
#include <sstream>
#include <curl/curl.h>
#include "../Define.hpp"

class WebHook
{
public:
    static void sendMsg(const std::string & p_URL, const std::string & p_Msg)
    {
        CURLcode l_Result;
        CURL *l_Curl = nullptr;
        struct curl_slist *list = nullptr;

        std::string l_Str = "{ \"content\" : \"" + p_Msg + "\" }";
        l_Curl = curl_easy_init();
        if (l_Curl != nullptr)
        {
            list = curl_slist_append(list, "Content-Type: application/json");
            list = curl_slist_append(list, "charsets: utf-8");

            curl_easy_setopt(l_Curl, CURLOPT_URL, "https://discordapp.com/api/webhooks/348181291973345281/wbCoES7jXWhNf9tbfSNCDmbGgRhIbKv5PtcpBHU5yoj9qv1bSiCrS60cCirCQZ2Lmvxj");
            curl_easy_setopt(l_Curl, CURLOPT_POST, 1);
            curl_easy_setopt(l_Curl, CURLOPT_HTTPHEADER, list);
            curl_easy_setopt(l_Curl, CURLOPT_SSL_VERIFYHOST, false);
            curl_easy_setopt(l_Curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(l_Curl, CURLOPT_POSTFIELDSIZE, l_Str.size());
            curl_easy_setopt(l_Curl, CURLOPT_POSTFIELDS, l_Str.c_str());
            l_Result = curl_easy_perform(l_Curl);
            curl_easy_cleanup(l_Curl);
            printf("Result = %d, %s\n", l_Result, l_Str.c_str());
        }
    }
};

