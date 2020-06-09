#include "Query.h"
#include "TextQuery.h"
#include <memory>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <regex>
using namespace std;

regex word{R"(\w+)"};
regex wordOrWord{R"(\s*\w+\s*OR\s*\w+\s*)"};
regex wordAndWord{R"(\s*\w+\s*AND\s*\w+\s*)"};
regex wordBesideWord{R"(\s*\w+\s*BESIDE\s*\w+\s*)"};
string ignoreSymbols = R"([.?,"'\s]?)";
string multipleSpace = R"(\s+)";
pair<string, string> splitBySpace(const string &l);

shared_ptr<QueryBase> QueryBase::factory(const string &l) {
   if (regex_match(l, word))
      return shared_ptr<QueryBase>(new WordQuery(l));

   else if (regex_match(l, wordOrWord)) {
      pair<string, string> words = splitBySpace(l);
      return shared_ptr<QueryBase>(new OrQuery(words.first, words.second));
   }

   else if (regex_match(l, wordAndWord)) {
      pair<string, string> words = splitBySpace(l);
      return shared_ptr<QueryBase>(new AndQuery(words.first, words.second));
   }
   
   else if (regex_match(l, wordBesideWord)) {
      pair<string, string> words = splitBySpace(l);
      return shared_ptr<QueryBase>(new BesideQuery(words.first, words.second));
   }
   // throw "Unrecognized search" when nothing fits to the above patterns
   else throw invalid_argument("Unrecognized search");  
}

QueryResult AndQuery::eval(const TextQuery &text) const {
   QueryResult left_result = text.query(lhs);
   QueryResult right_result = text.query(rhs);
   auto ret_lines = make_shared<set<line_no>>();
   set_intersection(left_result.begin(), left_result.end(),
                         right_result.begin(), right_result.end(),
                         inserter(*ret_lines, ret_lines->begin()));
   return QueryResult(rep(), ret_lines, left_result.get_file());
}

QueryResult OrQuery::eval(const TextQuery &text) const {
   QueryResult left_result = text.query(lhs);
   QueryResult right_result = text.query(rhs);
   auto ret_lines =
       make_shared<set<line_no>>(left_result.begin(), left_result.end());
   ret_lines->insert(right_result.begin(), right_result.end());
   return QueryResult(rep(), ret_lines, left_result.get_file());
}

QueryResult BesideQuery::eval(const TextQuery &text) const {
   QueryResult andResult = AndQuery::eval(text);
   QueryResult ans = QueryResult(rep(), make_shared<set<line_no>>(), andResult.get_file());
   if (andResult.begin() != andResult.end()) { // AndQuery is not empty
      auto iter = andResult.begin();
      auto ret_lines = make_shared<set<line_no>>();
      while (iter != andResult.end()) {
         string result = *(andResult.get_file()->begin() + *(iter));
         regex lhsBesideRhs{ignoreSymbols + lhs + ignoreSymbols +
                            multipleSpace +
                            ignoreSymbols + rhs + ignoreSymbols};
         if (regex_search(result, lhsBesideRhs)) // Found!
            ret_lines->insert(*(iter));
         iter++;
      }
      ans = QueryResult(rep(), ret_lines, andResult.get_file());
   }
   return ans;
}

// Help method to split the line by space
pair<string, string> splitBySpace(const string &l) {
   vector<string> result;
   istringstream iss(l);
   for (string s; iss >> s;)
      result.push_back(s);

   return make_pair(result.at(0), result.at(2));
}