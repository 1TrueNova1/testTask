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
		//and to keep track of the ordering
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

	auto findUniqueInRange = [](const std::string& text, int32_t start, int32_t end)
	{
		std::string letters;
		std::string word;
		word.reserve(30);
		int32_t prev, space;
		char result;

		//skip first symbols if they are not letters
		prev = space = firstLetter(text, start);
		while (space != -1 && space < end) {
			space = firstNotLetter(text, space);
			//special case for last word
			if (space == -1 && space < end) [[unlikely]] {
				word = text.substr(prev, end - 1);
				if ((result = firstUniqueLetter(word)) != 0)
					letters.push_back(result);
				break;
			}
				//substring word from text and find unique letters in it
			word = text.substr(prev, space - prev);
			if ((result = firstUniqueLetter(word)) != 0)
				letters.push_back(result);
			prev = space = firstLetter(text, space);
		}
		return firstUniqueLetter(letters);
	};
}

//compile under /std::c++latest for everything to work or at least /std::c++17 for it to build
int main()
{
	{
		using namespace PortaOneTask;

		std::cout << "Please, enter some text: \n";

		std::string text;
		std::getline(std::cin, text);

		//uncomment for case insensitivity
		//stringToLower(text);

		char result = 0;

		if (text.size() < 1000)
		{
			result = findUniqueInRange(text, 0, text.size());
		}
		else
		{
			//switch to multithreaded arhitecture if text is quite big
			std::vector<std::thread> threads;

			size_t nthreads = std::thread::hardware_concurrency();
			threads.reserve(nthreads);

			std::string letters;
			letters.resize(nthreads);

			size_t step = text.size() / nthreads;
			size_t lastStart = 0, lastEnd = step;
			std::vector<size_t> start(nthreads), end(nthreads);

			auto threadWork = [&text, &start, &end, &letters](size_t index) {
				letters[index] = findUniqueInRange(text, start[index], end[index]);
			};

			//diving the text on smaller batches and running a new thread for each one
			for (size_t i = 0; i < nthreads; ++i)
			{
				start[i] = lastStart;
				end[i] = lastEnd;
				lastStart = lastEnd;
				lastEnd += step;
				threads.push_back(std::move(std::thread(threadWork, i)));
			}

			for (std::thread& thread : threads)
			{
				thread.join();
			}

			result = firstUniqueLetter(letters);
		}

		if (result != 0)
			std::cout << result;
		else
			std::cout << "No unique letter found";
	}

	return 0;
}