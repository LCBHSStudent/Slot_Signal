#include <slot_signal>
#include <iostream>

using namespace std;
using namespace van;

void TestFunc1(int n, int m) {
	cout << n+m << endl;
}

void TestFunc2(int n, int m) {
	cout << n << " " << m << endl;
}

template <typename Signature>
using mySignal = Signal<Signature, TS_Policy_Safe<>>;

void Test1() {
	mySignal<void(int, int)> signal1;
	signal1.connect<TestFunc1>();
	signal1.connect<TestFunc2>();
	signal1.emit(1, 2);
}

//void Test2() {
//	std::thread
//}

int main(int argc, char* argv[]) {
	Test1();
	return 0;
}
