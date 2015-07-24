#ifndef __LOCK_H_
#define __LOCK_H_

#include <sys/sem.h>
#include <errno.h>

union semun {
	int val;
};

class Lock {
public:
	Lock():sem_id_(-1) {
	}
	~Lock() {
	}
	bool init(key_t key);
	bool clear();
	void lock();
	void unlock();
private:
	int sem_id_;
};

//implementation
bool Lock::init(key_t key) {
	sem_id_ = semget(key, 1, IPC_CREAT | 664);
	if (-1 == sem_id_) {
		return false; // encounter error
	}
	semun sem;
	sem.val = 1;
	if (-1 == semctl(sem_id_, 0, SETVAL, sem)) {
		return false;
	}
	return true;
}

bool Lock::clear() {
	if (-1 != sem_id_) {
		semun sem;
		sem.val = 0;
		semctl(sem_id_, 0, IPC_RMID, sem);
		sem_id_ = -1;
	}
}

void Lock::lock() {
	sembuf semb;
	semb.sem_num = 0;
	semb.sem_op = -1;
	semb.sem_flg = SEM_UNDO;
	while (-1 == semop(sem_id_, &semb, 1)) {
		if (EINTR == errno)
			continue;
	}
}

void Lock::unlock() {
	sembuf semb;
	semb.sem_num = 0;
	semb.sem_op =  +1;
	semb.sem_flg = SEM_UNDO;
	while (-1 == semop(sem_id_, &semb, 1)) {
		if (EINTR == errno)
			continue;
	}
}


#endif
