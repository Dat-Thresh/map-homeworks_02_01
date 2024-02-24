#include <iostream>
#include <thread>
#include <execution>
#include <chrono>

using namespace std::chrono_literals;

void client_queue(std::atomic<int>& clients) {
	while (clients.load() < 10 && clients.is_lock_free()) {
		std::this_thread::sleep_for(100ms);
		clients++;
		std::cout << "Client arrived: " << clients.load() << std::endl;
	}
}

void operationist(std::atomic<int>& clients) {
	while (clients.load() > 0 && clients.is_lock_free()) {
		std::this_thread::sleep_for(200ms);
		clients--;
		std::cout << "Operationist called. Now number of clients: " << clients.load() << std::endl;
	}
};


int main() {
	std::atomic<int> clients = 5;
	std::thread th1(client_queue, std::ref(clients));
	std::thread th2(operationist, std::ref(clients));
	th1.join();
	th2.join();

	return 0;

}