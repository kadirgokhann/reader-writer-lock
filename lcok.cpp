#include <iostream>
#include <shared_mutex>
#include <thread>
#include <vector>

std::shared_mutex rwLock;
int sharedData = 0;

// Reader function
void reader(int readerId) {
    while (true) {
        std::shared_lock<std::shared_mutex> lock(rwLock); // Acquire shared lock
        std::cout << "Reader " << readerId << " reads: " << sharedData << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Writer function
void writer(int writerId) {
    while (true) {
        std::unique_lock<std::shared_mutex> lock(rwLock); // Acquire exclusive lock
        sharedData++;
        std::cout << "Writer " << writerId << " writes: " << sharedData << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

int main() {
    std::vector<std::thread> readers, writers;

    // Start readers
    for (int i = 0; i < 3; ++i) {
        readers.emplace_back(reader, i);
    }

    // Start writers
    for (int i = 0; i < 2; ++i) {
        writers.emplace_back(writer, i);
    }

    // Join threads
    for (auto& reader : readers) {
        reader.join();
    }
    for (auto& writer : writers) {
        writer.join();
    }

    return 0;
}
