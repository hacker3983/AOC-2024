#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdbool>

bool adjacentdiff_valid(std::vector<int> report, size_t index) {
	int difference = abs(report[index] - report[index-1]);
	return difference <= 3 && difference >= 1;
}

bool isascending_adjdiff(std::vector<int> report) {
	for(size_t i=1;i<report.size();i++) {
		if(report[i] <= report[i-1]) {
			return false;
		}
		if(!adjacentdiff_valid(report, i)) {
			return false;
		}
	}
	return true;
}

bool isdescending_adjdiff(std::vector<int> report) {
	for(size_t i=1;i<report.size();i++) {
		if(report[i] >= report[i-1]) {
			return false;
		}
		if(!adjacentdiff_valid(report, i)) {
			return false;
		}
	}
	return true;
}

bool report_issafe(std::vector<int> report) {
	return isascending_adjdiff(report) || isdescending_adjdiff(report);
}

bool report_issafe_remove(std::vector<int> report) {
	std::vector<int> new_report;
	for(size_t i=0;i<report.size();i++) {
		for(size_t j=0;j<report.size();j++) {
			if(j == i) {
				continue;
			}
			new_report.push_back(report[j]);
		}
		if(report_issafe(new_report)) {
			new_report.clear();
			return true;
		}
		new_report.clear();
	}
	return false;
}

int main() {
	std::ifstream file("../puzzle.txt");
	std::string line;
	std::vector<int> report;
	int safe_count = 0;
	while(std::getline(file, line)) {
		std::stringstream ss(line);
		int level;
		while(ss >> level) {
			report.push_back(level);
		}
		if(report_issafe(report)) {
			safe_count++;
		} else if(report_issafe_remove(report)) {
			safe_count++;	
		}
		report.clear();
	}
	std::cout << "There are " << safe_count << " safe reports" << std::endl;
	file.close();
}
