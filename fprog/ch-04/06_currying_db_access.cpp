/*README:
Using currying with database access

This file has a theoritical explanation only - don't compile this code.
*/

/*[1] Theory

Let’s consider a real-world example. You’re writing an application that
connects to a database and performs queries on it. Maybe you want to get a
list of all people who rated a particular movie.

You’re using a library that allows you to create multiple database connections,
to initiate connection subsessions (for handling database transactions and
such), and, obviously, to query the stored data. Let’s say the main query
function looks like this:

result_t query(connection_t& connection,
                session_t& session,
                const std::string& table_name,
                const std::string& filter);


It queries all the rows in a given table that match the given filter. All the
queries need to have the connection and session specified so the library can
tell which database to query and what to include in a single database
transaction.

Many applications don’t need to create multiple connections, but only a single
one for all queries. One approach that library developers use in this case is
to make the query function a member function in the connection_t type. Another
viable alternative would be to create an overloaded query function that
wouldn’t have the connection as one of the arguments; it would use the default
system connection instead.

This situation becomes more entangled when you consider the possibility that
some users don’t need multiple sessions. If they access a read-only database,
there isn’t much point in defining transactions. The library author can also
add a query function to a session object, which the user will use throughout
the application, or you can get another overload of the original query
function, this time without both session and connection arguments.

After this, it would also be easy to imagine that parts of a user’s application
might need access to only a single table (in this case, you just need a table
that contains the user scores). The library might then provide a table_t class
that has its own query member function, and so on.

It isn’t easy to predict all the use cases a user might have. Those we’ve
predicted make the library more complex than it needs to be. Let’s see whether
having a single query function—like the preceding one, but curried—can be
sufficient to cover all these cases without making the library developer go
nuts over all the overloads and classes you force them to create for your use
cases.

If users need multiple database connections, they can either use the query
function directly, by always specifying the connection and session, or create
a separate function for each connection that will apply the first argument of
the curried function. The latter will also be useful if only one database
connection is needed for the entire application.

If only a single database is needed, and a single session, it’s easy to create
a function that covers that case exactly; you need to call the curried query
function and pass it the connection and session, and it will return a function
that you can use from there. And if you need to query a single table multiple
times, you can bind the table argument as well.

By providing a curried version (see driver1() below) of the query function,
you allow users to create exactly the functions needed for their particular
use case, without complicating your API. This is a big improvement in code
reusability because you aren’t creating separate classes that might end up
with separate bugs that are discovered only when a particular use case
appears and that specific class is used. Here, a single implementation can
be used in all the use cases.
*/

#if 0
void driver1() {
    auto table = "Movies";
    auto filter = "Name = \"Sintel\"";

    /*Uses the query function as a normal function by passing all required
      arguments to it and getting a list of query results directly*/
    results = query(local_connection, session, table, filter);


    /*Creates separate functions that are tied to their respective database
      connections. This is useful when you often use the same connection
      over and over*/
    auto local_query = query(local_connection);
    auto remote_query = query(remote_connection);
    results = local_query(session, table, filter);

    /*When you need only a single connection and a single session,
      you can create a function that binds these two values so that
      you can omit them in the rest of the program.*/
    auto main_query = query(local_connection, main_session);
    results = main_query(table, filter);

    /*If you often perform queries over the same table, you can create
      a function that always queries that table.*/
    auto movies_query = main_query(table);
    results = movies_query(filter); 
}
#endif

int main() {
    //driver1();
    return 0;
}