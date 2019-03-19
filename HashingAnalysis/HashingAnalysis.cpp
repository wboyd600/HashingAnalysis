// HashingAnalysis.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <random>
#include <map>
#include <time.h> 
#include "Element.h"
using namespace std;

// For each hash table size, randomly generate key values between 0 and 3 times the hash table size.
// (If the table size is 100, generate key values between 0 and 300).  As each key value is placed in the hash table, record the current load factor and the number of collisions that have occurred.  
// When counting collisions, for each key there is at most one collision.  
// (Even if the key requires multiple probes to find a spot, this counts as one collision.)


int Random(int min, int max);
mt19937 random_mt;
int Random(int min, int max) {
	uniform_int_distribution<int> dist(min, max);
	return dist(random_mt);
}


// just to record loadfactor vs collisionCount
struct Node {
	float loadFactor;
	int collisionCount;
};
int GetRandom(int range) {
	std::random_device dev;
	random_mt.seed(dev());
	int t = Random(0, range);
	return t;
}
class KeyAnalysis {
public:
	KeyAnalysis(int tableSize) {
		this->tableSize = tableSize;
		itemsAdded = 0;
		collisions = 0;
	}
	int Contains(int hash) {
		for (int i = 0; i < (int)this->ourElements.size(); i++)
		{
			if (ourElements[i]->hash == hash)
				return i;
		}
		return -1;
	}

	int MidSquareHashing(int key) {
		// key range between 0-3*table size
		// fit within 100
		// take middle two digits
		long squared = key * key;
		int count = 1;
		int down = squared / 10;
		while (down > 0) {
			count++;
			down = down / 10;
		}
		int current = 0;
		down = squared;
		while (count - current > (count / 2 + 1)) {
			down = down / 10;
			current++;
		}
		return (down % this->tableSize);
	}
	vector<Node *> GetPointsMidSquare() {
		vector<Node *> nodes;
		int range = 3 * this->tableSize;
		itemsAdded = 0;
		while (GetLoadFactor(this->itemsAdded, tableSize) < 1.0f) {
			int random = GetRandom(range);
			int hash = KeyModTableSize(random);
			Node * g = new Node;
			itemsAdded++;
			Element * temp = NULL;
			if (hash < tableSize) {
				int ind = Contains(hash);
				if (ind == -1) {
					temp = AddElement(temp, hash);
					ourElements.push_back(temp);
				}
				else {
					temp = ourElements[ind];
					temp = AddElement(temp, hash);
					if (GetChainLength(temp) == 2) {
						collisions++;
					}
				}
			}

			g->collisionCount = collisions;
			g->loadFactor = GetLoadFactor(itemsAdded, tableSize);
			nodes.push_back(g);
		}
		return nodes;
	}
	vector<Node *> GetPointsKeyMod() {
		vector<Node *> nodes;
		int range = 3 * this->tableSize;
		itemsAdded = 0;
		while (GetLoadFactor(this->itemsAdded, tableSize) < 1.0f) {
			int random = GetRandom(range);
			int hash = KeyModTableSize(random);
			Node * g = new Node;
			itemsAdded++;
			Element * temp = NULL;
			if (hash < tableSize) {
				int ind = Contains(hash);
				if (ind == -1) {
					temp = AddElement(temp, hash);
					ourElements.push_back(temp);
				}
				else {
					temp = ourElements[ind];
					temp = AddElement(temp, hash);
					if (GetChainLength(temp) == 2) {
						collisions++;
					}
				}
			}
			
			g->collisionCount = collisions;
			g->loadFactor = GetLoadFactor(itemsAdded, tableSize);
			nodes.push_back(g);
		}
		return nodes;
	}
	float GetLoadFactor(int itemNumber, int tableSize) {
		return (float)itemNumber / tableSize;
	}
	int KeyModTableSize(int key) {
		return key % this->tableSize;
	}
private:
	int collisions;
	int itemsAdded;
	int tableSize;
	vector<Element *> ourElements;
};

int GetBitCount(int toCount) {
	int bitCount = 0;
	while (toCount > 0) {
		bitCount = bitCount - (int)pow(2, bitCount);
		bitCount++;
	}
	return bitCount;
}

int main()
{
	// initial table size is 100
	int bit = GetBitCount(100);
	cout << bit;
	//keyanalysis guy(10);
	//vector<node * > toprint = guy.getpointskeymod();
	//for (int i = 0; i < (int) toprint.size(); i++)
	//{
	//	cout << "( " << toprint[i]->loadfactor << ", " << toprint[i]->collisioncount << ") ";
	//}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
