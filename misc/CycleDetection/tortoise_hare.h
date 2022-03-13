/*
 * Floyd's tortoise and hare algorithm for cycle detection.
 * Runs in O(n) time and O(1) space.
 *
 *  head         +-----------------+
 *   |           |                 |
 *   v           v                 |
 *   A --> B --> C --> D --> E --> F
 *                           ^
 *                           | meet
 *
 *   |<--  k  -->|<--  d  -->|
 *
 * f := fast (length that the hare has gone)
 * s := slow (length that the tortoise has gone)
 * f == 2*s (the hare moves twice as fast as the tortoise)
 * c := cycle (loop) length, in the above diagram c==4
 * k := length from the 1st element of the cycle to head
 * d := distance between the meeting pos and 1st elem of the cycle
 *
 * Now, let's do the math :)
 * f = k + d + m*c
 * s = k + d + n*c
 * f = 2*s
 * Here, m and n denotes on their first meeting how many cycles
 * the hare and the tortoise have gone through, respectively.
 *
 * Thus,      s = f - s = (m-n)*c
 * Therefore, k + d + n*c = (m-n)*c
 *      ==>   k + d = (m-2*n)*c
 *      ==>   k = (m-2*n)*c - d
 *
 * This means k+d is equal to some multiple times of the cycle length,
 * and k has the implication that now if we let one (say, the tortoise)
 * walk from the head and the other one (thus the hare) walk from the
 * position where they met (both move one step at each time), then they
 * will meet at the 1st element of the cycle (the hare may have gone
 * through many cycles, because of the factor (m-2*n) here). After that,
 * the cycle length c can be easily computed by letting one sit still
 * and the other walk from there (and count the steps) until they meet
 * again.
 *
 * See also https://en.wikipedia.org/wiki/Cycle_detection.
 *
 * Param:
 *   head: the list head, the list node must contain a `next` pointer.
 *   lambda: the loop length, if it has a cycle, otherwise set it zero.
 *   mu: distance between the head and the 1st element of the cycle. If
 *       there is no cycle, a zero is set.
 *
 * Returns:
 *   the pointer to the 1st element of the cycle, null if no cycle.
 */
template<typename NodePtr>
NodePtr tortoise_hare(NodePtr head, int& lambda, int& mu)
{
    if (head == nullptr) {
        lambda = mu = 0;
        return nullptr;
    }

    NodePtr tortoise = head;
    NodePtr hare = head;
    while (hare && hare->next) {
        tortoise = tortoise->next;
        hare = hare->next->next;     // hare moves twice as fast
        if (tortoise == hare) break; // where we fell in love
    }

    // the hare first arrived at the end, no cycle found, happy
    if (hare == nullptr || hare->next == nullptr) {
        lambda = mu = 0;
        return nullptr;
    }

    // now they must have met somewhere
    mu = 0;
    tortoise = head;
    while (hare != tortoise) {
        hare = hare->next;
        tortoise = tortoise->next;
        ++mu;
    }

    // origin of the evil :)
    NodePtr where_the_loop_begins = tortoise;

    // compute the loop length
    lambda = 1;
    hare = hare->next;
    while (hare != tortoise) {
        hare = hare->next;
        ++lambda;
    }

    return where_the_loop_begins;
}
