#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <cassert>

using namespace std;

enum class Side : int {
    BUY,
    SELL,
};

// simplified order representation according to `orders.csv`
struct Order {
    int id;
    Side side;
    string party;  // SSO, 32-byte inlined string
    double price;
    int quantity;
    int timestamp;
};
// static_assert(sizeof(Order) == 56);

const char *FF_DOS = nullptr;

unique_ptr<Order> fetch_an_order()
{
    static int line_no = 0;
    line_no++;
    const char newline_delim = FF_DOS ? '\r' : '\n';

    // parse the line to extract fields
    int id;
    string party;
    double price;
    int quantity;
    int timestamp;
    string side_str;

    // assume the input format is consistent
    // 70000001,Mal,73.21,100,100001,BUY
    char delimiter;
    if (!(cin >> id >> delimiter) || delimiter != ',' || !getline(cin, party, ',') ||
        !(cin >> price >> delimiter) || delimiter != ',' || !(cin >> quantity >> delimiter) ||
        delimiter != ',' || !(cin >> timestamp >> delimiter) || delimiter != ',' ||
        !getline(cin, side_str, newline_delim) || side_str.empty()) {
        if (cin.eof())
            return nullptr;
        cerr << "line_no=" << line_no << ": error parsing\n";
        exit(EXIT_FAILURE);
    }

    Side side;
    if (side_str == "BUY")
        side = Side::BUY;
    else /* if (side_str == "SELL") */
        side = Side::SELL;

    auto order = make_unique<Order>();
    order->id = id;
    order->side = side;
    order->party = move(party);
    order->price = price;
    order->quantity = quantity;
    order->timestamp = timestamp;

    return order;
}

// for bids, MaxPQ
struct OrderLess {
    bool operator()(const unique_ptr<Order> &a, const unique_ptr<Order> &b)
    {
        // price-time-size priority
        if (a->price < b->price)  // higher first
            return true;
        if (a->price == b->price) {
            if (a->timestamp > b->timestamp)  // earlier first
                return true;
            if (a->timestamp == b->timestamp && a->quantity < b->quantity)  // bigger size first
                return true;
        }
        return false;
    }
};

// for offers, MinPQ
struct OrderGreater {
    bool operator()(const unique_ptr<Order> &a, const unique_ptr<Order> &b)
    {
        // price-time-size priority
        if (a->price > b->price)  // lower first
            return true;
        if (a->price == b->price) {
            if (a->timestamp > b->timestamp)  // earlier first
                return true;
            if (a->timestamp == b->timestamp && a->quantity < b->quantity)  // bigger size first
                return true;
        }
        return false;
    }
};

struct NetPosition {
    int shares = 0;
    double earnings = 0.0;
};

int volume = 0;

void update_net_positions(const Order *offer, const Order *bid, int quantity,
                          unordered_map<string, NetPosition> &net_positions)
{
    auto &seller_net_position = net_positions[offer->party];
    seller_net_position.shares -= quantity;
    seller_net_position.earnings += offer->price * quantity;

    auto &buyer_net_position = net_positions[bid->party];
    buyer_net_position.shares += quantity;
    buyer_net_position.earnings -= offer->price * quantity;

    volume += quantity;
}

typedef priority_queue<unique_ptr<Order>, vector<unique_ptr<Order>>, OrderLess> MaxPQ;
typedef priority_queue<unique_ptr<Order>, vector<unique_ptr<Order>>, OrderGreater> MinPQ;

void do_trading(MinPQ &offers, MaxPQ &bids, unordered_map<string, NetPosition> &net_positions)
{
    if (offers.empty() || bids.empty())
        return;
    Order *offer = offers.top().get();
    Order *bid = bids.top().get();
    while (bid->price >= offer->price) {
        int quantity = min(offer->quantity, bid->quantity);
        update_net_positions(offer, bid, quantity, net_positions);

        if (bid->quantity == offer->quantity) {
            bids.pop();
            offers.pop();
            return;
        }
        if (bid->quantity > offer->quantity) {
            bid->quantity -= offer->quantity;
            offers.pop();
            if (offers.empty())
                return;
            offer = offers.top().get();
        } else {
            // bid->quantity < offer->quantity
            offer->quantity -= bid->quantity;
            bids.pop();
            if (bids.empty())
                return;
            bid = bids.top().get();
        }
    }
}

// limit orders
//
// 70000001,Mal,73.21,100,100001,BUY      // no seller
// 70000002,Wash,76.56,200,100016,SELL    // no bid with this price
// 70000003,Mal,85.04,100,100075,BUY      // buys 100 shares at 76.56 from Wash
// 70000004,Simon,72.54,100,100137,BUY    // no offer meets the given bid price
// 70000005,River,57.48,600,100142,BUY    // no offer meets the given bid price
// 70000006,Simon,90.96,100,100231,SELL   // no bid with this price
// 70000007,Simon,50.11,100,100292,BUY    // no offer meets the given bid price
// 70000008,Kaylee,64.4,100,100313,BUY    // no offer meets the given bid price
// 70000009,River,96.65,200,100334,SELL   // no bid with this price
// 70000010,Mal,71.09,200,100361,SELL     // sells 100 shares to Mal & Simon each
unordered_map<string, NetPosition> normal_priority_rules()
{
    //
    //  |****|
    //  |****|           bids
    //  |****|          +----+
    //  |****|          |~~~~|
    //  |****|          |~~~~|
    //  +----+          |~~~~|
    //  offers          |~~~~|
    //                  |~~~~|
    //
    // trades happen when there are bid prices >= offer prices
    //
    MinPQ offers;
    MaxPQ bids;

    unordered_map<string, NetPosition> net_positions;
    for (;;) {
        auto order = fetch_an_order();
        if (!order)
            return net_positions;
        if (order->side == Side::BUY)
            bids.push(move(order));
        else
            offers.push(move(order));
        do_trading(offers, bids, net_positions);
    }
    return net_positions;
}

struct OrderPQ {
    double price = 0.0;
    int quantity = 0;
};

struct matching_info_t {
    double matching_price;
    double max_transaction_amount;
};

// (price, quantity) tuples of bid-ask orders
//
// Ask set A = {                  Bid set B = {
//      (ap_1, aq_1),                  (bp_1, bq_1),
//      (ap_2, aq_2),                  (bp_2, bq_2),
//        ...                            ...
//      (ap_m, aq_m)                   (bp_n, bq_n)
// }                              }
//
//
//              |    |                                ^  higher price
//              |    |                                |
//              |    |           bids                 |
//              |    |          +----+                |
//              |    |          |    |                |
//              |    |          |    |
//              |    |          | BA |
//              |    |          |    |
//              |    |          |    |
//              |    |          |    |
//      ---------------------------------------- L, the matching price
//              |    |          |    |
//              |    |          |    |
//              |    |          |    |
//              | AB |          |    |
//              |    |          |    |
//              |    |          |    |
//              +----+          |    |
//               asks           |    |
//                              |    |
//
// Find the matching price that maximizes the total transaction amount.
matching_info_t compute_matching_price(const vector<OrderPQ> &asks, const vector<OrderPQ> &bids,
                                       int Q_A_b, int A_gt_b_idx)
{
    if (asks.empty() || bids.empty() || A_gt_b_idx < 1)
        return {-1, -1};  // no trade
    double a = asks[0].price;
    double b = bids[0].price;
    int Q_A_p = Q_A_b;
    int Q_B_p = bids[0].quantity;
    int a_idx = A_gt_b_idx - 1;
    int b_idx = 0;
    const int asks_sz = asks.size();
    const int bids_sz = bids.size();

    assert(asks[a_idx].price <= b);
    if (A_gt_b_idx < asks_sz)
        assert(asks[A_gt_b_idx].price > b);

    double max_transaction_amount = 0;
    double matching_price = -1;

    // try each bid/ask price in [a, b] in descending order
    for (double p = b; p >= a;) {
        double transaction_amount = p * min(Q_A_p, Q_B_p);
        if (transaction_amount > max_transaction_amount) {
            max_transaction_amount = transaction_amount;
            matching_price = p;
        }

        if (Q_A_p <= Q_B_p)
            // min(Q_A_p, Q_B_p) = Q_A_p, which's non-increasing as p decreases. Thus,
            // max_transaction_amount = p * min(Q_A_p, Q_B_p) will decrease if we continue.
            break;

        //  asks        bids
        //   |*         ~| <- p
        //   |          ~|
        //   |*          |
        //   |*         ~|
        //   |*         ~|
        if (asks[a_idx].price == p) {
            Q_A_p -= asks[a_idx].quantity;
            a_idx--;
        }
        if (bids[b_idx].price == p)
            b_idx++;

        if (a_idx < 0 || b_idx == bids_sz)
            break;

        // determine next price
        if (asks[a_idx].price <= bids[b_idx].price) {
            p = bids[b_idx].price;
            Q_B_p += bids[b_idx].quantity;
        } else {
            p = asks[a_idx].price;
        }
    }

    return {matching_price, max_transaction_amount};
}

template <typename Comp>
bool put_order(vector<OrderPQ> &orders, OrderPQ order, Comp comp)
{
    bool inserted = true;
    // e.g. put 5.7 in [5.3, 5.4, 5.6, 5.7, 5.8, 5.9]
    auto it = lower_bound(orders.begin(), orders.end(), order, comp);
    if (it != orders.end()) {
        if (it->price == order.price) {
            it->quantity += order.quantity;
            inserted = false;
        } else {
            orders.insert(it, order);
        }
    } else {
        orders.push_back(order);
    }
    return inserted;
}

bool put_ask_order(vector<OrderPQ> &asks, OrderPQ ask_order)
{
    return put_order(asks, ask_order, [](OrderPQ a, OrderPQ b) { return a.price < b.price; });
}

void put_bid_order(vector<OrderPQ> &bids, OrderPQ bid_order)
{
    put_order(bids, bid_order, [](OrderPQ a, OrderPQ b) { return a.price > b.price; });
}

matching_info_t auction_period_rules()
{
    vector<OrderPQ> asks;
    vector<OrderPQ> bids;
    double b = 0.0;      // max bid price
    int Q_A_b = 0;       // cumulative quantity of asks at or below price b
    int A_gt_b_idx = 0;  // index of first ask price > b
    matching_info_t matching_info{-1, -1};
    for (;;) {
        auto order = fetch_an_order();
        if (!order)
            break;
        if (order->side == Side::BUY) {
            double old_b = bids.empty() ? -1 : bids[0].price;
            put_bid_order(bids, {order->price, order->quantity});
            b = bids[0].price;
            if (b > old_b) {
                auto it = upper_bound(asks.begin(), asks.end(), OrderPQ{old_b, 0},
                                      [](OrderPQ a, OrderPQ b) { return a.price < b.price; });
                for (; it < asks.end() && it->price <= b; ++it)
                    Q_A_b += it->quantity;
                A_gt_b_idx = it - asks.begin();
            }
        } else {
            bool inserted = put_ask_order(asks, {order->price, order->quantity});
            if (order->price <= b) {
                Q_A_b += order->quantity;
                A_gt_b_idx += inserted;
            }
        }
        matching_info = compute_matching_price(asks, bids, Q_A_b, A_gt_b_idx);
        // display the matching price dynamically
        // ...
    }
    return matching_info;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " problem_no\n";
        exit(EXIT_FAILURE);
    }
    int problem_no = stoi(argv[1]);
    if (problem_no != 1 && problem_no != 2) {
        cerr << "problem_no is either 1 or 2\n";
        exit(EXIT_FAILURE);
    }

    // get input file format
    FF_DOS = getenv("FF_DOS");

    cout << std::setprecision(15);  // for double
    if (problem_no == 1) {
        unordered_map<string, NetPosition> net_positions = normal_priority_rules();
        for (const auto &[party, net_position] : net_positions)
            cout << party << ',' << net_position.shares << ',' << net_position.earnings << '\n';
        cerr << "volume: " << volume << '\n';
    } else {
        auto matching_info = auction_period_rules();
        cout << "matching price: " << matching_info.matching_price
             << ", max transaction amount: " << matching_info.max_transaction_amount << '\n';
    }

    return 0;
}
