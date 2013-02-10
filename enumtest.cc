#include <cstdio>
#include <vector>

using namespace std;

enum Hoi { A, B, };

int main() {
  vector<Hoi> ha;
  ha.push_back(A);
  ha.push_back(B);
  ha.push_back((long)5);
  for(int i = 0; i < ha.size(); i++)
    printf("%d\n", ha[i]);
  return 0;
}
