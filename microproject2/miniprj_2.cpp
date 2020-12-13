/*	Вариант 11. Иванова Виолетта. БПИ-195.
	Задача о магазине.В магазине работают три отдела, каждый отдел
	обслуживает один продавец. Покупатель, зайдя в магазин, делает покупки в
	произвольных отделах, и если в выбранном отделе продавец не свободен,
	покупатель становится в очередь и засыпает, пока продавец не освободится.
	Создать многопоточное приложение, моделирующее рабочий день магазина. */

#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

static const int  customersNumber = 1;
static const int  departmentsNumber = 3;
static int queueToDepartment[departmentsNumber];
static int customerInDepartment[departmentsNumber];

static std::atomic_int controlCustomers(0);
std::mutex wmtx, pmtx;


void customer(int order) {
	int m, k = -1;

	for (;;) {
		srand(time(0));

		if (k == -1) {
			do
			{
				// Customer choose dept. for the first time.
				k = rand() % (departmentsNumber + 1);
				// Check that first time will go to department.
			} while (k == departmentsNumber);
		}
		else
		{
			m = rand() % (departmentsNumber + 1);
			// Customer chose new dept.
			if (m != k) {
				std::unique_lock<std::mutex> plock(pmtx);
				customerInDepartment[k] = 0;
				std::cout << "Customer " << order << " left dept. number " << k + 1 << std::endl;
				k = m;
				plock.unlock();
			}

			// Customer stays in current dept.
			else
			{
				continue;
			}

		}

		// Go away from shop.
		if (k == departmentsNumber)
		{
			std::unique_lock<std::mutex> plock(pmtx);
			std::cout.width(2);
			std::cout << "Customer " << order << " left the shop. " << std::endl;
			std::cout << "Current customers who left the shop is " << ++controlCustomers << std::endl;

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
			plock.unlock();
			return;
		}

		// Go to department or queue.
		else
		{
			std::unique_lock<std::mutex> plock(pmtx);
			if (queueToDepartment[k] == customersNumber - 1) {
				std::cout << "Customer " << order << " shop in dept. number " << k + 1 << std::endl;
				customerInDepartment[k] = 1;
				queueToDepartment[k]++;
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				plock.unlock();
			}

			else
			{
				std::cout << "Customer " << order << " stay in queue to dept. " << k + 1 << std::endl;
				queueToDepartment[k]++;
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
				plock.unlock();


				while (true) {
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
					std::unique_lock<std::mutex> plock(pmtx);
					if (queueToDepartment[k] < customersNumber || customerInDepartment[k] == 0)
					{
						std::cout << "Customer " << order << " start to shop after queue in dept. number " << k + 1 << std::endl;
						customerInDepartment[k] = 1;
						queueToDepartment[k] -= 1;
						std::this_thread::sleep_for(std::chrono::milliseconds(500));
						break;
					}
					plock.unlock();
				}
			}
		}
	}
}


void startShopWork(int customersPerDay)
{
	std::vector<std::thread> customersVector;

	for (int i = 0; i < customersPerDay; i++) {
		std::thread customer(customer, i + 1);
		customersVector.push_back(std::move(customer));
	}
	for (auto& customer : customersVector)
		customer.join();
}


// Helper func to check if input is integer.
bool isNumber(std::string s)
{
	for (int i = 0; i < s.length(); i++)
		if (isdigit(s[i]) == false)
			return false;

	return true;
}

int main(int argc, char* argv[]) {

	std::string helperString = argv[1];
	if (!isNumber(helperString) || atoi(argv[1]) < 5 || atoi(argv[1]) > 1000)
	{
		std::cout << "Shop cannot work with this <" << helperString << "> amount of customers." << std::endl;
		std::cout << "Enter integer number in interval: [5; 1001)."<< std::endl;
		std::cout << "Ending the program..." << std::endl;
		return 1;
	}

	for (int i = 0; i < departmentsNumber; i++) queueToDepartment[i] = 0;
	for (int i = 0; i < departmentsNumber; i++) customerInDepartment[i] = 0;

	std::cout << "The day begins... " << std::endl << "Today's number of customers = " << helperString << std::endl;

	std::thread work(startShopWork, atoi(argv[1]));
	work.join();

	std::cout << "Day ended. Shop is closed." << std::endl;
	return 0;
}