#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <atomic>
#include <mutex>
#include <climits>

#define SERVER_IP "10.249.47.4"
#define SERVER_PORT 8080
#define NUM_CLIENTS 100
#define REQUESTS_PER_CLIENT 4000

// 定义包头结构
struct PacketHeader {
    unsigned short total_length;   // 总长度
    unsigned short msg_type;       // 消息类型代码
    int crc_checksum;     // CRC 校验码

    bool operator==(const PacketHeader& other) const {
        return total_length == other.total_length &&
               msg_type == other.msg_type &&
               crc_checksum == other.crc_checksum;
    }
};

// 统计信息
std::atomic<long> total_response_time(0);
std::atomic<long> max_response_time(0);
std::atomic<long> min_response_time(LONG_MAX);
std::mutex print_mutex;
std::atomic<int> success_count(0);
std::atomic<int> failure_count(0);
std::atomic<int> total_requests(0);

void send_request(int client_id) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Client " << client_id << ": Error creating socket\n";
        return;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Client " << client_id << ": Error connecting to server\n";
        close(sockfd);
        return;
    }

    for (int i = 0; i < REQUESTS_PER_CLIENT; ++i) {
        // 构造自定义协议的包头
        PacketHeader header;
        header.total_length = htons(sizeof(PacketHeader)); // 包头总长度
        header.msg_type = htons(0);                        // 假设消息类型代码为 1
        header.crc_checksum = htonl(0);                    // 假设 CRC 校验码为 0

        auto start_time = std::chrono::high_resolution_clock::now();

        if (send(sockfd, &header, sizeof(header), 0) < 0) {
            std::cerr << "Client " << client_id << ": Error sending request\n";
            failure_count++;
            total_requests++;
            break;
        }

        // 接收服务器的响应
        PacketHeader response;
        if (recv(sockfd, &response, sizeof(response), 0) < 0) {
            std::cerr << "Client " << client_id << ": Error receiving response\n";
            failure_count++;
            total_requests++;
            break;
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        long response_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

        total_response_time += response_time;
        total_requests++;

        // 更新最大和最小响应时间
        {
            std::lock_guard<std::mutex> lock(print_mutex);
            if (response_time > max_response_time) {
                max_response_time = response_time;
            }
            if (response_time < min_response_time) {
                min_response_time = response_time;
            }
        }

        // 验证接收到的数据是否与发送的数据相同
        if (header == response) {
            success_count++;
        } else {
            std::cerr << "Client " << client_id << ": Data mismatch\n";
            failure_count++;
        }

        // 模拟处理时间
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    close(sockfd);
}

int main() {
    std::vector<std::thread> clients;
    for (int i = 0; i < NUM_CLIENTS; ++i) {
        clients.emplace_back(send_request, i);
    }

    for (auto &client : clients) {
        client.join();
    }

    long total_requests_count = total_requests.load();
    long avg_response_time = total_requests_count > 0 ? total_response_time / total_requests_count : 0;

    std::cout << "Stress test completed\n";
    std::cout << "Total requests: " << total_requests_count << "\n";
    std::cout << "Successful responses: " << success_count.load() << "\n";
    std::cout << "Failed responses: " << failure_count.load() << "\n";
    std::cout << "Average response time: " << avg_response_time << " microseconds\n";
    std::cout << "Maximum response time: " << max_response_time.load() << " microseconds\n";
    std::cout << "Minimum response time: " << min_response_time.load() << " microseconds\n";

    return 0;
}