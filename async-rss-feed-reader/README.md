# Objective
Build a rss feed server that can aggregate news periodically and can be accessed via commandline in a decluttered format.

# Design Approach

## Networking core
1. Build a async server 
2. Build a thread safe queue
3. Build a client 
4. Build a messaging protocol

## For later
1. Extend to be used a an HTTP server
2. Add the XML import functionality for getting feeds
3. Design a weigtage based fetch mechanism
4. Thread safe store for all the fetched items to be searchable based on topics.
