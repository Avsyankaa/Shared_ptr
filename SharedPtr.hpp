#include<atomic>
#include<iostream>
class Control_block {
public:
	std::atomic_uint references_counter;
	Control_block();
	void increase_counter();
	void decrease_counter();
	bool CustomDeleter();
};
template <typename T>
class SharedPtr {
public:
	T * object;
	class Error {};
	Control_block * contr_block;

	SharedPtr() ;

	SharedPtr(const SharedPtr& shared) ;

	auto operator = (SharedPtr&)->SharedPtr&;

	SharedPtr(T ptr) ;

	void reset() ;

	void reset(T* ptr) ;

	void swap(SharedPtr& shared) ;

	T* get() ;

	T& operator*() const ;

	T* operator->() const ;
	//возвращает количество объектов shared_ptr, которые ссылаются на тот же управляемый объект
	size_t use_count() const ;
	// проверяет, указывает ли указатель на объект
	operator bool() const ;
	void Deleter() ;
	~SharedPtr() ;

};
template <typename T>
SharedPtr<T>::~SharedPtr() {
	if (contr_block == nullptr) return;
	contr_block->decrease_counter();
	if (contr_block->CustomDeleter()) {
		Deleter();
	}
};

template <typename T>
void SharedPtr<T>::Deleter() {
	std::cout << "delete" << std::endl;
	delete this->object;
	this->object = nullptr;
	delete this->contr_block;
	this->contr_block = nullptr;
};

template <typename T>
SharedPtr<T>::operator bool() const {
	if (object != nullptr) return true;
	else return false;
};

template <typename T>
size_t SharedPtr<T>::use_count() const {
	if (contr_block != nullptr)
		return size_t(contr_block->references_counter);
	else return 0;
};

template <typename T>
T* SharedPtr<T>::operator->() const {
	return object;
};

template <typename T>
T& SharedPtr<T>::operator*() const {
	if (object != nullptr) return *object;
	else throw Error();
};

template <typename T>
T* SharedPtr<T>::get() {
	if (object == nullptr) throw Error();
	else return object;
};

template <typename T>
void SharedPtr<T>::swap(SharedPtr& shared) {
	std::swap(contr_block, shared.contr_block);
	std::swap(object, shared.object);
};

template <typename T>
void SharedPtr<T>::reset(T* ptr) {
	if (contr_block != nullptr) {
		contr_block->decrease_counter();
		if (contr_block->CustomDeleter())
			Deleter();
	}
	object = ptr;
	contr_block = new Control_block();
	contr_block->increase_counter();
};

template <typename T>
SharedPtr<T>::SharedPtr() {
	object = nullptr;
	contr_block = nullptr;
};

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& shared) {
	object = shared.object;
	contr_block = shared.contr_block;
	contr_block->increase_counter();
};

template <typename T>
SharedPtr<T>::SharedPtr(T ptr) {
	object = new T(ptr);
	contr_block = new Control_block();
	contr_block->increase_counter();
	std::cout << *object << std::endl;
};

template <typename T>
void SharedPtr<T>::reset() {
	if (contr_block != nullptr) {
		contr_block->decrease_counter();
		if (contr_block->CustomDeleter())
			Deleter();
		object = nullptr;
		contr_block = nullptr;
	}
};

template <typename T>
auto SharedPtr<T>::operator = (SharedPtr<T> & shared) -> SharedPtr<T>& {
	if (object != nullptr) {
		contr_block->decrease_counter();
		if (contr_block->CustomDeleter()) {
			Deleter();
		}
	}
	object = shared.object;
	contr_block = shared.contr_block;
	contr_block->increase_counter();
	return *this;
};

bool Control_block::CustomDeleter() {
	if (references_counter == 0) return true;
	else return false;
};

void Control_block::decrease_counter() {
	references_counter--;
};

void Control_block::increase_counter() {
	references_counter++;
};

Control_block::Control_block() {
	references_counter = 0;
};
