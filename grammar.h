#pragma once

#include <string>
#include <vector>

typedef std::string terminalLetter;
typedef std::vector<terminalLetter> terminalLettersContainer;

typedef std::string nonterminalLetter;
typedef std::vector<terminalLetter> nonterminalLettersContainer;

typedef std::string predicateExpression;

bool inserts(std::vector<std::string> first, std::vector<std::string> second);

std::string getNumber(std::string& letter);

bool minimumNonterminalLetter(const nonterminalLetter &a, const nonterminalLetter &b);

bool minimumFirstNonterminalLetter(const nonterminalLettersContainer &a, const nonterminalLettersContainer &b);

bool maximumPairDistance(const std::pair<int, int> &a, const std::pair<int, int> &b);

void dot4DimCubeCoordinares(int index, int& first, int& second);

bool needEdgeCube(int firstIndex, int secondIndex);

double nodeRadius(std::string nodeName);

double nodeRadiusForCubes(std::string nodeName);

std::string letterToTex(std::string letter);

std::string toTexTableCell(std::string text);

std::string letterToDot(std::string letter, bool negation, int fontSize);

std::string toQuotes(std::string text);

static const int graphResolution = 100;
static const int graphFontSize = 14;
static const std::string graphFontName = "Times";
static const int graphSubscriptFontSize = 12;

enum symbol { none = 0, and = 1, or = 2, xor = 3 };
static const char * symbolString[] = { "none", "", "|", "^" };
static const char * symbolTexString[] = { "", "", "\\vee", "\\wedge" };
static const int white = -1;
static const int black = -2;
static const int any = -3;
static const int formInv = 1;
static const int form = 0;

enum triggerInputs { D, S = 2, R = 3, T = 4, J = 5, K = 6 };
static const std::vector<std::string> triggerInputsString = { "D", "S", "R", "T", "J", "K" };

static const std::vector<std::vector<int>> triggerFromDTable({
    std::vector<int>({ 0, white,    white,    any,        white,    white,  any }),
    std::vector<int>({ 1, white,    white,    black,      black,    any,    black }),
    std::vector<int>({ 0, black,    black,    white,      black,    black,  any }),
    std::vector<int>({ 1, black,    any,      white,      white,    any,    white }),
    std::vector<int>({ 0, form,     form,     white,      form,     form,   any }),
    std::vector<int>({ 1, form,     white,    formInv,    form,     any,    formInv }),
    std::vector<int>({ 0, any,      any,      any,        any,      any,    any }),
    std::vector<int>({ 1, any,      any,      any,        any,      any,    any })
});

void triggerFromDTableToTex(std::string fileName);

class inferenceRule
{
public: 
    nonterminalLetter left;
    terminalLettersContainer condition;
    nonterminalLetter right; 

public:
    
    inferenceRule() {}

    inferenceRule(nonterminalLetter left_, terminalLettersContainer condition_);

    inferenceRule(nonterminalLetter left_, terminalLettersContainer condition_, nonterminalLetter right);
    
    void changeNontetminal(terminalLettersContainer from, terminalLettersContainer to);

    void addRightIfEmpty(nonterminalLetter newRight);

    bool similar (const inferenceRule& other);

    bool operator < (const inferenceRule& other);

    bool operator == (const inferenceRule& other);

    std::string toTexString();

    void print();

};

typedef std::vector<inferenceRule> inferenceRulesContainer;

void linearToAutomatonGrammar(nonterminalLettersContainer inputNonterminalDict, inferenceRulesContainer inputRules,
    nonterminalLettersContainer& outputNonterminalDict, inferenceRulesContainer& outputRules);

class graph
{
public:
    nonterminalLettersContainer nodesNames;
    terminalLettersContainer edgesNames; 
    inferenceRulesContainer connections;
    nonterminalLettersContainer initialStates;
    nonterminalLettersContainer finalStates;
    nonterminalLettersContainer specialStates; 

public: 
    graph() {}
    graph(nonterminalLettersContainer nodesNames_, terminalLettersContainer edgesNames_, inferenceRulesContainer connections_, nonterminalLettersContainer specialStates_);
    graph& operator = (const graph& other);

    void determinancy();
    void placingStatesSlow(unsigned hammingWeight);
    void placingStates(unsigned hammingWeight, std::vector<unsigned> normesNumber);
    void placingStates(std::vector<unsigned> permutation);

    void toDotFile(std::string fileName);
    void toCubeDotFile(std::string fileName);
};

class graphTable
{
public:
    nonterminalLettersContainer nodesNames;
    terminalLettersContainer edgesNames;
    nonterminalLettersContainer specialStates;
    nonterminalLetter errorState;
    std::vector<std::vector<nonterminalLetter>> table;

public:

    graphTable(graph g, nonterminalLetter error);
    void toCsvFile(std::string fileName);
    void toTexFile(std::string fileName);

    bool linesMaybeEqual(int first, int second, nonterminalLettersContainer& difference);

};

class minimizationTable
{
public:
    nonterminalLettersContainer vertical;
    nonterminalLettersContainer horizontal;
    nonterminalLettersContainer notEqual;
    nonterminalLettersContainer equal; 
    std::vector<std::vector<nonterminalLettersContainer>> table;
    std::vector<std::vector<bool>> strickedTable;

public:
    minimizationTable(nonterminalLettersContainer v, nonterminalLettersContainer h, graphTable stateMachine);
    void minimize();
    graph minimizedGraph(graph oldGraph, nonterminalLetter newLetter);

    void toCsvFile(std::string fileName);
    void toTexFile(std::string fileName);
};

class predicate
{
public:
    std::vector<terminalLetter> variables;
    std::vector<symbol> symbols;
    std::vector<bool> negations;

public:
    predicate() {}
    predicate(std::string cubePredicate, char negationSymbol);
    void addVariable(terminalLetter variable, bool negation, symbol symb  = none);
    void morganInversion();
    std::string toString();
    std::string toTexString();
    std::string toDotString();
};

class triggerTable
{
public:
    nonterminalLettersContainer symbols;
    std::vector<predicate> jumpConditions; 
    std::vector<std::vector<unsigned>> jumpConditionsNumbers;
    std::string triggerName; 

public:
    triggerTable(graph cubedGraph);
    triggerTable(const triggerTable& other);
    triggerTable(const triggerTable& other, int input);
    void invertion();
    std::vector<std::vector<unsigned>> combinations(unsigned n, unsigned r);
    void find—oating(std::vector<int> realNodes);
    void minimization(terminalLetter axeName);
    void minimizationBad();
    void toCsvFile(std::string fileName);
    void toTexFiles(std::string fileName);
    void toDotFiles(std::string fileName);
};

