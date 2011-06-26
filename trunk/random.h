#ifndef MY_RANDOM_H
#define MY_RANDOM_H
typedef unsigned int ui;
struct myRandom{
	ui mat[624];
	int index;
	myRandom(int seed){
		reseed(seed);
	}
	ui randomBits() {
		if(index == 0) {
			refillArray();
		}
		ui y = mat[index];
		y ^= (y>>11);
		y ^= ((y<<7)&2636928640);
		y ^= ((y<<15)&4022730752);
		y ^= (y>>18);
		index = (index + 1)%624;
		return y;
	}
	void reseed(ui seed){
		mat[0] = seed;
		for(int i=1;i<624;i++){
			mat[i] = 1812433253ULL*((mat[i-1]^(mat[i-1]>>30)) + i);
		}
		index = 0;
	}
	inline int rnd(ui mod){
		return randomBits()%mod;
	}
	void refillArray() {
		for(int i=0;i<624;i++) {
			ui upper = 1<<31;
			ui lower = upper-1;
			ui y = mat[i]&upper + mat[(i+1)%624]&lower;
			mat[i] = mat[(i + 397) % 624] ^ (y>>1);
			if (y & 1){ 
				mat[i] ^= 2567483615;
			}
		}
	}
};
myRandom rander(43);
#endif
