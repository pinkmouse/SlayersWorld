#pragma once
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

        l_Curl = curl_easy_init();
        if (l_Curl != nullptr)
        {
            list = curl_slist_append(list, "Content-Type: application/json");

            curl_easy_setopt(l_Curl, CURLOPT_URL, p_URL.c_str());
            curl_easy_setopt(l_Curl, CURLOPT_POST, 1);
            curl_easy_setopt(l_Curl, CURLOPT_HTTPHEADER, list);
            curl_easy_setopt(l_Curl, CURLOPT_SSL_VERIFYHOST, false);
            curl_easy_setopt(l_Curl, CURLOPT_SSL_VERIFYPEER, false);
            curl_easy_setopt(l_Curl, CURLOPT_POSTFIELDS, p_Msg.c_str());
            l_Result = curl_easy_perform(l_Curl);
            curl_easy_cleanup(l_Curl);
            printf("Result = %d\n", l_Result);
        }
    }
};

