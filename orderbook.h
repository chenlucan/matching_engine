#include <map>
#include <vector>

struct Order {
  int action; // 1 - new, 2 - cancel
  int id;
  int side; // 1 buy, 2 sell
  int qty;
  int price;
  int working_qty;
  bool isDone = false;
};

struct Trade {
    int id;
    int qty;
    int price;
};

class OrderBook {
    public:
    void Add(Order ord);
    void Cancel(Order ord);
    void Print();

    private:
    std::map<int, std::vector<Order*>> bidBook{};
    std::map<int, std::vector<Order*>> askBook{};

    std::map<int, Order> orders{};
    std::vector<Trade> trades{};
    int tradeId;
};