This project was started on July 13th, 2025.
The goal of this project is to create a Redis server from scratch. 
This project is a network server that uses TCP and handles peer connections through asynchronous I/O utilizing event loops.
The underlying data storage uses a key-value database. To minimize latency during database resizing, a custom database implementing progressive hashing was constructed.
The higher-level table currently supports the following 3 commands:
1. get
2. set
3. del

Sorted Sets were initially implemented using AVL trees for faster production. Later AVL trees will be replaced with B-Trees to replicate Redis logic.
More information will be added as the scope of the project becomes clear.