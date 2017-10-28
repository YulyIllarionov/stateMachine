#pragma once

#include "grammar.h"
#include <vector>

terminalLettersContainer workTerminalDictionary({ "c1", "c2","c3","c4","c5","c6","c7","c8","c9","c10","c11","c12", "c13","c14","c15","c16","c17","c18" });

nonterminalLettersContainer workNonterminalDictionary({ "S","A","B","C","D","E","F" });

inferenceRulesContainer workInferenceRules({
    inferenceRule("S", {"c1","c2","c3"}, "A"),
    inferenceRule("S", { "c1","c4","c5" }, "B"),
    inferenceRule("S", { "c6" }, "C"),
    inferenceRule("S", { "c7" }, "F"),
    inferenceRule("A", { "c8" }, "D"),
    inferenceRule("A", { "c9" }),
    inferenceRule("B", { "c8" }, "E"),
    inferenceRule("B", { "c9" }),
    inferenceRule("C", { "c8" }, "E"),
    inferenceRule("C", { "c9" }),
    inferenceRule("D", { "c10" }, "S"),
    inferenceRule("D", { "c11" }),
    inferenceRule("E", { "c11" }, "S"),
    inferenceRule("E", { "c11" }),
    inferenceRule("F", { "c12", "c13", "c14", "c15" }),
    inferenceRule("F", { "c16", "c13", "c14", "c15" }),
    inferenceRule("F", { "c17", "c18", "c15" }),
});

std::vector<terminalLettersContainer> russianAlphabet({
    terminalLettersContainer({ "�", "�", "�", "�" }),
    terminalLettersContainer({ "�", "�", "�", "�" }),
    terminalLettersContainer({ "�", "�", "�", "�" }),
    terminalLettersContainer({ "�", "�", "�", "�" }),
    terminalLettersContainer({ "�", "�", "�", "�" }),
    terminalLettersContainer({ "�", "�", "�", "�", " " }),
    terminalLettersContainer({ "�", "�", "�" }),
    terminalLettersContainer({ "�", "�", "�", "�" })
});

int letterNumberInRussianAlphabet(terminalLetter let)
{
    for (unsigned int i = 0; i < russianAlphabet.size(); i++)
    {
        for (int j = 0; j < russianAlphabet[i].size(); j++)
        {
            if (russianAlphabet[i][j] == let)
                return i;
        }
    }
    return -1;
}

//static void xAlphabetToRussian()
//{
//    
//    russian['�'] =     X1;
//    russian['�'] =                     X5;
//    russian['�'] =         X2;
//    russian['�'] =                 X4;
//    russian['�'] =                         X6;
//    russian['�'] =                         X6;
//    russian['�'] =                 X4;
//    russian['�'] =             X3;
//    russian['�'] =             X3;
//    russian['�'] = X0;
//    russian['�'] =                            X7;
//    russian['�'] = X0;
//    russian['�'] =             X3;
//    russian['�'] =                            X7;
//    russian['�'] =                 X4;
//    russian['�'] =                     X5;
//    russian['�'] = X0;
//    russian['�'] =                 X4;
//    russian['�'] =                     X5;
//    russian['�'] =                            X7;
//    russian['�'] =         X2;
//    russian['�'] =                     X5;
//    russian['�'] =     X1;
//    russian['�'] =         X2;
//    russian['�'] =         X2;
//    russian['�'] = X0;
//    russian['�'] =     X1;
//    russian['�'] =                         X6;
//    russian['�'] =     X1;
//    russian['�'] =            X3;
//    russian['�'] =                            X7;
//    russian['_'] =                    X5;
//}