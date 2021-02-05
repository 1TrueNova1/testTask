#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <thread>

namespace PortaOneTask
{

	char firstUniqueLetter(const std::string& text)
	{
		//to count how many times each letter was found
		std::unordered_map<char, uint16_t> map;
		//and to keep sight on the ordering
		std::vector<char> order;

		for (char letter : text)
		{
			if (map.find(letter) != map.end())
				map[letter]++;
			else {
				map.insert(std::make_pair(letter, 1));
				order.push_back(letter);
			}
		}

		for (char letter : order)
		{
			if (map[letter] == 1)
				return letter;
		}

		//naught if no unique letter exists
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

		std::string text = R"(aabbcb, c)";

		//uncomment for case insensitivity
		//stringToLower(text);

		const int lettersInWord = 20;
		std::string letters;
		letters.reserve(text.size() / lettersInWord);

		std::string word;
		word.reserve(lettersInWord);

		int32_t prev, space;
		char result;



		//skip first symbols if they are not letters
		prev = space = firstLetter(text, 0);
		while (space != -1) {
			space = firstNotLetter(text, space);
			//special case for last word
			if (space == -1) [[unlikely]] {
				word = text.substr(prev, text.size() - 1);
				if ((result = firstUniqueLetter(word)) != 0)
					letters.push_back(result);
				break;
			}
			word = text.substr(prev, space - prev);
			if ((result = firstUniqueLetter(word)) != 0)
				letters.push_back(result);
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

		result = firstUniqueLetter(letters);

		if (result != 0)
			std::cout << result;
		else
			std::cout << "No unique letter";
	}

	return 0;
}