# Objective
Build a RSS feed server that can aggregate news periodically and can be accessed via commandline in a decluttered format. If running centerally on a closed network the feeds can be made richer with more user preferences being loaded.

# Prequisites
1. gcc (or any other compiler) with C++17 support. (Developer on gcc-10)
2. Using ASIO as standalone library. Can be downloaded from [here](https://think-async.com/Asio/). Update the path in Makefile to reflect the location of the ASIO headers.

# Design Approach

## Networking core
1. Build a async server 
2. Build a thread safe queue
3. Build a client 
4. Build a messaging protocol

## TODO
1. A more cleaner namespace structure
2. TDD and test suite
3. Abstract interfaces should be possible
4. Logger
5. More exception handling

# Functionality For later
1. Extend to be used a an HTTP server
2. Add the XML import functionality for getting feeds
3. Design a weigtage based fetch mechanism
4. Thread safe store for all the fetched items to be searchable based on topics.
5. A white list and black list mechanism
