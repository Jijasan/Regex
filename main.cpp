#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <string.h>
#include <vector>
#include <unordered_map>
using namespace std;

class RegexChecker {
private:
    struct Node{
        unordered_map<char, int> nxt;
        int pr = -1;
        int len = -1;
        int link = 0;
    };
    vector<Node> tr;
    vector<int> cnt, u;
    int qcnt = 0;
    int l = 0, v = 1;
    int sz = 1;

    void add(char c) {
        v = sz;
        ++sz;
        tr[v].pr = l;
        tr[v].len = tr[l].len + 1;
        int p = l;
        while (p != -1 && tr[p].nxt.find(c) == tr[p].nxt.end()) {
            tr[p].nxt[c] = v;
            p = tr[p].link;
        }
        if (p != -1) {
            int q = tr[p].nxt[c];
            if (tr[q].pr == p)
                tr[v].link = q;
            else {
                int nw = sz;
                tr[nw] = tr[q];
                ++sz;
                tr[nw].pr = p;
                tr[q].link = nw;
                tr[v].link = nw;
                while (p != -1 && tr[p].nxt[c] == q) {
                    tr[p].nxt[c] = nw;
                    p = tr[p].link;
                }
            }
        }
        l = v;
    }

public:
    RegexChecker(const string& s) {
        u.resize(s.size());
        tr.resize(2 * s.size());
        cnt.resize(s.size());
        tr[0].link = -1;
        for (char c : s)
            add(c);
    }

    void check(const string& pattern) {
        int it = pattern.size() - 1;
        while (it >= 0 && pattern[it] == '?')
            --it;
        if (it == -1)
            for (int i = 0; i < cnt.size(); ++i)
                cout << i << ' ';
        else {
            int c = 0;
            int v = 0;
            int b = 0;
            for (int i = 0; i < pattern.size(); ++i) {
                if (pattern[i] == '?'){
                    if (b != i)
                        ++c;
                    int p = v;
                    while (p > 0 && tr[p].len >= i - 1) {
                        ++cnt[tr[p].len - i + 1];
                        if (i == it + 1 && cnt[tr[p].len - i + 1] == c)
                            cout << tr[p].len - i + 1 << ' ';
                        p = tr[p].link;
                    }
                    v = 0;
                    b = i + 1;
                    continue;
                }
                while (v >= 0 && tr[v].nxt.find(pattern[i]) == tr[v].nxt.end())
                    v = tr[v].link;
                if (v == -1)
                    v = 0;
                else
                    v = tr[v].nxt[pattern[i]];
            }
            ++c;
            int p = v;
            b = pattern.size() - 1;
            while (p > 0 && tr[p].len >= b) {
                ++cnt[tr[p].len - b];
                if (cnt[tr[p].len - b] == c)
                    cout << tr[p].len - b << ' ';
                p = tr[p].link;
            }
        }
        qcnt++;
    }
};

int main(){
    string s;
    cin >> s;
    RegexChecker checker(s);
    int q;
    cin >> q;
    for (int i = 0; i < q; ++i) {
        cin >> s;
        checker.check(s);
        cout << '\n';
    }

    return 0;
}
