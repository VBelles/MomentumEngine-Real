#pragma once

namespace VTemplate {

	using VariableCallback = std::function<std::string(std::string)>;

	struct TTemplateAny {
		virtual std::string toString(VariableCallback& variableCallback) = 0;
	};

	struct TTemplateText : TTemplateAny {
		std::string text;
		TTemplateText(std::string text) : text(text) {}
		std::string toString(VariableCallback& variableCallback) override { return text; }
	};

	struct TemplateVariable : TTemplateAny {
		std::string name;
		TemplateVariable(std::string name) : name(name) {}
		std::string toString(VariableCallback& variableCallback) override { return variableCallback(name); }
	};

	struct TTemplate {
		std::vector<TTemplateAny*> texts;
		std::string toString(VariableCallback& variableCallback) {
			std::string s;
			for (auto text : texts) s += text->toString(variableCallback);
			return s;
		}
		~TTemplate() {
			for (auto& text : texts) {
				delete text;
			}
		}
	};


	void compileTemplate(std::string templateText, TTemplate* t);
}