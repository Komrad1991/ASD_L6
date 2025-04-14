#include <iostream>
#include <map>
#include <fstream>
#include <set>
#include <queue>
#include <cctype>
#include <algorithm>
#include <numeric>
#include <string>

class comp1
{
public:
	bool operator() (std::pair<std::string, int>& p1, std::pair<std::string, int>& p2)
	{
		return p1.second < p2.second;
	}
};

void manyestWords(std::string fileName)
{
	std::fstream file;
	file.open(fileName);
	if (file.fail()) throw std::exception("no file");
	std::string word;
	std::map<std::string, int> map;
	while (file >> word)
	{
		if (map.contains(word)) ++map[word];
		else map[word] = 1;
	}
	std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, comp1> res(map.begin(), map.end());
	for (int i = 0; i < 40; i++)
	{
		auto r = res.top();
		res.pop();
		std::cout << r.first << " : " << r.second << "\n";
	}
}

void manyestSelfWords(std::string fileName)
{
	std::fstream file;
	file.open(fileName);
	if (file.fail()) throw std::exception("no file");
	std::string word;
	std::map<std::string, int> map;
	while (file >> word)
	{
		if ('A' <= word[0] && 'Z' >= word[0])
		{
			if (map.contains(word)) ++map[word];
			else map[word] = 1;
		}
	}
	std::priority_queue<std::pair<std::string, int>, std::vector<std::pair<std::string, int>>, comp1> res(map.begin(), map.end());
	for (int i = 0; i < 20; i++)
	{
		auto r = res.top();
		res.pop();
		std::cout << r.first << " : " << r.second << "\n";
	}
}

std::map<char, int> convertToMap(std::string& str)
{
	std::map<char, int> res;
	for (int i = 0; i < str.size(); i++)
	{
		if (res.contains(str[i])) ++res[str[i]];
		else res[str[i]] = 1;
	}
	return res;
}

class group
{
public:
	bool operator() (std::pair<std::map<char, int>, std::set<std::string>>& p1, std::pair<std::map<char, int>, std::set<std::string>>& p2)
	{
		return p1.second.size() < p2.second.size();
	}
};

class biggestWord
{
public:
	bool operator() (std::pair<std::map<char, int>, std::set<std::string>>& p1, std::pair<std::map<char, int>, std::set<std::string>>& p2)
	{
		auto lambda = [](int acc, std::pair<char, int> p) {return acc + p.second; };
		return std::accumulate(p1.first.begin(), p1.first.end(), 0, lambda) < std::accumulate(p2.first.begin(), p2.first.end(), 0, lambda);
	}
};

void anagrams(std::string fileName)
{
	std::fstream file;
	file.open(fileName);
	if (file.fail()) throw std::exception("no file");
	std::string word;
	std::map<std::map<char,int>, std::set<std::string>> map;
	while (file >> word)
	{
		map[convertToMap(word)].insert(word);
	}
	std::priority_queue<std::pair<std::map<char, int>, std::set<std::string>>, std::vector<std::pair<std::map<char, int>, std::set<std::string>>>, group> bigGroup(map.begin(), map.end());
	std::priority_queue<std::pair<std::map<char, int>, std::set<std::string>>, std::vector<std::pair<std::map<char, int>, std::set<std::string>>>, biggestWord> bigWord(map.begin(), map.end());
	auto set_res = bigGroup.top().second;
	auto map_res = bigWord.top().second;
	while (map_res.size() < 2)
	{
		bigWord.pop();
		map_res = bigWord.top().second;
	}
	std::cout << "Biggest group of anagrams: " << std::endl;
	for (auto& x : set_res)
	{
		std::cout << x << " ";
	}
	std::cout << "\nBiggest anagrams: " << std::endl;
	for (auto& x : map_res)
	{
		std::cout << x << " ";
	}
}

std::vector <std::string> parseLine(std::string& str)
{
	std::vector<std::string> res;
	size_t start = 0;
	size_t end = 0;
	while ((end = str.find(',', start)) != std::string::npos) {
		res.push_back(str.substr(start, end - start));
		start = end + 1;
	}
	res.push_back(str.substr(start));
	return res;
}

void global_sales(std::string fileName)
{
	std::fstream file;
	file.open(fileName);
	if (file.fail()) throw std::exception("no file");
	std::string line;
	std::getline(file, line); //garbage at 0 line
	std::map<std::pair<std::string, std::string>, double> statistic;
	//9 - global sales, 4 - publisher, 3 - genre
	int i = 0;
	while (std::getline(file,line))
	{
		if (line.length() > 1)
		{
			auto parsedLine = parseLine(line);
			auto pubGen = std::make_pair(parsedLine[4], parsedLine[3]);
			statistic[pubGen] += strtod(parsedLine[9].c_str(), NULL);
		}
	}
	for (auto& x : statistic)
	{
		std::cout << x.first.first << " in " << x.first.second << " is " << x.second << "\n";
	}
}

void best_genre(std::string fileName)
{
	std::fstream file;
	file.open(fileName);
	if (file.fail()) throw std::exception("no file");
	std::string line;
	std::getline(file, line); //garbage at 0 line
	std::map<std::string, double> statistic;
	//9 - global sales, 4 - publisher, 3 - genre
	int i = 0;
	while (std::getline(file, line))
	{
		if (line.length() > 1)
		{
			auto parsedLine = parseLine(line);
			
			statistic[parsedLine[3]] += strtod(parsedLine[9].c_str(), NULL);
		}
	}
	auto lambda = [](const std::pair<std::string, double> p1, const std::pair<std::string, double> p2) -> bool {return p1.second < p2.second; };
	auto m = std::max_element(statistic.begin(), statistic.end(), lambda);
	std::cout << (*m).first << " " << (*m).second << std::endl;
}

class sortDevs
{
public:
	bool operator() (const std::pair<std::string, double> p1, const std::pair<std::string, double> p2)
	{
		return p1.second < p2.second;
	}
};

void best_dev(std::string fileName)
{
	std::fstream file;
	file.open(fileName);
	if (file.fail()) throw std::exception("no file");
	std::string line;
	std::getline(file, line); //garbage at 0 line
	std::map<std::string, std::map<std::string,double>> statistic;
	//9 - global sales, 4 - publisher, 3 - genre
	int i = 0;
	while (std::getline(file, line))
	{
		if (line.length() > 1)
		{
			auto parsedLine = parseLine(line);
			(statistic[parsedLine[4]])[parsedLine[14]] += std::strtod(parsedLine[9].c_str(),NULL);
		}
	}
	for (auto x : statistic)
	{
		std::priority_queue<std::pair<std::string, double>, std::vector<std::pair<std::string, double>>, sortDevs> devHeap(x.second.begin(), x.second.end());
		std::cout << "Best devs of " << x.first << " : ";
		for (int i = 0; i < 5; i++)
		{
			if (devHeap.size() > 0)
			{
				auto currTop = devHeap.top();
				devHeap.pop();
				std::cout << currTop.first << " ";
			}
		}
		std::cout << "\n";
	}
}

int main()
{
	std::cout << "-------TASK1---------" << std::endl;
	manyestWords("holmes.txt");
	std::cout << "-------TASK2---------" << std::endl;
	manyestSelfWords("holmes.txt");
	std::cout << "-------TASK3---------" << std::endl;
	anagrams("holmes.txt");
	std::cout << "\n-------TASK4---------" << std::endl;
	global_sales("Video_Games.csv");
	std::cout << "-------TASK5---------" << std::endl;
	best_genre("Video_Games.csv");
	std::cout << "-------TASK6---------" << std::endl;
	best_dev("Video_Games.csv");
}



