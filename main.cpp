#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

ifstream fin("nfa.txt");

int main() {
    int nrFinal, x, nrTranz, qfrom, qto;
    char val;
    vector<int> final;
    fin>>nrFinal;
    for (int i = 0; i < nrFinal; ++i) {
        fin>>x;
        final.push_back(x);
    }
    fin>>nrTranz;
    vector<vector<pair<char, int>>> tranz(nrTranz);
    for (int i = 0; i < nrTranz; ++i) {
        fin>>qfrom>>val>>qto;
        tranz[qfrom].push_back(make_pair(val, qto));
    }
    int nrStari;
    fin>>nrStari;
    queue<int> current;
    for (int i = 0; i < nrStari; ++i) {
        current.push(i);
    }
    map<vector<int>, int> viz;
    viz[{0}] = 1;
    map<vector<int>, vector<pair<char, vector<int>>>> baseStates;
    map<vector<int>, vector<pair<char, vector<int>>>> dfaStates;
    queue<vector<int>> newStates;
///     ^^^^^^^^^^               ^^^^   ^^^^^^^^^
///     state            tranz symbol   to state

    while(!current.empty()){
        map<char, vector<int>> newTranz;
        int next = current.front();
        for (int i = 0; i < tranz[next].size(); ++i) {
            newTranz[tranz[next][i].first].push_back(tranz[next][i].second);
        }
        vector<int> aux = {next};
        map<char, vector<int>>::iterator itr;
        for (itr = newTranz.begin(); itr != newTranz.end(); ++itr){
            baseStates[aux].push_back(make_pair(itr->first, itr->second));
            if (viz[{next}] == 1){
                if (viz[itr->second] != 1){
                    viz[itr->second] = 1;
                    newStates.push(itr->second);
                }
            }
        }
        current.pop();
    }
    map<vector<int>, vector<pair<char, vector<int>>>>::iterator itr;
    for (itr = baseStates.begin(); itr != baseStates.end(); ++itr) {
        if (viz[itr->first] == 1){
            dfaStates[itr->first] = itr->second;
        }
    }

    while(!newStates.empty()){
        map<char, vector<int>> newTranz;
        map<char, vector<int>>::iterator itrTranz;
        vector<int> next = newStates.front();
        for (itr = baseStates.begin(); itr != baseStates.end(); ++itr){
            for (int i = 0; i < next.size(); ++i) {
                if (itr->first[0] == next[i]){
                    for (int j = 0; j < itr->second.size(); ++j) {
                        for (int k = 0; k < itr->second[j].second.size(); ++k) {
                            newTranz[itr->second[j].first].push_back(itr->second[j].second[k]);
                        }
                    }
                }
            }
            for (itrTranz = newTranz.begin(); itrTranz != newTranz.end(); ++itrTranz) {
                sort(itrTranz->second.begin(), itrTranz->second.end());
                for (int i = 0; i < itrTranz->second.size() - 1; ++i) {
                    if (itrTranz->second[i] == itrTranz->second[i + 1]){
                        for (int j = i + 1; j < itrTranz->second.size() - 1; ++j) {
                            itrTranz->second[j] = itrTranz->second[j + 1];
                        }
                        itrTranz->second.resize(itrTranz->second.size() - 1);
                        i--;
                    }
                }
                if (viz[itrTranz->second] != 1){
                    viz[itrTranz->second] = 1;
                    newStates.push(itrTranz->second);
                }
            }
        }
        for (itrTranz = newTranz.begin(); itrTranz != newTranz.end(); ++itrTranz){
            dfaStates[next].push_back(make_pair(itrTranz->first,itrTranz->second));
        }
        newStates.pop();
    }

    for (itr = dfaStates.begin(); itr != dfaStates.end(); ++itr){
        int ok = 0;
        for (int i = 0; i < final.size(); ++i) {
            for (int j = 0; j < itr->first.size(); ++j) {
                if (itr->first[j] == final[i]){
                    cout<<"Final State: ";
                    ok = 1;
                    break;
                }
                if (ok == 1){
                    break;
                }
            }
        }
        if (ok == 0){
            cout<<"State: ";
        }
        for (int i = 0; i < itr->first.size(); ++i) {
            cout<<itr->first[i]<<' ';
        }
        cout<<endl;
        for (int i = 0; i < itr->second.size(); ++i) {
            cout<<"/ "<<itr->second[i].first<<" -> ";
            for (int j = 0; j < itr->second[i].second.size(); ++j) {
                cout<<itr->second[i].second[j]<<' ';
            }
            cout<<endl;
        }
        cout<<endl<<"--------------------------"<<endl;
    }
    return 0;
}
