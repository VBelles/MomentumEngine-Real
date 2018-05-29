#include "mcv_platform.h"
#include "template_engine.h"
#include <regex>

using namespace VTemplate;

void VTemplate::compileTemplate(std::string templateText, TTemplate * t) {
	std::regex re("\\{\\{(.+?)\\}\\}");
	std::sregex_token_iterator iter(templateText.begin(), templateText.end(), re, { -1, 0, 1 });
	std::sregex_token_iterator end;
	bool nextIsVar = false;
	for (; iter != end; ++iter) {
		if (nextIsVar) { //Variable
			t->texts.push_back(new TemplateVariable(iter->str()));
			nextIsVar = false;
		}
		else if (std::regex_search(iter->str(), re)) { //Next one is variable
			nextIsVar = true;
		}
		else { //Just text
			t->texts.push_back(new TTemplateText(iter->str()));
			nextIsVar = false;
		}
	}
}
