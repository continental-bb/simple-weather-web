#include <iostream>
#include <curl/curl.h>

using namespace std;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {

    char answer;

    cout << "Do you want to see your current location? (y/n): ";
    cin >> answer;

    if(answer != 'y')
        return 0;

    CURL* curl;
    CURLcode res;
    string response;

    curl = curl_easy_init();

    if(curl) {

        curl_easy_setopt(curl, CURLOPT_URL, "https://ipinfo.io/json");

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        curl_easy_cleanup(curl);

        cout << "\nAPI Response:\n";
        cout << response << endl;
    }

    return 0;
}