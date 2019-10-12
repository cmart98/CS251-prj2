#ifndef _SERVICE_QUEUE_H
#define _SERVICE_QUEUE_H

#include <iostream>
#include <vector>
#include <utility>

using namespace std;

class ServiceQueue {

private:
	int lengthh;
	int counter;
	int tmp;

	struct Node { //double link list 
		int buzzerNumber;
		Node* prev;
		Node* next;
		Node(int buzzer = 0, Node* _prev = nullptr, Node* _next = nullptr) {
			buzzerNumber = buzzer;
			prev = _prev;
			next = _next;
		}
	};

	struct BuzzerNode
	{
		bool inBucket;
		Node* location;
		BuzzerNode(bool inOut = false, Node* loc = nullptr) {
			inBucket = inOut;
			location = loc;
		}
	};

	struct list {
		Node* tail;
		Node *front;
		list(Node* _tail = nullptr, Node* _front = nullptr) {
			tail = _tail;
			front = _front;

		}
	};

	list buzzerBucket ;
	vector<BuzzerNode*> fullList;
	list waitingList;

	Node* pushback(list &x, int num) {
		Node *newNode = new Node;
		newNode->buzzerNumber = num;

		if (x.front == nullptr) {
			x.front = newNode;
			x.tail = x.front;;
			//x.front->prev = nullptr;
			//x.tail->next = nullptr;

		}
		else {
			x.tail->next = newNode;
			newNode->prev = x.tail;
			//newNode->next = nullptr;
			x.tail = x.tail->next;
		}
		return x.tail;
	}

	Node* popfront(list& list) {
		Node *tmp = list.front;

		if (list.front == nullptr) {
			return nullptr;
		}
		if (list.front->next == nullptr || list.tail->prev == nullptr) {
		//	cout << "only one item to pop";
			list.front = nullptr;
			list.tail = nullptr;
			return tmp;
		}

		list.front = list.front->next;
		list.front->prev = nullptr;

		//cout << "end of pop front" << endl;
		return tmp;

	}

	Node* pushFront(list& list, int num) {
		 Node* newNode = new Node;
		newNode->buzzerNumber = num;

		if (list.front == nullptr) {
			list.front = newNode;
			list.tail = newNode;
		}
		else {
			newNode->next = list.front;
			list.front->prev = newNode;
			list.front = list.front->prev;
		}

		return list.front;
	}

public:
	ServiceQueue() {
	//	buzzer = 0;
		//list buzzerBucket ;
		//vector<BuzzerNode*> fullList;
		//list waitingList;
		lengthh = 0;
		counter = 0;
		tmp = -1;
		int buzzerNumber = 0;

		Node* prev = nullptr;
		Node* next = nullptr;
		BuzzerNode* location = nullptr;
		BuzzerNode* inBucket = false;
		//list* tail = nullptr;
		//list* front = nullptr;
	}
	
	~ServiceQueue() {

	}

	void snapshot(std::vector<int> &buzzers) {
		buzzers.clear();   // you don't know the history of the 
						   //   buzzers vector, so we had better
						   //   clear it first.

		Node *tmp = waitingList.front;
		while (tmp != NULL) {
			buzzers.push_back(tmp->buzzerNumber);
			tmp = tmp->next;
		}

	}

	int  length() {

		return lengthh;   // placeholder

	}

	
	int  give_buzzer() {
		//works dont change you buffoon
		Node *forVector = nullptr;
		BuzzerNode *b = new BuzzerNode;

		if (buzzerBucket.front == NULL) {
			forVector =	pushback(waitingList, counter);
			// space for adding to vector
			b->inBucket = false;
			b->location = forVector;
			fullList.push_back(b);
			//space for adding to vector
			counter++;
			lengthh++;
			return counter - 1;
		}
		else {
			
			forVector = popfront(buzzerBucket);
			forVector = pushback(waitingList, forVector->buzzerNumber);
			//editiing for adding to vector
			fullList[forVector->buzzerNumber]->inBucket = false;
			fullList[forVector->buzzerNumber]->location = forVector;

			lengthh++;
			return forVector->buzzerNumber;
		}
		return 0;  // placeholder
	}

	int seat() {
		if (waitingList.front == NULL) {
		//	cout << "no ones in line birdbrain" << endl;
			return -1;
		}
		else {

			Node* op =popfront(waitingList);
			tmp = op->buzzerNumber;
			//cout << " pop number " <<  tmp << endl;
	    	Node* final =pushFront(buzzerBucket, tmp);
			//editing for adding to vector
			fullList[final->buzzerNumber]->location = final;
			fullList[final->buzzerNumber]->inBucket = true;

			lengthh--;
			return tmp;

		}
		return -1;  // placeholder
	}

	bool kick_out(int buzzer) {

		if (buzzer > counter) {
     		return false;
	}
		BuzzerNode* b = fullList[buzzer];
		Node* temp = new Node;

		temp = b->location;
	
		
		if (b->inBucket == false) {
			
			if (temp == NULL) {
			//	cout << "NULL" << endl;
				return false;
			}
			lengthh--;
			if (temp->next == NULL) {
			//	cout << "last element" << endl;
				Node *veryTmp =waitingList.tail;
				waitingList.tail = waitingList.tail->prev;
				waitingList.tail->next = NULL;
				pushFront(buzzerBucket, veryTmp->buzzerNumber);
				//update the vector
				fullList[buzzer]->location = veryTmp;
				fullList[buzzer]->inBucket = true;
				return true;
			}
			if (temp->prev == NULL) {
				//cout << "first element" << endl;
				//pop front;
				Node *veryTemp;
				veryTemp = popfront(waitingList);
				pushFront(buzzerBucket, veryTemp->buzzerNumber);
				// update the vector shiza
				fullList[buzzer]->location = veryTemp;
				fullList[buzzer]->inBucket = true;

				return true;
			}

			else {
				Node *veryTemp;
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				veryTemp =pushFront(buzzerBucket, temp->buzzerNumber);
				//update the fing thing one more time
				fullList[buzzer]->location = veryTemp;
				fullList[buzzer]->inBucket = true;
				//
				//cout << "middle element" << endl;
 				return true;
			}
		}
		cout << "In the buzzer bucket lol, How you going to kick out a buzzer" << endl;
		return false;
	}

	
	bool take_bribe(int k) {
		
		if (k == waitingList.front->buzzerNumber) {
		//	cout << "already at the front" << endl;
			return true; 
		}
		if (k < counter && fullList[k]->inBucket == false) {	
			kick_out(k);

			Node* verytmp =pushFront(waitingList, k);
			popfront(buzzerBucket);
			fullList[k]->inBucket = false;
			fullList[k]->location = verytmp;
			lengthh++;
			return true;  // placeholder
		}
		return false;
	}
};   // end ServiceQueue class

#endif
