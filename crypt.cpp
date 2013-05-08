/*
Copyright 2013 by Henrik Mühe and Florian Funke

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <set>
using namespace std;


std::vector<uint8_t> generatePermutation() {
	std::vector<uint8_t> result;
	for (uint64_t index=0;index<='z'-'a';++index)
		result.push_back(index);
	std::random_shuffle(result.begin(),result.end());
	return std::move(result);
}

std::vector<uint8_t> reversePermutation(const std::vector<uint8_t>& perm) {
	std::vector<uint8_t> result(perm.size());
	for (uint64_t index=0;index<='z'-'a';++index)
		result[perm[index]]=index;
	return std::move(result);
}

std::vector<uint64_t> histogram(const std::string& str) {
	std::vector<uint64_t> result('z'-'a'+1);
	for (auto c : str)
		if (c>='a'&&c<='z')
			++result[c-'a'];
	return std::move(result);
}

void showHistogram(const std::string& str) {
	auto h=histogram(str);
	auto m=max_element(h.begin(),h.end());


	std::vector<char> letters;
	for (char c='a';c<='z';++c) letters.push_back(c);
	std::sort(letters.begin(),letters.end(),[&h](char c1,char c2) {
		return h[c1-'a']>h[c2-'a'];
	});

	for (char c : letters) {
		cout << c << "  ";
		cout << setw(5) << right << h[c-'a'] << "  ";
		cout << string((h[c-'a']/(double)*m)*100,'#');
		cout << endl;
	}
}

std::string applyPermutation(std::string str,const std::vector<uint8_t>& permutation) {
	for (auto& c : str)
		if (c>='a'&&c<='z')
			c=permutation[c-'a']+'a';
	return str;
}

template<class T>
std::ostream& operator<<(std::ostream& out,const std::vector<T>& vec) {
	for (auto& item : vec)
		out << item;
	return out;
}

template<>
std::ostream& operator<<(std::ostream& out,const std::vector<uint8_t>& vec) {
	for (auto& item : vec)
		out << (char)(item+'a');
	return out;
}

std::vector<uint8_t> readPermuation(const std::string& fileName) {
	std::vector<uint8_t> perm('z'-'a'+1);
	std::ifstream inFile(fileName.c_str());
	std::string arrow;
	char from,to;
	std::set<char> in, out;
	for (char c='a';c<='z';++c) {
		in.insert(c);
		out.insert(c);
	}
	while (inFile >> from >> arrow >> to) {
		auto inIter=in.find(from);
		auto outIter=out.find(to);
		assert(inIter!=in.end());
		assert(outIter!=out.end());
		in.erase(inIter);
		out.erase(outIter);
		perm[from-'a'] = to-'a';
	}
	assert(!in.size());
	assert(!out.size());
	return std::move(perm);
}

std::string readText() {
	std::string result, tmp;
	while (cin >> tmp)
		result+=tmp+" ";
	return std::move(result);
}

// cat kafka.txt | sed 's#[üÜ]#ue#g' | sed 's#[äÄ]#ae#g' | sed 's#[öÖ]#oe#g' | sed 's#ß#ss#g'  | sed 's#[^a-zA-Z \n]##g' | tr '[:upper:]' '[:lower:]' | tr '\n' ' ' | pbcopy
int main(int argc,char* argv[]) {

	if (argc < 2) {
		std::cerr << argv[0] << " <command> [<permFile>]" << endl;
		return -1;
	}


	std::string text=readText();


	if (string(argv[1])=="histogram") {
		showHistogram(text);
	} else if (string(argv[1])=="encrypt") {
		auto vec=generatePermutation();
		cout << applyPermutation(text,vec) << endl;
	} else if (string(argv[1])=="decrypt") {
		if (argc < 3) {
			std::cerr << argv[0] << " decrypt <permFile>" << endl;
			return -1;
		}
		auto decrypt=readPermuation(argv[2]);
		cout << applyPermutation(text, decrypt) << endl;
	}

	/*srand(time(nullptr));
	auto vec=generatePermutation();
	cout << applyPermutation(text,vec) << endl << endl;
	cout << applyPermutation(applyPermutation("hello",vec),revperm) << endl;
	showHistogram(text);*/
}
