#include <iostream>
#include "substring.h"

using namespace std;

int main() {
  cout << substring_position("this", "this is a simple exercise") << endl;
  cout << substring_position("is", "this is a simple exercise") << endl;
  cout << substring_position("is a", "this is a simple exercise") << endl;
  cout << substring_position("is an", "this is a simple exercise") << endl;
  cout << substring_position("exercise", "this is a simple exercise") << endl;
  cout << substring_position("simple exercise", "this is a simple") << endl;
  cout << substring_position("", "this is a simple exercise") << endl;
  cout << substring_position("", "") << endl;

  cout << endl << "Tests for is_prefix:" << endl;

  cout << is_prefix("indent", "indentation") << " " << is_prefix("indent", "ind") << " " << is_prefix("indent", "applecake") << endl;

  return 0;
}