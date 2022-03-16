#include <string>

/*
 * Edit distance (often interchangeably with Levenshtein distance):
 * mimimum number of edits (ins, del, sub) required to turn the
 * source string into the target string.
 *
 * Runs in O(|S||T|) time.
 *
 * See also https://en.wikipedia.org/wiki/Edit_distance and
 *          https://en.wikipedia.org/wiki/Levenshtein_distance.
 */
int edit_distance(const std::string& S, const std::string& T);
