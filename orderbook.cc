#include "orderbook.h"

#include <iostream>

void OrderBook::Add(Order ord) {
  if (ord.action != 1) {
    return;
  }
  // new order
  if (orders.find(ord.id) != orders.end()) {
    return;
  }
  orders[ord.id] = ord;
  auto p = &orders[ord.id];
  std::vector<std::vector<Order*>> ord_vv{};

  if (p->side == 1) { // buy
    for (auto it = askBook.begin(); it != askBook.end(); ++it) {
      if (it->first <= p->price) {
        ord_vv.push_back(it->second);
      }
    }
  } else { // sell
    for (auto it = bidBook.rbegin(); it != bidBook.rend(); ++it) {
      if (it->first >= p->price) {
        ord_vv.push_back(it->second);
      }
    }
  }

  // match
  std::vector<int> matched_ids{};
  std::vector<Trade> matched_trades{};
  for (auto it = ord_vv.begin(); it != ord_vv.end(); ++it) {
    for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
      if ((*it2)->isDone) {
        (*it2)->working_qty = 0;
        continue;
      }
      if (p->working_qty == 0) {
        p->isDone = true;
        break;
      }

      if ((*it2)->working_qty >= p->working_qty) {
        Trade t{tradeId++, p->working_qty, (*it2)->price};
        (*it2)->working_qty -= p->working_qty;
        p->working_qty = 0;
        p->isDone = true;
        matched_trades.push_back(t);
        matched_ids.push_back((*it2)->id);
        if ((*it2)->working_qty == 0) {
          (*it2)->isDone = true;
        }
        break;
      }
      Trade t{tradeId++, (*it2)->working_qty, (*it2)->price};
      p->working_qty -= (*it2)->working_qty;
      (*it2)->working_qty = 0;
      (*it2)->isDone = true;
      matched_trades.push_back(t);
      matched_ids.push_back((*it2)->id);
    }
  }
  if (!p->isDone) {
    if (p->side == 1) {
      bidBook[p->price].push_back(p);
    } else {
      askBook[p->price].push_back(p);
    }
  }

  // print trades
  for (auto & t : matched_trades) {
    std::cout << "Trade - id:" << t.id << ", qty:" << t.qty << ", price:" << t.price << std::endl;
  }

  // flush orders from bid/ask book
  auto book = &bidBook;
  if (p->side == 1) {
    book = &askBook;
  }
  for (auto & id : matched_ids) {
    if (orders[id].isDone) {
      for (auto it = book->begin(); it != book->end(); /*++it*/) {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); /*++it2*/) {
          if ((*it2)->id == id) {
            it->second.erase(it2);
          } else {
            ++it2;
          }
        }
        if (it->second.size() == 0) {
          it = book->erase(it);
        } else {
          ++it;
        }
      }
    }
  }
}

void  OrderBook::Cancel(Order ord) {
  if (ord.action == 2) {
    orders[ord.id].isDone = true;
    orders[ord.id].working_qty = 0;
  }
}

void  OrderBook::Print() {
  std::cout << "=================" << std::endl;
  std::cout << "ASK" << std::endl;
  for (auto it = askBook.rbegin(); it != askBook.rend(); ++it) {
    std::cout << it->first << ": ";
    for (auto & ord : it->second) {
      if (ord->isDone) {
        continue;
      }
      std::cout << ord->working_qty << "(" << ord->id << ") ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << "------------" << std::endl;
  std::cout << std::endl;

  for (auto it = bidBook.rbegin(); it != bidBook.rend(); ++it) {
    std::cout << it->first << ": ";
    for (auto & ord : it->second) {
      if (ord->isDone) {
        continue;
      }
      std::cout << ord->working_qty << "(" << ord->id << ") ";
    }
    std::cout << std::endl;
  }
  std::cout << "BID:" << std::endl;
  std::cout << "=================" << std::endl;
}
