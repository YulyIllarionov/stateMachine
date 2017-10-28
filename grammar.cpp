
#include "stdafx.h"
#include "grammar.h"
#include <iostream>
#include <algorithm>  
#include <fstream>
#include <cmath>
#include <ctime>

using namespace std;

bool inserts(std::vector<std::string> first, std::vector<std::string> second)
{
	for (unsigned i = 0; i < first.size(); i++)
		for (unsigned j = 0; j < second.size(); j++)
			if (first[i] == second[j])
				return true;
	return false;
}

std::string getNumber(std::string& letter)
{
	unsigned i;
	for (i = 0; i < letter.length(); i++)
	{
		if (((letter[i] < 'a') || (letter[i] > 'z')) && ((letter[i] < 'A') || (letter[i] > 'Z')))
			break;
	}
	if (i == letter.length())
		return "";
	else
	{
		std::string idx = letter.substr(i);
		letter.erase(letter.begin() + i, letter.end());
		return idx;
	}
}

bool minimumNonterminalLetter(const nonterminalLetter &a, const nonterminalLetter &b)
{
	unsigned beginA = 0;
	unsigned endA = 0;
	unsigned beginB = 0;
	unsigned endB = 0;

	while ((endA < a.length()) && (endB < b.length()))
	{
		for (unsigned i = beginA; i < a.length(); i++)
		{
			if ((a[i] >= '0') && (a[i] <= '9'))
			{
				beginA = i;
				break;
			}
		}
		endA = a.length();
		for (unsigned i = beginA; i < a.length(); i++)
		{
			if ((a[i] < '0') || (a[i] > '9'))
			{
				endA = i;
				break;
			}
		}

		for (unsigned i = beginB; i < b.length(); i++)
		{
			if ((b[i] >= '0') && (b[i] <= '9'))
			{
				beginB = i;
				break;
			}
		}
		endB = b.length();
		for (unsigned i = beginB; i < b.length(); i++)
		{
			if ((b[i] < '0') || (b[i] > '9'))
			{
				endB = i;
				break;
			}
		}

		int first = stoi(a.substr(beginA, endA - beginA));
		int second = stoi(b.substr(beginB, endB - beginB));

		if (first < second)
			return true;
		if (first > second)
			return false;
	}
	return (a.size() < b.size());
}

bool minimumFirstNonterminalLetter(const nonterminalLettersContainer &a, const nonterminalLettersContainer &b)
{
	return minimumNonterminalLetter(a[0], b[0]);
}

bool maximumPairDistance(const std::pair<int, int> &a, const std::pair<int, int> &b)
{
	return ((a.second - a.first) > (b.second - b.first));
}

std::string letterToTex(std::string letter)
{
	if (letter.empty())
		return letter;
	string idx = getNumber(letter);
	return "{" + letter + "_" + "{" + idx + "}" + "}";
}

std::string toTexTableCell(std::string text)
{
	return "\\(" + text + "\\)";
}

std::string letterToDot(std::string letter, bool negation, int fontSize)
{
	if (letter.empty())
		return letter;
	std::string idx = getNumber(letter);
	if (idx.empty())
		return letter;
	std::string toDot = "";
	if (!negation)
		toDot += "<O>";
	toDot += letter;
	if (!negation)
		toDot += "</O>";
	toDot += "<sub>";
	toDot += "<font point-size=\"";
	toDot += to_string(fontSize);
	toDot += "\">" + idx + "</font>" + "</sub>";
	return toDot;
}

void triggerFromDTableToTex(std::string fileName)
{
	ofstream texFile(fileName);
	if (!texFile.is_open())
		return;

	texFile << "\\begin{tabular}{" << "|c|";
	for (unsigned i = 0; i < triggerFromDTable.size(); i++)
		texFile << "c|";
	texFile << "}" << endl;

	texFile << "\\hline" << endl;

	texFile << toTexTableCell("Q_t") << " & ";
	texFile << toTexTableCell("Q_{t+1}");
	for (unsigned i = 1; i < triggerInputsString.size(); i++)
		texFile << " & " << toTexTableCell(triggerInputsString[i]);
	texFile << "\\\\" << "\\hline" << endl;

	for (unsigned i = 0; i < triggerFromDTable.size(); i++)
	{
		for (unsigned j = 0; j < triggerFromDTable[i].size(); j++)
		{
			if (j == 0)
				texFile << to_string(triggerFromDTable[i][j]);
			else
			{
				if (triggerFromDTable[i][j] == white)
					texFile << "0";
				if (triggerFromDTable[i][j] == black)
					texFile << "1";
				if (triggerFromDTable[i][j] == any)
					texFile << "*";
				if (triggerFromDTable[i][j] == form)
					texFile << toTexTableCell("\\alpha");
				if (triggerFromDTable[i][j] == formInv)
					texFile << toTexTableCell("\\xoverline{\\alpha}");
			}
			texFile << " ";
			if (j < (triggerFromDTable[i].size() - 1))
				texFile << "& ";
		}
		texFile << "\\\\" << " \\hline" << endl;
	}
	texFile << "\\end{tabular}" << endl;
	texFile.close();
}

std::string toQuotes(std::string text)
{
	return "<" + text + ">";
}

inferenceRule::inferenceRule(nonterminalLetter left_, terminalLettersContainer condition_)
{
	left = left_;
	condition = condition_;
}

inferenceRule::inferenceRule(nonterminalLetter left_, terminalLettersContainer condition_, nonterminalLetter right_)
{
	left = left_;
	condition = condition_;
	right = right_;
}

std::string inferenceRule::toTexString()
{
	std::string texString = "";
	//texString += " \\begin{ equation} ";
	texString += letterToTex(this->left);
	texString += "\\rightarrow{";
	for (unsigned i = 0; i < condition.size(); i++)
		texString += letterToTex(condition[i]);
	if (!right.empty())
		texString += (" " + letterToTex(this->right));
	texString += "}";
	//texString += " \\end{ equation } ";
	return texString;
}

void inferenceRule::print()
{
	cout << left;
	cout << "->";
	for (unsigned i = 0; i < condition.size(); i++)
		cout << condition[i];
	if (!right.empty())
		cout << " " << right;
	//cout << "\t";
}

void inferenceRule::changeNontetminal(terminalLettersContainer from, terminalLettersContainer to)
{
	int first = find(from.begin(), from.end(), left) - from.begin();
	int second = find(from.begin(), from.end(), right) - from.begin();
	left = to[first];
	right = to[second];
}

void inferenceRule::addRightIfEmpty(nonterminalLetter newRight)
{
	if (right.empty())
		right = newRight;
}

bool inferenceRule::similar(const inferenceRule& other)
{
	if (condition.size() != other.condition.size())
		return false;
	for (unsigned i = 0; i < condition.size(); i++)
		if (condition[i] != other.condition[i])
			return false;
	if (left != other.left)
		return false;
	//if (right != other.right)
	//    return false;

	return true;
}

bool inferenceRule::operator < (const inferenceRule& other)
{
	return (this->left < other.left);
}

bool inferenceRule::operator == (const inferenceRule& other)
{
	if ((this->left == other.left) &&
		(this->right == other.right) &&
		(this->condition == other.condition))
		return true;
	return false;

}


void linearToAutomatonGrammar(nonterminalLettersContainer inputNonterminalDict, inferenceRulesContainer inputRules,
	nonterminalLettersContainer& outputNonterminalDict, inferenceRulesContainer& outputRules)
{
	vector<unsigned> nonterminalLetterNumber(inputNonterminalDict.size(), 0);
	for (unsigned i = 0; i < inputRules.size(); i++)
	{
		if (inputRules[i].condition.size() <= 1)
		{
			outputRules.push_back(inputRules[i]);
		}
		else
		{
			nonterminalLetter currentLeft = inputRules[i].left;
			terminalLettersContainer currentCondition;
			nonterminalLetter currentRight;
			int numberInNonterminalLettersContainer = find(inputNonterminalDict.begin(),
				inputNonterminalDict.end(), currentLeft) - inputNonterminalDict.begin();

			for (unsigned j = 0; j < inputRules[i].condition.size() - 1; j++)
			{
				currentRight = inputRules[i].left + to_string(
					++nonterminalLetterNumber[numberInNonterminalLettersContainer]);
				currentCondition = terminalLettersContainer({ inputRules[i].condition[j] });
				outputRules.push_back(inferenceRule(currentLeft, currentCondition, currentRight));
				currentLeft = currentRight;
			}
			currentCondition = terminalLettersContainer({
				inputRules[i].condition[inputRules[i].condition.size() - 1] });
			currentRight = inputRules[i].right;
			outputRules.push_back(inferenceRule(currentLeft, currentCondition, currentRight));
		}
	}
	for (unsigned i = 0; i < nonterminalLetterNumber.size(); i++)
	{
		outputNonterminalDict.push_back(inputNonterminalDict[i]);
		for (unsigned j = 0; j < nonterminalLetterNumber[i]; j++)
			outputNonterminalDict.push_back(inputNonterminalDict[i] + to_string(j + 1));
	}
}

graph::graph(nonterminalLettersContainer nodesNames_, terminalLettersContainer edgesNames_, inferenceRulesContainer connections_, nonterminalLettersContainer specialStates_)
{
	nodesNames = nodesNames_;
	edgesNames = edgesNames_;
	connections = connections_;
	specialStates = specialStates_;
}

graph& graph::operator = (const graph& other)
{
	this->connections = other.connections;
	this->edgesNames = other.edgesNames;
	this->nodesNames = other.nodesNames;
	return *this;
}

void graph::toDotFile(std::string fileName)
{
	terminalLettersContainer dot(nodesNames.size(), "node");
	for (unsigned i = 0; i < dot.size(); i++)
		dot[i] += to_string(i);

	inferenceRulesContainer connectionsToDraw(connections);
	for (unsigned i = 0; i < connectionsToDraw.size(); i++)
	{
		for (unsigned j = i + 1; j < connectionsToDraw.size(); j++)
		{
			if ((connectionsToDraw[i].left == connectionsToDraw[j].left) &&
				(connectionsToDraw[i].right == connectionsToDraw[j].right))
			{
				connectionsToDraw[i].condition.insert(connectionsToDraw[i].condition.end(),
					connectionsToDraw[j].condition.begin(), connectionsToDraw[j].condition.end());
				connectionsToDraw.erase(connectionsToDraw.begin() + j);
				sort(connectionsToDraw[i].condition.begin(), connectionsToDraw[i].condition.end(), minimumNonterminalLetter);
			}
		}
	}
	for (unsigned i = 0; i < connectionsToDraw.size(); i++)
	{
		for (unsigned j = 1; j < connectionsToDraw[i].condition.size(); j += 2)
		{
			connectionsToDraw[i].condition.insert(connectionsToDraw[i].condition.begin() + j, symbolString[symbol:: or ]);
		}
	}

	ofstream dotFile(fileName);
	if (!dotFile.is_open())
		return;
	dotFile << "digraph " << "stateMachine" << endl;
	dotFile << "{" << endl;
	dotFile << "graph [ resolution = " << graphResolution << " ];" << endl;
	dotFile << "node [ fontname = " << graphFontName << ", fontsize = " << graphFontSize << " ];" << endl;
	dotFile << "edge [ fontname = " << graphFontName << ", fontsize = " << graphFontSize << " ];" << endl;
	dotFile << "rankdir = LR;" << endl;
	dotFile << "overlap = scale;" << endl;
	//dotFile << "concentrate = true;" << endl;
	for (unsigned i = 0; i < nodesNames.size(); i++)
	{
		dotFile << dot[i] << " [ label = ";
		dotFile << toQuotes(letterToDot(nodesNames[i], true, graphSubscriptFontSize)) << ", shape = circle, fixedsize = True, ";
		dotFile << "width = " << nodeRadius(nodesNames[i]) << ", ";
		if (find(specialStates.begin(), specialStates.end(), nodesNames[i]) != specialStates.end())
			dotFile << "style = filled, fillcolor = grey";
		dotFile << " ]; " << endl;
	}
	dotFile << endl;

	for (unsigned i = 0; i < connectionsToDraw.size(); i++)
	{
		dotFile << dot[std::find(nodesNames.begin(), nodesNames.end(), connectionsToDraw[i].left) - nodesNames.begin()] << "->";
		dotFile << dot[std::find(nodesNames.begin(), nodesNames.end(), connectionsToDraw[i].right) - nodesNames.begin()] << " ";
		std::string connectionName("");
		for (unsigned j = 0; j < connectionsToDraw[i].condition.size(); j++)
			connectionName += letterToDot(connectionsToDraw[i].condition[j], true, graphSubscriptFontSize);
		dotFile << "[ label = " << toQuotes(connectionName) << " ]" << endl;
	}

	dotFile << "{ rank=min; " << dot[0] << " };" << endl;
	dotFile << "{ rank=max; " << dot[dot.size() - 1] << " };" << endl;
	dotFile << "}" << endl;

	dotFile.close();
}

void graph::toCubeDotFile(std::string fileName)
{
	terminalLettersContainer dot(nodesNames.size(), "node");
	for (unsigned i = 0; i < dot.size(); i++)
		dot[i] += to_string(i);

	inferenceRulesContainer connectionsToDraw(connections);
	for (unsigned i = 0; i < connectionsToDraw.size(); i++)
	{
		for (unsigned j = i + 1; j < connectionsToDraw.size(); j++)
		{
			if ((connectionsToDraw[i].left == connectionsToDraw[j].left) &&
				(connectionsToDraw[i].right == connectionsToDraw[j].right))
			{
				connectionsToDraw[i].condition.insert(connectionsToDraw[i].condition.end(),
					connectionsToDraw[j].condition.begin(), connectionsToDraw[j].condition.end());
				connectionsToDraw.erase(connectionsToDraw.begin() + j);
				sort(connectionsToDraw[i].condition.begin(), connectionsToDraw[i].condition.end(), minimumNonterminalLetter);
			}
		}
	}
	for (unsigned i = 0; i < connectionsToDraw.size(); i++)
	{
		for (unsigned j = 1; j < connectionsToDraw[i].condition.size(); j += 2)
		{
			connectionsToDraw[i].condition.insert(connectionsToDraw[i].condition.begin() + j, symbolString[symbol:: or ]);
		}
	}

	ofstream dotFile(fileName);
	if (!dotFile.is_open())
		return;
	dotFile << "digraph " << "stateMachine" << endl;
	dotFile << "{" << endl;
	dotFile << "graph [ resolution = " << graphResolution << " ];" << endl;
	dotFile << "node [ fontname = " << graphFontName << ", fontsize = " << graphFontSize << " ];" << endl;
	dotFile << "edge [ fontname = " << graphFontName << ", fontsize = " << graphFontSize << " ];" << endl;
	dotFile << "overlap = scale; " << endl;
	dotFile << "concentrate = true;" << endl;
	//dotFile << "splines = ortho; " << endl;
	for (unsigned i = 0; i < nodesNames.size(); i++)
	{
		int x, y;
		dot4DimCubeCoordinares(i, x, y);
		dotFile << dot[i] << "[ " << "label = " << toQuotes(letterToDot(nodesNames[i], true, graphSubscriptFontSize)) << ", ";
		dotFile << "pos = \"" << x << ", " << y << "\", pin = true, ";
		dotFile << "shape = circle, fixedsize = True, ";
		dotFile << "width = " << nodeRadius(nodesNames[i]);
		if (find(specialStates.begin(), specialStates.end(), nodesNames[i]) != specialStates.end())
			dotFile << ", style = filled, fillcolor = grey";
		dotFile << " ]; " << endl;

	}
	dotFile << endl;

	for (unsigned i = 0; i < connectionsToDraw.size(); i++)
	{
		dotFile << dot[std::find(nodesNames.begin(), nodesNames.end(), connectionsToDraw[i].left) - nodesNames.begin()] << "->";
		dotFile << dot[std::find(nodesNames.begin(), nodesNames.end(), connectionsToDraw[i].right) - nodesNames.begin()] << " ";
		std::string connectionName("");
		for (unsigned j = 0; j < connectionsToDraw[i].condition.size(); j++)
			connectionName += letterToDot(connectionsToDraw[i].condition[j], true, graphSubscriptFontSize);
		dotFile << "[ label = " << toQuotes(connectionName) << " ]" << endl;
	}

	for (unsigned i = 0; i < nodesNames.size(); i++)
	{
		for (unsigned j = i + 1; j < nodesNames.size(); j++)
		{
			if (needEdgeCube(i, j))
			{
				bool notDrawn = true;
				for (unsigned m = 0; m < connectionsToDraw.size(); m++)
				{
					if (((connectionsToDraw[m].left == nodesNames[i]) &&
						(connectionsToDraw[m].right == nodesNames[j])) ||
						((connectionsToDraw[m].left == nodesNames[j]) &&
						(connectionsToDraw[m].right == nodesNames[i])))
						notDrawn = false;
				}
				if (notDrawn)
				{
					dotFile << dot[i] << " -> ";
					dotFile << dot[j] << " [ arrowhead  = none, ";
					dotFile << "style = dotted ];" << endl;
				}
			}
		}
	}

	dotFile << "}" << endl;

	dotFile.close();
}

void graph::placingStatesSlow(unsigned hammingWeight)
{
	unsigned size = static_cast<unsigned>(std::pow(2, int(std::log2(nodesNames.size())) + 1));//TODO   
	std::vector<unsigned> permutation(size);
	for (unsigned i = 0; i < permutation.size(); i++)
		permutation[i] = i;
	std::vector<std::pair<int, int>> rules(this->connections.size());
	for (unsigned i = 0; i < rules.size(); i++)
	{
		rules[i].first = find(nodesNames.begin(), nodesNames.end(), connections[i].left) - nodesNames.begin();
		rules[i].second = find(nodesNames.begin(), nodesNames.end(), connections[i].right) - nodesNames.begin();
	}
	unsigned minNorm = std::numeric_limits<int>::max();
	std::vector<unsigned> min_permutation;
	int firstIdx, secondIdx;
	//long long index = 0;
	//std::time_t startTime = std::time(nullptr);
	do
	{
		unsigned currentNorm = 0;
		for (unsigned i = 0; i < rules.size(); i++)
		{
			firstIdx = find(permutation.begin(), permutation.end(), rules[i].first) - permutation.begin();
			secondIdx = find(permutation.begin(), permutation.end(), rules[i].second) - permutation.begin();
			unsigned bits = (firstIdx^secondIdx);
			unsigned hammingWeight = 0;
			for (unsigned j = 0; j < size; j++)
			{
				hammingWeight += bits & 1;
				bits >>= 1;
			}
			currentNorm += hammingWeight;
		}
		if (currentNorm < minNorm)
		{
			minNorm = currentNorm;
			min_permutation = permutation;
		}
		if (minNorm <= hammingWeight)
			break;
		//index++;
		//if (index % 100000000 == 0)
		//    cout << "Time: " << std::time(nullptr) - startTime << "\tHamming: " << minNorm <<  endl;
	} while (std::next_permutation(permutation.begin(), permutation.end()));

	nonterminalLettersContainer newNodesNames;
	for (unsigned i = 0; i < min_permutation.size(); i++)
	{
		if (min_permutation[i] < nodesNames.size())
			newNodesNames.push_back(nodesNames[min_permutation[i]]);
		else
			newNodesNames.push_back("-");
	}
	nodesNames = newNodesNames;
	//cout << "MIN PERMUATATION" << endl;
	//for (int i = 0; i < permutation.size(); i++)
	//    cout << permutation[i] << "\t";
}

void graph::placingStates(unsigned hammingWeight, std::vector<unsigned> normesNumber)
{
	const unsigned degree = static_cast<unsigned>(std::ceil(std::log2(nodesNames.size())));
	const unsigned size = static_cast<unsigned>(std::pow(2, degree));
	unsigned* permutation = new unsigned[size];
	unsigned* min_permutation = new unsigned[size];
	for (unsigned i = 0; i < size; i++)
		permutation[i] = i;
	unsigned** rules = new unsigned*[size];
	for (unsigned i = 0; i < size; i++)
		rules[i] = new unsigned[size];

	for (unsigned i = 0; i < size; i++)
		for (unsigned j = 0; j < size; j++)
			rules[i][j] = 0;

	for (unsigned i = 0; i < connections.size(); i++)
	{
		unsigned firstIdx = find(nodesNames.begin(), nodesNames.end(), connections[i].left) - nodesNames.begin();
		unsigned secondIdx = find(nodesNames.begin(), nodesNames.end(), connections[i].right) - nodesNames.begin();
		rules[firstIdx][secondIdx] ++;
	}

	long long index = 0;
	std::time_t startTime = std::time(nullptr);

	unsigned minNorm = std::numeric_limits<unsigned>::max();
	std::vector<unsigned> currentNormesNumber(normesNumber.size(), 0);
	do
	{
		fill(currentNormesNumber.begin(), currentNormesNumber.end(), 0);
		unsigned currentNorm = 0;
		for (unsigned i = 0; i < size; i++)
		{
			for (unsigned j = 0; j < size; j++)
			{
				//if (rules[permutation[i]][permutation[j]] != 0)
				//{
				for (unsigned l = 0; l < rules[permutation[i]][permutation[j]]; l++)
				{
					int bits = (i^j);
					int currentWeight = 0;
					for (unsigned k = 0; k < degree; k++)
					{
						currentWeight += (bits & 1);
						bits >>= 1;
					}
					currentNorm += currentWeight;
					currentNormesNumber.at(currentWeight)++;
				}
				//}
			}
		}
		if (currentNorm < minNorm)
		{
			minNorm = currentNorm;
			for (unsigned i = 0; i < size; i++)
			{
				min_permutation[i] = permutation[i];
				cout << min_permutation[i] << " ";
			}
			cout << "\tHamming: " << minNorm;
			cout << " Normes: ";
			for (unsigned i = 1; i < currentNormesNumber.size(); i++)
				cout << currentNormesNumber[i] << ", ";
			cout << endl;
			bool goodPermutation = false;
			if (minNorm <= hammingWeight)
			{
				goodPermutation = true;
				for (unsigned i = 0; i < normesNumber.size(); i++)
					if (normesNumber[i] < currentNormesNumber[i])
						goodPermutation = false;
			}
			if (goodPermutation)
				break;
		}
		if (index >= 10000000)
		{
			index = 0;
			cout << "Time: " << std::time(nullptr) - startTime << endl;
		}
		index++;

	} while (std::next_permutation(permutation, permutation + size));
	ofstream file("goodPermutation.txt");
	for (unsigned i = 0; i < size; i++)
		file << min_permutation[i] << ", ";
	file << endl;
	file.close();
	nonterminalLettersContainer newNodesNames;
	for (unsigned i = 0; i < size; i++)
	{
		if (min_permutation[i] < nodesNames.size())
			newNodesNames.push_back(nodesNames[min_permutation[i]]);
		else
			newNodesNames.push_back("-");
	}
	nodesNames = newNodesNames;
}

void graph::placingStates(std::vector<unsigned> permutation)
{
	nonterminalLettersContainer newNodesNames;
	for (unsigned i = 0; i < permutation.size(); i++)
	{
		if (permutation[i] < nodesNames.size())
			newNodesNames.push_back(nodesNames[permutation[i]]);
		else
			newNodesNames.push_back("-");
	}
	nodesNames = newNodesNames;
}

void graph::determinancy()
{
	nonterminalLettersContainer wasteNodes;
	for (unsigned i = 0; i < connections.size() - 1; i++)
	{
		vector<int> duplicates;
		duplicates.push_back(i);
		for (unsigned j = i + 1; j < connections.size(); j++)
		{
			if (connections[i].similar(connections[j]))
				duplicates.push_back(j);
		}
		if (duplicates.size() > 1)
		{
			nonterminalLetter newRight = connections[i].right;
			bool isSpecial = false;
			for (unsigned k = 1; k < duplicates.size(); k++)
			{
				newRight += ",";
				newRight += to_string(find(nodesNames.begin(), nodesNames.end(), connections[duplicates[k]].right) - nodesNames.begin());
				if (find(specialStates.begin(), specialStates.end(), connections[duplicates[k]].right) != specialStates.end())
					isSpecial = true;
			}
			if (isSpecial)
				specialStates.push_back(newRight);
			nodesNames.push_back(newRight);

			inferenceRule newRule(connections[i].left, connections[i].condition, newRight);
			connections.push_back(newRule);

			for (unsigned k = 0; k < duplicates.size(); k++)
			{
				for (unsigned l = 0; l < connections.size(); l++)
				{
					if (connections[duplicates[k]].right == connections[l].left)
					{
						connections.push_back(inferenceRule(newRight, connections[l].condition, connections[l].right));
					}
				}
			}

			for (unsigned k = 0; k < duplicates.size(); k++)
			{
				bool wasteNode = true;
				for (unsigned l = 0; l < connections.size(); l++)
				{
					if (l != duplicates[k])
					{
						if (connections[l].right == connections[duplicates[k]].right)
							wasteNode = false;
					}
				}
				if (wasteNode)
				{
					wasteNodes.push_back(connections[duplicates[k]].right);
				}
			}

			for (unsigned k = 0; k < duplicates.size(); k++)
			{
				connections.erase(connections.begin() + duplicates[k] - k);
			}

			i--;
		}
	}
	for (unsigned j = 0; j < wasteNodes.size(); j++)
	{
		for (unsigned i = 0; i < connections.size(); i++)
		{
			if (connections[i].left == wasteNodes[j])
			{
				connections.erase(connections.begin() + i);
				i--;
			}
		}
		for (unsigned i = 0; i < nodesNames.size(); i++)
		{
			if (nodesNames[i] == wasteNodes[j])
			{
				nodesNames.erase(nodesNames.begin() + i);
				i--;
			}
		}
	}

}

graphTable::graphTable(graph g, nonterminalLetter error)
{
	nodesNames = g.nodesNames;
	edgesNames = g.edgesNames;
	specialStates = g.specialStates;
	table.resize(nodesNames.size());
	for (unsigned i = 0; i < nodesNames.size(); i++)
	{
		table[i].resize(edgesNames.size());
		//for (int j = 0; j < edgesNames.size(); j++)
		//{
		//    table[i][j] = " ";
		//}
	}

	for (unsigned i = 0; i < g.connections.size(); i++)
	{
		int v = find(nodesNames.begin(), nodesNames.end(), g.connections[i].left) - nodesNames.begin();
		int h = find(edgesNames.begin(), edgesNames.end(), g.connections[i].condition[0]) - edgesNames.begin();
		table[v][h] = g.connections[i].right;
	}
}

void graphTable::toTexFile(std::string fileName)
{
	ofstream texFile(fileName);
	if (!texFile.is_open())
		return;
	texFile << "\\begin{tabular}{" << "| l | ";
	for (unsigned i = 0; i < edgesNames.size(); i++)
		texFile << "c |";
	texFile << "}" << endl;

	texFile << "\\hline" << endl;
	for (unsigned i = 0; i < edgesNames.size(); i++)
		texFile << " & " << "\\(" << letterToTex(edgesNames[i]) << "\\)";
	texFile << " \\\\ \\hline" << endl;
	for (unsigned i = 0; i < nodesNames.size(); i++)
	{
		texFile << "\\(" << letterToTex(nodesNames[i]) << "\\)";
		for (unsigned j = 0; j < edgesNames.size(); j++)
		{
			texFile << " & ";
			texFile << "\\(" << letterToTex(table[i][j]) << "\\)";
		}
		texFile << " \\\\ \\hline" << endl;
	}
	texFile << "\\end{tabular}" << endl;
	texFile.close();
}

void graphTable::toCsvFile(std::string fileName)
{
	ofstream csvFile(fileName);
	if (!csvFile.is_open())
		return;
	for (unsigned i = 0; i < edgesNames.size(); i++)
		csvFile << "; " << edgesNames[i];
	csvFile << endl;
	for (unsigned i = 0; i < nodesNames.size(); i++)
	{
		csvFile << nodesNames[i];
		for (unsigned j = 0; j < edgesNames.size(); j++)
		{
			csvFile << "; ";
			csvFile << table[i][j];
		}
		csvFile << endl;
	}
	csvFile.close();
}

bool graphTable::linesMaybeEqual(int first, int second, nonterminalLettersContainer& difference)
{
	difference.clear();
	for (unsigned i = 0; i < edgesNames.size(); i++)
	{
		if (table[first][i] != table[second][i])
		{
			if ((table[first][i] == errorState) || (table[second][i] == errorState))
			{
				return false;
			}
			else
			{
				difference.push_back(table[first][i]);
				difference.push_back(table[second][i]);
			}
		}
	}
	return true;
}

minimizationTable::minimizationTable(nonterminalLettersContainer v, nonterminalLettersContainer h, graphTable stateMachine)
{
	notEqual = nonterminalLettersContainer({ "X" });
	equal = nonterminalLettersContainer({ "-" });

	//if (v.size() != h.size()); //Some strange logick maybe return

	vertical = v;
	horizontal = h;

	table.resize(vertical.size());
	strickedTable.resize(vertical.size());
	for (unsigned i = 0; i < table.size(); i++)
	{
		table[i].resize(i + 1);
		strickedTable[i].resize(i + 1);
		for (unsigned j = 0; j < i + 1; j++)
		{
			int first = find(stateMachine.nodesNames.begin(), stateMachine.nodesNames.end(), vertical[i]) - stateMachine.nodesNames.begin();
			int second = find(stateMachine.nodesNames.begin(), stateMachine.nodesNames.end(), horizontal[j]) - stateMachine.nodesNames.begin();

			nonterminalLettersContainer difference;

			if (stateMachine.linesMaybeEqual(first, second, difference))
			{
				if (difference.size() == 0)
				{
					table[i][j] = equal;
				}
				else
				{
					table[i][j] = difference;
				}
				strickedTable[i][j] = true;
			}
			else
			{
				table[i][j] = notEqual;
				strickedTable[i][j] = false;
			}

		}
	}
}

void minimizationTable::minimize()
{
	int cnt;
	do
	{
		cnt = 0;
		for (unsigned i = 0; i < table.size(); i++)
		{
			for (unsigned j = 0; j < table[i].size(); j++)
			{
				if (table[i][j] == notEqual)
					continue;
				if (table[i][j] == equal)
					continue;
				for (unsigned k = 0; k < table[i][j].size(); k += 2)
				{
					int v = find(vertical.begin(), vertical.end(), table[i][j][k]) - vertical.begin();
					int h = find(horizontal.begin(), horizontal.end(), table[i][j][k + 1]) - horizontal.begin();
					if ((v == vertical.size()) || (h == horizontal.size()))
					{
						strickedTable[i][j] = false;
						cnt++;
						table[i][j] = notEqual;
						continue;
					}
					if (!strickedTable[v][h])
					{
						strickedTable[i][j] = false;
						cnt++;
						table[i][j] = notEqual;
					}
				}
			}
		}
	} while (cnt > 0);
}

graph minimizationTable::minimizedGraph(graph oldGraph, nonterminalLetter newLetter)
{
	nonterminalLettersContainer nodesNamesNew;
	terminalLettersContainer edgesNamesNew;
	inferenceRulesContainer connectionsNew;

	std::vector<nonterminalLettersContainer> equalClasses;

	for (unsigned i = 0; i < table.size(); i++)
	{
		for (unsigned j = 0; j < table[i].size(); j++)
		{
			if (strickedTable[i][j])
			{
				equalClasses.push_back(nonterminalLettersContainer({ vertical[i],horizontal[j] }));
			}
		}
	}
	cout << endl;
	for (unsigned i = 0; i < equalClasses.size(); i++)
	{
		std::string temp = "";
		for (unsigned j = 0; j < equalClasses[i].size(); j++)
		{
			temp += toTexTableCell(letterToTex(equalClasses[i][j]));
			if (j < (equalClasses[i].size() - 1))
				temp += ", ";
		}
		temp = "(" + temp + ")";
		cout << temp << ", " << endl;
	}
	cout << endl;

	int count;

	do
	{
		count = 0;
		for (unsigned i = 0; i < equalClasses.size(); i++)
		{
			for (unsigned j = i + 1; j < equalClasses.size(); j++)
			{
				if (inserts(equalClasses[i], equalClasses[j]))
				{
					count++;
					equalClasses[i].insert(equalClasses[i].begin(), equalClasses[j].begin(), equalClasses[j].end());
					sort(equalClasses[i].begin(), equalClasses[i].end(), minimumNonterminalLetter);
					auto last = unique(equalClasses[i].begin(), equalClasses[i].end());
					equalClasses[i].erase(last, equalClasses[i].end());

					equalClasses.erase(equalClasses.begin() + j);
					j--;
				}
			}
		}
	} while (count > 0);

	for (unsigned i = 0; i < oldGraph.nodesNames.size(); i++)
	{
		bool contains = false;
		for (unsigned j = 0; j < equalClasses.size(); j++)
		{
			if (find(equalClasses[j].begin(), equalClasses[j].end(), oldGraph.nodesNames[i]) != equalClasses[j].end())
				contains = true;
		}
		if (!contains)
			equalClasses.push_back(nonterminalLettersContainer({ oldGraph.nodesNames[i] }));
	}

	for (unsigned i = 0; i < equalClasses.size(); i++)
		sort(equalClasses[i].begin(), equalClasses[i].end(), minimumNonterminalLetter);
	sort(equalClasses.begin(), equalClasses.end(), minimumFirstNonterminalLetter);

	cout << endl;
	for (unsigned i = 0; i < equalClasses.size(); i++)
	{
		std::string temp = "";
		for (unsigned j = 0; j < equalClasses[i].size(); j++)
		{
			temp += toTexTableCell(letterToTex(equalClasses[i][j]));
			if (j < (equalClasses[i].size() - 1))
				temp += ", ";
		}
		cout << "\\{" << temp << "\\}" << ", " << endl;
	}
	cout << endl;

	nonterminalLettersContainer newDictionary(equalClasses.size(), newLetter);
	for (unsigned i = 0; i < newDictionary.size(); i++)
		newDictionary[i] += to_string(i);

	nonterminalLettersContainer newSpecialStates;
	for (unsigned i = 0; i < oldGraph.specialStates.size(); i++)
	{
		for (unsigned j = 0; j < equalClasses.size(); j++)
		{
			if (find(equalClasses[j].begin(), equalClasses[j].end(), oldGraph.specialStates[i]) != equalClasses[j].end())
				newSpecialStates.push_back(newDictionary[j]);
		}
	}


	inferenceRulesContainer newRules;
	for (unsigned i = 0; i < oldGraph.connections.size(); i++)
	{
		inferenceRule currentRule;
		for (unsigned j = 0; j < equalClasses.size(); j++)
		{
			if (find(equalClasses[j].begin(), equalClasses[j].end(), oldGraph.connections[i].left) != equalClasses[j].end())
			{
				currentRule.left = newDictionary[j];
				break;
			}
		}
		currentRule.condition = oldGraph.connections[i].condition;
		if (!oldGraph.connections[i].right.empty())
		{
			for (unsigned j = 0; j < equalClasses.size(); j++)
			{
				if (find(equalClasses[j].begin(), equalClasses[j].end(), oldGraph.connections[i].right) != equalClasses[j].end())
				{
					currentRule.right = newDictionary[j];
					break;
				}
			}
		}
		newRules.push_back(currentRule);
	}
	sort(newRules.begin(), newRules.end());
	auto last = unique(newRules.begin(), newRules.end());
	newRules.erase(last, newRules.end());
	for (unsigned i = 0; i < newRules.size(); i++)
	{
		cout << toTexTableCell(newRules[i].toTexString()) << "; ";
		cout << endl;
	}
	cout << endl;
	return graph(newDictionary, oldGraph.edgesNames, newRules, newSpecialStates);
}

void minimizationTable::toCsvFile(std::string fileName)
{
	ofstream csvFile(fileName);
	if (!csvFile.is_open())
		return;


	for (unsigned i = 0; i < vertical.size(); i++)
	{
		csvFile << vertical[i];
		for (unsigned j = 0; j < table[i].size(); j++)
		{
			csvFile << "; ";
			for (unsigned k = 0; k < table[i][j].size(); k++)
				csvFile << table[i][j][k] << " ";
		}
		csvFile << endl;
	}
	for (unsigned i = 0; i < horizontal.size(); i++)
		csvFile << "; " << horizontal[i];
	csvFile << endl;

	csvFile.close();
}

void minimizationTable::toTexFile(std::string fileName)
{
	ofstream texFile(fileName);
	if (!texFile.is_open())
		return;
	texFile << "\\begin{tabular}{" << "l ";
	for (unsigned i = 0; i < horizontal.size(); i++)
		texFile << "c ";
	texFile << "}" << endl;

	for (unsigned i = 0; i < vertical.size(); i++)
	{
		texFile << "\\cline{2-" << i + 2 << "}" << endl;
		texFile << "\\multicolumn{1}{l|}{" << toTexTableCell(letterToTex(vertical[i])) << "}";
		for (unsigned j = 0; j <= i; j++)
		{
			texFile << "& \\multicolumn{1}{c|}{";
			if (table[i][j] == notEqual)
			{
				texFile << toTexTableCell("\\times");
			}
			else if (table[i][j] == equal)
			{
				texFile << toTexTableCell("\\leftrightarrow");
			}
			else
			{
				for (unsigned k = 0; k < table[i][j].size(); k++)
				{
					texFile << toTexTableCell(letterToTex(table[i][j][k]));
					if (k < table[i][j].size() - 1)
						texFile << ", ";
				}
			}
			texFile << "} ";
		}
		texFile << "\\\\ ";
	}
	texFile << endl << "\\cline{2-" << vertical.size() + 1 << "}" << endl;
	for (unsigned i = 0; i < horizontal.size(); i++)
		texFile << "& " << toTexTableCell(letterToTex(horizontal[i]));
	texFile << endl;
	texFile << "\\end{tabular}";
	texFile.close();
}

predicate::predicate(std::string cubePredicate, char negationSymbol)
{
	//char current; Unused variable, don't remember why
	unsigned begin = 0, end = 0;
	while (begin < cubePredicate.length())
	{
		while (cubePredicate[begin] != '[')
		{
			begin++;
			if (begin >= cubePredicate.length())
			{
				this->symbols.pop_back();
				return;
			}
		}
		end = begin;
		while (cubePredicate[end] != ']')
		{
			end++;
			if (end >= cubePredicate.length())
			{
				this->symbols.pop_back();
				return;
			}
		}
		std::string part = cubePredicate.substr(begin + 1, end - begin - 1);
		//part.erase(std::remove_if(part.begin(), part.end(), ' '), part.end());
		for (unsigned i = 0; i < part.length(); i++)
		{
			if (part[i] != ' ')
			{
				string pred = "";
				do
				{
					pred += part[i];
					i++;
				} while ((part[i] != ' ') && (i < part.length()));

				bool negation = true;
				if (pred[0] == negationSymbol)
				{
					negation = false;
					pred.erase(pred.begin());
				}
				this->negations.push_back(negation);
				this->symbols.push_back(and);
				this->variables.push_back(pred);
			}
		}
		this->symbols.back() = or ;
		begin = end + 1;
	}
	this->symbols.pop_back();
}

void predicate::addVariable(terminalLetter variable, bool negation, symbol symb)
{
	if (symb != none)
		if (!variables.empty())
			symbols.push_back(symb);
	variables.push_back(variable);
	negations.push_back(negation);
}

void predicate::morganInversion()
{
	if (!symbols.empty())
	{
		for (unsigned i = 0; i < symbols.size(); i++)
		{
			if (symbols[i] != symbols[0])
				return;
		}
	}

	for (unsigned i = 0; i < variables.size(); i++)
		negations[i] = !negations[i];

	for (unsigned i = 0; i < symbols.size(); i++)
	{
		if (symbols[i] == or)
			symbols[i] = and;
		else if (symbols[i] == and)
			symbols[i] = or ;
	}
}

std::string predicate::toString()
{
	std::string formula;
	for (unsigned i = 0; i < variables.size(); i++)
	{
		if (!negations[i])
			formula += "¬";
		formula += variables[i];
		if (i < symbols.size())
			formula += symbolString[symbols[i]];
	}
	return formula;
}

std::string predicate::toTexString()
{
	std::string formula;
	for (unsigned i = 0; i < variables.size(); i++)
	{
		if (!negations[i])
			formula += "\\xoverline";
		formula += "{";
		std::string letter = variables[i];
		std::string idx = getNumber(letter);
		std::transform(letter.begin(), letter.end(), letter.begin(), ::tolower);
		formula += letter;
		formula += "_{";
		formula += idx;
		formula += "}}";
		if (i < symbols.size())
			formula += symbolTexString[symbols[i]];
	}
	return formula;
}

std::string predicate::toDotString()
{
	std::string formula;
	for (unsigned i = 0; i < variables.size(); i++)
	{
		formula += letterToDot(variables[i], negations[i], graphSubscriptFontSize);
		if (i < symbols.size())
		{
			formula += symbolString[symbols[i]];
			//formula += " ";
		}
	}
	return toQuotes(formula);
}

triggerTable::triggerTable(graph cubedGraph)
{
	triggerName = triggerInputsString[0];
	symbols = cubedGraph.nodesNames;
	unsigned size = static_cast<unsigned>(std::log2(symbols.size()));
	jumpConditionsNumbers.resize(symbols.size());
	for (unsigned i = 0; i < jumpConditionsNumbers.size(); i++)
	{
		jumpConditionsNumbers[i].resize(size);
		for (unsigned j = 0; j < jumpConditionsNumbers[i].size(); j++)
			jumpConditionsNumbers[i][j] = -4;
	}
	for (unsigned i = 0; i < jumpConditionsNumbers.size(); i++)
	{
		bool foundInRules = false;
		for (unsigned j = 0; j < cubedGraph.connections.size(); j++)
		{
			if (cubedGraph.connections[j].left == symbols[i])
			{
				foundInRules = true;
				for (unsigned k = 0; k < jumpConditionsNumbers[i].size(); k++)
				{
					int left = (i >> k) & 1;
					int right = ((find(symbols.begin(), symbols.end(), cubedGraph.connections[j].right) - symbols.begin()) >> k) & 1;
					if (left != right)
					{
						if (jumpConditionsNumbers[i][k] < 0)
						{
							jumpConditions.push_back(predicate());
							jumpConditionsNumbers[i][k] = jumpConditions.size() - 1;
						}

						if (left == 0)
						{
							jumpConditions[jumpConditionsNumbers[i][k]].addVariable(
								cubedGraph.connections[j].condition[0], true, or );
						}
						if (right == 0)
						{
							jumpConditions[jumpConditionsNumbers[i][k]].addVariable(
								cubedGraph.connections[j].condition[0], false, and);
						}
					}
					else if (jumpConditionsNumbers[i][k] < 0)
					{
						if (left == 1)
							jumpConditionsNumbers[i][k] = black;
						else
							jumpConditionsNumbers[i][k] = white;
					}
				}
			}
		}
		if (!foundInRules)
		{
			for (unsigned j = 0; j < cubedGraph.connections.size(); j++)
				if (cubedGraph.connections[j].right == symbols[i])
					foundInRules = true;
			if (foundInRules)
			{
				for (unsigned k = 0; k < jumpConditionsNumbers[i].size(); k++)
				{
					if (((i >> k) & 1) == 0)
						jumpConditionsNumbers[i][k] = white;
					else
						jumpConditionsNumbers[i][k] = black;
				}
			}
			else
			{
				for (unsigned k = 0; k < jumpConditionsNumbers[i].size(); k++)
					jumpConditionsNumbers[i][k] = any;
			}
		}
	}
}

triggerTable::triggerTable(const triggerTable& other)
{
	symbols = other.symbols;
	jumpConditions = other.jumpConditions;
	jumpConditionsNumbers = other.jumpConditionsNumbers;
	triggerName = other.triggerName;
}


triggerTable::triggerTable(const triggerTable& other, int input)
{
	triggerName = triggerInputsString[input - 1];
	symbols = other.symbols;
	unsigned size = static_cast<unsigned>(std::log2(symbols.size()));
	jumpConditions = other.jumpConditions;
	for (unsigned i = 0; i < other.jumpConditionsNumbers.size(); i++)
	{
		jumpConditionsNumbers.push_back(std::vector<unsigned>(size));
		for (unsigned j = 0; j < jumpConditionsNumbers[i].size(); j++)
		{
			int left = (i >> j) & 1;
			int right = other.jumpConditionsNumbers[i][j];
			for (unsigned k = 0; k < triggerFromDTable.size(); k++)
			{
				if (triggerFromDTable[k][0] == left)
				{
					if (right < 0)
					{
						if (triggerFromDTable[k][1] == right)
						{
							jumpConditionsNumbers[i][j] = triggerFromDTable[k][input];
						}
					}
					else
					{
						if (triggerFromDTable[k][1] == form)
						{
							if (triggerFromDTable[k][input] < 0)
								jumpConditionsNumbers[i][j] = triggerFromDTable[k][input];
							else
							{
								jumpConditionsNumbers[i][j] = right;
								if (triggerFromDTable[k][input] == formInv)
									jumpConditions[right].morganInversion();
							}
						}
					}
				}
			}
		}
	}

}

void triggerTable::invertion()
{
	for (unsigned i = 0; i < jumpConditionsNumbers.size(); i++)
	{
		for (unsigned j = 0; j < jumpConditionsNumbers[i].size(); j++)
		{
			if (jumpConditionsNumbers[i][j] < 0)
			{
				if (jumpConditionsNumbers[i][j] == white)
					jumpConditionsNumbers[i][j] = black;
				else if (jumpConditionsNumbers[i][j] == black)
					jumpConditionsNumbers[i][j] = white;
			}
			else
				jumpConditions[jumpConditionsNumbers[i][j]].morganInversion();
		}
	}
}

std::vector<std::vector<unsigned>> triggerTable::combinations(unsigned n, unsigned r)
{
	std::vector<std::vector<unsigned>> output;
	std::vector<bool> v(n);
	std::fill(v.begin() + n - r, v.end(), true);

	do {
		std::vector<unsigned> current;
		for (unsigned i = 0; i < n; i++)
			if (v[i])
				current.push_back(i);
		//std::sort(current.begin(), current.end());
		output.push_back(current);
	} while (std::next_permutation(v.begin(), v.end()));

	return output;
}

void triggerTable::findСoating(std::vector<int> realNodes)
{
	unsigned size = static_cast<unsigned>(std::log2(symbols.size()));
	for (unsigned r = 1; r <= size; r++)
	{
		std::vector<std::vector<unsigned>> comb = combinations(size, r);
		std::vector<std::vector<unsigned>> candidats;
		for (unsigned i = 0; i < comb.size(); i++)
		{
			std::vector<unsigned> currentNodes;
			int mask = 0;
			for (unsigned j = 0; j < comb[i].size(); j++)
				mask |= (1 << comb[i][j]);
			for (unsigned j = 0; j < symbols.size(); j++)
			{
				if ((mask & j) == mask)
					currentNodes.push_back(j);
			}
			bool isPart = true;
			for (unsigned i = 0; i < currentNodes.size(); i++)
				if (find(realNodes.begin(), realNodes.end(), currentNodes[i]) == realNodes.end())
					isPart = false;
			//if (std::includes(realNodes.begin(), realNodes.end(), currentNodes.begin(), currentNodes.end()))
			if (isPart)
				candidats.push_back(currentNodes);
		}
		comb.clear();
	}
}

void triggerTable::minimization(terminalLetter axeName)
{
	unsigned size = static_cast<unsigned>(std::log2(symbols.size()));
	terminalLettersContainer axes(size, axeName);

	for (unsigned k = 0; k < size; k++)
	{
		std::vector<int> realNodes;
		for (unsigned i = 0; i < jumpConditionsNumbers.size(); i++)
		{
			if ((jumpConditionsNumbers[i][k] == black) || (jumpConditionsNumbers[i][k] == any))
				realNodes.push_back(i);
		}
		findСoating(realNodes);
	}
}

void triggerTable::minimizationBad()
{
	unsigned size = static_cast<unsigned>(std::log2(symbols.size()));
	terminalLettersContainer axes(size, "Z");
	for (unsigned i = 0; i < axes.size(); i++)
		axes[i] += to_string(i);
	std::vector<predicate> formulas(size);

	std::vector<std::vector<int>> vertices;
	for (unsigned k = 0; k < size; k++)
	{
		for (unsigned i = 0; i < jumpConditionsNumbers.size(); i++)
		{
			if ((jumpConditionsNumbers[i][k] == black) || (jumpConditionsNumbers[i][k] == any))
			{
				std::vector<int> current(4, 0);
				for (unsigned j = 0; j < size; j++)
				{
					current[j] = (i >> j) & 1;
				}
				vertices.push_back(current);
			}
		}
		std::vector<int> sum(size, 0);
		for (unsigned i = 0; i < vertices.size(); i++)
		{
			for (unsigned j = 0; j < vertices[i].size(); j++)
			{
				sum[j] += vertices[i][j];
			}
		}

		std::vector<bool> captured(vertices.size(), false);

		bool needAxe;
		do
		{
			std::vector<int> indices;
			indices.push_back(max_element(sum.begin(), sum.end()) - sum.begin());
			for (unsigned i = indices[0] + 1; i < sum.size(); i++)
			{
				if (sum[i] == sum[indices[0]])
					indices.push_back(i);
			}
			int min_count = INT_MAX;
			int min_index = INT_MAX;
			for (unsigned j = 0; j < indices.size(); j++)
			{
				std::vector<bool> current_captured(captured);
				int count = 0;
				for (unsigned i = 0; i < vertices.size(); i++)
				{
					if (vertices[i][indices[j]] > 0)
					{
						if (!current_captured[i])
							count++;
						current_captured[i] = true;
					}
				}
				if (count < min_count)
				{
					min_count = count;
					min_index = indices[j];
				}
			}

			for (unsigned i = 0; i < vertices.size(); i++)
				if (vertices[i][min_index] > 0)
					captured[i] = true;
			sum[min_index] = -1;
			formulas[k].addVariable(axes[min_index], true, and);

			needAxe = false;
			for (unsigned i = 0; i < vertices.size(); i++)
				if (!captured[i])
					needAxe = true;


		} while (needAxe);
	}
}

void triggerTable::toCsvFile(std::string fileName)
{

	ofstream csvFile(fileName);
	if (!csvFile.is_open())
		return;

	unsigned size = static_cast<unsigned>(std::log2(symbols.size()));
	csvFile << ";" << "z1 z2 z3 z4 ; ";
	for (unsigned i = 0; i < size; i++)
		csvFile << triggerName << i + 1 << " ; ";
	csvFile << endl;

	for (unsigned i = 0; i < symbols.size(); i++)
	{
		csvFile << symbols[i] << " ; ";
		for (unsigned j = 0; j < size; j++)
			csvFile << ((i >> j) & 1) << " ";
		for (unsigned j = 0; j < size; j++)
		{
			string cell;
			if (jumpConditionsNumbers[i][j] < 0)
			{
				if (jumpConditionsNumbers[i][j] == black)
					cell = "1";
				if (jumpConditionsNumbers[i][j] == white)
					cell = "0";
				if (jumpConditionsNumbers[i][j] == any)
					cell = "*";
			}
			else
				cell = jumpConditions[jumpConditionsNumbers[i][j]].toString();
			csvFile << " ; " << cell;
		}
		csvFile << endl;
	}

	csvFile.close();
}

void triggerTable::toTexFiles(std::string fileName)
{
	ofstream texFile(fileName);
	if (!texFile.is_open())
		return;

	unsigned size = static_cast<unsigned>(std::log2(symbols.size()));
	texFile << "\\begin{tabular}{" << "|c|c|";
	for (unsigned i = 0; i < size; i++)
		texFile << "c|";
	texFile << "}" << endl;

	texFile << "\\hline" << endl;

	texFile << "Символ" << "\t";
	texFile << "& Код" << "\t";
	texFile << "& \\multicolumn{" << size << "}{c|}{Функции возбуждения}" << "\t";
	texFile << "\\\\ \\cline{2-" << size + 2 << "}" << "\t" << endl;
	std::string temp = "";
	for (unsigned i = 0; i < size; i++)
		temp += ("{z_" + to_string(i) + "}");
	texFile << "\t";
	texFile << "&" << toTexTableCell(temp) << "\t";
	for (unsigned i = 0; i < size; i++)
		texFile << " &" << toTexTableCell("{" + triggerName + "_" + to_string(i + 1) + "}") << "\t";
	texFile << "\\\\ \\hline" << "\t" << endl;

	for (unsigned i = 0; i < symbols.size(); i++)
	{
		temp = symbols[i];
		getNumber(temp);
		if (temp != "") //kostil
			temp = letterToTex(symbols[i]);
		else
			temp = symbols[i];
		texFile << toTexTableCell(temp) << " & " << "\t";
		temp = "";
		for (unsigned j = 0; j < size; j++)
			temp += (to_string(((i >> j) & 1)) + " ");
		//temp += "";
		texFile << temp;
		temp = "";
		for (unsigned j = 0; j < size; j++)
		{
			if (jumpConditionsNumbers[i][j] < 0)
			{
				if (jumpConditionsNumbers[i][j] == black)
					temp = "1";
				if (jumpConditionsNumbers[i][j] == white)
					temp = "0";
				if (jumpConditionsNumbers[i][j] == any)
					temp = "*";
			}
			else
				temp = jumpConditions[jumpConditionsNumbers[i][j]].toTexString();
			texFile << "\t" << " & " << toTexTableCell(temp);
		}
		texFile << "\t" << "\\\\ \\hline" << "\t" << endl;
	}

	texFile << "\\end{tabular}" << endl;

	texFile.close();
}

void triggerTable::toDotFiles(std::string fileName)
{
	unsigned size = static_cast<unsigned>(std::log2(symbols.size()));
	double radius = 0.7;
	terminalLettersContainer dot(symbols.size(), "node");
	for (unsigned i = 0; i < dot.size(); i++)
		dot[i] += to_string(i);

	for (unsigned k = 0; k < size; k++)
	{
		ofstream dotFile(fileName + to_string(k + 1) + ".dot");
		if (!dotFile.is_open())
			return;
		dotFile << "graph " << "stateMachine" << endl;
		dotFile << "{" << endl;
		dotFile << "graph [ resolution = " << graphResolution << " ];" << endl;
		dotFile << "node [ fontname = " << graphFontName << ", fontsize = " << graphFontSize << " ];" << endl;
		dotFile << "edge [ fontname = " << graphFontName << ", fontsize = " << graphFontSize << " ];" << endl;
		dotFile << "overlap = scale;" << endl;
		for (unsigned i = 0; i < symbols.size(); i++)
		{
			int x, y;
			dot4DimCubeCoordinares(i, x, y);
			if (jumpConditionsNumbers[i][k] < 0)
			{

				if (jumpConditionsNumbers[i][k] == black)
				{
					dotFile << dot[i] << " [ label = \"\", ";
					dotFile << "pos = \"" << x << ", " << y << "\", ";
					dotFile << "style=filled, fillcolor = black, ";
					dotFile << "width = " << nodeRadiusForCubes("") << ", ";
					dotFile << "shape = circle, fixedsize = True ];" << endl;
				}
				if (jumpConditionsNumbers[i][k] == white)
				{
					dotFile << dot[i] << " [ label = \"\", ";
					dotFile << "pos = \"" << x << ", " << y << "\", ";
					dotFile << "width = " << nodeRadiusForCubes("") << ", ";
					dotFile << "shape = circle, fixedsize = True ];" << endl;
				}
				if (jumpConditionsNumbers[i][k] == any)
				{
					dotFile << dot[i] << " [ label = \"*\", ";
					dotFile << "pos = \"" << x << ", " << y << "\", ";
					dotFile << "color = white, ";
					dotFile << "width = " << nodeRadius("*") << ", ";
					dotFile << "shape = circle, fixedsize = True ];" << endl;
				}
			}
			else
			{
				dotFile << dot[i];
				dotFile << " [ label = " << jumpConditions[jumpConditionsNumbers[i][k]].toDotString() << ", ";
				dotFile << "pos = \"" << x << ", " << y << "\", ";
				dotFile << "style=filled, fillcolor = grey, ";
				dotFile << "width = " << nodeRadiusForCubes(jumpConditions[jumpConditionsNumbers[i][k]].toString()) << ", ";
				dotFile << "shape = circle, fixedsize = True ];" << endl;
			}
		}
		dotFile << endl;

		for (unsigned i = 0; i < symbols.size(); i++)
		{
			for (unsigned j = i + 1; j < symbols.size(); j++)
			{
				if (needEdgeCube(i, j))
				{
					dotFile << dot[i] << " -- ";
					dotFile << dot[j] << "; " << endl;
				}
			}
		}

		dotFile << "}" << endl;
		dotFile.close();
	}
}

void dot4DimCubeCoordinares(int index, int& x, int& y)
{
	int edge = 130;
	int xDiagEdge = static_cast<int>(std::sqrt(edge*edge * 3) / 5);
	int yDiagEdge = static_cast<int>(std::sqrt(edge*edge * 3) / 4);
	int secondCubeX = edge * 2;
	int secondCubeY = -yDiagEdge / 2;

	int planeIndex = index % 4;
	switch (planeIndex)
	{
	case 0:
	{
		x = 0;
		y = 0;
	}
	break;
	case 1:
	{
		x = -xDiagEdge;
		y = -yDiagEdge;
	}
	break;
	case 2:
	{
		x = edge;
		y = 0;
	}
	break;
	case 3:
	{
		x = edge - xDiagEdge;
		y = -yDiagEdge;
	}
	break;
	default:
		break;
	}
	int highPlaneIndex = index % 8;
	if ((highPlaneIndex > 3) && (highPlaneIndex < 8))
		y += edge;
	if (index > 7)
	{
		x += secondCubeX;
		y += secondCubeY;
	}
}

bool needEdgeCube(int firstIndex, int secondIndex)
{
	if (firstIndex == secondIndex)
		return false;
	int bits = (firstIndex^secondIndex);
	if (((bits >> 3) & 1) == 1) //Maybe probem in braces
		return false;
	int currentWeight = 0;
	for (unsigned k = 0; k < 3; k++)
	{
		currentWeight += (bits & 1);
		bits >>= 1;
	}
	if (currentWeight == 1)
		return true;
	return false;
}

double nodeRadius(std::string nodeName)
{
	double radius = 0.4;
	double k = 0.03;
	if (nodeName.length() > 3)
		radius += (double)nodeName.length()*k;
	return radius;
}

double nodeRadiusForCubes(std::string nodeName)
{
	double radius = 0.5;
	double k = 0.018;
	if (nodeName.length() > 3)
		radius += (double)nodeName.length()*k;
	return radius;
}





