//1. C

//	1. System call is provided by kernel (e.g. write(), read())
//	   Library call is provided by Library or user (e.g. printf()
//	2. * is dereference operator, & is reference operator
//	3. when string has nothing but a NULL terminator
//	4. char array, fail to alloc memory for NULL terminator
//	5. 
		void trunc(char* str, int max)
		{
			assert(str != NULL); 
			if (strlen(str) > max)
				str[max] = 0;
		}
//	6.
		void deep_copy(char** argv, char*** result); 
		{
			int count = 0;
			while (argv != NULL)
			{
				count++;
			}
			*result = malloc(count * sizeof(char*));
			for (int i = 0; i < count; i++)
			{
				memcpy((*result)[i], argv[i], strlen(argv[i]) + 1);
			}
			return 0;
		}

//	7.	
		int main(int argc, char const *argv[])
		{
			char* buffer = NULL;
			size_t length = 0;
			while ((getline(&buffer, &length, stdin)) != -1)
			{
				printf("%s", buffer);
			}
			free(buffer);
			return 0;
		}

//2. Memory

//	1.	4KB page -> 12 offset bits
//		64bit machine -> Topmost 52 bits splited to hold the index of multi-level tables
//	2.	Knuth's memory allocation scheme is based on linked list that manage the free space
//		The buddy memory allocation technique is a memory allocation algorithm that divides memory into partitions to try to satisfy a memory request as suitably as possible. 
//	3.	MMU(Memory Management Unit) is part of the CPU. It converts a virtual memory address into a physical address.
//		TLB(Translational Lookaside Buffer) is like the cache that stored the most recently used physical memory
//	4.	0x12345678  -> offset = 0x678, index = 0x12345
//	5.	Page fault: A page fault is when a running program tries to access some virtual memory in its address space that is not mapped to physical memory.
//		Error: ??
//		Internal Fragmentation: Unusable memory is contained within an allocated region.
//		External Fragmentation: External fragmentation arises when free memory is separated into small blocks and is interspersed by allocated memory.

//3. Processes and Threads 

//	1. Glocal variable and data on heap
//	2. Switch page context and use new address space
//	   Save the process state includes all the registers that the process may be using, especially the program counter, plus any other operating system specific data that may be necessary.
//	   Put into the task queue and reload when it's its turn to run, switch kernel stack and hardware context
//	   When the scheduler switches the CPU from executing one process to execute another, the context switcher saves the content of all processor registers for the process being removed from the CPU, in its process descriptor. The context of a process is represented in the process control block of a process.
//	3. Save the process state includes all the registers that the process may be using, especially the program counter, plus any other operating system specific data that may be necessary.
//	   Put into the task queue and reload when it's its turn to run, switch kernel stack and hardware context
//	4. An orphan process is a computer process whose parent process has finished or terminated, though it remains running itself. 
//     Any orphan process will be automatically adopted by init process
//	5. A zombie process or defunct process is a process that has completed execution (via the exit system call) but still has an entry in the process table, we can create a zombie process by not wait them to finish
//	6. 1). Thread returns; 2). pthread_exit()； 3). pthread_cancel(); 4). Process exits (call exit() or signal to exit such as SEGV 11 i.e. segfault)

//4. Scheduling
//	1. Arrival time: The time that a process ready to be scheduled
//	   Pre-emption: The interruption of a computer process without its cooperation in order to perform another task
//	   Turnaround time: Turnaround time is the total time taken between the submission of a program/process/thread/task (Linux) for execution and the return of the complete output to the customer/user. 
//	   Waiting time: Time a process waited to be scheduled and performed, wait time  = (end - start) - execution time
//	   Response time: Response time is the total amount of time it takes to respond to a request for service.
//	2. Shortest job first
//	3. First-come/first-serve due to the convoy effect.
//	4. RR: Each task can execute in certain quantum, and when its current quantum expired, it has to switch so that other task can be executed
//	   Advantage: Fair
//	   Disadvantage: No priority, Unequal Task Lengths, Larger waiting time and Response time, Context Switches take a lot of time
//	5. FCFS: First come task will be executed at first and other tasks will wait in queue until their previous tasks finish
//	   Convoy Effect: In a multiprogramming system, if multiple processes are waiting for the CPU for execution in an F.C.F.S. system, and a slow processing process is utilizing the CPU then due to the convoy all fast processes waiting for CPU waits for unnecessarily long time.
//	6. Pre-emptive SJF: Shortest Total Time First
//	   Non-preemptive SJF:  Shortest Remaining Time First
//  7. If the quantum is too short, more time will be used to schedule the tasks than executing them, thus worsen the performance
//	   In the limit of large time slices Round Robin is identical to FCFS
//	8. Quantum expired, Preempted

//5. Synchronization and Deadlock
//	1. Circular Wait - a cycle where every process is waiting on a resource some other process has while holding resources required by at least one other process 
//	   Mutual Exclusion - resources are not shared
//	   Hold and Wait - process that has incomplete set of resources will keep holding onto resources while waiting for remaining requisite resources
//	   No Preemption - once a resource is acquired, a process will not release the resource nor could it be taken forcibly 
//	2. Deadlock
//  3. Deadlock Prevention: Preventing deadlocks by constraining how requests for resources can be made in the system and how they are handled.
//	   Break the Coffman Condition so that Deadlock is not going to happen.
//	   Deadlock Avoidance: The system dynamically considers every request and decides whether it is safe to grant it at this point.
//	   Deadlock Detection: Use resources allocation graph or other algorithms to detent the probability of deadlock
//	4.
		#define 
		void resources_barrier()
		{
			pthread_mutex_lock(&m);
			if (ready)
			{
				pthread_cond_broadcast(&cv, &m);
			}
			else
			{
				while (not_ready)
				{
					pthread_cond_wait(&cv, &m);
				}
			}
			pthread_mutex_unlock(&m);
		}


//	5. 
		#define MAX 100

		void insert(void* data, queue_t* queue)
		{
			pthread_mutex_lock(&m);
			while (queue_size(queue) == MAX)
			{
				pthread_cond_wait(&cv, &m);
			}
			if (queue->tail == NULL)
			{
				queue->tail = malloc(sizeof(queue_node_t));
				queue->tail->data = data;
				queue->tail->next = NULL;
				queue->head = queue->tail;
			}
			else
			{
				queue->tail->next = malloc(sizeof(queue_node_t));
				queue->tail = queue->tail->next;
				queue->tail->data = data;
				queue->tail->next = NULL;
			}
			queue->size++;
			pthread_cond_signal(&cv);
			pthread_mutex_unlock(&m);
		}
		

		void* remove(queue_t* queue)
		{
			queue_node_t* ret_node = queue->head;
			while (queue_size(queue) == 0)
			{
				pthread_cond_wait(&cv, &m);
			}
			queue_node_t* ret_node = queue->head;
			if (ret_node == NULL)
			{
				queue->size--;
				pthread_cond_signal(&cv);
				pthread_mutex_unlock(&m);
				return ret_node;
			}
			else
			{
				void* temp = ret_node->data;
				queue->head = queue->head->next;
				free(ret);
				ret = NULL;
				queue->size--;
				pthread_cond_signal(&cv);
				pthread_mutex_unlock(&m);
				return temp;
			}
		}
//	6. 
		//Incorrect:
		void produce()
		{
			size++;
		}

		void consume()
		{
			size--;
		}
//	7. ...

//6. IPC and Signals
//	1.

		close(STDOUT_FILENO);
		dup2(output_file, STDOUT_FILENO);
		close(output_file);
//	2.  
		if (fork() == 0)
		{
			close(pipe[0]);
			close(STDOUT_FILENO);
			dup2(pipe[1], STDOUT_FILENO);
			close(pipe[1]);
			//execute
		}
//	3.  SIGINT, SIGKILL
//		kill(pid, SIGUSR1); or raise(SIGUSR1);
//	4.	Signals other than SIGKILL
//	5.	Signals other than SIGKILL and SIGSTOP can be caught
//		Singal disposition: what action will occur when a signal is delivered to the process
//	6.  
		void handler(int sig)
		{
			//Handle process
		}

		int int main(int argc, char const *argv[])
		{
			struct sigaction act;
			act.sa_handler = handler;
			sigemptyset(&act.sa_mask);
			act.sa_flag = 0;
			int ret = sigaction(SIG_ALRM, &act, NULL);
			//Do other things
			return 0;
		}
//	7. These functions are not reentrant -- they can use malloc and free and cannot be resume safely after interrupted

//7. Networking
//	1. Socket: Create a new socket
//	   Bind: Bind a socket to a specified port
//	   Listen: Make a socket able to accept connection from other socket
//	   Accept: Accept a connection on a socket and return a file descriptor
//	2. 
		#define PORT 5000
		#define BACKLOG 100
		int main(int argc, char const *argv[])
		{
			int UDP_sock = socket(AF_INET, SOCK_DGRAM, 0);
			struct addrinfo hints, *result;
			bzero(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_flags = AI_PASSIVE;
			getaddrinfo(NULL/*Localhost*/, PORT, &hints, &result);
			bind(UDP_sock, result->ai_addr, result->ai_addrlen);
			listen(UDP_sock, BACKLOG);
			int fd = accept(UDP_sock, NULL, NULL);
			char* buffer;
			size_t length;
			while (recv(fd, buffer, length, 0) != 0 || strcmp(buffer, "!") != 0)
			{
				printf("%s", buffer);
			}
			free(buffer);
			buffer = NULL;
			return 0;
		}
//	3. 
		#define PORT 5000
		#define BACKLOG 100
		int main(int argc, char const *argv[])
		{
			int TCP_sock = socket(AF_INET, SOCK_STREAM, 0);
			struct addrinfo hints, *result;
			bzero(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_flags = AI_PASSIVE;
			getaddrinfo(NULL/*Localhost*/, PORT, &hints, &result);
			bind(UDP_sock, result->ai_addr, result->ai_addrlen);
			listen(TCP_sock, BACKLOG);
			int fd = accept(TCP_sock, NULL, NULL);
			char* buffer;
			size_t length;
			while (getline(fd, &buffer, &length) != 0 || strcmp(buffer, "!") != 0)
			{
				printf("%s", buffer);
			}
			free(buffer);
			buffer = NULL;
			return 0;
		}
// 4. select() will poll to check if any file descriptor is ready to fetch data from which will take lots of CPU resources
//	  epoll() will keep a set of fds and when fds are ready, it will be wake up by a certain event and pushed into a queue
// 5. TCP provide service based on connection, so it automatically manage resending packets, ignoring duplicate packets, re-arranging out-of-order packets and changing the rate at which packets are sent. 
//	  UDP is connectless protocol and will not provide features above
// 6. 
//			Server             Client
//			  |					  |
//	socket	  |					  | socket						
//	bind	  |					  | connect
//	listen	  |					 /| (connect block)
//	accept	  |_______SYN_______/ |
//	(block)	  |\				  |
//			  |	\__SYN + 1, ACK __| (connect return)
//  	      |                  /|  
//(read block)|_____ ACK + 1 ___/ |
//            |					  |
//            |					  |
//            |                   |
//
// In HTTP 1.1, connection will be kept so that you don't need to build connection every time you want to send a request like in HTTP 1.0
//	7. 
		#define MAX 16*1024
		void read(int fd, char* buffer)
		{
			size_t accumulator = 0;
			buffer = calloc(sizeof(char), 16*1024);
			char* buf;
			size_t length;
			again:
			while (getline(&buf, &length, fd) != 0)
			{
				if (length + accumulator < MAX)
				{
					strcat(buffer, buf);
					accumulator += length;
				}
				else
					break;
			}
			if (errno == EINTR)
			{
				goto again;
			}
			if (errno == 0)
			{
				//TODO
			}
			free(buf);
			buf = NULL;
		}
//	8. DNS is a system that translate domain name to IP address, the service is implemented with UDP protocol
//	9. NAT is a methodology of remapping one IP address space into another by modifying network address information in Internet Protocol (IP) datagram packet headers while they are in transit across a traffic routing device.
//	   The technique was originally used for ease of rerouting traffic in IP networks without renumbering every host. It has become a popular and essential tool in conserving global address space allocations in face of IPv4 address exhaustion by sharing one Internet-routable IP address of a NAT gateway for an entire private network.

//8. Files
//	1.
		fopen("input_file", "r");
		if (!file)
			exit(1);
		fseek(file, 0，SEEK_END);
		int offset = ftell(file);
		fseek(file, offset，SEEK_SET);
		char* buffer;
		size_t length;
		while (getline(&buffer, &length, file) != -1)
			write(pipe[1], buffer, length);
//	2. 
		int input = open(input_filename, O_RDONLY);
		struct buf;
		fstat(input, &buf);
		size_t size = buf.st_size;
		void* file = mmap(NULL, size, PROT_READ, MAP_PRIVATE, input, 0);
		if (file == MAP_FAILED)
		{
			mmapFail(input_filename);
			return -1;
		}
		while (size != 0)
		{
			printf("%c", file[size - 1]);
		}
//	3.	Symbolic link:
		symlink("/etc/password", path);
//		Hard link: 
		link("/etc/password", path);
//	4. Because /secret.txt does not exist
//	5. Sticky bit is a user ownership access right flag that can be assigned to files and directories on Unix-like systems.
//	   Setuid bits and setgid (short for "set user ID upon execution" and "set group ID upon execution", respectively)[1] are Unix access rights flags that allow users to run an executable with the permissions of the executable's owner or group respectively and to change behaviour in directories. 
//	6.
		bool is_dir(char* path)
		{
			if (!path)
				return false;
			else
			{
				struct stat buf;
				if (stat(path, &buf) == 0)
				{
					if (buf.st_mode & S_IFDIR)
						return true;
					else
						return false;
				}
			}
		}

//	7.
		char* path_to(char* path, char* file)
		{
			assert(file != NULL && path != NULL);
			DIR* dir = opendir(path);
			struct dirent* ptr;
			while ((ptr = readdir(dir)) != NULL)
			{
				if (is_dir(ptr))
				{
					strcat(path, ptr);
					path_to(path, file);
				}
				else
				{
					if (strcmp(ptr, file) == 0)
					{
						strcat(path, file);
						return path;
					}
				}
			}
			return NULL;
		}
//	8. sudo chmod 777 installmeplz
//	   sudo chmod +x installmeplz

//9. File System
//	1. Assuming 4B entries, each block holds 1024:
//	   The inode is 128 bytes.
//	   The 10 direct blocks hold 10(4KB) = 40KB.
//	   The full indirect block holds 1024(4KB) = 4MB. The block itself uses (4 KB).
//	   The double-indirect block holds one entry (4KB) which points to a half-full indirect block (4 KB) which holds 512(4KB) = 2MB.
//
//	   Total memory used: 128B + 40KB + 4MB + 4KB + 4KB + 4KB + 2MB = 6MB 52KB 128B
//	2. 3 accesses for a hard link, 4 accesses for a symbolic link.
//	3. Stored: inode number, access permissions, file size, last modification time, last change time, last access time
//	   Not Stored: File name, file path, file type
//  4. 
		struct stat src;
		int file_err = lstat(“file.c”, &src);
		if(!file_err)	
		{
			if(S_ISDIR(s.st_mode))
				return -2; // file is directory
			else if (S_ISLNK(s.st_mode)) 
				return -3; // file is symlink
		}
		else 
			return -1; //could not open file
//	5. Let x be the number of direct blocks.
//	   Let y be the number of single-indirect blocks.
// 	   Assuming all blocks are completely filled:

// 	   x(4KB) + y(1024)(4KB) = x(4KB) + y(4MB).
//	6. When you are attempting to get information regarding a symbolic link















