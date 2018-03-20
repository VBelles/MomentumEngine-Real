#include "mcv_platform.h"
#include "IBehaviorTree.h"

void IBehaviorTree::create(std::string s) {
	name = s;
}

BehaviorTreeNode *IBehaviorTree::createNode(std::string s) {
	if (findNode(s) != NULL) {
		printf("Error: node %s already exists\n", s.c_str());
		return NULL;	// error: node already exists
	}
	BehaviorTreeNode *btn = new BehaviorTreeNode(s);
	tree[s] = btn;
	return btn;
}

BehaviorTreeNode *IBehaviorTree::findNode(std::string s) {
	if (tree.find(s) == tree.end()) return NULL;
	else return tree[s];
}

BehaviorTreeNode *IBehaviorTree::createRoot(std::string s, int type, BehaviorTreeCondition btc, BehaviorTreeAction bta) {
	BehaviorTreeNode *r = createNode(s);
	r->setParent(NULL);
	root = r;
	r->setType(type);
	if (btc != NULL) addCondition(s, btc);
	if (bta != NULL) addAction(s, bta);

	current = NULL;
	return r;
}

BehaviorTreeNode *IBehaviorTree::addChild(std::string parent, std::string son, int type, BehaviorTreeCondition btc, BehaviorTreeAction bta) {
	BehaviorTreeNode *p = findNode(parent);
	BehaviorTreeNode *s = createNode(son);
	p->addChild(s);
	s->setParent(p);
	s->setType(type);
	if (btc != NULL) addCondition(son, btc);
	if (bta != NULL) addAction(son, bta);
	return s;
}

void IBehaviorTree::recalc() {
	if (current == NULL) root->recalc(this);	// I'm not in a sequence, start from the root
	else current->recalc(this);				// I'm in a sequence. Continue where I left
}

void IBehaviorTree::setCurrent(BehaviorTreeNode *nc) {
	current = nc;
}

void IBehaviorTree::addCondition(std::string s, BehaviorTreeCondition cond) {
	if (conditions.find(s) != conditions.end()) {
		printf("Error: node %s already has a condition\n", s.c_str());
		return;	// if condition already exists don't insert again...
	}
	conditions[s] = cond;
}

bool IBehaviorTree::testCondition(std::string s) {
	if (conditions.find(s) == conditions.end()) {
		return true;	// error: no condition defined, we assume TRUE
	}
	return (this->*conditions[s])();
}

void IBehaviorTree::addAction(std::string s, BehaviorTreeAction act) {
	if (actions.find(s) != actions.end()) {
		printf("Error: node %s already has an action\n", s.c_str());
		return;	// if action already exists don't insert again...
	}
	actions[s] = act;
}

int IBehaviorTree::execAction(std::string s) {
	if (actions.find(s) == actions.end()) {
		printf("ERROR: Missing node action for node %s\n", s.c_str());
		return LEAVE; // error: action does not exist
	}
	return (this->*actions[s])();
}