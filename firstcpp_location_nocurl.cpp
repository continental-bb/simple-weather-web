#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cctype>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#endif

// simple HTTP GET over sockets (works for ip-api.com on port 80)
std::string http_get(const std::string& host, const std::string& path) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) return "";
#endif

    struct addrinfo hints;
    struct addrinfo* res = nullptr;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), "80", &hints, &res) != 0) {
#ifdef _WIN32
        WSACleanup();
#endif
        return "";
    }

    int sockfd = -1;
    struct addrinfo* p;
    for (p = res; p != nullptr; p = p->ai_next) {
#ifdef _WIN32
        sockfd = (int)socket(p->ai_family, p->ai_socktype, p->ai_protocol);
#else
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
#endif
        if (sockfd == -1) continue;
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0) break;
#ifdef _WIN32
        closesocket(sockfd);
#else
        close(sockfd);
#endif
        sockfd = -1;
    }

    freeaddrinfo(res);

    if (sockfd == -1) {
#ifdef _WIN32
        WSACleanup();
#endif
        return "";
    }

    std::ostringstream req;
    req << "GET " << path << " HTTP/1.1\r\n";
    req << "Host: " << host << "\r\n";
    req << "Connection: close\r\n";
    req << "User-Agent: cpp-location/1.0\r\n";
    req << "Accept: */*\r\n\r\n";

    std::string request = req.str();
    int sent = send(sockfd, request.c_str(), (int)request.size(), 0);
    if (sent <= 0) {
#ifdef _WIN32
        closesocket(sockfd);
        WSACleanup();
#else
        close(sockfd);
#endif
        return "";
    }

    std::string response;
    char buffer[4096];
    while (true) {
        int n = recv(sockfd, buffer, (int)sizeof(buffer), 0);
        if (n <= 0) break;
        response.append(buffer, buffer + n);
    }

#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif

    // split headers/body
    size_t pos = response.find("\r\n\r\n");
    if (pos == std::string::npos) return response;
    return response.substr(pos + 4);
}

std::string extract_json_string(const std::string& json, const std::string& key) {
    std::string needle = "\"" + key + "\"";
    size_t pos = json.find(needle);
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos + needle.size());
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && isspace((unsigned char)json[pos])) pos++;
    if (pos < json.size() && json[pos] == '"') {
        pos++;
        size_t end = pos;
        std::string out;
        while (end < json.size()) {
            if (json[end] == '\\') {
                if (end + 1 < json.size()) { out.push_back(json[end+1]); end += 2; continue; }
            }
            if (json[end] == '"') break;
            out.push_back(json[end]);
            end++;
        }
        return out;
    }
    size_t end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && !isspace((unsigned char)json[end])) end++;
    return json.substr(pos, end - pos);
}

int main(int argc, char** argv) {
    std::string host = "ip-api.com";
    std::string path = "/json";
    if (argc >= 2) { path = "/json/" + std::string(argv[1]); }

    std::string body = http_get(host, path);
    if (body.empty()) {
        std::cerr << "Failed to fetch location (network error).\n";
        return 1;
    }

    std::cout << "Raw JSON response:\n" << body << "\n\n";

    std::string status = extract_json_string(body, "status");
    if (status != "success") {
        std::cerr << "Geolocation failed: " << extract_json_string(body, "message") << "\n";
        return 1;
    }

    std::string query = extract_json_string(body, "query");
    std::string city = extract_json_string(body, "city");
    std::string region = extract_json_string(body, "regionName");
    std::string country = extract_json_string(body, "country");
    std::string lat = extract_json_string(body, "lat");
    std::string lon = extract_json_string(body, "lon");

    std::cout << "Detected location for IP: " << query << "\n";
    std::cout << "City: " << city << "\n";
    std::cout << "Region: " << region << "\n";
    std::cout << "Country: " << country << "\n";
    std::cout << "Latitude: " << lat << "\n";
    std::cout << "Longitude: " << lon << "\n";

    return 0;
}
