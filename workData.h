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
    terminalLettersContainer({ "é", "ë", "ð", "ù" }),
    terminalLettersContainer({ "à", "ö", "û", "ý" }),
    terminalLettersContainer({ "â", "ô", "÷", "ø" }),
    terminalLettersContainer({ "ç", "è", "ì", "þ" }),
    terminalLettersContainer({ "ã", "æ", "î", "ñ" }),
    terminalLettersContainer({ "á", "ï", "ò", "õ", " " }),
    terminalLettersContainer({ "ä", "å", "ü" }),
    terminalLettersContainer({ "ê", "í", "ó", "ÿ" })
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
//    russian['à'] =     X1;
//    russian['á'] =                     X5;
//    russian['â'] =         X2;
//    russian['ã'] =                 X4;
//    russian['ä'] =                         X6;
//    russian['å'] =                         X6;
//    russian['æ'] =                 X4;
//    russian['ç'] =             X3;
//    russian['è'] =             X3;
//    russian['é'] = X0;
//    russian['ê'] =                            X7;
//    russian['ë'] = X0;
//    russian['ì'] =             X3;
//    russian['í'] =                            X7;
//    russian['î'] =                 X4;
//    russian['ï'] =                     X5;
//    russian['ð'] = X0;
//    russian['ñ'] =                 X4;
//    russian['ò'] =                     X5;
//    russian['ó'] =                            X7;
//    russian['ô'] =         X2;
//    russian['õ'] =                     X5;
//    russian['ö'] =     X1;
//    russian['÷'] =         X2;
//    russian['ø'] =         X2;
//    russian['ù'] = X0;
//    russian['û'] =     X1;
//    russian['ü'] =                         X6;
//    russian['ý'] =     X1;
//    russian['þ'] =            X3;
//    russian['ÿ'] =                            X7;
//    russian['_'] =                    X5;
//}