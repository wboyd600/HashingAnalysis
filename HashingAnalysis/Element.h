class Element {
public:
	Element * next = NULL;
	int hash;
};
Element* AddElement(Element* head, int value)
{
	Element * traverse = head;
	if (traverse == NULL) {
		Element * guy = new Element;
		guy->hash = value;
		return guy;
	}
	else {
		while (traverse != NULL) {
			if (traverse->next == NULL) {
				Element * guy = new Element;
				guy->hash = value;
				traverse->next = guy;
				return head;
			}
			traverse = traverse->next;
		}
	}
	return head;
}
bool HasNext(Element * head) {
	if (head != NULL) {
		if (head->next != NULL) {
			return true;
		}
	}
	return false;
}