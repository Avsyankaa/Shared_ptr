#include"SharedPtr.hpp"
int main() {
	try {
		SharedPtr <int> shared1(18);
		SharedPtr <int> shared3(shared1);
		SharedPtr <int> shared2(15);
		shared1.swap(shared2);
		std::cout << shared1.use_count() << std::endl;
		std::cout << shared2.use_count() << std::endl;
		shared1.reset();
		std::cout << shared2.operator->() << std::endl;
	}
	catch (SharedPtr<int>::Error()) {
		std::cout << "An error. The field is empty." << std::endl;
	}
	system("pause");
}
