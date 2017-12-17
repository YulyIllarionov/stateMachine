// stateMachine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>    
#include "grammar.h"
#include <string>
#include <vector>
#include "workData.h"
#include <fstream>

using namespace std;
typedef unsigned char uchar;

int main()
{
    setlocale(LC_ALL, "Russian");

    system("pause");
    
    //Задание имени
    string nameString("костусев владислав андреевич");
    nameString.resize(workTerminalDictionary.size());
    terminalLettersContainer name;
    for (unsigned i = 0; i < nameString.size(); i++)
        name.push_back(terminalLetter(1, nameString[i]));

    //Отображение имени на Xi
    terminalLettersContainer secondWorkTerminalDictionary(8, "x");
    for (unsigned i = 0; i < secondWorkTerminalDictionary.size(); i++)
        secondWorkTerminalDictionary[i] += to_string(i);

    terminalLettersContainer nameInX;

    for (unsigned i = 0; i < name.size(); i++)
    {
        nameInX.push_back(secondWorkTerminalDictionary.at(
            letterNumberInRussianAlphabet(name[i])));
    }
    
    //Вывод имени и словарей в tex таблицу
    cout << "Начало таблицы 1" << endl << endl;
    cout << "\\begin{tabular}{" << "|c|";
    for (unsigned i = 0; i < workTerminalDictionary.size(); i++)
        cout << "c|";
    cout << "}" << endl;

    //Вывод исходного терминального словаря 
    cout << "\\hline" << endl;
    cout << toTexTableCell("c_i");
    for (unsigned i = 0; i < workTerminalDictionary.size(); i++)
    {
        cout << " & ";
        cout << toTexTableCell(letterToTex(workTerminalDictionary[i])) << " ";
    }
    cout << " \\\\ \\hline" << endl;

    //Вывод имени 
    cout << toTexTableCell("s_i");
    for (unsigned i = 0; i < name.size(); i++)
    {
        cout << " & ";
        cout << name[i] << " ";
    }
    cout << " \\\\ \\hline" << endl;

    //Вывод нового терминального словаря  
    cout << toTexTableCell("x_i");
    for (unsigned i = 0; i < name.size(); i++)
    {
        cout << " & ";
        cout << toTexTableCell(letterToTex(nameInX[i])) << " ";
    }
    cout << " \\\\ \\hline" << endl;
    cout << "\\end{tabular}" << endl;

    cout << endl << "Конец таблицы 1" << endl << endl;
    

    //Вывод таблицы правил перехода к новому словарю 
    std::vector <std::string> russianLeters;
    for (char i = 'а'; i <= 'я'; i++)
        russianLeters.push_back(std::string(1,i));
    russianLeters.erase(find(russianLeters.begin(), russianLeters.end(), "ъ"));
    russianLeters.push_back(" ");

    cout << "Начало таблицы 2" << endl << endl;
    cout << "\\begin{tabular}{" << "|";
    for (unsigned i = 0; i < russianLeters.size()/2; i++)
        cout << "c|";
    cout << "}" << endl;
    //Вывод первой половины русского алфавита
    for (unsigned i = 0; i < russianLeters.size()/2; i++)
    {
        cout << russianLeters[i];
        if (i != russianLeters.size() / 2 - 1)
            cout << " & ";
    }
    cout << " \\\\ \\hline" << endl;
    for (unsigned i = 0; i < russianLeters.size() / 2; i++)
    {
        for (unsigned j = 0; j < russianAlphabet.size(); j++)
        {
            if (find(russianAlphabet[j].begin(), russianAlphabet[j].end(), russianLeters[i]) != russianAlphabet[j].end())
            {
                cout << toTexTableCell(letterToTex(secondWorkTerminalDictionary[j]));
                break;
            }
        }
        if (i != russianLeters.size() / 2 - 1)
            cout << " & ";
    }
    cout << " \\\\ \\hline" << endl;
    //Вывод второй половины русского алфавита
    for (unsigned i = russianLeters.size() / 2; i < russianLeters.size(); i++)
    {
        cout << russianLeters[i];
        if (i != russianLeters.size() - 1)
            cout << " & ";
    }
    cout << " \\\\ \\hline" << endl;
    for (unsigned i = russianLeters.size() / 2; i < russianLeters.size(); i++)
    {
        for (unsigned j = 0; j < russianAlphabet.size(); j++)
        {
            if (find(russianAlphabet[j].begin(), russianAlphabet[j].end(), russianLeters[i]) != russianAlphabet[j].end())
            {
                cout << toTexTableCell(letterToTex(secondWorkTerminalDictionary[j]));
                break;
            }
        }
        if (i != russianLeters.size() - 1)
            cout << " & ";
    }
    cout << " \\\\ \\hline" << endl;
    cout << "\\end{tabular}" << endl;
    cout << endl << "Конец таблицы 2" << endl << endl;


    //Вывод правил вывода
    cout << "Начало правил вывода" << endl << endl;
    for (unsigned i = 0; i < workInferenceRules.size(); i++)
    {
        cout << toTexTableCell(workInferenceRules[i].toTexString()) << ";\t" << endl;
    }
    cout << endl << "Конец правил вывода" << endl;
    
    //Переход от C к X в правилах вывода
    for (unsigned i = 0; i < workInferenceRules.size(); i++)
    {
        for (unsigned j = 0; j < workInferenceRules[i].condition.size(); j++)
        {
            workInferenceRules[i].condition[j] = nameInX[find(workTerminalDictionary.begin(),
                workTerminalDictionary.end(), workInferenceRules[i].condition[j]) - workTerminalDictionary.begin()];
        }
    }

    //Вывод правил вывода
    cout << "Начало правил вывода" << endl << endl;
    for (unsigned i = 0; i < workInferenceRules.size(); i++)
    {
        cout << toTexTableCell(workInferenceRules[i].toTexString()) << ";\t" << endl;
    }
    cout << endl << "Конец правил вывода" << endl;

    nonterminalLettersContainer workAutomatonNonterminalDictionary;
    inferenceRulesContainer inferenceAutomatonRules;

    //Переход к автоматной грамматике
    linearToAutomatonGrammar(workNonterminalDictionary, workInferenceRules,
        workAutomatonNonterminalDictionary, inferenceAutomatonRules);

    //Вывод правил вывода
    cout << "Начало правил вывода" << endl << endl;
    for (unsigned i = 0; i < inferenceAutomatonRules.size(); i++)
    {
        cout << toTexTableCell(inferenceAutomatonRules[i].toTexString()) << ";\t" << endl;
    }
    cout << endl << "Конец правил вывода" << endl;

    //Вывод расширенного нетерминального словаря
    for (unsigned i = 0; i < workAutomatonNonterminalDictionary.size(); i++)
        cout << toTexTableCell(letterToTex(workAutomatonNonterminalDictionary[i])) << ", ";
    cout << workAutomatonNonterminalDictionary.size() << endl<< endl;


    workAutomatonNonterminalDictionary.push_back("-");
    for (unsigned i = 0; i < inferenceAutomatonRules.size(); i++)
        inferenceAutomatonRules[i].addRightIfEmpty("-");

    for (unsigned i = 0; i < workAutomatonNonterminalDictionary.size(); i++)
        cout << workAutomatonNonterminalDictionary[i] << "\t";
    cout << endl << endl;
    
    for (unsigned i = 0; i < inferenceAutomatonRules.size(); i++)
    {
        inferenceAutomatonRules[i].print();
        cout << endl;
    }
    cout << endl;

    //Замена нетерминальных символов на состояния автомата
    nonterminalLettersContainer secondWorkNonterminalDictionary(
        workAutomatonNonterminalDictionary.size(), "q");
    for (unsigned i = 0; i < secondWorkNonterminalDictionary.size(); i++)
        secondWorkNonterminalDictionary[i] += to_string(i);

    for (unsigned i = 0; i < inferenceAutomatonRules.size(); i++)
    {
        inferenceAutomatonRules[i].changeNontetminal(
            workAutomatonNonterminalDictionary, secondWorkNonterminalDictionary);
    }

    for (unsigned i = 0; i < inferenceAutomatonRules.size(); i++)
    {
        inferenceAutomatonRules[i].print();
        cout << endl;
    }
    cout << endl;

    graph stateMachine(secondWorkNonterminalDictionary,
        secondWorkTerminalDictionary, inferenceAutomatonRules, { secondWorkNonterminalDictionary.back() });

    stateMachine.toDotFile("stateMachine.dot");
    graphTable stateMachineTable(stateMachine, " ");
    stateMachineTable.toTexFile("stateMachineTable.txt");

    stateMachine.determinancy();

    sort(stateMachine.nodesNames.begin(), stateMachine.nodesNames.end(), minimumNonterminalLetter);

    //sort(stateMachine.connections.begin(), stateMachine.connections.end());

    for (unsigned i = 0; i < stateMachine.nodesNames.size(); i++)
        cout << stateMachine.nodesNames[i] << "\t";
    cout << endl << endl;

    for (unsigned i = 0; i < stateMachine.connections.size(); i++)
    {
        stateMachine.connections[i].print();
        cout << endl;
    }
    cout << endl;

    stateMachine.toDotFile("stateMachineDeterminated.dot");

    graphTable determStateMachineTable(stateMachine, " ");
    determStateMachineTable.toTexFile("determStateMachineTable.txt");

    nonterminalLettersContainer minimizationVertical(stateMachine.nodesNames);
    nonterminalLettersContainer minimizationHorizontal(stateMachine.nodesNames);

    for (unsigned i = 0; i < stateMachine.nodesNames.size(); i++)
        cout << stateMachine.nodesNames[i] << "\t";
    cout << endl;

	//TODO удаляем начальное , конечное состояние
	//и состояние обладающее свойствами обоих, у нас оно оказалось вторым
	//хардкод, нужно сделать поиск этого состояния
    minimizationVertical.erase(minimizationVertical.begin(),
        minimizationVertical.begin() + 3);
    minimizationVertical.erase(minimizationVertical.end() - 1);
    for (unsigned i = 0; i < minimizationVertical.size(); i++)
        cout << minimizationVertical[i] << "\t";
    cout << endl;

    minimizationHorizontal.erase(minimizationHorizontal.begin(),
        minimizationHorizontal.begin() + 2);
    minimizationHorizontal.erase(minimizationHorizontal.end() - 2,
        minimizationHorizontal.end());
    for (unsigned i = 0; i < minimizationHorizontal.size(); i++)
        cout << minimizationHorizontal[i] << "\t";
    cout << endl;

    minimizationTable stateMachineMinimizationTable(minimizationVertical, minimizationHorizontal, determStateMachineTable);
    stateMachineMinimizationTable.toTexFile("stateMachineMinimizationTable.txt");

    stateMachineMinimizationTable.minimize();
    stateMachineMinimizationTable.toTexFile("stateMachineMinimizationTableNew.txt");

    graph minimizedStateMachine = stateMachineMinimizationTable.minimizedGraph(stateMachine, "r");
    minimizedStateMachine.toDotFile("minimizedStateMachine.dot");
    
    graphTable minimizedStateMachineTable(minimizedStateMachine, " ");
    minimizedStateMachineTable.toTexFile("minimizedStateMachineTable.txt");

    //minimizedStateMachine.placingStates(/*31, std::vector<unsigned>({0, 21, 7, 1, 1, })*/);
    minimizedStateMachine.placingStates(std::vector<unsigned>({ 0, 1, 2, 3, 6, 8, 4, 15, 7, 9, 5, 10, 14, 13, 12, 11, }));
    minimizedStateMachine.toCubeDotFile("cubeMinimizedStateMachine.dot");


	system("pause");

    std::vector<triggerTable> triggers;

    triggerFromDTableToTex("triggerConvertionTable.txt");

    triggers.push_back(triggerTable(minimizedStateMachine));
    triggers.back().toTexFiles(".//triggersTables//" + triggers.back().triggerName + "_triggerTable.txt");
    triggers.back().toDotFiles(".//triggerCubes//" + triggers.back().triggerName);
    triggerTable triggerInv(triggers.back());
    triggerInv.invertion();
    triggerInv.toDotFiles(".//triggerCubesInv//" + triggerInv.triggerName);
    for (unsigned i = S; i <= K; i++)
    {
        triggers.push_back(triggerTable(triggers[0], i));
        triggers.back().toTexFiles("triggersTables//" + triggers.back().triggerName + "_triggerTable.txt");
        triggers.back().toDotFiles(".//triggerCubes//" + triggers.back().triggerName);
        triggerTable triggerInv(triggers.back());
        triggerInv.invertion();
        triggerInv.toDotFiles(".//triggerCubesInv//" + triggerInv.triggerName);
    }

    //triggers.minimization();

    ifstream triggers_file("stateMachineFormulas.txt", ios_base::in);
    ofstream triggers_file_tex("stateMachineFormulasToTex.txt");
    ofstream triggers_file_csv("stateMachineFormulasSizes.csv");
    ofstream report_tex_file("reportTexFile.txt");

    if ((triggers_file.is_open()) && (triggers_file_tex.is_open()) && (triggers_file_csv.is_open()) && (report_tex_file.is_open()))
    {
        string line;
        while (std::getline(triggers_file, line))
        {
            if (!line.empty())
            {
                bool negation = true;
                std::string trigName = line.substr(0, line.find('=')-1);
                report_tex_file << "\\begin{figure}[H]" << endl << "\\centering" << endl;
                if (trigName[0] == 'N')
                {
                    negation = false;
                    trigName.erase(trigName.begin());
                    report_tex_file << "\\includegraphics[width=\\textwidth]{triggerCubesInv/" << trigName << ".pdf}" << endl;
                    report_tex_file << "\\caption{Геометрическое представление функции " <<
                        toTexTableCell("\\xoverline" + letterToTex(trigName)) << "}" << endl;
                    report_tex_file << "\\label{fig:triggersN" << trigName << "}" << endl;
                }
                else
                {
                    negation = true;
                    report_tex_file << "\\includegraphics[width=\\textwidth]{triggerCubes/" << trigName << ".pdf}" << endl;
                    report_tex_file << "\\caption{Геометрическое представление функции " <<
                        toTexTableCell(letterToTex(trigName)) << "}" << endl;
                    report_tex_file << "\\label{fig:triggers" << trigName << "}" << endl;
                }
                report_tex_file << "\\end{figure}" << endl;

                line.erase(line.begin(), line.begin() + line.find('=') + 1);
                predicate currentCubePred(line, 'N');
                int formulaSize = currentCubePred.variables.size();
                if (!negation)
                    formulaSize++;
                std::string trigNumber = getNumber(trigName);
                if (!negation)
                    report_tex_file << toTexTableCell("\\xoverline{" + trigName + "_" + trigNumber + "}" + " = " + currentCubePred.toTexString());
                else
                    report_tex_file << toTexTableCell("{" + trigName + "_" + trigNumber + "}" + " = " + currentCubePred.toTexString());
                report_tex_file << /*", " <<*/ endl;
                //triggers_file_tex << toTexTableCell(trigName);
                //triggers_file_tex << endl << endl;

                string formulaSizeString = std::to_string(formulaSize);

                if (trigName[0] == 'D')
                    formulaSizeString += "+4";
                else if (trigName[0] == 'T')
                    formulaSizeString += "+1";
                else
                    formulaSizeString += "+2";
                
                if (negation)
                    triggers_file_csv << trigName << trigNumber << "; " << toTexTableCell(formulaSizeString) << "; ";
                else
                {
                    triggers_file_csv << "N" << trigName << trigNumber << "; " << toTexTableCell(formulaSizeString) << endl;
                    triggers_file_csv << "& ; " << "& ; " << "& ; " << "& ; " << endl;
                }

                //if (negation)
                //    report_tex_file << "\\quad " << "\\( O(" << "\\xoverline" << letterToTex(trigName + "_" + trigNumber) << ") = ";
                //else
                //    report_tex_file << "\\quad " << "\\( O(" << letterToTex(trigName + "_" + trigNumber) << ") = ";
                //report_tex_file << formulaSizeString << "\\). " << endl;
                report_tex_file << endl;
            }
        }
        triggers_file.close();
        triggers_file_tex.close();
        triggers_file_csv.close();
        report_tex_file.close();
    }

    system("pause");

    return 0;
}

