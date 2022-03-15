#include <iostream>
#include <fstream>
#include <string>
#include "orderbook.h"

int main() {
  OrderBook book;

  std::string line;
  std::ifstream r("market_data_file.txt");
  if (r.is_open())
  {
    while (getline(r, line))
    {
      // std::cout << line << '\n';
      Order ord{};
      std::string a = line.substr(0, line.find(","));
      if (a == "A") {
        ord.action = 1;
      } else if (a == "X") {
        ord.action = 2;
      }
      line = line.substr(line.find(",") + 1);

      a = line.substr(0, line.find(","));
      ord.id = std::stoi(a);
      line = line.substr(line.find(",") + 1);

      a = line.substr(0, line.find(","));
      if (a == "B") {
        ord.side = 1;
      } else if (a == "S") {
        ord.side = 2;
      }
      line = line.substr(line.find(",") + 1);

      a = line.substr(0, line.find(","));
      ord.qty = std::stoi(a);
      ord.working_qty = ord.qty;
      ord.isDone = false;

      line = line.substr(line.find(",") + 1);
      ord.price = std::stoi(line);
      // std::cout << "order: " << ord.action << " " << ord.id << " " << ord.side << " " << ord.qty << " " << ord.price << " " << ord.working_qty << " " << ord.isDone << '\n';

      if (ord.action == 1) {
        book.Add(ord);
      } else if (ord.action == 2) {
        book.Cancel(ord);
      }

    }

    book.Print();
    r.close();
  }
}


// no error handling, input format
// assume order id is int
// centos7 c++11 enabled, gcc 4.8.3
// g++ main.cc orderbook.cc -std=c++11