#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;
namespace lib {

class Task {
public:
    virtual void load(std::istream& in) = 0;
    virtual double f(const std::vector<int>& x) = 0;
    virtual std::vector<int> neighbour(const std::vector<int>& x) = 0;
    virtual std::vector<int> random() = 0;
};

class Solver {
public:
    virtual std::vector<int> solve() = 0;
};

}



class Task : public lib::Task {
public:
    int n = 0, W = 0;
    vector<int> v, w;

    void load(istream& in) override {
        in >> n >> W;
        v.resize(n);
        w.resize(n);
        for (int i = 0; i < n; i++)
            in >> v[i] >> w[i];
    }

    double f(const vector<int>& x) override {
        int sumV = 0, sumW = 0;
        for (int i = 0; i < n; i++) {
            if (x[i] == 1) {
                sumV += v[i];
                sumW += w[i];
            }
        }
        if (sumW > W) return 0;
        return sumV;
    }

    vector<int> neighbour(const vector<int>& x0) override {
        vector<int> x = x0;
        int i = rand() % n;
        x[i] = 1 - x[i];
        return x;
    }

    vector<int> random() override {
        vector<int> x(n);
        int sumW = 0;
        for (int i = 0; i < n; i++) {
            x[i] = rand() % 2;
            if (x[i] == 1) sumW += w[i];
        }
        while (sumW > W) {
            int i = rand() % n;
            if (x[i] == 1) {
                x[i] = 0;
                sumW -= w[i];
            }
        }
        return x;
    }
};

static vector<int> sortedByDensity(Task& t) {
    vector<int> order(t.n);
    for (int i = 0; i < t.n; i++) order[i] = i;
    sort(order.begin(), order.end(), [&](int a, int b) {
        return (double)t.v[a] / t.w[a] > (double)t.v[b] / t.w[b];
    });
    return order;
}


class Greedy : public lib::Solver {
    Task& t;
public:
    explicit Greedy(Task& task) : t(task) {}

    vector<int> solve() override {
        vector<int> order = sortedByDensity(t);
        vector<int> x(t.n, 0);
        int sumW = 0;
        for (int i = 0; i < t.n; i++) {
            int id = order[i];
            if (sumW + t.w[id] <= t.W) {
                x[id] = 1;
                sumW += t.w[id];
            }
        }
        return x;
    }
};


int main() {
    srand(time(0));
    Task t;
    t.load(cin);
    cout << t.f(Greedy(t).solve()) << endl;
    return 0;
}
