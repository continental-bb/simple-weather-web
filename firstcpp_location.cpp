#include <iostream>
#include <string>
#include <curl/curl.h>

using namespace std;

size_t writeCallback(void* contents, size_t size, size_t nmemb, string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string getValue(string data, string key) {
    size_t pos = data.find(key);
    if (pos == string::npos) return "";

    pos = data.find(":", pos) + 1;
    size_t end = data.find(",", pos);

    // Remove quotes and whitespace
    string value = data.substr(pos, end - pos);
    value.erase(remove(value.begin(), value.end(), '\"'), value.end());
    value.erase(0, value.find_first_not_of(" \n\r\t"));
    value.erase(value.find_last_not_of(" \n\r\t") + 1);
    return value;
}

int main() {
    cout << "Press ENTER to get weather in your location...";
    cin.get();


    CURL* curl = curl_easy_init();
    if (!curl) {
        cerr << "Failed to initialize CURL." << endl;
        return 1;
    }
    string locationData;

    curl_easy_setopt(curl, CURLOPT_URL, "http://ip-api.com/json/");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &locationData);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "CURL error: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(curl);
        return 1;
    }
    curl_easy_cleanup(curl);

    string lat = getValue(locationData, "\"lat\"");
    string lon = getValue(locationData, "\"lon\"");

    if (lat.empty() || lon.empty()) {
        cerr << "Failed to get location data." << endl;
        return 1;
    }

    string apiKey = "4a1546192515ef26110f63749100a987";

    string weatherURL =
        "https://api.openweathermap.org/data/2.5/weather?lat=" +
        lat + "&lon=" + lon +
        "&appid=" + apiKey +
        "&units=metric";

    curl = curl_easy_init();
    if (!curl) {
        cerr << "Failed to initialize CURL." << endl;
        return 1;
    }
    string weatherData;

    curl_easy_setopt(curl, CURLOPT_URL, weatherURL.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &weatherData);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        cerr << "CURL error: " << curl_easy_strerror(res) << endl;
        curl_easy_cleanup(curl);
        return 1;
    }
    curl_easy_cleanup(curl);

    cout << "\nWeather Data:\n";
    cout << weatherData << endl;

    return 0;
}