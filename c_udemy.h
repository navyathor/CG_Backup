IPC: Inter process communication is the communication between the processes. It totally depends on the specific requirements of the processes.

Pipes: Pipes are used when we want to transfer small amount of data b/w related processes.Pipe is half duplex.
message queue: it is used when you want to transfer large amount of data or data to transfer asynchronously.
shared memory: it is also used to transfer large amount of data/when you need to synchronize data between processes.
sockets:when you need to transfer data b/w processes on different machines or to transfer data over a network.
file lock:file locking where multiple processes access same file such as file or dta log, this also prevents the race condition and when multiple proceses modifying same data in memory periodically yflushing data to the disk 
signals:signals when a process needs to respond to an event such as arrival of data on socket or the expiration of a timer. signals can also `be used in IPC allowing one process to notify or another process event or condition.