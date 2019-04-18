#pragma once

#include<stdint.h>


//128 bit 
class Bitmap {
public:
	Bitmap(){ 
		bitset[0] = 0; 
		bitset[1] = 0; 
	}
	~Bitmap() {}
	Bitmap(uint8_t i){
		int idx;
		if (i < 64) idx = 0;
		else idx = 1;
		bitset[idx] = (uint64_t)1 << (i % 64);
	}
	
	Bitmap(uint8_t start, uint8_t end){
		uint64_t low = ~(((uint64_t)1 << (start % 64)) - 1);
		uint64_t high = ((uint64_t)1 << (end % 64 + 1)) - 1;
		if (start > 63) {
			bitset[0] = 0;
			bitset[1] = low & high;
		}
		else if (end <= 63) {
			bitset[0] = low & high;
			bitset[1] = 0;
		}
		else {
			bitset[1] = high;
			bitset[0] = low;
		}
	}

	Bitmap& operator |=(const Bitmap& set) {
		auto i = bitset[0];
		auto j = bitset[1];
		bitset[0] |= set.bitset[0];
		bitset[1] |= set.bitset[1];
		return *this;
	}

	void reverse() {
		bitset[0] = ~bitset[0];
		bitset[1] = ~bitset[1];
	}

	void all_1() {
		bitset[0] = ~(uint64_t)0;
		bitset[1] = ~(uint64_t)0;
	}

	void set(uint8_t idx) {
		auto i = bitset[0];
		auto j = bitset[1];
		uint64_t temp = (uint64_t)1 << (idx % 64);
		if (idx < 64) bitset[0] |= temp;
		else bitset[1] |= temp;
	}

	void set(uint8_t start, uint8_t end) {
		uint64_t low = ~(((uint64_t)1 << (start % 64)) - 1);
		uint64_t high = ((uint64_t)1 << (end % 64 + 1)) - 1;
		auto i = bitset[0];
		auto j = bitset[1];
		if (start > 63) {
			bitset[1] |= (low & high);
		}
		else if (end <= 63) {
			bitset[0] |= (low & high);
		}
		else {
			bitset[1] |= high;
			bitset[0] |= low;
		}
	}

	bool isSet(uint8_t idx) const{
		uint64_t temp = (uint64_t)1 << (idx % 64);
		if (idx < 64) return bitset[0] & temp;
		else return bitset[1] & temp;
	}

	bool empty() const {
		return !bitset[0] && !bitset[1];
	}
private:
	uint64_t bitset[2];
};