// HashingAnalysis.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <random>
#include <map>
#include <time.h> 
#include "Element.h"
#include <fstream>
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
		toCount = toCount - (int) pow(2.0, (double)bitCount);
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
	while (i < (int) ourElements.size()) {
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
		while (i < (int) ourElements.size()) {
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
	ofstream fileOut;
	fileOut.open("HashData.csv");


	// First experiment with chaining using Key Mod Hash with sizes 10,50 and 97
	KeyAnalysisChaining chainKeyMod1(10);
	fileOut << "Key mod hashing with chaining, \n" << "\n";
	fileOut << "Current Size = 10, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > chainKeyModPrint1 = chainKeyMod1.GetPointsKeyMod();
	for (size_t i = 0; i < chainKeyModPrint1.size(); i++)
	{
		fileOut << chainKeyModPrint1[i]->loadFactor << ", " << chainKeyModPrint1[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

	KeyAnalysisChaining chainKeyMod2(50);
	fileOut << "Current Size = 50, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > chainKeyModPrint2 = chainKeyMod2.GetPointsKeyMod();
	for (size_t i = 0; i < chainKeyModPrint2.size(); i++)
	{
		fileOut << chainKeyModPrint2[i]->loadFactor << ", " << chainKeyModPrint2[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

	KeyAnalysisChaining chainKeyMod3(97);
	fileOut << "Current Size = 97 (prime), \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > chainKeyModPrint3 = chainKeyMod3.GetPointsKeyMod();
	for (size_t i = 0; i < chainKeyModPrint3.size(); i++)
	{
		fileOut << chainKeyModPrint3[i]->loadFactor << ", " << chainKeyModPrint3[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

	// Second experiment with chaining using Mid Square Hashing with sizes 10,50 and 97
	KeyAnalysisChaining chainMidSquare1(10);
	fileOut << "Mid Square hashing with chaining, \n" << "\n";
	fileOut << "Current Size = 10, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > chainMidSquarePrint1 = chainMidSquare1.GetPointsMidSquare();
	for (size_t i = 0; i < chainMidSquarePrint1.size(); i++)
	{
		fileOut << chainMidSquarePrint1[i]->loadFactor << ", " << chainMidSquarePrint1[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

	KeyAnalysisChaining chainMidSquare2(50);
	fileOut << "Current Size = 50, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > chainMidSquarePrint2 = chainMidSquare2.GetPointsMidSquare();
	for (size_t i = 0; i < chainMidSquarePrint2.size(); i++)
	{
		fileOut << chainMidSquarePrint2[i]->loadFactor << ", " << chainMidSquarePrint2[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

	KeyAnalysisChaining chainMidSquare3(97);
	fileOut << "Current Size = 97, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > chainMidSquarePrint3 = chainMidSquare3.GetPointsMidSquare();
	for (size_t i = 0; i < chainMidSquarePrint3.size(); i++)
	{
		fileOut << chainMidSquarePrint3[i]->loadFactor << ", " << chainMidSquarePrint3[i]->collisionCount << ", \n";
	}
	fileOut << "\n";


	// Third experiment with open adressing using Key Mod Hashing with sizes 10,50 and 97
	KeyAnalysisOpenAdressing openAdressingKeyMod1(10);
	fileOut << "Key Mod hashing with open adressing, \n" << "\n";
	fileOut << "Current Size = 10, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > openAdressingKeyModPrint1 = openAdressingKeyMod1.GetPointsKeyMod();
	for (size_t i = 0; i < openAdressingKeyModPrint1.size(); i++)
	{
		fileOut << openAdressingKeyModPrint1[i]->loadFactor << ", " << openAdressingKeyModPrint1[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

	KeyAnalysisOpenAdressing openAdressingKeyMod2(50);
	fileOut << "Current Size = 50, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > openAdressingKeyModPrint2 = openAdressingKeyMod2.GetPointsKeyMod();
	for (size_t i = 0; i < openAdressingKeyModPrint2.size(); i++)
	{
		fileOut << openAdressingKeyModPrint2[i]->loadFactor << ", " << openAdressingKeyModPrint2[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

	KeyAnalysisOpenAdressing openAdressingKeyMod3(97);
	fileOut << "Current Size = 97, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > openAdressingKeyModPrint3 = openAdressingKeyMod3.GetPointsKeyMod();
	for (size_t i = 0; i < openAdressingKeyModPrint3.size(); i++)
	{
		fileOut << openAdressingKeyModPrint3[i]->loadFactor << ", " << openAdressingKeyModPrint3[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

	// Third experiment with open adressing using Mid Square Hashing with sizes 10,50 and 97
	KeyAnalysisOpenAdressing openAdressingMidSquare1(10);
	fileOut << "Mid Square hashing with open adressing, \n";
	fileOut << "Current Size = 10, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > openAdressingMidSquarePrint1 = openAdressingMidSquare1.GetPointsMidSquare();
	for (size_t i = 0; i < openAdressingMidSquarePrint1.size(); i++)
	{
		fileOut << openAdressingMidSquarePrint1[i]->loadFactor << ", " << openAdressingMidSquarePrint1[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

	KeyAnalysisOpenAdressing openAdressingMidSquare2(50);
	fileOut << "Current Size = 50, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > openAdressingMidSquarePrint2 = openAdressingMidSquare2.GetPointsMidSquare();
	for (size_t i = 0; i < openAdressingMidSquarePrint2.size(); i++)
	{
		fileOut << openAdressingMidSquarePrint2[i]->loadFactor << ", " << openAdressingMidSquarePrint2[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

	KeyAnalysisOpenAdressing openAdressingMidSquare3(97);
	fileOut << "Current Size = 97, \n";
	fileOut << "Load Factor, Collision Count, \n";
	vector<Node * > openAdressingMidSquarePrint3 = openAdressingMidSquare3.GetPointsMidSquare();
	for (size_t i = 0; i < openAdressingMidSquarePrint3.size(); i++)
	{
		fileOut << openAdressingMidSquarePrint3[i]->loadFactor << ", " << openAdressingMidSquarePrint3[i]->collisionCount << ", \n";
	}
	fileOut << "\n";

}
