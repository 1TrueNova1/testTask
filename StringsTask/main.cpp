#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <map>

namespace PortaOneTask
{

	//overload for the first part of task
	char firstUniqueLetter(const std::string& text)
	{
		std::unordered_set<char> checkedLetters;

		char current;
		int count;
		bool repeats;
		for (size_t i = 0; i < text.size(); ++i)
		{
			current = text[i];
			//if letter was already observed, can skip iteration
			if (checkedLetters.find(current) != checkedLetters.end())
				continue;
			count = 0;
			repeats = false;
			for (size_t j = i + 1; j < text.size(); ++j)
			{
				if (text[j] == current) [[unlikely]]
				{
					repeats = true;
					break;
				}
			}
			if (!repeats)
				return current;

			checkedLetters.insert(current);
		}
		return 0;
	}


	//overload for the second part of task
	char firstUniqueLetter(const std::vector<char>& letters)
	{
		std::unordered_set<char> checkedLetters;

		char current;
		int count;
		bool repeats;
		for (size_t i = 0; i < letters.size(); ++i)
		{
			current = letters[i];
			if (current == 0) [[unlikely]]
				continue;
			//if letter was already observed, can skip iteration
			if (checkedLetters.find(current) != checkedLetters.end())
				continue;
			count = 0;
			repeats = false;
			for (size_t j = i + 1; j < letters.size(); ++j)
			{
				if (letters[j] == current) [[unlikely]]
				{
					repeats = true;
					break;
				}
			}
			if (!repeats)
				return current;

			checkedLetters.insert(current);
		}
		return 0;
	}

	auto stringToLower = [](std::string& text) {
		std::transform(text.begin(), text.end(), text.begin(), [](unsigned char symbol) {
			return std::tolower(symbol);
			});
	};

	auto firstNotLetter = [](const std::string& text, uint32_t offset) -> int32_t {
		for (size_t i = offset; i < text.size(); ++i)
		{
			if (!isalpha(text[i]))
				return i;
		}
		return -1;
	};

	auto firstLetter = [](const std::string& text, uint32_t offset) -> int32_t {
		for (size_t i = offset; i < text.size(); ++i)
		{
			if (isalpha(text[i]))
				return i;
		}
		return -1;
	};
}

//compile under /std::c++latest for everything to work or at least /std::c++17 for it to build
int main()
{
	{
		using namespace PortaOneTask;

		std::string text = "..Ave aMmria, Deus Vult";
		stringToLower(text);

		std::vector<char> letters;
		letters.reserve(text.size() / 30);

		std::string word;
		word.reserve(30);

		int32_t prev, space;
		//skip first symbols if they are not letters
		prev = space = firstLetter(text, 0);
		while (space != -1) {
			space = firstNotLetter(text, space);
			//special case for last word
			if (space == -1) [[unlikely]] {
				word = text.substr(prev, text.size() - 1);
				letters.push_back(firstUniqueLetter(word));
				break;
			}
			word = text.substr(prev, space - prev);
			letters.push_back(firstUniqueLetter(word));
			prev = space = firstLetter(text, space);
		}

		std::cout << text << "\n";
		for (char letter : letters)
		{
			if (letter != 0)
				std::cout << letter;
			else
				std::cout << "No unique letter";
			std::cout << "\n";
		}
		std::cout << "\n\n";

		char result = firstUniqueLetter(letters);

		if (result != 0)
			std::cout << result;
		else
			std::cout << "No unique letter";
	}

	return 0;
}