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
int GetBitCount(int toCount) {
	int bitCount = 0;
	while (toCount > 0) {
		toCount = toCount - pow(2.0, (double)bitCount);
		bitCount++;
	}
	return bitCount;
}
float GetLoadFactor(int itemNumber, int tableSize) {
	return (float)itemNumber / tableSize;
}
int MidSquareHashing(int key, int sizeBitCount,int tableSize) {
	// key range between 0-3*table size
	// take middle two digits
	int squared = key * key;
	int divideBy2 = (GetBitCount(squared) - sizeBitCount) / 2;
	int divided = squared;
	if (divideBy2 > 0)
		divided = divided >> divideBy2;
	return (divided % tableSize);
}
int KeyModTableSize(int key, int tableSize) {
	return key % tableSize;
}
void AddToVector(int hash, vector<int> &ourElements) {
	int i = hash + 1;
	bool added = false;
	while (i < ourElements.size()) {
		if (ourElements[i] != -1) {
			i++;
		}
		else {
			ourElements[i] = hash;
			added = true;
		}
	}
	if (!added) {
		i = 0;
		while (i < ourElements.size()) {
			if (ourElements[i] != -1) {
				i++;
			}
			else {
				ourElements[i] = hash;
			}
		}
	}
}
class KeyAnalysisOpenAdressing {
public:
	KeyAnalysisOpenAdressing(int tableSize) {
		this->tableSize = tableSize;
		itemsAdded = 0;
		collisions = 0;
		sizeBitCount = GetBitCount(tableSize);
		for (int i = 0; i < tableSize; i++)
		{
			ourElements.push_back(-1);
		}
	}
	int Contains(int hash) {
		for (int i = 0; i < (int)this->ourElements.size(); i++)
		{
			if (ourElements[i] == hash)
				return i;
		}
		return -1;
	}
	vector<Node *> GetPointsKeyMod() {
		vector<Node *> nodes;
		int range = 3 * this->tableSize;
		itemsAdded = 0;
		while (GetLoadFactor(this->itemsAdded, tableSize) < 1.0f) {
			int random = GetRandom(range);
			int hash = KeyModTableSize(random, this->tableSize);
			Node * g = new Node;
			itemsAdded++;
			if (hash < tableSize) {
				int ind = Contains(hash);
				if (ind == -1) {
					ourElements[hash] = hash;
				}
				else {
					AddToVector(hash, ourElements);
					collisions++;
				}
			}
			g->collisionCount = collisions;
			g->loadFactor = GetLoadFactor(itemsAdded, tableSize);
			nodes.push_back(g);
		}
		return nodes;
	}
	vector<Node *> GetPointsMidSquare() {
		vector<Node *> nodes;
		int range = 3 * this->tableSize;
		itemsAdded = 0;
		while (GetLoadFactor(this->itemsAdded, tableSize) < 1.0f) {
			int random = GetRandom(range);
			int hash = MidSquareHashing(random,sizeBitCount, this->tableSize);
			Node * g = new Node;
			itemsAdded++;
			if (hash < tableSize) {
				int ind = Contains(hash);
				if (ind == -1) {
					ourElements[hash] = hash;
				}
				else {
					AddToVector(hash, ourElements);
					collisions++;
				}
			}
			g->collisionCount = collisions;
			g->loadFactor = GetLoadFactor(itemsAdded, tableSize);
			nodes.push_back(g);
		}
		return nodes;
	}
private:
	int tableSize;
	int itemsAdded;
	int collisions;
	int sizeBitCount;
	std::vector<int> ourElements;
};
class KeyAnalysisChaining {
public:
	KeyAnalysisChaining(int tableSize) {
		this->tableSize = tableSize;
		itemsAdded = 0;
		collisions = 0;	
		sizeBitCount = GetBitCount(tableSize);
	}
	int Contains(int hash) {
		for (int i = 0; i < (int)this->ourElements.size(); i++)
		{
			if (ourElements[i]->hash == hash)
				return i;
		}
		return -1;
	}

	
	vector<Node *> GetPointsMidSquare() {
		vector<Node *> nodes;
		int range = 3 * this->tableSize;
		itemsAdded = 0;
		while (GetLoadFactor(this->itemsAdded, tableSize) < 1.0f) {
			int random = GetRandom(range);
			int hash = MidSquareHashing(random,sizeBitCount,tableSize);
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
			int hash = KeyModTableSize(random,this->tableSize);
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
	
	
private:
	int collisions;
	int itemsAdded;
	int tableSize;
	int keySquaredBitCount;
	int sizeBitCount;
	vector<Element *> ourElements;
};



int main()
{
	// initial table size is 100
	//KeyAnalysisChaining guy(1000);
	//vector<Node * > toprint = guy.GetPointsKeyMod();
	//for (int i = 0; i < (int) toprint.size(); i++)
	//{
	//	cout << "( " << toprint[i]->loadFactor << ", " << toprint[i]->collisionCount << ") ";
	//}
	//cout << "\n";
	KeyAnalysisChaining chain(97);
	vector<Node * > chainPrint = chain.GetPointsKeyMod();
	for (size_t i = 0; i < chainPrint.size(); i++)
	{
		cout << "( " << chainPrint[i]->loadFactor << ", " << chainPrint[i]->collisionCount << ") ";
	}
	cout << "\n" << "\n";;
	KeyAnalysisChaining chain1(97);
	vector<Node * > chainPrint1 = chain1.GetPointsMidSquare();
	for (size_t i = 0; i < chainPrint1.size(); i++)
	{
		cout << "( " << chainPrint1[i]->loadFactor << ", " << chainPrint1[i]->collisionCount << ") ";
	}
	cout << "\n" << "\n";;
	KeyAnalysisOpenAdressing guy1(97);
	vector<Node * > toprint1 = guy1.GetPointsKeyMod();
	for (int i = 0; i < (int)toprint1.size(); i++)
	{
		cout << "( " << toprint1[i]->loadFactor << ", " << toprint1[i]->collisionCount << ") ";
	}
	cout << "\n" << "\n";;
	KeyAnalysisOpenAdressing guy(97);
	vector<Node * > toprint = guy.GetPointsMidSquare();
	for (int i = 0; i < (int)toprint1.size(); i++)
	{
		cout << "( " << toprint[i]->loadFactor << ", " << toprint[i]->collisionCount << ") ";
	}
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
