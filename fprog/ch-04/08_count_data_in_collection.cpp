/*README:
Function composition.

The task is to "read a file of text, determine the n most frequently used
words, and print out a sorted list of those words along with their
frequencies."

UNIX shell script that solved this task:
tr -cs A-Za-z '\n' |
    tr A-Z a-z |
    sort |
    uniq -c |
    sort -rn |
    sed ${1}q


We’re going to analyze this problem to see how we can decompose it in C++.
The process, consists of the following transformations:

1. You have a file. You can easily open it and read the text it contains.
   Instead of a single string, you want to get a list of words that appear in
   that text: std::vector<std::string>

2. Put all those words into a std::unordered_map<std::string, unsigned int>,
   where you’ll keep a count for each word you find. (You’re using
   std::unordered_map because it’s faster than std::map and you don’t need the
   items to be sorted at this point.)

3. Iterate through all items in the map (the items will be instances of
   std::pair<const std::string, unsigned int>), and swap each pair around to
   have the count first and then the word.

4. Sort the collection of pairs lexicographically (first on the first item of
   the pair, but descending, and then on the second). This gives you the most
   frequent words first and the least frequent ones last.

5. Print out the words.

Looking at this example, you need to create five functions. All functions
should do only one simple thing and be created in a way that makes them compose
easily. The output of one function can be easily passed as the input of another.

1. First function:
The original file can be seen as a collection of characters—say,
a std::string — so your first function, let’s call it words, needs to be able
to receive the std::string as its argument. Its return type should be a
collection of words, so you can use std::vector<std::string>.

std::vector<std::string> words(const std::string& text);

2. Second function:
Let’s call it count_occurrences — gets a list of words, so, again
a std::vector<std::string>. It creates a std::unordered_map<std::string, 
unsigned int> that stores all the words and the number of times each word
appears in the text. You can even make it a function template so it can be
used for other things, not just strings:

template <typename T>
std::unordered_map<T, unsigned int> count_occurrences(
        const std::vector<T>& items);

With a bit of light template magic:
You also could make a template for the collection, not only the contained type:

template <typename C, typename T = typename C::value_type>
std::unordered_map<T, unsigned int> count_occurrences(
        const C& collection)

This function would be able to accept any collection that allows you to deduce
the type of the contained items (C::value_type). You could invoke it to count
occurrences of characters in a string, strings in a vector of strings, integer
values in a list of integers, and so on.

3. Third function:
The third function takes each pair of values from the container and creates a
new pair with the elements reversed. It needs to return a collection of newly
created pairs. Because you’re planning to sort it later, and you want to create
easily composable functions, you can return it as a vector:

template <typename C,
          typename P1 = typename C::value_type::first_type,
          typename P2 = typename C::value_type::second_type
         >
std::vector<std::pair<P2, P1>> reverse_pairs(const C& collection);

4,5. After that, you only need a function that sorts a vector
(sort_by_frequency) and a function that can write that vector to the standard
output (print_pairs).

When you compose these functions, you get the final result. Generally, function
composition in C++ is done by composing the functions at the point in code
where you want to call them on a specific original value. In this case, it
looks like this:

void print_common_words(const std::string& text)
{
    return print_pairs(
        sort_by_frequency(
            reverse_pairs(
                count_occurrences(
                    words(text)
                )
            )
        )
    );
}

In this example, you’ve seen how a problem can be split into a few functions
that are fairly small and easy to implement. You started with a bigger problem
and, instead of analyzing the steps you need to perform to achieve the result,
began thinking about what transformations you need to perform on the input.
You created a short and simple function for each of those transformations. And,
finally, you composed them into one bigger function that solves the problem.
*/

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>


template <typename C, typename T = typename C::value_type>
std::unordered_map<T, unsigned int> count_occurences(
        const C &collection)
{
    std::unordered_map<T, unsigned int> result;

    for (const auto &c: collection) {
        result[c]++;
    }

    return result;
}


/*Reverses all pairs in a collection.

Initializes type C to be the type of the collection, P1 to be the type of the
first item in a pair coming from the source collection (with const removed),
and P2 to be the type of second item in the pair.

std::remove_cv:
Obtains the type T without any top-level const or volatile qualification.
*/
template <
    typename C,
    typename P1 = typename std::remove_cv<
             typename C::value_type::first_type>::type,
    typename P2 = typename C::value_type::second_type
    >
std::vector<std::pair<P2, P1>> reverse_pairs(const C &items)
{
    std::vector<std::pair<P2, P1>> result(items.size());

    // For each pair (first, second) in the source collection,
    // put (second, first) into the destination collection
    std::transform(
        std::begin(items), std::end(items),
        std::begin(result),
        [] (const std::pair<const P1, P2> &p)
        {
            return std::make_pair(p.second, p.first);
        }
    );

    return result;
}


int main(int argc, char *argv[])
{
    std::string sentence = "Hello world";
    std::vector<std::string> words { std::string("The"),
        std::string("Farm"),
        std::string("from"),
        std::string("the"),
        std::string("Animal"),
        std::string("Farm")
    };

    for (const auto& item: reverse_pairs(count_occurences(sentence))) {
        std::cout << item.first << " " << item.second << std::endl;
    }

    for (const auto& item: reverse_pairs(count_occurences(words))) {
        std::cout << item.first << " " << item.second << std::endl;
    }

    return 0;
}
