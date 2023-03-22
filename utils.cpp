#include "utils.h"
#include <iostream>
#include <cstring>
#include <curl/curl.h>
#include <QDebug>

size_t curlSaveResponseToStdString(void *contents, size_t size, size_t nmemb, std::string *s) {
    size_t newLength = size * nmemb;
    size_t oldLength = s->size();
    s->resize(oldLength + newLength);
    std::copy((char*)contents, (char*)contents+newLength, s->begin()+oldLength);

    return size * nmemb;
}

int doLogin(std::string userName, std::string userPasswd)
{
    // 初始化 curl
    CURL *curl = curl_easy_init();

    std::string curlPostFields = "username=" + userName + "&userpwd=" + userPasswd;

    if (curl) {
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, "https://192.168.5.5/cgi-bin/ace_web_auth.cgi");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlSaveResponseToStdString); // 保存响应到 string 中
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);                       // 请求的响应保存到变量 response 中
        curl_easy_setopt(curl, CURLOPT_POST, 1);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, curlPostFields.c_str());         // POST 参数: GET 请求中的中文要进行 URL 编码，否则提示错误
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

        CURLcode code = curl_easy_perform(curl);
        if (code == CURLE_OK) {
            qDebug() << QString::fromStdString(response);                           // 使用 qDebug() 输出，UTF-8 的中文不会乱码
        }
    } else {
        qDebug() << "Error";
    }

    // 释放 curl 资源
    curl_easy_cleanup(curl);

    return 0;
}

int doLogout(void)
{
    // 初始化 curl
    CURL *curl = curl_easy_init();

    if (curl) {
        std::string response;
        curl_easy_setopt(curl, CURLOPT_URL, "https://192.168.5.5/cgi-bin/ace_web_auth.cgi?logout=1");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlSaveResponseToStdString); // 保存响应到 string 中
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);                       // 请求的响应保存到变量 response 中
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

        CURLcode code = curl_easy_perform(curl);
        if (code == CURLE_OK) {
            qDebug() << QString::fromStdString(response);                           // 使用 qDebug() 输出，UTF-8 的中文不会乱码
        }
    } else {
        qDebug() << "Error";
    }

    // 释放 curl 资源
    curl_easy_cleanup(curl);

    return 0;
}
